# Smart Home Panel

<p align="center">
  <img src="images/SmartPanelRev1.png" alt="Smart Home Panel Prototype" width="750">
</p>

<p align="center">
  <em>Figure 1: Smart Home Panel prototype revision</em>
</p>

---

## Table of Contents

- [Overview](#overview)
- [Problem Statement and Motivation](#problem-statement-and-motivation)
- [Project Aim](#project-aim)
- [System Architecture](#system-architecture)
- [System Functionality](#system-functionality)
- [System Sequence Diagram](#system-sequence-diagram)
- [Hardware Design](#hardware-design)
- [Software Design](#software-design)
- [ThingsBoard Rule Chains](#thingsboard-rule-chains)
- [Enclosure Design](#enclosure-design)
- [Current Project Status](#current-project-status)
- [Future Work](#future-work)

---

## Overview

The **Smart Home Panel** is an embedded IoT system designed to monitor room conditions, detect human presence, display live sensor data on a touchscreen interface, and trigger basic smart home automation.

The project combines environmental sensing, mmWave human presence detection, a local LVGL touchscreen interface, cloud telemetry using ThingsBoard, MQTT-based automation, and basic Matter-over-Thread smart home integration.

This project was developed as part of my **3rd Year Computer Engineering project** and demonstrates practical experience with embedded systems, PCB design, sensor integration, IoT platforms, cloud dashboards, and smart home technologies.

---

## Problem Statement and Motivation

Many smart home lighting systems use PIR motion sensors, which mainly detect changes in infrared radiation caused by movement. This can be unreliable when a person is sitting still, for example while studying or working, because the system may incorrectly assume that the room is empty and turn the lights off.

This project explores a more reliable room-monitoring system using mmWave human presence detection. Unlike PIR sensors, mmWave radar can detect small movements, including micro-movements from a stationary person. This makes it more suitable for presence-based lighting, room monitoring, and basic alarm features.

The project also gave me the opportunity to explore and combine several technologies into one working embedded system. This included sensor integration, touchscreen UI development, web-based dashboards, MQTT communication, ThingsBoard rule chains, and *Matter-over-Thread experimentation. It was decided to keep main features within one Smart Panel device to stay realistic with the given time/ make the system easier to prototype.

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

<p align="center">
  <img src="images/System_Architecture.png" alt="Smart Home Panel System Architecture" width="750">
</p>

<p align="center">
  <em>Figure 2: High-level system architecture</em>
</p>

The system is built around a central **ESP32-S3 Smart Panel**. This device reads sensor data, updates the local touchscreen interface, and sends telemetry to a ThingsBoard server over Wi-Fi using MQTT.

ThingsBoard is used to store telemetry, display dashboards, generate alarms, send email notifications, and trigger automation rules. A separate ESP32-S3 light device subscribes to RPC commands from ThingsBoard and can be controlled automatically based on human presence detection.

A secondary Matter-over-Thread subsystem was also tested using an ESP32-H2 as a Matter light device and an ESP32-C6 as a Thread Border Router. This Matter subsystem currently works as a separate demonstration, with full integration planned as future work.

## System Functionality

### Smart Panel

<p align="center">
  <img src="images/Smart_Panel_Front.jpg" alt="Smart Panel Front View" width="750">
</p>

<p align="center">
  <em>Figure 3: Smart Panel front view during testing</em>
</p>

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

<p align="center">
  <img src="images/Room_Testing.jpg" alt="Room Testing with Smart Panel" width="750">
</p>

<p align="center">
  <em>Figure 4: Room testing of the Smart Panel and mmWave presence detection</em>
</p>

Human presence is detected using an **RD-03D mmWave radar sensor**. This was chosen instead of a traditional PIR sensor because mmWave sensors can detect small movements, including micro-movements from a stationary person.

This makes the system more suitable for smart home automation, where a person may be sitting still but should still be detected as present.

---

### ThingsBoard Dashboard

<p align="center">
  <img src="images/Thingsboard_Dashboard.jpg" alt="ThingsBoard Dashboard Overview" width="750">
</p>

<p align="center">
  <em>Figure 5: ThingsBoard dashboard overview</em>
</p>

<p align="center">
  <img src="images/Thingsboard_Smart_Panel_Dashboard.png" alt="ThingsBoard Smart Panel Dashboard" width="750">
</p>

<p align="center">
  <em>Figure 6: ThingsBoard Smart Panel sensor dashboard</em>
</p>

<p align="center">
  <img src="images/Light_Device_Dashboard.png" alt="ThingsBoard Light Device Dashboard" width="750">
</p>

<p align="center">
  <em>Figure 7: ThingsBoard light device dashboard</em>
</p>

ThingsBoard is used as the IoT backend for the project. The Smart Panel sends telemetry to ThingsBoard using MQTT.

ThingsBoard provides:

- Live dashboards
- Sensor data visualisation
- Alarm generation
- Email alerts
- Rule chain automation
- RPC control of the light device

A rule chain was created so that when human presence is detected, ThingsBoard sends an RPC command to the ESP32-S3 light device to turn the light on. When presence is no longer detected, the light automatically turns off. The light device can be manually turned off using the light device dashboard if the lights were left on due to false presence detection from a pet for example. 

---

### Light Device and Thread Border Router

<p align="center">
  <img src="images/OTBR%26Light.jpg" alt="OTBR and Light Device" width="750">
</p>

<p align="center">
  <em>Figure 8: Thread Border Router and light device testing</em>
</p>

A separate ESP32-S3 light device was used to demonstrate remote actuation through ThingsBoard RPC commands.

The project also explores Matter-over-Thread using:

- ESP32-H2 Zero as a Matter light device
- ESP32-C6 as a Thread Border Router
- Amazon Alexa as a Matter controller

The Matter subsystem currently works as a separate demonstration. Full communication between the ESP32-S3 Smart Panel and ESP32-H2 Matter coprocessor is planned as future work.

---

## System Sequence Diagram

<p align="center">
  <img src="images/System_Sequence_Diagram.jpg" alt="System Sequence Diagram" width="750">
</p>

<p align="center">
  <em>Figure 9: Smart Panel system sequence diagram</em>
</p>

The sequence diagram shows how the Smart Panel communicates with the sensors, mmWave radar, ThingsBoard, the rule chain, the light device, and the SMTP email service.

The Smart Panel does not directly control the light. Instead, it sends telemetry to ThingsBoard. The ThingsBoard rule chain then decides whether the light should turn on or off and whether an email alert should be sent.

---

## Hardware Design

<p align="center">
  <img src="images/PCB.jpg" alt="Smart Panel PCB" width="750">
</p>

<p align="center">
  <em>Figure 10: Smart Panel PCB</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/1390bd56-32ba-431b-892c-bc66b8cec42b" alt="Smart Panel Schematic" width="750">
</p>

<p align="center">
  <em>Figure 11: Smart Panel schematic</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/00727843-fd7e-4299-ba27-825f9a194aaa" alt="Smart Panel PCB Layout" width="750">
</p>

<p align="center">
  <em>Figure 12: Smart Panel PCB layout</em>
</p>

A custom PCB was designed to connect the ESP32-S3, display, sensors, buzzer, buck converter power circuitry, and expansion headers. The PCB helped reduce wiring complexity compared to the early breadboard prototype and made the project closer to a complete embedded system.

### Main Components

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

### Communication Interfaces

| Protocol | Used For |
|---|---|
| SPI | TFT display and touch controller |
| I2C | AHT20, BMP280, BH1750, and SGP40 sensors |
| UART | RD-03D radar and ESP32-H2 coprocessor communication |
| Wi-Fi | MQTT communication with ThingsBoard |
| Thread | Matter smart home communication |

---

## Software Design

The Smart Panel firmware was designed around a **non-blocking service-loop approach**. Instead of using long `delay()` calls, the main loop repeatedly updates each subsystem when it is due to run. This keeps the touchscreen responsive while the ESP32-S3 reads sensors, checks the mmWave radar, updates alarm logic, maintains Wi-Fi/MQTT communication, and sends telemetry to ThingsBoard.

The project does not currently use a custom RTOS task structure. Although the ESP32 runs on FreeRTOS internally, the application logic is mainly organised as repeated service functions inside the Arduino `loop()`. This approach kept the prototype easier to develop and debug.

A future version could separate the display, sensor reading, MQTT communication, and alarm handling into dedicated FreeRTOS tasks. The firmware is also planned to be modularised into separate files for display handling, sensor services, cloud communication, shared data, configuration, and secrets.

### Firmware Flowchart

<p align="center">
  <img src="images/Smart_Panel_Flow.jpg" alt="Smart Panel Firmware Flowchart" width="750">
</p>

<p align="center">
  <em>Figure 13: Smart Panel firmware flowchart</em>
</p>

### State Chart

<p align="center">
  <img src="images/Smart_Panel_State.jpg" alt="Smart Panel State Chart" width="750">
</p>

<p align="center">
  <em>Figure 14: Smart Panel state chart</em>
</p>

---

## ThingsBoard Rule Chains

The rule chains handle the cloud-side automation. When the Smart Panel sends new telemetry, ThingsBoard checks the human presence value. If presence is detected, the rule chain can create an alarm, send an email alert, and send an RPC command to turn on the light device. If no presence is detected, the light can be turned off.

<p align="center">
  <img src="images/Root_Rule_Chain.png" alt="ThingsBoard Root Rule Chain" width="750">
</p>

<p align="center">
  <em>Figure 15: ThingsBoard root rule chain</em>
</p>

<p align="center">
  <img src="images/Light_Control_Rule_Chain.png" alt="ThingsBoard Light Control Rule Chain" width="750">
</p>

<p align="center">
  <em>Figure 16: ThingsBoard light control rule chain</em>
</p>

<p align="center">
  <img src="images/Thingsboard_Alarms_and_SMTP_Rule_Chain.png" alt="ThingsBoard Alarms and SMTP Rule Chain" width="750">
</p>

<p align="center">
  <em>Figure 17: ThingsBoard alarms and SMTP email rule chain</em>
</p>

---

## Enclosure Design

### Smart Panel Enclosure

<p align="center">
  <img src="images/Smart_Panel_Enclosure.png" alt="Smart Panel Enclosure Design" width="750">
</p>

<p align="center">
  <em>Figure 18: Smart Panel enclosure design</em>
</p>

The Smart Panel enclosure was designed to house the touchscreen, ESP32-S3, sensors, radar module, buzzer, and internal wiring. The current enclosure is a prototype and would need further refinement for a more professional or manufacturable version.

### Light / OTBR Enclosure

<p align="center">
  <img src="images/Light_OTBR_Enclosure.png" alt="Light and OTBR Enclosure Design" width="750">
</p>

<p align="center">
  <em>Figure 19: Light device and Thread Border Router enclosure design</em>
</p>

A separate enclosure was designed for the light device and Thread Border Router hardware. This helped separate the main sensor panel from the smart home actuation and Thread testing hardware.

---

## Current Project Status

| Requirement | Status |
|---|---|
| Sensor monitoring | Working |
| Human presence detection | Working |
| Touchscreen UI | Working |
| MQTT telemetry to ThingsBoard | Working |
| ThingsBoard dashboards | Working |
| ThingsBoard RPC light control | Working |
| Email alerts through SMTP | Working |
| Matter-over-Thread light commissioning | Partially working |
| ESP32-S3 to ESP32-H2 communication | Future work |
| Final enclosure refinement | Future work |
| Fully modular firmware | Future work |

---

## Future Work

Planned improvements include:

- Reliability and quality of the current system/ finding issues
- Add UART communication between the ESP32-S3 and ESP32-H2
- Send Smart Panel sensor states to the Matter subsystem
- Improve the LVGL interface design
- Add Wi-Fi setup directly from the touchscreen
- Refactor the firmware into cleaner modules
- Improve the PCB layout and power design
- Refine the enclosure
- Explore deeper Alexa, Google Home, or ESP RainMaker integration
- Add more smart home features such as voice control or additional sensors

---
