const int sensorPin = A4; // Pin analógico donde está conectado el sensor
const int relayPin = 2;    // Pin digital donde está conectado el relé
const float thresholdVoltage = 4.0; // Umbral en volts

void setup() {
    pinMode(relayPin, OUTPUT); // Configuramos el pin del relé como salida
    digitalWrite(relayPin,LOW);
    Serial.begin(9600); // Iniciamos la comunicación serie para depuración
}

void loop() {
    float  sensorValue = analogRead(sensorPin); // Leemos la señal del sensor
    // Convertimos el valor analógico a voltaje
    float voltage = sensorValue * (5.0 / 1023.0); // Convertimos el valor a volts
    // Imprimimos el valor del voltaje para depuración
    Serial.print("Voltaje: ");
    Serial.println(voltage);
    // Verificamos si el voltaje supera el umbral
    if (voltage >= thresholdVoltage) {
        digitalWrite(relayPin, HIGH); // Activamos el relé
        Serial.println("Sensor activo");
    } else {
        digitalWrite(relayPin, LOW); // Desactivamos el relé
        Serial.println("Sensor inactivo");
    }
    delay(1000); // Esperamos 100 ms antes de la próxima lectura
}
