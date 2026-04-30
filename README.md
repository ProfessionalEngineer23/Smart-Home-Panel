### Smart Home Panel

![alt text](image-1.png)

# This is a Smart Home Panel device that measures room conditions like temperature, humidity, pressure, light levels, air quality and human presence. 

# System Architecture:
![System Architecture Diagram](images/System_Architecture.png)

# The system consists of:
- The Smart Panel which acts as a sensor hub and Thread light device
- The Wifi Router for routing data packets
- The Light device which acts as a controllable device
- Thingsboard, a server service that processes data from the smart panel and light device. Allows the user to control the smart panel and light device via a dashboard.
-  Thread Border Router, middle man device that communicates with thread devices and connects these to wifi communicating services like Thingsboard or smart home services like Amazon Alexa app/ Google Home/ Smart Things

# What is Thingsboard?
Thingsboard is a backend service that anyone can deploy via Docker containers. It allows to deploy a webservice for viewing sensor data remotely through dashboards, automate devices with rule chains. An admin account who controls the service settings like smtp, tenant accounts, tenant permissions.

# What is Thread? 
Thread is a low power communication protocol that allows creating a mesh network for smart home devices to communicate with each other. There are thread radio compatable esp microcontrollers like the esp32C and esp32H series. 

# What is Matter?
Matter is a unifying solution to make it easier to program smart home devices and have them compatable with any ecosystem that supports matter. Espressif has their implementation called Matter SDK available in ESP IDF allowing development of thread/ wifi Matter Esp devices.

# Why do we need a Thread Border Router?
The thread border router acts as a middle man that allows devices that only speak thread to communicate with other thread devices or translate thread messages to wifi and send to wifi devices.  

# Do I need an Amazon Alexa for Thread? 
An Amazon Alexa is acts as a controller, meaning that it accepts wifi commands and tells the smart home devices to do what you want them to do. You can make a matter controller using a Raspberry Pi...

# What the project currently does: 
- Smart Panel with a touchscreen LVGL UI to view realtime room conditions and set a human presence alarm that sounds a built in buzzer if human presence detected, send sensor data to Thingsboard.
- Light Device turns on/ off light based on human presence detection rule chain configured in Thingsboard. User can choose to remotely turn off the light via Thingsboard if lights were left on or if animals are triggering human presence. 
- Thingsboard shows detailed dashboards for smart panel sensor data, alerts notifications if human presence is detected. Sends an email to defined tenant user using smtp. Light Device also has a dashboard for viewing light status and manual control if required.  
- Matter Thread light device integration into the Smart Panel. The smart panel has a dual processor set up. The main Esp32S3 does the heavy lifting of rendering the UI, measuring sensor data, sending to Thingsboard via publisher MQTT. The coprocessor is an Esp32H2 acts as a Thread Matter light end device controllable through the Alexa home app. 

Future work includes updating the LVGL UI with user wifi and password entry, updating the firmware on both smart panel processors to make the Esp32S3 communicate with the Esp32H2 coprocessor via UART. The Esp32H2 will interpret the sensor data for sending to Smart Home applications via Thread. The current hardware supports this, however due to time constraints this was put aside for now.  





