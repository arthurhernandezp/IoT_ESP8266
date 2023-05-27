#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ArduinoJson.h>

const char* ssid = "INOVUERJ_LACIPI";      // WiFi SSID
const char* password = "feudodoinov";     // WiFi password
const char* host = "www.dweet.io";

int contaFalhas=0;
StaticJsonDocument<1024> doc;

WiFiClient client;

void setup(){
  Serial.begin(115200);
  Serial.println();
  
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);    
  }  
}

void(* resetFunc) (void) = 0;

void loop(){


    if (client.connect(host, 80)) { 
        contaFalhas=0;    
        

     String caminho = "GET /get/latest/dweet/for/B4HUERJIoT";
        client.println(caminho); 
        client.println();
        Serial.println("[Response:]");
        while (client.connected()){
          if (client.available())      {
            String line = client.readStringUntil('\n');
            Serial.println(line);

            DeserializationError error = deserializeJson(doc,line);            
            if (error) {
              Serial.println("JSON parsing failed!");      
             }

             else{
                  String entraram = doc["with"][0];
                  Serial.println(entraram);   
                  error = deserializeJson(doc,entraram);

                   String content = doc["content"];                
                   Serial.println(content);  

                   error = deserializeJson(doc,content);

                   String temperatura = doc["temperatura"];                
                   Serial.println(temperatura); 
             }
          }
       } 
      client.stop();
      Serial.println("\n[Disconnected]");    
  }
  else {     
    client.stop();
    contaFalhas++;
    if (contaFalhas>5) resetFunc();
  }
  delay(30000);
}
