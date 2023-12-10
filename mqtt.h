#include <PubSubClient.h>

const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;

const char* mqttTopic = "yourname/potValue";
const char* subscribeTopic = "dusara1235"; //For Subscription

WiFiClient espClient;
PubSubClient client(espClient);

void mqtt_connect_sub(PubSubClient client){
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
}

// void loop() {
//   client.loop(); // For Subscription
//   // put your main code here, to run repeatedly:

//   //Converte the Integer Value to a String
//   char valueStr[5];

//   if (client.connected()) {
//     client.publish(mqttTopic, valueStr);
//     //Serial.println("Published to MQTT: " + String(valueStr));
//   }

//   delay(1000);
// }

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
