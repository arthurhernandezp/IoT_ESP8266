#include <esp_now.h>
#include <WiFi.h>
 
uint8_t macSlaves[][6] = { {0x84, 0xF3, 0xEB, 0x58, 0x41, 0xD6}, {0x5C, 0xCF, 0x7F, 0x53, 0xB8, 0x97}};
 
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
  
    peerInfo.channel = 1; 
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

  esp_now_register_send_cb(OnDataSent);
}
  
 void send(){ 
   
  esp_err_t result;
  dados_struct dados;
  dados.temperatura = random(26,42);
  dados.umidade_solo = random(-4,100);
  char macStr[18];
  for(int i=0; i<slavesCount; i++){
 
    result = esp_now_send(macSlaves[i],(uint8_t *) &dados, sizeof(dados_struct));    
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", macSlaves[i][0], macSlaves[i][1], macSlaves[i][2], macSlaves[i][3], macSlaves[i][4], macSlaves[i][5]);
    Serial.print(macStr);
    if (result == ESP_OK) {      
      Serial.println(" Envio Sucesso - ");
    }
    else {
      Serial.println(" Envio Fracasso - ");
    }
  }
}
 void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  //Copiamos o Mac Address destino para uma string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  
  Serial.print("Enviado para: "); 
  Serial.print(macStr); 
  Serial.print(" - Status: "); 
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Recebeu!!!" : "Não recebeu!!!");
 
  
}
  void loop() {
  send();
  delay(10000);
 }
