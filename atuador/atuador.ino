#include <WiFi.h>
#include <WiFiUdp.h>
#include <Timer.h>
#define LED_PIN 2

WiFiUDP udp;
char packetBuffer[255];
unsigned int localPort = 1322;
const char *ssid = "mauricio";
const char *password = "olnu7936";

int pwmRecebido = 0;
int inicioTempo = 0;
int finalTempo = 0;
Timer send_message;

void ValidatePWM(int pwm) {
  if(pwm < 255 && pwm > 0){
    pwmRecebido = pwm;
    inicioTempo = millis();
  }
}

void setup() {
  Serial.begin(115200);
  // Connect to Wifi network.
  WiFi.begin(ssid, password);
  
  pinMode(LED_PIN, OUTPUT);
  while (WiFi.status() != WL_CONNECTED){
    delay(500); Serial.print(F("."));
  }
  udp.begin(localPort);
  Serial.printf("UDP server : %s:%i \n", WiFi.localIP().toString().c_str(), localPort);
  digitalWrite(LED_PIN, HIGH);


}

void loop(){
  finalTempo = millis();

  if ((finalTempo - inicioTempo) > 1000){
    // setar o pwm para 0;
    Serial.println("Tempo excedido.");
  }

  int packetSize = udp.parsePacket();
  // Serial.print(" Received packet from : "); Serial.println(udp.remoteIP());
  // Serial.print(" Size : "); Serial.println(packetSize);
  if (packetSize) {
    int pwm;
    udp.read((char*)&pwm, sizeof(pwm));
    ValidatePWM(pwm);
    Serial.printf("pwm %d \n", pwmRecebido);  
  }
  else{
    inicioTempo = millis();
  }



  // Serial.println("\n");
  // delay(500);
  // Serial.print("[Server Connected] ");
  // Serial.println (WiFi.localIP());




}