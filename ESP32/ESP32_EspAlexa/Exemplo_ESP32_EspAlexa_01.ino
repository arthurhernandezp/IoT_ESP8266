#include <WiFi.h>
#include <Espalexa.h>

void funcaoSala(uint8_t brightness);
void funcaoQuartoRaquel(uint8_t brightness);
void funcaoSuite(uint8_t brightness);

const char* ssid = "**************";
const char* password =  "**************";

int ledPin21 = 21;  //GPIO21
int ledPin19 = 19;  //GPIO21

int freq = 5000;
int ledChannel19 = 0;
int ledChannel21 = 1;
int resolution = 8;

Espalexa espalexa;
EspalexaDevice* dispositivoSuite; 

void setup(){
    Serial.begin(115200);
 
    pinMode(ledPin19,OUTPUT);
    pinMode(ledPin21,OUTPUT);
    //analogWrite do ESP32 
    ledcSetup(ledChannel19,freq, resolution);
    ledcSetup(ledChannel21,freq, resolution);
      
    ledcAttachPin(ledPin19,ledChannel19);
    ledcAttachPin(ledPin21,ledChannel21);
   
    WiFi.begin(ssid,password);   
    while (WiFi.status() != WL_CONNECTED) {
          Serial.print(". ");
          delay(500);
    }  
    Serial.println("Conectado ");
    Serial.print("IP Address  ");
    Serial.println(WiFi.localIP());      
    
    espalexa.addDevice("Sala", funcaoSala); 
    espalexa.addDevice("Quarto Raquel", funcaoQuartoRaquel); 
    
    dispositivoSuite = new EspalexaDevice("Suite", funcaoSuite); 
    espalexa.addDevice(dispositivoSuite); 
    dispositivoSuite->setValue(0); 

    espalexa.begin();
  
}
 
void loop(){
   espalexa.loop();
   delay(1);
}


void funcaoSala(uint8_t brightness) {
  // Coloque o que quiser aqui
}

void funcaoQuartoRaquel(uint8_t brightness) {
  Serial.println("Quarto Raquel Estado: ");  
    if (brightness) {
      Serial.print("ON, brightness ");
      Serial.println(brightness);
    }
    else  {
      Serial.println("OFF");
    }
    ledcWrite(ledChannel21,brightness);
}

void funcaoSuite(uint8_t brightness) {
  ledcWrite(ledChannel19,brightness);
}
