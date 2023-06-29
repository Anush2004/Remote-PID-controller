# PID Control Virtual Labs Dashboard

### Created as a part of ESW (Embedded Systems Workshop) as a final project

#### Brief Description:
This code opens up a Virtual Lab login page where the user is authenticated and sent to Virtual Labs portal. 
The design and flow of the website is inspired from IIITH's official Virtual Labs website. 
The user can choose from several Virtual Lab Experiments, however only the PID Control experiment is fully implemented.
Any user from anywhere in the world can perform the experiment and observe how the Proportional, Integral, and Derivative constants affect a DC motor's rotation angle. 
Although this part has an additional Arduino code set up to knit together the hardware and the software components, the code is not shown.
Special components in the experiment page include live streaming of the experiment's hardware setup and interactive controls given to the user.
The user can choose the angle of rotation they wish the DC motor should rotate to and can see how the PID control makes sure the DC motor turns to that angle accurately.
More info is provided in the pdf.

#### Languages used:
ExpressJs & NodeJs extensively.  
MongoDB is used to store the user's login information.

