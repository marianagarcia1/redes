#include <WiFi.h>
#include <WiFiUdp.h>

// Configurações da rede WiFi
const char* ssid = "mauricio";
const char* password = "olnu7936";

// Configurações do UDP
WiFiUDP udp;
const char* remoteIP = "192.168.102.30";  // IP do ESP32 receptor
const int remotePort = 1322;            // Porta para enviar mensagens
const int localPort = 1322;             // Porta local para o transmissor (opcional)

void setup() {
  Serial.begin(115200);

  // Conecta à rede WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à WiFi...");
  }
  Serial.println("Conectado à WiFi");

  // Inicia o UDP
  udp.begin(localPort);
  Serial.printf("UDP iniciado na porta %d\n", localPort);
}

void loop() {
  // Mensagem a ser enviada
  const char *message = "Olá, ESP32 receptor!";

  // Envia a mensagem
  udp.beginPacket(remoteIP, remotePort);
  udp.write((uint8_t*)message, strlen(message));
  udp.endPacket();

  Serial.printf("Mensagem enviada para %s:%d\n", remoteIP, remotePort);

  delay(2000); // Aguarda 2 segundos antes de enviar a próxima mensagem
}
