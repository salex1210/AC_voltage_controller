#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Redmi";
const char* password = "hola1234";
const char* mqtt_server = "broker.emqx.io"; // Broker público MQTT

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a la red WiFi con IP: ");
  Serial.println(WiFi.localIP());
}

// Genera un número aleatorio entre 18.00 y 50.00
float generarTemperatura() {
  return 18.0 + ((float)random(0, 3201) / 100.0); // 0 a 32.00 sumado a 18.00
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conectar al broker MQTT...");
    
    // Usa un Client ID único para evitar desconexiones con otros clientes
    String clientId = "ESP32Client_" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado.");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = generarTemperatura();
  char buffer[10];
  dtostrf(temp, 4, 2, buffer);

  Serial.print("Temperatura generada: ");
  Serial.println(buffer);

  client.publish("sensor/temperatura", buffer);
  Serial.println("Dato publicado en el topic 'sensor/temperatura'");

  delay(10000); // Espera 10 segundos
}
