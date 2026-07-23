# Files for Thingsboard Light Device

# ThingsBoard ESP32-S3 RPC Light Device sketch

This project implements a remotely controlled light device using an
ESP32-S3 DevKitC and ThingsBoard.

The ESP32-S3 connects to ThingsBoard over Wi-Fi using MQTT. It can receive
server-side RPC commands, react to shared attribute updates and report its
current state and network information back to ThingsBoard.

The built-in LED on GPIO 38 is used to simulate a smart light.

This is done to demonstrate device-to-device automation
through the ThingsBoard Rule Engine.

A separate Smart Panel device detects human presence and publishes a
`presence` telemetry value to ThingsBoard. A ThingsBoard rule chain evaluates
this value and sends an RPC command to the ESP32-S3 light device.

When presence is detected, the light is turned on. When presence is no longer
detected, the light is turned off.

