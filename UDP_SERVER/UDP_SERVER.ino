#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FastLED.h>

#define NUM_LEDS 7
#define DATA_PIN 14
CRGB leds[NUM_LEDS];

const char* ssid = "HaRaPlaSi";
const char* password = "1WonderfulLlamaAlpacaNet";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back


void setup()
{
  FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical

  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

String receiv() {
  int packetSize=0;
  while (true) {
    packetSize = Udp.parsePacket();
    if (packetSize) {
      break;
    }
  }

  // receive incoming UDP packets
  Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
  int len = Udp.read(incomingPacket, 255);
  if (len > 0)
  {
    incomingPacket[len] = 0;
  }
  //Serial.printf("UDP packet contents: %s\n", incomingPacket);

  // send back a reply, to the IP address and port we got the packet from
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(replyPacket);
  Udp.endPacket();
  return incomingPacket;
}

void setLed(String Set){
  // Turn the LED on, then pause
  Set.trim();
  int len = Set.length();
  char towrite[][7]={"000000","000000","000000","000000","000000","000000","000000"};
  for (int i = 0;i<7;i++){
    for (int j = 0;j<6;j++){
      if((i*6)+j<len){
        towrite[i][j]=Set.charAt((i*6)+j);
      }
    }
  }
  Serial.println("I Write This To The Leds");
  for (int i = 0;i<7;i++){
    leds[i] = strtol(towrite[i], 0, 16);
    FastLED.show();
    Serial.println(towrite[i]);
  }
}

void loop()
{
  setLed(receiv() );
  
}
