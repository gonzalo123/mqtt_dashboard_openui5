#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

const int photocellPin = 1;
const int tempPin = 0;
const int relayPin = 9;
bool lightStatus = false;

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// mqtt configuration
const char* mqttServer = "192.168.1.104";
const int mqttPort = 1883;
const String topicLightChange = "sensors/arduino/light/change";
const String topicLightStatus = "sensors/arduino/light/status";
const String topicTemp = "sensors/arduino/temperature/room1";
const String topicLight = "sensors/arduino/light/room1";
const char* clientName = "com.gonzalo123.arduino";

EthernetClient ethClient;
PubSubClient client(ethClient);

void mqttReConnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientName)) {
      Serial.println("connected");
      client.subscribe(topicLightChange.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttEmit(String topic, String value) {
  if (client.publish((char*) topic.c_str(), (char*) value.c_str())) {
    //Serial.print("Publish ok (topic: ");
    //Serial.print(topic);
    //Serial.print(", value: ");
    //Serial.print(value);
    //Serial.println(")");
  } else {
    Serial.println("Publish failed");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] payload: ");
  String data;
  for (int i = 0; i < length; i++) {
    data += (char)payload[i];
  }

  if (strcmp(topic, topicLightChange.c_str()) == 0) {
    lightStatus = (data == "1") ? true : false;
    Serial.print(data);
  }

  Serial.println("");
}

void setup()
{
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }

  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    mqttReConnect();
  }

  client.loop();

  if (lightStatus == 1) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
  mqttEmit(topicLightStatus, lightStatus ? "1" : "0");
  mqttEmit(topicLight, (String) analogRead(photocellPin));
  mqttEmit(topicTemp, (String) ((5.0 * analogRead(tempPin) * 100.0) / 1024.0));

  delay(500);
}