# Smart Home Panel

![Smart Home Panel Prototype](images/SmartPanelRev1.png)

## Overview

#The problems: 
Anecdote: It can be annoying when you are studying in a library and you're the only one left all alone. Most smart home areas use PIR sensors which rely on infrared light from detecting moving heat radiation. But what if you are not actively moving and practicing exam problems? PIR doesnt detect a person still in the room and turns off the lights. Have you ever had a moment when you sat in a toilet stall and got carried away reading an interesting artical and all of a sudden the lights turn off? That is because of outdated PIR sensors! After some research I discovered mmWave sensors which send out mm wavelength waves ranging from 30 to 300 Ghz. These can detect micromovements like breathing, blinking or even simple twitching. This technology allowed me to control and keep on lights while still implementing energy savings when no one is actually in a room. 
I problem I had was coming up with a project, I used Esp32, TFT SPI screens, I2C sensors, networking protocols, Apis, pcb design. I wanted to combine all of my knowledge of these technologies together and solve my problem while also expanding my knwoledge with other technologies like matter and thread protocol and doing this through practical problem solving and research. 

The **Smart Home Panel** is an embedded IoT system designed to monitor room conditions, detect human presence, display live sensor data on a touchscreen interface, and trigger basic smart home automation.

Currently the project combines:

- Environmental sensing
- Human presence detection
- Local touchscreen interaction
- Cloud telemetry using ThingsBoard
- MQTT-based automation integrated with Thingsboard Rule Chains
- Basic Matter-over-Thread smart home integration (Work in progress)

---

---


## Project Aim

The aim of this project was to design and build a smart home panel capable of:

- Measuring room temperature, humidity, pressure, light level, and air quality
- Detecting human presence using mmWave radar
- Displaying live readings on a local LVGL touchscreen UI
- Sending telemetry to a ThingsBoard server using MQTT
- Controlling a separate ESP32-S3 light device using ThingsBoard RPC
- Demonstrating Matter-over-Thread device commissioning using an ESP32-H2 and ESP32-C6 Thread Border Router

---

## System Architecture

![System Architecture](images/System_Architecture.png)

The system is built around a central **ESP32-S3 Smart Panel**. This device reads sensor data, updates the local touchscreen interface, and sends telemetry to a ThingsBoard server over Wi-Fi using MQTT.

ThingsBoard is used to store telemetry, display dashboards, generate alarms, send email notifications, and trigger automation rules. A separate ESP32-S3 light device subscribes to RPC commands from ThingsBoard and can be controlled automatically based on human presence.

A secondary Matter-over-Thread subsystem was also tested using an ESP32-H2 as a Matter light device and an ESP32-C6 as a Thread Border Router.

---

## Main Components

| Component | Purpose |
|---|---|
| ESP32-S3 | Main controller for the Smart Panel |
| ESP32-H2 Zero | Matter-over-Thread coprocessor / light device |
| ESP32-C6 | Thread Border Router |
| ILI9488 / ST7796 TFT Touchscreen | Local graphical user interface |
| AHT20 | Temperature and humidity sensing |
| BMP280 | Atmospheric pressure sensing |
| BH1750 | Ambient light sensing |
| SGP40 | Air quality / VOC index sensing |
| RD-03D mmWave Radar | Human presence detection |
| ThingsBoard | IoT dashboard, telemetry storage, alarms, and rule chains |
| ESP32-S3 Light Device | Remote light device controlled using RPC |

---

## What the System Currently Does

### Smart Panel

![Smart Panel Front](images/Smart_Panel_Front.jpg)

The Smart Panel acts as the main sensor hub. It reads environmental values, detects human presence, displays live data on the touchscreen, and sends telemetry to ThingsBoard.

Current measured values include:

- Room temperature
- Humidity
- Atmospheric pressure
- Light level
- Air quality / VOC index
- Human presence

The interface was built using **LVGL**, allowing the panel to show live readings, detail screens, and alarm controls.

---

### Human Presence Detection

![Room Testing](images/Room_Testing.jpg)

Human presence is detected using an **RD-03D mmWave radar sensor**. This was chosen instead of a traditional PIR sensor because mmWave sensors can detect small movements, including micro-movements from a stationary person.

This makes the system more suitable for smart home automation, where a person may be sitting still but should still be detected as present.

---

### ThingsBoard Dashboard

![ThingsBoard Dashboard](images/Thingsboard_Dashboard.jpg)

ThingsBoard is used as the IoT backend for the project. The Smart Panel sends telemetry to ThingsBoard using MQTT.

ThingsBoard provides:

- Live dashboards
- Sensor data visualisation
- Alarm generation
- Email alerts
- Rule chain automation
- RPC control of the light device

A rule chain was created so that when human presence is detected, ThingsBoard sends an RPC command to the ESP32-S3 light device to turn the light on. When presence is no longer detected, the light can be turned off.

---

### Light Device and Thread Border Router

![OTBR and Light Device](images/OTBR&Light.jpg)

A separate ESP32-S3 light device was used to demonstrate remote actuation through ThingsBoard RPC commands.

The project also explores Matter-over-Thread using:

- ESP32-H2 Zero as a Matter light device
- ESP32-C6 as a Thread Border Router
- Amazon Alexa as a Matter controller

The Matter subsystem currently works as a separate demonstration. Full communication between the ESP32-S3 Smart Panel and ESP32-H2 Matter coprocessor is planned as future work.

---

## Hardware Design

![PCB Layout](images/PCB.jpg)

<img width="1292" height="916" alt="image" src="https://github.com/user-attachments/assets/1390bd56-32ba-431b-892c-bc66b8cec42b" />

<img width="1292" height="916" alt="image" src="https://github.com/user-attachments/assets/00727843-fd7e-4299-ba27-825f9a194aaa" />

A custom PCB was designed to connect the ESP32-S3, sensors, display, buzzer, buck converter power circuitry, and expansion headers.

The system uses several communication protocols:

| Protocol | Used For |
|---|---|
| SPI | TFT display and touch controller |
| I2C | AHT20, BMP280, BH1750, SGP40 sensors |
| UART | RD-03D radar and ESP32-H2 coprocessor communication |
| Wi-Fi | MQTT communication with ThingsBoard |
| Thread | Matter smart home communication |

---

## Enclosure Design

### Smart Panel Enclosure

![Smart Panel Enclosure](images/Smart_Panel_Enclosure.png)

The Smart Panel enclosure was designed to house the touchscreen, ESP32-S3, sensors, radar module, buzzer, and internal wiring.

The enclosure was designed as a prototype and would need further refinement for a more professional or manufacturable version.

### Light / OTBR Enclosure

![Light and OTBR Enclosure](images/Light_OTBR_Enclosure.png)

A separate enclosure was designed for the light device and Thread Border Router hardware. This helped separate the main sensor panel from the smart home actuation and Thread testing hardware.

---
