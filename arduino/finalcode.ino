// includes
#include "mbedtls/base64.h"
#include "mbedtls/md.h"
#include "analogWrite.h"
#include <time.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <base64.h>
#include <PubSubClient.h>
#include <ThingSpeak.h>
#include <WebServer.h>
#include "headers.h"

void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
  // Print the details of the message that was received to the serial monitor.
  char* fields[4];
  fields[0] = strtok((char*)payload, ",");
  for(int i = 0; i < 3; i++)
  {
    fields[0] = strtok(NULL, ",");
  }
  fields[1] = strtok(NULL, ",");
  fields[2] = strtok(NULL, ",");
  fields[3] = strtok(NULL, ",");

  float k_p, k_i, k_d, input_target;
  for(int i = 0; i < 4; i++)
  {
    int j = 10;
    char temp[255];
    int k = 0;
    while(1)
    {
      if(fields[i][j] == '"')
        break;
      temp[k++] = fields[i][j++];
    }
    temp[k] = '\0';
    if(i == 0)
      k_p = atof(temp);
    else if(i == 1)
      k_i = atof(temp);
    else if(i == 2)
      k_d = atof(temp);
    else if(i == 3)
      input_target = atof(temp);
  }

  Serial.print("KP: ");
  Serial.println(k_p);
  Serial.print("KI: ");
  Serial.println(k_i);
  Serial.print("KD: ");
  Serial.println(k_d);
  Serial.print("Target Angle: ");
  Serial.println(input_target);

  noInterrupts();
  posi = 0;
  interrupts();
  Experiment_ID++;
  Serial.println("Experiment ID: " + String(Experiment_ID));
  
  statusCode = 0;

  onem2mBuffer = "["+String(Experiment_ID)+","+String(input_target)+","+String(k_p)+","+String(k_d)+","+String(k_i)+",";
  jsonBody = "";

  if(input_target != previous_target){
    PID_control(input_target,k_p,k_d,k_i,Experiment_ID);
    previous_target = input_target;
    setMotor(0,0,PWM,IN1,IN2);  
  }

  delay(5000);
  if(previous_target != 0.0){
    PID_reset(0,K_P,K_D,K_I);
    previous_target = 0;
    setMotor(0,0,PWM,IN1,IN2); 

  }
  delay(5000);
}

void mqttSubscribe( long subChannelID ){
  String myTopic = "channels/"+String( subChannelID )+"/subscribe";
  mqtt_client.subscribe(myTopic.c_str());
}


void setup() {

    live_Json  = "{\"write_api_key\": \"" ;
    live_Json +=  LIVE_WRITE_API_KEY     ;
    live_Json += "\",\"updates\":" ;
    
  // put your setup code here, to run once:
    Serial.begin(9600);

    pinMode(ENCA,INPUT);
    pinMode(ENCB,INPUT);
    attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);

    pinMode(PWM,OUTPUT);
    //analogWriteResolution(PWM, 8);
    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);

    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }
    
    Serial.print("\nConnected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    mqtt_client.setCallback( mqttSubscriptionCallback );
    mqtt_client.setBufferSize( 2048 );
}


void loop() {
  // put your main code here, to run repeatedly:
  while (mqtt_client.connected() == NULL) {
    Serial.println("connecting");
    mqtt_client.connect(mqtt_clientid, mqtt_clientid, mqtt_pass);
    mqttSubscribe( LIVE_CHANNEL_ID );
  }
  mqtt_client.loop();
}

void PID_reset(float target,float kp,float kd,float ki) 
{
    target = target*-1;
    startTime = millis();
    lastTime = millis();
    eprev = 0;
    use_integral = false;
    md = 0;
    while(millis()- startTime < PID_TIMER){
      if(millis() - lastTime > SAMPLING_RATE){

        currT = micros();
        deltaT = ((float) (currT - prevT))/( 1.0e6 );
        prevT = currT;

        //Read the position
        pos = 0; 
        
        noInterrupts(); // disable interrupts temporarily while reading
        pos = (float)posi*0.85714285714f;//* 0.64056939501 
        Serial.print(" ");
        Serial.print(pos);
        interrupts(); // turn interrupts back on
        
        // error
        e = target-pos;

        // derivative
        dedt = (e-eprev)/(deltaT);

        // integral
        if(dedt == 0)
          use_integral = true;
        if (use_integral == true)
        {
            if (pos > target)
            {
              if (md == -1)
                eintegral = 0;
              md = 1;
            }
            else if (pos < target)
            {
              if (md == 1)
                eintegral = 0;
              md = -1;
            }
           
        
          eintegral = eintegral + e*deltaT;
        }

        // control signal
        u = kp*e + kd*dedt + ki*eintegral;

        // motor power
        pwr = fabs(u);
        if( pwr > 255 ){
            pwr = 255;
        }

        // motor direction
        dir = 1;
        if(u<0){
            dir = -1;
        }

        // signal the motor
        setMotor(dir,pwr,PWM,IN1,IN2);


        // store previous error
        eprev = e;

        lastTime = millis();

        if(fabs(target-pos)<0.5){
          break;
        }
      }
    }
    setMotor(0,0,PWM,IN1,IN2);
    eprev = 0;
}

void PID_control(float target,float kp,float kd,float ki,int ID) 
{
    
    startTime = millis();
    lastTime = millis();
    lastpubTime = millis();
    md = 0;
    onem2mBuffer = onem2mBuffer +"(";
    eprev = 0;
    use_integral = false;
    jsonBody = "[";
    while(millis()- startTime < PID_TIMER){

      if(millis() - lastTime > SAMPLING_RATE){
        // time difference
        currT = micros();
        deltaT = ((float) (currT - prevT))/( 1.0e6 );
        prevT = currT;

        //Read the position
        pos = 0; 
        
        noInterrupts(); // disable interrupts temporarily while reading
        pos = (float)posi*0.85714285714f;//* 0.64056939501  ;
        Serial.print(" ");
        Serial.print(pos);
        interrupts(); // turn interrupts back on
        
        // error
        e = target - pos;

        // derivative
        dedt = (e-eprev)/(deltaT);

        // integral
        if(dedt == 0)
          use_integral = true;
        if (use_integral == true)
        {
            if (pos > target)
            {
              if (md == -1)
                eintegral = 0;
              md = 1;
            }
            else if (pos < target)
            {
                if (md == 1)
                eintegral = 0;
              md = -1;
            }
           
        
          eintegral = eintegral + e*deltaT;
        }

        // control signal
        u = kp*e + kd*dedt + ki*eintegral;

        // motor power
        pwr = fabs(u);
        if( pwr > 255 ){
            pwr = 255;
        }

        // motor direction
        dir = 1;
        if(u<0){
            dir = -1;
        }

        // signal the motor
        setMotor(dir,pwr,PWM,IN1,IN2);


        // store previous error
        eprev = e;

        lastTime = millis();
        delta_pt = int((lastTime - lastpubTime)/float(100));

        onem2mBuffer = onem2mBuffer + "{";
        onem2mBuffer += String(deltaT) ;
        onem2mBuffer += ",";  
        onem2mBuffer += String(pos)    ; 
        onem2mBuffer += "},";
        
        Temp="{\"delta_t\":\""+String(delta_pt)+"\",";
        jsonBody += Temp;
        
        Temp="\"field5\":\""+String(pos)+"\"";
        jsonBody += Temp;

        Temp="},";
        jsonBody += Temp;
        
        if(fabs(target-pos)<0.5){
          break;
        }
    }
      
  }
  
   setMotor(0,0,PWM,IN1,IN2);
   jsonBody[jsonBody.length()-1] = ']';
   onem2mBuffer[onem2mBuffer.length()-1] = ')';
   
   Temp="}";  
   jsonBody += Temp;
   onem2mBuffer += "]";

   // Doing a bulk update
   tsPOST = "";
   tsPOST += live_Json;
   tsPOST += jsonBody;
   //Serial.print(tsPOST);
   //Serial.println();

   thingspeakPOST(tsPOST, String(LIVE_CHANNEL_ID));
   
   onem2mPOST(onem2mBuffer);

   jsonBody = "";
   Temp = "";
   onem2mBuffer = "";
   tsPOST = "";
   eprev = 0;
}

void thingspeakPOST(String rep,String channel_id) {
  delay(10000);
  http_Client.begin("https://api.thingspeak.com/channels/"+channel_id+"/bulk_update.json");

  http_Client.addHeader("Content-Type", "application/json");

  Serial.println("POST");
  
  postStatusCode = 0;
  while(postStatusCode != 202){
    postStatusCode = http_Client.POST(rep);
      Serial.print(postStatusCode);
      Serial.println();
    if(postStatusCode != 202)
      delay(15000);
  }
  http_Client.end();
  //Serial.println(postStatusCode);
  postStatusCode = 0;
}

void onem2mPOST(String rep) {
    delay(10000);
    http_Client.begin(Post_URL);
  
    http_Client.addHeader("X-M2M-Origin", USER_PASS);
    http_Client.addHeader("Content-Type", "application/json;ty=4");
    http_Client.addHeader("Content-Length", "1000");
    Serial.println(rep);
    onem2mrequest= String() + "{\"m2m:cin\": {"

      +
      "\"con\": \"" + rep + "\","

      +
      "\"lbl\": \"" + "V1.0.0" + "\","

      +
      "\"cnf\": \"text\""

      +
      "}}";
    //Serial.println(onem2mrequest);

    onem2mcode = 0;
    while(onem2mcode != 201)
    {
      onem2mcode = http_Client.POST(onem2mrequest);
      Serial.println(onem2mcode);
      if(onem2mcode  != 201){
        delay(15000);
      }
    }
    http_Client.end();
    //Serial.println(onem2mcode);
    onem2mcode = 0;
    onem2mrequest = "";
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
    if(dir == 1){
        digitalWrite(in1,HIGH);
        digitalWrite(in2,LOW);
    }
    else if(dir == -1){
        digitalWrite(in1,LOW);
        digitalWrite(in2,HIGH);
    }
    else{
        digitalWrite(in1,LOW);
        digitalWrite(in2,LOW);
    }  
    analogWrite(pwm,pwmVal);
}

void readEncoder(){
    int b = digitalRead(ENCB);
    if(b > 0){
        posi--;
    }
    else{
        posi++;
    }
}
