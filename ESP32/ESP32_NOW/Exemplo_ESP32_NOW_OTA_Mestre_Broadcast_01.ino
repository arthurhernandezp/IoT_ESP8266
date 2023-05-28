#include <esp_now.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

uint8_t macSlaves[][6] = { {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

const char* ssid = "sssssssssssssssss";
const char* password =  "ppppppppppppppppp";

AsyncWebServer server(80);

typedef struct dados_struct {
  float temperatura;
  float umidade_solo;
} dados_struct;

esp_now_peer_info_t peerInfo;

int slavesCount;
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro Iniciação do ESP-NOW!!!");
    ESP.restart();
  }
  else {
    Serial.println("ESP-NOW OK!!!");
    
  }
  slavesCount = sizeof(macSlaves)/6/sizeof(uint8_t);

 
   
  for(int i=0; i<slavesCount; i++){
  
    peerInfo.channel = 0; 
    peerInfo.encrypt = false;  
    memcpy(peerInfo.peer_addr, macSlaves[i], sizeof(macSlaves[i]));
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Erro na montagem do peer!!!");
      return;
    }
    else{
      Serial.println("Montagem OK!!!");

    }  
 
  }

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
 //   request->send(200, "text/plain", "Hi! I am ESP32.");
 // });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("OTA OK");

  
}
  
 void send(){ 
   
  esp_err_t result;
  dados_struct dados;
  dados.temperatura = random(26,42);
  dados.umidade_solo = random(-4,100);
  
  for(int i=0; i<slavesCount; i++){ 
    result = esp_now_send(macSlaves[i],(uint8_t *) &dados, sizeof(dados_struct));  
    if (result == ESP_OK) {      
      Serial.println("Envio Sucesso - ");
    }
    else {
      Serial.println("Envio Fracasso - ");
    }
  }
}
 
  void loop() {
  send();
  delay(20000);
 }
