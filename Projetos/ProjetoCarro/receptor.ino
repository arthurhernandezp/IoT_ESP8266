#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver;

void setup() {
  Serial.begin(115200);

  if (!driver.init()) {
    Serial.println("Erro na inicialização do driver!");
  } else {
    Serial.println("Pronto para receber!");
  }
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN]; // Armazena a mensagem recebida
  uint8_t buflen = sizeof(buf); // Armazena o tamanho da mensagem

  if (driver.recv(buf, &buflen)) { // Checa se chegou algo. Recepciona pelo pino 11 (válido para o Arduino)
    buf[buflen] = '\0'; // Adiciona um terminador de string ao final

    Serial.println((char *)buf);
  }
}
