#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <PubSubClient.h>   // from MQTT


// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11

const char* clientid ="ESP2805 RicardoDS";  //
const char* mqtt_server = "broker.hivemq.com";  //

WiFiClient espClient; //
PubSubClient client(espClient); //


/*Put your SSID & Password*/
const char* ssid = "INFINITUM38C2_2.4";  // Enter SSID here
const char* password = "3831720734";  //Enter Password here


WebServer server(80);

// DHT Sensor
uint8_t DHTPin = 4; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;


//===========================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
      if (client.connect(clientid)) {
      Serial.println("connected");
      Serial.print("Return code: ");
      Serial.println(client.state());
      }else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);}}}
    
//===========================
 
void setup() {
  Serial.begin(115200);
  delay(100);

  client.setServer(mqtt_server, 1883); //

  pinMode(DHTPin, INPUT);
  dht.begin();              

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();

  Serial.println("HTTP server started");

}
void loop() {
  
  server.handleClient();

//===============
if (!client.connected()) {
  reconnect( );}
client.loop();

//char msg[30];
//snprintf (msg, 50, "Air quailty good!", Temperature);

char msg[60];
snprintf(msg, sizeof(msg), "Temperature= %f Humedity %f", Temperature,Humidity );

//Serial.println(Temperature); // prueba
Serial.println(msg); // prueba

//client.publish( "CIATEQ/IOT/TestingESP32", msg, 1);
//client.publish( "CIATEQ/IOT/TestingESP32", msg);  funciona bien
client.publish( "RDS/HOUSE/ROOM1", msg);
delay(1000);  
//===============
  
}

void handle_OnConnect() {

 Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 
  server.send(200, "text/html", SendHTML(Temperature,Humidity)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP32 Weather Report</h1>\n";
  
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
