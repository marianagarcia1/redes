#include <WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#define LED_PIN 2

WiFiUDP udp;
unsigned int localPort = 1322;
const char *ssid = "mauricio";
const char *password = "olnu7936";

int pwmRecebido = 0;
unsigned long inicioTempo = 0;
unsigned long finalTempo = 0;
Ticker sendTemperatureTicker;
float temperatura = 25.0; // Valor inicial da temperatura

void ValidatePWM(int pwm) {
  if (pwm < 255 && pwm > 0) {
    pwmRecebido = pwm;
    inicioTempo = millis();
  }
}

void SendTemperature() {
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  udp.write((uint8_t*)&temperatura, sizeof(temperatura));  // Envia o valor da temperatura diretamente como bytes
  udp.endPacket();
  Serial.printf("Temperatura enviada: %.2f\n", temperatura);
}

void setup() {
  Serial.begin(115200);
  // Conecta à rede WiFi
  WiFi.begin(ssid, password);

  pinMode(LED_PIN, OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  udp.begin(localPort);
  Serial.printf("Servidor UDP: %s:%i \n", WiFi.localIP().toString().c_str(), localPort);
  digitalWrite(LED_PIN, HIGH);

  // Configura o ticker para enviar a temperatura a cada 2.5 segundos
  sendTemperatureTicker.attach(2.5, SendTemperature);
}

void loop() {
  finalTempo = millis();

  if ((finalTempo - inicioTempo) > 1000) {
    // Setar o PWM para 0 e printar a mensagem
    pwmRecebido = 0;
    Serial.println("Tempo excedido.");
    inicioTempo = millis();
  }

  int packetSize = udp.parsePacket();
  if (packetSize) {
    int pwm;
    udp.read((char*)&pwm, sizeof(pwm));
    ValidatePWM(pwm);
    Serial.printf("PWM recebido: %d\n", pwmRecebido);
  }
}

