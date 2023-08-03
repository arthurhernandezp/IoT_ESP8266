#include <RH_ASK.h>
#include <SPI.h>
const byte pin13=13;
RH_ASK driver;
void setup () {
  Serial.begin(115200);
  pinMode(pin13,OUTPUT);
  if (!driver.init()){
    Serial.println("Erro na carga do driver!");
  }
  else{
    Serial.println("Pronto para receber!");
  }
}

void loop () {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN]; // Armazena a mensagem recebida
  uint8_t buflen = sizeof(buf); // Armazena o tamanho da mensagem
  if (driver.recv(buf, &buflen)) { // Checa se chegou algo. Recepciona pelo pino 11(válido para o Arduino) 
    Serial.println((char *)buf);
    if ((char)buf[0]=='I' ){
      digitalWrite(pin13,HIGH);
      delay(10000);
    }
    digitalWrite(pin13,LOW);
  }
}
