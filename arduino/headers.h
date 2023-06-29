#ifndef H_H
#define H_H

#define WIFI_SSID 
#define WIFI_PASSWD

// ThingSpeak
const char* mqtt_server = "mqtt3.thingspeak.com";

const char* mqtt_clientid = "";
const char* mqtt_ursnm = "";
const char* mqtt_pass = "";
    
//Live Channel ID
#define LIVE_CHANNEL_ID    
#define LIVE_WRITE_API_KEY 
#define LIVE_READ_API_KEY  
// Onem2m
#define USER_PASS 
#define Post_URL 
    

// PID constants
    #define K_P 10
    #define K_D 0.025
    #define K_I 5

// Details

  // Pin Details
  
    int IN1 = 18;     // Input Pin 1 [motor driver]
    int IN2 = 23;     // Input Pin   [motor driver]
    int PWM = 2;     // PWM Pin     [motor driver]
    int ENCA = 34;    // Encoder A   [DC motor]
    int ENCB = 32;    // Encoder B   [DC motor]
  
  // Wifi
    char ssid[] = WIFI_SSID;
    char password[] = WIFI_PASSWD;
  
  // Thingspeak 
      // Live
          unsigned long liveWriteChannelID    = LIVE_CHANNEL_ID;
          const char*   liveWriteAPIKey       = LIVE_WRITE_API_KEY;
          const char*   liveReadAPIKey        = LIVE_READ_API_KEY;
	WiFiClient client;
	PubSubClient mqtt_client(mqtt_server, 1883, client);

   HTTPClient http_Client;

// Variables

    int Experiment_ID = 0;

  // PID constants (can be taken as input also)
    float k_p = K_P;
    float k_d = K_D;
    float k_i = K_I;

  // Strings for json formatting
    String live_Json = "";
    String Temp = "";  
    String jsonBody = "";   //JSON body
    String tsPOST = "";
    String onem2mBuffer = "";
    String onem2mrequest = "";

  // Time
    #define PID_TIMER 10000
    
    uint wifi_delay = 5000;
    uint startTime;
    uint lastTime;
    uint lastpubTime;
    
    int delta_pt =0 ;
    int SAMPLING_RATE = 100;
    long prevT = 0;
    long currT;
    float deltaT;
    
  // PID variables
    int dir;
    int md = 0;
    bool use_integral = false;
    float eprev = 0;
    float eintegral = 0;
    float e = 0;
    float dedt = 0;
    float u = 0;
    float pwr = 0;
    volatile int posi = 0;
 
  // Angles
    float input_target = 0.0;
    float previous_target=0.0;
    float pos = 0;

  // StatusCode
    int statusCode     = 0;
    int postStatusCode = 0;
    int onem2mcode     = 0;


#endif