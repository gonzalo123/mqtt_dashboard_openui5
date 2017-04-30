#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const int potentiometerPin = 0;

// Wifi configuration
const char* ssid = "my_wifi_ssid";
const char* password = "my_wifi_password";

// mqtt configuration
const char* server = "192.168.1.104";
const char* topic = "sensors/nodemcu/potentiometer/room1";
const char* clientName = "com.gonzalo123.nodemcu";

int value;
int percent;
String payload;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void wifiConnect() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (client.connect(clientName)) {
    Serial.print("Connected to MQTT broker at ");
    Serial.print(server);
    Serial.print(" as ");
    Serial.println(clientName);
    Serial.print("Topic is: ");
    Serial.println(topic);
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}

void mqttReConnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientName)) {
      Serial.println("connected");
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttEmit(String topic, String value)
{
  if (client.publish((char*) topic.c_str(), (char*) value.c_str())) {
    Serial.print("Publish ok (topic: ");
    Serial.print(topic);
    Serial.print(", value: ");
    Serial.print(value);
    Serial.println(")");
  } else {
    Serial.println("Publish failed");
  }
}

void setup() {
  Serial.begin(9600);
  client.setServer(server, 1883);
  wifiConnect();
  delay(10);
}

void loop() {
  if (!client.connected()) {
    mqttReConnect();
  }

  mqttEmit(topic, (String) (int) ((analogRead(potentiometerPin) * 100) / 1010));

  delay(200);
}