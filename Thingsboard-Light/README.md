# Files for Thingsboard Light Device

# ThingsBoard ESP32-S3 RPC Light Device sketch
/*
 * ThingsBoard ESP32-S3 Light Device using MQTT, RPC and Attributes
 *
 * Hardware:
 * - ESP32-S3 DevKitC
 * - Built-in LED on GPIO 38
 *
 * Purpose:
 * This sketch simulates a remotely controlled smart light. The ESP32-S3
 * connects to Wi-Fi and communicates with a ThingsBoard server using MQTT.
 *
 * The device can be controlled in two ways:
 *
 * 1. Server-side RPC
 *    ThingsBoard sends an immediate command to the ESP32-S3.
 *
 *    Supported RPC methods:
 *    - setLedState
 *      Turns the LED on or off.
 *
 *      Accepted parameters:
 *        true
 *        false
 *        { "ledState": true }
 *        { "ledState": false }
 *
 *    - setLedMode
 *      Selects steady or blinking operation.
 *
 *      Accepted parameters:
 *        0
 *        1
 *        { "ledMode": 0 }
 *        { "ledMode": 1 }
 *
 *      Modes:
 *        0 = steady LED state
 *        1 = blinking LED
 *
 * 2. ThingsBoard attributes
 *    The device subscribes to shared attribute updates from ThingsBoard.
 *
 *    Shared attributes:
 *    - ledState
 *      Controls whether the LED is on or off.
 *
 *    - blinkingInterval
 *      Controls the blinking period in milliseconds.
 *
 *    Client attribute:
 *    - ledMode
 *      Stores the current operating mode.
 *
 * The device also reports its current LED state, operating mode, Wi-Fi
 * information and simulated temperature data to ThingsBoard.
 *
 * Example project use:
 * A Smart Panel sends human-presence telemetry to ThingsBoard. A ThingsBoard
 * rule chain evaluates the presence value and sends a setLedState RPC command
 * to this ESP32-S3 light device. Presence turns the light on, while no
 * presence turns it off.
 *
 * Communication path:
 * Smart Panel -> ThingsBoard telemetry -> Rule Chain -> RPC request
 * -> ESP32-S3 Light -> LED output
 *
 * Based on the ThingsBoard ESP32 light control example:
 * Add the relevant ThingsBoard documentation URL here.
 */
