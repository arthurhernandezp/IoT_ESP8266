#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, 11, 12);  // Taxa de transmissão (bps), Pino Data, Pino Clock

// Pinos do joystick
const int pinX = A0;
const int pinY = A1;

void setup(void) {
  Serial.begin(115200);
  if (!driver.init()) {
    Serial.println("Erro na inicialização do driver!");
  } else {
    Serial.println("Pronto para transmitir!");
  }
}

void loop(void) {
  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);

  String direction = determineDirection(xValue, yValue);

  Serial.println(direction);

  sendDirection(direction);

  delay(1000); // Atraso aumentado para evitar transmissões excessivas
}

String determineDirection(int x, int y) {
  if (x < 300 && y > 700) return "Cima-Esquerda";
  if (x > 700 && y > 700) return "Cima-Direita";
  if (x < 300 && y < 300) return "Baixo-Esquerda";
  if (x > 700 && y < 300) return "Baixo-Direita";
  if (x < 300) return "Esquerda";
  if (x > 700) return "Direita";
  if (y < 300) return "Baixo";
  if (y > 700) return "Cima";

  return "Centro";
}

void sendDirection(const String &direction) {
  char msg[100];  // Tamanho máximo da mensagem

  direction.toCharArray(msg, sizeof(msg));  // Converte a String em um array de caracteres

  driver.send((uint8_t *)msg, strlen(msg));  // Envia mensagem pelo rádio
  driver.waitPacketSent();  // Aguarda a transmissão ser concluída
}
