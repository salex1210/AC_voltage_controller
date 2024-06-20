const int crucePorCeroPin = 2;  // Pin conectado al detector de cruce por cero
const int salidaPin = 3;         // Pin donde se enviará el pulso después del cruce por cero
volatile boolean cruce_cero = 0;
void setup() {
  pinMode(crucePorCeroPin, INPUT);  // Establece el pin de cruce por cero como entrada
  pinMode(salidaPin, OUTPUT);        // Establece el pin de salida como salida
  attachInterrupt(0,deteccion_cruce_cero,RISING);
}

void deteccion_cruce_cero(){
  cruce_cero = true;
  digitalWrite(salidaPin, LOW);
}

void loop() {
  // Espera hasta que se detecte un cruce por cero
  while(digitalRead(crucePorCeroPin) == LOW) {
    delayMicroseconds(10); // Pequeña pausa para evitar el uso excesivo de recursos
  }
  
  // Espera 2.33ms (2330 microsegundos) después del cruce por cero
  delayMicroseconds(2566);
  
  // Envía un pulso en el pin de salida
  digitalWrite(salidaPin, HIGH);
  delayMicroseconds(10);  // Duración del pulso (opcional, ajusta según sea necesario)
  
}

