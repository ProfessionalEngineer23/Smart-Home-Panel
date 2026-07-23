// Thingsboad Light device using RPC through MQTT and Thingsboard 
// Hardware:
// Esp32S3 Devkit C
// Used to simulate a light device utilizing the LED on GPIO 38
// Has rpc handler functions for controlling led state and mode, allows to set led to blinking mode/ turn on/off.
// Esp32S3 subscribes to Thingsboard via mqtt. Thingsboard controls if LED turns on/off based on human presence detection value 
// Based on ThingsboardClientSDK example found here:
// https://github.com/thingsboard/thingsboard-client-sdk/blob/master/examples/0010-esp8266_esp32_rpc/0010-esp8266_esp32_rpc.ino?utm_source=chatgpt.com

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#define THINGSBOARD_ENABLE_PROGMEM 0
#elif defined(ESP32) || defined(RASPBERRYPI_PICO) || defined(RASPBERRYPI_PICO_W)
#include <WiFi.h>
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 38
#endif

#include <Arduino_MQTT_Client.h>
#include <Server_Side_RPC.h>
#include <Attribute_Request.h>
#include <Shared_Attribute_Update.h>
#include <ThingsBoard.h>

constexpr char WIFI_SSID[] = ""; // 2.4Ghz wifi ssid
constexpr char WIFI_PASSWORD[] = ""; // Passwd

constexpr char TOKEN[] = ""; // Thinsboard Device token
constexpr char THINGSBOARD_SERVER[] = ""; // Ip address of server
constexpr uint16_t THINGSBOARD_PORT = 1883U;

constexpr uint32_t MAX_MESSAGE_SIZE = 1024U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;
constexpr size_t MAX_ATTRIBUTES = 3U;
constexpr uint64_t REQUEST_TIMEOUT_MICROSECONDS = 5000U * 1000U;

constexpr const char BLINKING_INTERVAL_ATTR[] = "blinkingInterval";
constexpr const char LED_MODE_ATTR[] = "ledMode";
constexpr const char LED_STATE_ATTR[] = "ledState";

constexpr uint16_t BLINKING_INTERVAL_MS_MIN = 10U;
constexpr uint16_t BLINKING_INTERVAL_MS_MAX = 60000U;

constexpr int16_t telemetrySendInterval = 2000U;

WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);

// Increased sizes a little because we now support more RPC handling.
Server_Side_RPC<10U, 10U> rpc;
Attribute_Request<2U, MAX_ATTRIBUTES> attr_request;
Shared_Attribute_Update<3U, MAX_ATTRIBUTES> shared_update;

const std::array<IAPI_Implementation*, 3U> apis = {
  &rpc,
  &attr_request,
  &shared_update
};

ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE, Default_Max_Stack_Size, apis);

// State
volatile bool attributesChanged = false;
volatile int ledMode = 0;                // 0 = steady, 1 = blinking
volatile bool ledState = false;
volatile uint16_t blinkingInterval = 1000U;

uint32_t previousStateChange = 0U;
uint32_t previousDataSend = 0U;

// Shared attributes that this device listens to
constexpr std::array<const char*, 2U> SHARED_ATTRIBUTES_LIST = {
  LED_STATE_ATTR,
  BLINKING_INTERVAL_ATTR
};

// Client attributes requested on boot
constexpr std::array<const char*, 1U> CLIENT_ATTRIBUTES_LIST = {
  LED_MODE_ATTR
};

void applyLedState() {
  if (LED_BUILTIN != 99) {
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
  }
}

void InitWiFi() {
  Serial.println("Connecting to AP...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to AP");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

const bool reconnect() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }

  InitWiFi();
  return true;
}

/// RPC: setLedMode
/// Supports either:
///   params = 0 / 1
/// or
///   params = { "ledMode": 0 / 1 }
void processSetLedMode(const JsonVariantConst &data, JsonDocument &response) {
  Serial.println("Received RPC method: setLedMode");

  int new_mode = -1;

  if (data.is<int>()) {
    new_mode = data.as<int>();
  } else if (data.is<JsonObjectConst>()) {
    JsonObjectConst obj = data.as<JsonObjectConst>();
    if (obj[LED_MODE_ATTR].is<int>()) {
      new_mode = obj[LED_MODE_ATTR].as<int>();
    }
  }

  StaticJsonDocument<64> response_doc;

  if (new_mode != 0 && new_mode != 1) {
    response_doc["error"] = "Unknown mode";
    response.set(response_doc);
    return;
  }

  ledMode = new_mode;
  attributesChanged = true;

  Serial.print("LED mode changed to: ");
  Serial.println(ledMode);

  response_doc["newMode"] = ledMode;
  response.set(response_doc);
}

/// RPC: setLedState
/// Supports either:
///   params = true / false
/// or
///   params = { "ledState": true / false }
void processSetLedState(const JsonVariantConst &data, JsonDocument &response) {
  Serial.println("Received RPC method: setLedState");

  bool new_state = ledState;
  bool valid = false;

  if (data.is<bool>()) {
    new_state = data.as<bool>();
    valid = true;
  } else if (data.is<JsonObjectConst>()) {
    JsonObjectConst obj = data.as<JsonObjectConst>();
    if (obj[LED_STATE_ATTR].is<bool>()) {
      new_state = obj[LED_STATE_ATTR].as<bool>();
      valid = true;
    }
  }

  StaticJsonDocument<64> response_doc;

  if (!valid) {
    response_doc["error"] = "Invalid or missing ledState";
    response.set(response_doc);
    return;
  }

  ledState = new_state;
  applyLedState();
  attributesChanged = true;

  Serial.print("LED state changed to: ");
  Serial.println(ledState ? "true" : "false");

  response_doc["ledState"] = ledState;
  response.set(response_doc);
}

const std::array<RPC_Callback, 2U> callbacks = {
  RPC_Callback{ "setLedMode", processSetLedMode },
  RPC_Callback{ "setLedState", processSetLedState }
};

void processSharedAttributes(const JsonObjectConst &data) {
  for (auto it = data.begin(); it != data.end(); ++it) {
    const char* key = it->key().c_str();

    if (strcmp(key, BLINKING_INTERVAL_ATTR) == 0) {
      const uint16_t new_interval = it->value().as<uint16_t>();

      if (new_interval >= BLINKING_INTERVAL_MS_MIN &&
          new_interval <= BLINKING_INTERVAL_MS_MAX) {
        blinkingInterval = new_interval;
        Serial.print("Blinking interval set to: ");
        Serial.println(blinkingInterval);
      }
    } else if (strcmp(key, LED_STATE_ATTR) == 0) {
      ledState = it->value().as<bool>();
      applyLedState();

      Serial.print("LED state set from shared attribute to: ");
      Serial.println(ledState ? "true" : "false");
    }
  }

  attributesChanged = true;
}

void processClientAttributes(const JsonObjectConst &data) {
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (strcmp(it->key().c_str(), LED_MODE_ATTR) == 0) {
      const int new_mode = it->value().as<int>();
      if (new_mode == 0 || new_mode == 1) {
        ledMode = new_mode;
        Serial.print("LED mode set from client attribute to: ");
        Serial.println(ledMode);
      }
    }
  }
}

void requestTimedOut() {
  Serial.printf(
    "Attribute request timed out after %llu microseconds. "
    "Ensure the device is connected and the keys exist.\n",
    REQUEST_TIMEOUT_MICROSECONDS
  );
}

const Shared_Attribute_Callback<MAX_ATTRIBUTES> attributes_callback(
  &processSharedAttributes,
  SHARED_ATTRIBUTES_LIST.cbegin(),
  SHARED_ATTRIBUTES_LIST.cend()
);

const Attribute_Request_Callback<MAX_ATTRIBUTES> attribute_shared_request_callback(
  &processSharedAttributes,
  REQUEST_TIMEOUT_MICROSECONDS,
  &requestTimedOut,
  SHARED_ATTRIBUTES_LIST
);

const Attribute_Request_Callback<MAX_ATTRIBUTES> attribute_client_request_callback(
  &processClientAttributes,
  REQUEST_TIMEOUT_MICROSECONDS,
  &requestTimedOut,
  CLIENT_ATTRIBUTES_LIST
);

void connectThingsBoard() {
  Serial.print("Connecting to ThingsBoard: ");
  Serial.print(THINGSBOARD_SERVER);
  //Serial.print(" with token ");
  //Serial.println(TOKEN);

  if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
    Serial.println("Failed to connect to ThingsBoard");
    return;
  }

  Serial.println("Connected to ThingsBoard");

  tb.sendAttributeData("macAddress", WiFi.macAddress().c_str());

  Serial.println("Subscribing for RPC...");
  if (!rpc.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
    Serial.println("Failed to subscribe for RPC");
    return;
  }

  Serial.println("Subscribing for shared attribute updates...");
  if (!shared_update.Shared_Attributes_Subscribe(attributes_callback)) {
    Serial.println("Failed to subscribe for shared attribute updates");
    return;
  }

  Serial.println("Requesting current shared attributes...");
  if (!attr_request.Shared_Attributes_Request(attribute_shared_request_callback)) {
    Serial.println("Failed to request shared attributes");
    return;
  }

  Serial.println("Requesting current client attributes...");
  if (!attr_request.Client_Attributes_Request(attribute_client_request_callback)) {
    Serial.println("Failed to request client attributes");
    return;
  }

  Serial.println("ThingsBoard subscriptions ready");
}

void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);

  if (LED_BUILTIN != 99) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
  }

  delay(1000);
  InitWiFi();
}

void loop() {
  delay(10);

  if (!reconnect()) {
    return;
  }

  if (!tb.connected()) {
    connectThingsBoard();
  }

  if (attributesChanged) {
    attributesChanged = false;

    if (ledMode == 0) {
      previousStateChange = millis();
    }

    tb.sendTelemetryData(LED_MODE_ATTR, ledMode);
    tb.sendTelemetryData(LED_STATE_ATTR, ledState);

    tb.sendAttributeData(LED_MODE_ATTR, ledMode);
    tb.sendAttributeData(LED_STATE_ATTR, ledState);
  }

  if (ledMode == 1 && millis() - previousStateChange > blinkingInterval) {
    previousStateChange = millis();
    ledState = !ledState;
    applyLedState();

    tb.sendTelemetryData(LED_STATE_ATTR, ledState);
    tb.sendAttributeData(LED_STATE_ATTR, ledState);

    Serial.print("Blink toggle LED state to: ");
    Serial.println(ledState ? "true" : "false");
  }

  if (millis() - previousDataSend > telemetrySendInterval) {
    previousDataSend = millis();

    tb.sendTelemetryData("temperature", random(10, 20));
    tb.sendAttributeData("rssi", WiFi.RSSI());
    tb.sendAttributeData("channel", WiFi.channel());
    tb.sendAttributeData("bssid", WiFi.BSSIDstr().c_str());
    tb.sendAttributeData("localIp", WiFi.localIP().toString().c_str());
    tb.sendAttributeData("ssid", WiFi.SSID().c_str());
  }

  tb.loop();
}
