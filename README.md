# Auto_power_off
Automatically power off 3D printer after print completes and printer cools down

 

What's Required: 

A smart 15-amp power plug 

An Alexa Echo device 

An ESP8266 module 

An IFTTT account 

A Sonic Pad 

 

Beneficial Background: 

Experience with Arduino-based projects 

Familiarity with IFTTT (If This Then That) 

Knowledge of Alexa routines 

 

Project Overview: 

This project harnesses an ESP8266 module to monitor Moonraker on the Sonic Pad, providing real-time updates on the status of your 3D printer. When the 3D printer job is complete, signifying that your print has finished, the ESP8266 sends a webhook signal to IFTTT. IFTTT then triggers an Alexa routine, prompting your Alexa device to patiently wait for the 3D printer to cool down. Once the cool-down period is over, it takes care of shutting off the smart power plug. The beauty of this setup is that it operates wirelessly via Wi-Fi, eliminating the need for any complex wiring. If you are already equipped with Echo devices (Alexa) and comfortable using the Arduino IDE to program the ESP8266 module, this project becomes even more straightforward. 

Moreover, this system offers more than just power control. It leverages Alexa to notify you when the sensor powers up and initializes, as well as when the print job has been paused. 

 
 
