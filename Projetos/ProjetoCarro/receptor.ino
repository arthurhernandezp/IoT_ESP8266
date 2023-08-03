#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver;

// Pinos de controle do SN754410
const int motorAForward = 2;  // Sentido horário
const int motorABackward = 3; // Sentido anti-horário
const int motorBForward = 4;  // Sentido horário
const int motorBBackward = 5; // Sentido anti-horário

void setup() {
  Serial.begin(115200);

  // Configura os pinos como saídas para controle dos motores
  pinMode(motorAForward, OUTPUT);
  pinMode(motorABackward, OUTPUT);
  pinMode(motorBForward, OUTPUT);
  pinMode(motorBBackward, OUTPUT);

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
    
    // Controla os motores baseado nas direções recebidas
    if (strcmp((char *)buf, "Cima") == 0) {
      moverFrente();
    }
    else if (strcmp((char *)buf, "Baixo") == 0) {
      moverTras();
    }
    else if (strcmp((char *)buf, "Esquerda") == 0) {
      virarEsquerda();
    }
    else if (strcmp((char *)buf, "Direita") == 0) {
      virarDireita();
    }
    // Mais direções podem ser adicionadas conforme necessário
    else {
      pararMotores();
    }
  }
}

void moverFrente() {
  digitalWrite(motorAForward, HIGH);
  digitalWrite(motorABackward, LOW);
  digitalWrite(motorBForward, HIGH);
  digitalWrite(motorBBackward, LOW);
}

void moverTras() {
  digitalWrite(motorAForward, LOW);
  digitalWrite(motorABackward, HIGH);
  digitalWrite(motorBForward, LOW);
  digitalWrite(motorBBackward, HIGH);
}

void virarEsquerda() {
  digitalWrite(motorAForward, LOW);
  digitalWrite(motorABackward, HIGH);
  digitalWrite(motorBForward, HIGH);
  digitalWrite(motorBBackward, LOW);
}

void virarDireita() {
  digitalWrite(motorAForward, HIGH);
  digitalWrite(motorABackward, LOW);
  digitalWrite(motorBForward, LOW);
  digitalWrite(motorBBackward, HIGH);
}

void pararMotores() {
  digitalWrite(motorAForward, LOW);
  digitalWrite(motorABackward, LOW);
  digitalWrite(motorBForward, LOW);
  digitalWrite(motorBBackward, LOW);
}
