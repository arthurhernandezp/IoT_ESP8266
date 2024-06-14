#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <AsyncElegantOTA.h>

const char* ssid = "*************";
const char* password = "*************";

const int ledPin12 = 13;
const int ledPin13 = 19;
const int ledPin14 = 23;

String ledState12,ledState13,ledState14;

AsyncWebServer server(80);

String processor(const String& var){
  
  if(var == "LOCAL1"){
    if(digitalRead(ledPin12)){
      ledState12 = "LIGADO";
    }
    else{
      ledState12 = "DESLIGADO";
    }    
    return ledState12;
  }
  if(var == "LOCAL2"){
    if(digitalRead(ledPin13)){
      ledState13 = "LIGADO";
    }
    else{
      ledState13 = "DESLIGADO";
    }   
    return ledState13;
  }
  if(var == "LOCAL3"){
    if(digitalRead(ledPin14)){
      ledState14 = "LIGADO";
    }
    else{
      ledState14 = "DESLIGADO";
    }    
    return ledState14;
  }
  return String();
}

 
void setup(){
  
  Serial.begin(115200);
  pinMode(ledPin12, OUTPUT);
  pinMode(ledPin13, OUTPUT);
  pinMode(ledPin14, OUTPUT);

  
  if(!SPIFFS.begin(true)){
    Serial.println("Erro no SPIFFS!!!");
    return;
  }

  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi..");
  }

  
  Serial.println(WiFi.localIP());

 // Pagina index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Arquivo css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Tratamento GPIO12 - Ligar
  server.on("/on1", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin12, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Tratamento GPIO12 - Desligar
  server.on("/off1", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin12, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

   // Tratamento GPIO13 - Ligar
  server.on("/on2", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin13, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Tratamento GPIO13 - Desligar
  server.on("/off2", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin13, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Tratamento GPIO14 - Ligar
  server.on("/on3", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin14, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Tratamento GPIO14 - Desligar
  server.on("/off3", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin14, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("OTA OK!!!");
}
 
void loop(){
  
}
