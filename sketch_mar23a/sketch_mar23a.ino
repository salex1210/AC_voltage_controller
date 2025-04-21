// Definir el pin PWM
const int pwmPin = 3;  // Pin 9 (PWM compatible en Arduino Uno)

void setup() {
  // Configurar el Timer1 para PWM de 10 bits
  TCCR1A = _BV(COM1A1) | _BV(WGM11) | _BV(WGM10);  // Modo Fast PWM, 10 bits
  TCCR1B = _BV(CS10);  // Sin preescalador (máxima frecuencia)
  pinMode(pwmPin, OUTPUT);
}

void loop() {
  // Generar una rampa LSV
  for (int dutyCycle = 0; dutyCycle < 1024; dutyCycle++) {
    OCR1A = dutyCycle;  // Establecer el ciclo de trabajo (10 bits)
    delay(5);  // Controlar la velocidad de barrido (5 ms por paso)
  }
}