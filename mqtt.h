void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  char payloadStr[length + 1];
  memcpy(payloadStr, payload, length);
  payloadStr[length] = '\0';

  Serial.print("Payload: ");
  Serial.println(payloadStr);
}

#define MQTTSERVER  "broker.hivemq.com"
#define MQTTPORT  1883


#define TOPIC1  "room1" 
#define TOPIC2  "room2"