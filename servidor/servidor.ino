#include <WiFi.h>
#include <WiFiUdp.h>
#include <Timer.h>
#define LED_PIN 2

WiFiUDP udp;
char packetBuffer[255];
unsigned int localPort = 1322;
const char *ssid = "mauricio";
const char *password = "olnu7936";
Timer send_message;

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

  int packetSize = udp.parsePacket();
  Serial.print(" Received packet from : "); Serial.println(udp.remoteIP());
  Serial.print(" Size : "); Serial.println(packetSize);
  if (packetSize) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len - 1] = 0;
    Serial.printf("Data : %s\n", packetBuffer);
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.printf("UDP packet was received OK\r\n");
    udp.endPacket();
  }
  Serial.println("\n");
  delay(500);
  Serial.print("[Server Connected] ");
  Serial.println (WiFi.localIP());

}