#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "INOVUERJ_LACIPI";
const char* password =  "feudodoinov";
//const char* mqtt_server = "192.168.1.65";      // Endereço do Mosquitto - Broker
const char* mqtt_server = "test.mosquitto.org";      // Endereço do Mosquitto - Broker
int rele = 23;
WiFiClient espClient;
PubSubClient client(espClient);

void setup(){
   pinMode(rele,OUTPUT);
   Serial.begin(115200);
   WiFi.begin(ssid,password);   
    while (WiFi.status() != WL_CONNECTED) {
          delay(500);
    }  
   client.setServer(mqtt_server, 1883);  // Conecta-se a porta 1883 do Mosquitto
   client.setCallback(callback);           // Indica o nome da função que vai tratar o retorno   
}
void reconnect() { 
   while (!client.connected()) {
     Serial.print("Conectando....."); 
         if (client.connect("ESP32 Assinando")) {     // Trocar pela sua identificação
          Serial.println("Conectado");      
          client.subscribe("temperatura_ambiente_01");        // Assinando
          client.subscribe("umidade_solo_01");        // Assinando          
   } else {
          Serial.print("failed, rc=");
          Serial.print(client.state());
          delay(5000);
  }
 }
}
void callback(char* topic, byte* payload, unsigned int length) {
  String valorTemperatura;
  String valorUmidade; 
  Serial.print("Messagem do topico :");
  Serial.print(topic);
  Serial.print(" - ");
  digitalWrite(rele,HIGH);
  delay(1000);
  digitalWrite(rele,LOW);
  delay(1000);
}

void loop(){
   if (!client.connected()) {
      reconnect();
   }
   client.loop();
}
