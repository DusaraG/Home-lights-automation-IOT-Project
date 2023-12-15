#include <WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include "fauxmoESP.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include "Mqtt.h"
#include "Led.h"
#include "Wifi_credentials.h"

void Wifi_init();
CRGB room1[NUM_LEDS];

WiFiClient espClient;
PubSubClient client(espClient);
fauxmoESP fauxmo;


void setup() {

  Serial.begin(115200);
  Wifi_init();

  client.setServer(MQTTSERVER, MQTTPORT);
  client.setCallback(callback); // For Subscription

  while (!client.connected()) {
    Serial.println("Connecting to MQTT..");
    if (client.connect("ESP32SAHRKs")) {
      Serial.print("Connected to MQTT");
      client.subscribe(TOPIC1); // For Subscription
      client.subscribe(TOPIC2);
    } else {
      Serial.println("MQTT Failed to connect");
      delay(5000);
    }
  }
  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices
  fauxmo.enable(true);
  fauxmo.addDevice("room1");
  fauxmo.addDevice("room2");
  fauxmo.addDevice("room3");
  fauxmo.addDevice("room4");
  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        //client.publish();
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        
         Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.

        if (strcmp(device_name, "room1")==0) {
           digitalWrite(5, state ? HIGH : LOW);
        }else if (strcmp(device_name, "room2")==0) {
           digitalWrite(6, state ? HIGH : LOW);
        }

    });
}

void loop() {
  client.loop(); // For Subscription
  fauxmo.handle();
}

// For Subscription

