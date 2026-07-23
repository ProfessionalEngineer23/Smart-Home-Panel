# Files for ThingsBoard Light Device

## ThingsBoard ESP32-S3 RPC Light Device Sketch

This sketch implements an ESP32-S3 light device that communicates with ThingsBoard using MQTT, server-side RPC, and ThingsBoard attributes.

## Hardware

- ESP32-S3 DevKitC
- Built-in LED on GPIO 38

## Purpose

The sketch simulates a remotely controlled smart light.

The ESP32-S3 connects to Wi-Fi and communicates with a ThingsBoard server over MQTT. The built-in LED represents the light output.

The device can be controlled through server-side RPC commands or ThingsBoard shared attributes.

## Device Control

### 1. Server-Side RPC

ThingsBoard can send immediate commands to the ESP32-S3 using server-side RPC.

RPC stands for **Remote Procedure Call**. It allows ThingsBoard to request that the ESP32-S3 executes a specific function.

### Supported RPC Methods

#### `setLedState`

Turns the LED on or off.

Accepted parameters:

```json
true
