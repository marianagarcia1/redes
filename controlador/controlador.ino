#include <WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>

#define SETPOINT_PIN 36
Ticker send_data;

// Configurações da rede WiFi
// unsigned int localPort = 1322;
const char *ssid = "Computador_do_Nied";
const char *password = "fuscaverde";

// Configurações do UDP
WiFiUDP udp;
const char* remoteIP = "192.168.137.250";  // IP do ESP32 receptor
const int remotePort = 1322;              // Porta para enviar mensagens
const int localPort = 1322;     

// double PID;
// double kp = 0.19236, ki = 0.008971, kd = 0;
// double P, I, D;
// double erro = 0;
float temperatura = 0;
// double temperatura_anterior;
// long ultimo_processo;
// int diferenca_tempo;

// float set_point = 30.0;
// int pwm = 0;
// float temp = 30.0;
unsigned long lastTempReceived = 0;       // Último tempo em que a temperatura foi recebida
unsigned long timeoutInterval = 1000;     // Intervalo de timeout em milissegundos
// int setpoint = 0;

unsigned long tempoInicioBmais = 0;
unsigned long tempoInicioBmenos = 0;
unsigned long tempoBotaoMais = 0;
unsigned long tempoBotaoMenos = 0;
bool estadoBotaoMais;
bool estadoBotaoMenos;
bool estadoBotaoMaisAnt;
bool estadoBotaoMenosAnt;

// double temperatura = 25.0;
double temperatura_anterior;
long ultimo_processo;
int diferenca_tempo;
double set_point = 70;
double erro = 0;
double PID;
int pwm; //0 a 255
int pwm_porcent;
double P, I, D;

double
  kp = 0.19236,
  ki = 0.008971,
  kd = 0;
//double
  //kp = 0.00047702,
  //ki = kp * 1.5e2,
  //kd = 0;
void SendDataUDP() {
  udp.beginPacket(remoteIP, remotePort);
  udp.write((uint8_t*)&pwm, sizeof(pwm));  // Envia o valor de pwm diretamente como bytes
  udp.endPacket();
  Serial.printf("PWM enviado: %d\n", pwm); // Adiciona uma mensagem de depuração
}

void setup() {
  Serial.begin(115200);

  // // Conecta à rede WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à WiFi...");
  }
  Serial.println("Conectado à WiFi");

  // Inicia o UDP
  udp.begin(localPort);
  Serial.printf("UDP iniciado na porta %d\n", localPort);
  // pinMode(SETPOINT_PIN, INPUT);
  send_data.attach(0.1, SendDataUDP);
  // controller.attach(1, ExecuteControl);  // ExecuteControl será chamado a cada 1 segundo
}

void loop() {
  unsigned long currentMillis = millis();
  set_point = (float(analogRead(SETPOINT_PIN)) * 3.3 / (4095.0));
  set_point = map(set_point * 10, 0, 33, 15, 80);
  // int analog = analogRead(SETPOINT_PIN);

  // Serial.printf("Set_point: %d %.2f %d PWM: %d\n", set_point, set_point, analog, pwm);

  // Verificar se o timeout foi excedido
  if ((currentMillis - lastTempReceived) > 5000 ) {
    Serial.println("Tempo Excedido!");
    pwm = 0;
  }

  int packetSize = udp.parsePacket();
  if (packetSize) {
    udp.read((char*)&temperatura, sizeof(temperatura));
    lastTempReceived = millis(); // Atualizar o tempo da última recepção de temperatura
    Serial.printf("Temperatura recebida: %.2f\n", temperatura);
  }

  delay(500);


  //Implementando PID
  diferenca_tempo = (millis() - ultimo_processo) / 500.0;
  ultimo_processo = millis();

  erro = set_point - temperatura;

  P = erro * kp;

  I = (I + erro * ki) * diferenca_tempo;

  D = (temperatura_anterior - temperatura) * kd * diferenca_tempo;

  temperatura_anterior = temperatura;

  PID = P + I + D;

  //Gerando PWM a partir do PID
  //pwm = PID;
  pwm = map(PID, 10, 90, 0, 255);

  if(pwm >= 255){
    pwm = 255;
  }
  if(pwm <= 0){
    pwm = 0;
  }

  // pwm_porcent = map(pwm, 0, 255, 0, 100);

  // Serial.print("  PWM: ");
  // Serial.println(pwm_porcent);
  // Serial.println(PID);
  // Serial.println(" % ");
}
