# Feedback Control and Remote Labs

This repository contains the group project developed by Anush, Mukta, Ronak, and Sriya for the Embedded Systems Workshop (ESW) in our second year (November 2022). The project aims to demonstrate the principles of feedback control and the value of remote labs through a software dashboard. Please refer to the `Report.pdf` file for detailed information about the project.

## Contents

- `Report.pdf`: This file contains the comprehensive report of the project, providing an in-depth analysis and description of the work done.

- `presentation.pdf`: This file contains the presentation slides that offer an overview of the project, highlighting its key aspects and outcomes.

- `dashboard`: This directory contains the code for the software dashboard. It includes the necessary files and resources to run the dashboard interface.

- `headers.h`: This file contains the header files used in the code for the ESP32 module.

- `finalcode.cpp`: This file contains the code for the ESP32 module, which is responsible for implementing the feedback control mechanism.

## Project Description

The ESW group project focuses on the development of a feedback control system and remote lab implementation. The project's hardware setup is shown in the accompanying `setup.jpeg` image.

We utilized an ESP32 module and developed code to implement the feedback control mechanism. The software dashboard provides a user-friendly interface to monitor and control the system remotely (though we cant do it any longer as the hardware has been dismantled :/ ).

![Hardware Setup](./docs/setup.jpeg)

## Usage

To run the project and use the software dashboard, please follow the instructions below:

1. Create a `.env` file in the root directory of the project.

2. In the `.env` file, define the necessary configuration variables. For example, you can include:

   ```
   PORT=your_port_number
   MONGODB_URI=mongodb+srv://dummy:gpHMbuYZJQoe8knl@logininfo.ylrwwwj.mongodb.net/?retryWrites=true&w=majority&ssl=true
   SESSION_SECRET=your_session_secret
   ```

   Replace the values with the appropriate configurations for your environment. Ensure that there are no spaces around the equal sign (`=`) when defining the variables. Default Port Number is 3000.

3. Install the required dependencies by running the following command in the project's root directory:

   ```
   npm install
   ```

4. Start the server by running the following command:

   ```
   node server.js
   ```
5. Access the software dashboard by opening a web browser and navigating to `http://localhost:3000` (assuming you have kept the default `PORT` value).

## Feedbacks and Questions

If you have any questions regarding the project, please feel free to contact any of the team members:

- Anush Anand
  - GitHub: [anush](https://github.com/Anush2004)

- Mukta Chanda
  - GitHub: [mukta](https://github.com/muktachanda)

- Ronak Dhingra 
  - GitHub: [ronak](https://github.com/Ronak-Dhingra)

- Sriya Venepally
  - GitHub: [sriya](https://github.com/sriyav17)
  
We will be glad to assist you.

