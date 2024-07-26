#include <WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>

Ticker controller, send_data;




// Configurações da rede WiFi
const char* ssid = "mauricio";
const char* password = "olnu7936";

// Configurações do UDP
WiFiUDP udp;
const char* remoteIP = "192.168.102.30";  // IP do ESP32 receptor
const int remotePort = 1322;            // Porta para enviar mensagens
const int localPort = 1322; 
            // Porta local para o transmissor (opcional)

int pwm = 0;

void SendDataUDP() {
  udp.beginPacket(remoteIP, remotePort);
  udp.write((uint8_t*)&pwm, sizeof(pwm));  // Envia o valor de pwm diretamente como bytes
  udp.endPacket();
  Serial.printf("Valor enviado: %d\n", pwm); // Adiciona uma mensagem de depuração

}

void ExecuteControl(){
  pwm+=1;

  //mandar via udp para o atuador o valor do pwm;
  SendDataUDP();

}

void setup() {
  Serial.begin(115200);
  // udp.beginPacket(remoteIP, remotePort);


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

  controller.attach(1, ExecuteControl);
  send_data.attach(0.001, SendDataUDP);
}

void loop() {


}
