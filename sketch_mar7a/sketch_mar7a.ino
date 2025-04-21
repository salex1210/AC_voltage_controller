// --- CONFIGURACIÓN DE PINES (Timer1) ---
const int pwmPin_LSV = 9;   // Salida PWM de 10 bits (Timer1, Canal A)
const int pwmPin_DPV = 10;  // Salida PWM de 10 bits (Timer1, Canal B)

// --- CONFIGURACIÓN PARA LSV ---
const int lsv_delay = 5;  // Tiempo entre pasos (ms)
int lsv_dutyCycle = 0;
unsigned long lastLSVUpdate = 0;

// --- CONFIGURACIÓN PARA DPV ---
const int V_start = 0;
const int V_end = 1023;   // Rango de 10 bits
const int step = 4;
const int pulse_amp = 40;  
const int t_step = 400;   
const int t_pulse = 200;  
const int sampling_delay = 10; 

int dpv_base_val = V_start;
unsigned long lastDPVUpdate = 0;
bool pulseActive = false;

void setup() {
    Serial.begin(9600);
    
    // Configurar Timer1 para PWM de 10 bits en los pines 9 y 10
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
    TCCR1B = _BV(CS11);  // Preescalador 8
    
    pinMode(pwmPin_LSV, OUTPUT);
    pinMode(pwmPin_DPV, OUTPUT);

    Serial.println("Sistema Inicializado");
}

// --- FUNCIÓN PARA CONTROLAR PWM DE 10 BITS ---
void set_pwm_LSV(int dutyCycle) {
    OCR1A = dutyCycle;  // Salida en pin 9 (Canal A de Timer1)
}

void set_pwm_DPV(int dutyCycle) {
    OCR1B = dutyCycle;  // Salida en pin 10 (Canal B de Timer1)
}

void loop() {
    unsigned long currentMillis = millis();

    // --- Control de LSV (onda en diente de sierra en pin 9) ---
    if (currentMillis - lastLSVUpdate >= lsv_delay) {
        lastLSVUpdate = currentMillis;

        lsv_dutyCycle++;
        if (lsv_dutyCycle >= 1023) {
            lsv_dutyCycle = 0;  // Reinicia LSV al llegar al máximo
            Serial.println("LSV REINICIADO");
        }
        
        set_pwm_LSV(lsv_dutyCycle);
    }

    // --- Control de DPV (escalones con pulso diferencial en pin 10) ---
    if (!pulseActive && (currentMillis - lastDPVUpdate >= t_step)) {
        lastDPVUpdate = currentMillis;
        set_pwm_DPV(dpv_base_val);
        Serial.print("DPV Base: "); Serial.println(dpv_base_val);

        // Activar pulso diferencial
        pulseActive = true;
    }

    if (pulseActive && (currentMillis - lastDPVUpdate >= sampling_delay)) {
        int pulse_val = dpv_base_val + pulse_amp;
        if (pulse_val > V_end) pulse_val = V_end;
        
        set_pwm_DPV(pulse_val);
        Serial.print("DPV Pulso: "); Serial.println(pulse_val);

        lastDPVUpdate = currentMillis;
        pulseActive = false;

        // Actualizar base_val para el siguiente escalón
        dpv_base_val += step;
        if (dpv_base_val > V_end) dpv_base_val = V_start;
    }
}
