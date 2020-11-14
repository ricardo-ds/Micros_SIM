#include <WiFi.h>
#include <PubSubClient.h>


const char* ssid     = "INFINITUM38C2_2.4"; // Your ssid
const char* password = "3831720734"; // Your Password 


const char* clientid ="ESP280591";
const int ledPin = 2;

//const char* mqtt_server = "192.168.100.156";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);Serial.print(".");}
  Serial.println("");
  Serial.println("WiFi connected");}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0] == '1') 
   {digitalWrite(ledPin, HIGH);   } 
    else 
   { digitalWrite(ledPin, LOW); }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientid)) {
      Serial.println("connected");
 //     client.subscribe("CIATEQ/IOT/LED");
      client.subscribe("CIATEQ/IOT/LED/RICARDO");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);}}}

void setup() {
  pinMode(ledPin, OUTPUT);     
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
  reconnect();}
  client.loop();
  delay(1000);
}
