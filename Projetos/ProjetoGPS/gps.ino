#include <ESP8266WiFi.h>        // Incluindo a Biblioteca
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

const char* ssid = "******************";
const char* password = "**************";

const unsigned int writeInterval = 60000; // write interval (in ms)

static const int RXPin = 12, TXPin = 14;
static const uint32_t GPSBaud = 9600;


const char* mqtt_server = "test.mosquitto.org";      // Endereço do Mosquitto - Broker
unsigned int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
TinyGPSPlus gps; 
SoftwareSerial ss(RXPin, TXPin); 

void setup() {
  Serial.begin(115200);
  Serial.println(ssid);
  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("->WiFi connected");
  Serial.println("->IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port); 

  ss.begin(GPSBaud);

}
void loop() {
  if (!client.connected()) 
    reconnect();
    client.loop();

    while (ss.available() > 0)
      if (gps.encode(ss.read())){
        displayInfo();
      }
      if (millis() > 5000 && gps.charsProcessed() < 10){
        Serial.println(F("No GPS detected: check wiring."));
        while(true);
      }
}
void displayInfo() {
  if (gps.location.isValid()) {
    double latitude = (gps.location.lat());
    double longitude = (gps.location.lng());  
    double altitud =(gps.altitude.meters());
    Serial.println("*Publicando no Mosquitto");
    char mqtt_payload[30] = "";
    snprintf (mqtt_payload, 30, "latitude=%lf", latitude);
    client.publish("Latitude_UERJ_IOT",mqtt_payload);
    snprintf (mqtt_payload, 30, "longitude = %lf",longitude);
    client.publish("Longitude_UERJ_IOT",mqtt_payload);
    snprintf (mqtt_payload, 30, "altitude = %lf",altitud);
    client.publish("Altitude_UERJ_IOT",mqtt_payload);

    Serial.println("Publicado!");   
    delay(writeInterval);// delay 
  } 
  else {
     Serial.println("Dados Inválidos!!!");
  }
}

void reconnect() {  
  while (!client.connected()) {
    Serial.print("********** Attempting MQTT connection...");
      if (client.connect("ProjetoGPS ESP8266 Publicando","","","problemaGps",1,true,"Fora do Ar")){
       Serial.println("Cliente Conectado ao Mosquitto");  
    }
    else{ 
        Serial.println(client.state());     
        delay(2000);
    }
  }
}
