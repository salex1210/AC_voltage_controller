// Definir el pin PWM correcto para Timer1
const int pwmPin = 9;  // Pin 9 (OCR1A en Arduino Uno)

void setup() {
  // Configurar el Timer1 para PWM de 10 bits
  TCCR1A = _BV(COM1A1) | _BV(WGM11) | _BV(WGM10);  // Fast PWM, 10 bits
  TCCR1B = _BV(CS10);  // Sin preescalador (frecuencia máxima)
  pinMode(pwmPin, OUTPUT);
}

void loop() {
  // Generar una rampa LSV
  for (int dutyCycle = 0; dutyCycle < 1024; dutyCycle++) {
    OCR1A = dutyCycle;  // Establecer ciclo de trabajo (10 bits)
    delay(5);  // Controlar velocidad de barrido
  }
}
