// Pinos do joystick
const int pinX = A0;
const int pinY = A1;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Lê os valores analógicos dos eixos X e Y
  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);

  // Determina a direção com base nos valores lidos
  String direction = determineDirection(xValue, yValue);

  // Imprime a direção no monitor serial
  Serial.println(direction);

  delay(100);
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
