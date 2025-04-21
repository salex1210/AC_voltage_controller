// Pines PWM en Arduino Uno (SOLO pines compatibles con PWM)
const int pwmPinLSV = 9;   // Canal LSV (conectado a INA del LTC2644)
const int pwmPinSWV = 8;  // Canal SWV (conectado a INB del LTC2644)

void setup() {
  pinMode(pwmPinLSV, OUTPUT);
  pinMode(pwmPinSWV, OUTPUT);
}

void loop() {
  // Barrido lineal (LSV)
  for (int duty = 0; duty <= 255; duty++) {
    analogWrite(pwmPinLSV, duty);  // PWM de 8 bits (0 a 255)
    delay(10);                     // Ajusta la velocidad del barrido
  }
  for (int duty = 255; duty >= 0; duty--) {
    analogWrite(pwmPinLSV, duty);
    delay(10);
  }
  
  // Onda cuadrada (SWV)
  for (int i = 0; i < 10; i++) {
    analogWrite(pwmPinSWV, 255);  // Nivel alto (aproximadamente VCC)
    delay(100);                   // Mantiene el nivel alto por 100ms
    analogWrite(pwmPinSWV, 0);    // Nivel bajo (0V)
    delay(100);                   // Mantiene el nivel bajo por 100ms
  }
}
