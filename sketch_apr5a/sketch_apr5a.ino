const int pwmPin = 9;       // Salida PWM en pin 9 (OCR1A)
const int buttonPin = 2;    // Entrada del pulsador (usa GND para activar)

// LSV
const int lsv_delay = 5;
int lsv_dutyCycle = 0;
unsigned long lastLSVUpdate = 0;

// DPV
const int V_start = 0;
const int V_end = 1023;
const int step = 4;
const int pulse_amp = 40;
const int t_step = 400;
const int t_pulse = 200;
const int sampling_delay = 10;

int dpv_base_val = V_start;
unsigned long dpv_lastUpdate = 0;
bool dpv_pulseActive = false;
bool dpv_waitingNextStep = false;

void setup() {
  Serial.begin(9600);

  // Configurar PWM 10-bit en pin 9 (Timer1)
  TCCR1A = _BV(COM1A1) | _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS11);  // preescaler 8

  pinMode(pwmPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // pulsador a GND

  Serial.println("Sistema iniciado");
}

void loop() {
  unsigned long now = millis();
  bool pulsadorPresionado = digitalRead(buttonPin) == LOW;

  // Diagnóstico: imprimir estado del botón
  static bool lastState = !pulsadorPresionado;
  if (pulsadorPresionado != lastState) {
    lastState = pulsadorPresionado;
    Serial.print("Modo activo: ");
    Serial.println(pulsadorPresionado ? "LSV" : "DPV");
  }

  if (pulsadorPresionado) {
    // --- Ejecutar LSV ---
    if (now - lastLSVUpdate >= lsv_delay) {
      lastLSVUpdate = now;
      lsv_dutyCycle++;
      if (lsv_dutyCycle > 1023) {
        lsv_dutyCycle = 0;
        Serial.println("LSV reiniciado");
      }
      OCR1A = lsv_dutyCycle;
    }

    // Resetear DPV cuando no está activa
    dpv_base_val = V_start;
    dpv_pulseActive = false;
    dpv_waitingNextStep = false;

  } else {
    // --- Ejecutar DPV ---
    if (!dpv_pulseActive && !dpv_waitingNextStep && now - dpv_lastUpdate >= sampling_delay) {
      int pulse_val = dpv_base_val + pulse_amp;
      if (pulse_val > V_end) pulse_val = V_end;

      OCR1A = pulse_val;
      Serial.print("DPV Pulso: "); Serial.println(pulse_val);

      dpv_lastUpdate = now;
      dpv_pulseActive = true;
    }

    if (dpv_pulseActive && now - dpv_lastUpdate >= t_pulse) {
      OCR1A = dpv_base_val;
      Serial.print("DPV Base: "); Serial.println(dpv_base_val);

      dpv_lastUpdate = now;
      dpv_pulseActive = false;
      dpv_waitingNextStep = true;
    }

    if (dpv_waitingNextStep && now - dpv_lastUpdate >= (t_step - t_pulse)) {
      dpv_base_val += step;
      if (dpv_base_val > V_end) {
        dpv_base_val = V_start;
        Serial.println("DPV reiniciada");
      }
      dpv_lastUpdate = now;
      dpv_waitingNextStep = false;
    }
  }
}
