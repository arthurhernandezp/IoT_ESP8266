#include <esp_now.h>
#include <WiFi.h>

typedef struct dados_struct {
  float temperatura;
  float umidade_solo;
} dados_struct;

dados_struct dados;
int rele = 14;
void setup() {
  Serial.begin(115200);
   
  
  WiFi.mode(WIFI_STA);
  pinMode(rele,OUTPUT);
  Serial.print("Mac Address Slave:"); 
  Serial.println(WiFi.macAddress());
 
  if (esp_now_init() != 0) {
    Serial.println("Erro Iniciação do ESP-NOW!!!");
    ESP.restart();
  }
  else {
    Serial.println("ESP-NOW OK!!!");
    
  }
//esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
esp_now_register_recv_cb(OnDataRecv);
 
  
}

void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
 
  Serial.print("Tamanho da Mensagem : "); 
  Serial.println(len);
  Serial.println("");
  Serial.print("Mac do Master : "); 
  Serial.println(macStr);
  Serial.println("");
 memcpy(&dados, incomingData, sizeof(dados));
  Serial.print("Temperatura : ");  
  Serial.println(dados.temperatura);
  Serial.print("Umidade do solo : ");  
  Serial.println(dados.umidade_solo);
  if(dados.temperatura > 32){
    digitalWrite(rele,HIGH);
  }else{
    digitalWrite(rele,LOW);
  }
}

void loop() {
}
