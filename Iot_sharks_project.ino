#include <WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include "fauxmoESP.h"
#include <Arduino.h>

const char* ssid = "SLT-FIBER";
const char* password = "SgSgSg1956##";

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

const char* mqttTopic = "yourname/potValue";
const char* subscribeTopic = "yowfe/command"; //For Subscription

WiFiClient espClient;
PubSubClient client(espClient);
fauxmoESP fauxmo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(ssid, password);  //Initialize WiFi Object

  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address : ");
  Serial.print(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback); // For Subscription

  while (!client.connected()) {
    Serial.println("Connecting to MQTT..");
    if (client.connect("ESP8266ClientPasindu")) {
      Serial.print("Connected to MQTT");
      client.subscribe(subscribeTopic); // For Subscription
    } else {
      Serial.println("MQTT Failed to connect");
      delay(5000);
    }
  }
  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices
  fauxmo.enable(true);
  fauxmo.addDevice("ID_YELLOW");
  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        
         Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.

        if (strcmp(device_name, "ID_YELLOW")==0) {
           digitalWrite(5, state ? HIGH : LOW);
        }else if (strcmp(device_name, "ID_GREEN")==0) {
           digitalWrite(6, state ? HIGH : LOW);
        }

    });
}

void loop() {
  client.loop(); // For Subscription
  fauxmo.handle();
}

// For Subscription
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  char payloadStr[length + 1];
  memcpy(payloadStr, payload, length);
  payloadStr[length] = '\0';

  Serial.print("Payload: ");
  Serial.println(payloadStr);
}
