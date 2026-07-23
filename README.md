# Smart Home Panel

![Smart Home Panel Prototype](images/SmartPanelRev1.png)

## Problem Statement and Motivation

Many smart home lighting systems use PIR motion sensors, which mainly detect changes in infrared radiation caused by movement. This can be unreliable when a person is sitting still, for example while studying or working, because the system may incorrectly assume the room is empty and turn the lights off.

This project explores a more reliable room-monitoring system using mmWave human presence detection. Unlike PIR sensors, mmWave radar can detect small movements, including micro-movements from a stationary person. This makes it more suitable for presence-based lighting, room monitoring, and basic alarm features.

The project also provided an opportunity to combine several areas of Computer Engineering, including embedded programming, PCB design, sensor integration, touchscreen UI development, MQTT communication, ThingsBoard dashboards, and Matter-over-Thread smart home experimentation.

## Overview

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

![Thingsboard Smart Panel Dashboard](images/Thingsboard Smart Panel Dashboard.png)

![Light Device Dashboard](images/Light Device Dashboard.png)

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

# System_Sequence_Diagram
![System_Sequence_Diagram](images/System_Sequence_Diagram.jpg)

## Hardware Design

![PCB Layout](images/PCB.jpg)

<img width="1292" height="916" alt="image" src="https://github.com/user-attachments/assets/1390bd56-32ba-431b-892c-bc66b8cec42b" />

<img width="1292" height="916" alt="image" src="https://github.com/user-attachments/assets/00727843-fd7e-4299-ba27-825f9a194aaa" />

A custom PCB was designed to connect the ESP32-S3, sensors, display, buzzer, buck converter power circuitry, and expansion headers.

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

The system uses several communication protocols:

| Protocol | Used For |
|---|---|
| SPI | TFT display and touch controller |
| I2C | AHT20, BMP280, BH1750, SGP40 sensors |
| UART | RD-03D radar and ESP32-H2 coprocessor communication |
| Wi-Fi | MQTT communication with ThingsBoard |
| Thread | Matter smart home communication |

---

## Software Design

The Smart Panel firmware was designed around a non-blocking service-loop approach. Instead of using long `delay()` calls, the main loop repeatedly updates each subsystem when it is due to run. This allows the touchscreen interface to stay responsive while the ESP32-S3 also reads sensors, checks the mmWave radar, updates the alarm logic, maintains Wi-Fi/MQTT communication, and sends telemetry to ThingsBoard.

The project does not currently use a custom RTOS task structure. Although the ESP32 runs on FreeRTOS internally, the application logic is mainly organised as repeated service functions inside the Arduino `loop()`. I opted for this because it kept the prototype development faster and debugging easier. A future version could separate the display, sensor reading, MQTT communication, and alarm handling into dedicated FreeRTOS tasks.

The firmware is planned to be modularised into separate files for display handling, sensor services, cloud communication, shared data, configuration, and secrets. This makes the project easier to maintain compared to keeping all logic inside one large sketch.

## Firmware Flowchart:
![Firmware Flowchart](images/Smart_Panel_Flow.jpg)
## State Chart:
![State Chart](images/Smart_Panel_State.jpg)
## Thingsboard Rule Chain:
![Root Rule Chain](images/Root_Rule_Chain.png)
![Light Control Rule Chain](images/Light_Control_Rule_Chain.png)
![Thingsboard Alarms and SMTP Rule Chain](images/Thingsboard_Alarms_and_SMTP_Rule_Chain.png)
## Enclosure Design

### Smart Panel Enclosure

![Smart Panel Enclosure](images/Smart_Panel_Enclosure.png)

The Smart Panel enclosure was designed to house the touchscreen, ESP32-S3, sensors, radar module, buzzer, and internal wiring.

The enclosure was designed as a prototype and would need further refinement for a more professional or manufacturable version.

### Light / OTBR Enclosure

![Light and OTBR Enclosure](images/Light_OTBR_Enclosure.png)

A separate enclosure was designed for the light device and Thread Border Router hardware. This helped separate the main sensor panel from the smart home actuation and Thread testing hardware.

Acknowledgements:
While I didn't get the project to the level that I was initially hoping, I did manage to solve my problem of lights turning off when still present in a room, receiving alarms if someone occupies a room, room health monitoring and alerts. I learned a lot from this project. I realized how important project design and careful consideration are. Throughout the actual implementation of the project hardware I had unexpected issues like when I connected all my peripherals to my Esp32S3 devkit, my display was experiencing brownouts due to unstable current spikes in wifi / dips from the Esp32S3 voltage regulator. I decided to use a Buck Step down Converter in order to convert Higher voltages ranging from 12-5V to a stable 5V output. I read datasheets on how to implement one on a pcb and designed one accordingly with the specs provided by the datasheets, the ic module I used was the "name of ic" , it was suitable because it was capable of taking in 12V and stepping it down to 5V... I decided to use a buck converter because these are more power efficient compared to voltage regulators which release enerhy through heat when under high load. I didn't want my device to overheat and potentially catch fire considering that I was connecting many sensors and display with touch support. There were many problems throughout this project that I overcame, one noticable one was when I got my pcb manufactured I had no idea that the charge up time of the Buck converter circuit would cause my main Esp32S3 to not be able to start up correctly. 

---
