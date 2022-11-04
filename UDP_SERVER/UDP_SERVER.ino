#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <base64.h>
#include <Crypto.h>

#define NUM_LEDS 7
#define DATA_PIN 14
CRGB leds[NUM_LEDS];

const char* ssid = "HaRaPlaSi";
const char* password = "1WonderfulLlamaAlpacaNet";
const char* secret ="Das Geheimnis muss geheim sein";
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back
#include <CustomJWT.h>
#include <time.h>
#include <string.h>
#define MAX_FIELD_SIZE 44

char key[] = "test";
CustomJWT jwt(key, 256);

const char *  getRGB(CustomJWT jwt,char pattern[]){
   char * payload = jwt.payload;
  Serial.println(payload);
  int patternlen=strlen(pattern);
  int i =0;
  int outputC=0;
  int charsfound=0;
  
  char * output= (char*) malloc(MAX_FIELD_SIZE);

  while(payload[i]!=0){
    if(payload[i]==pattern[charsfound]) charsfound++;
    else charsfound=0;
    if(charsfound==patternlen) break;
    i++;
  }
  if(!charsfound) return "XXX";

  while(payload[i]!= ':') i++;
  while(payload[i]!= '"') i++;
  i++;
  while(payload[i]!= '"'&&(outputC+1)<MAX_FIELD_SIZE){
    output[outputC]=payload[i];
    outputC++;
    i++;
  }
  output[outputC]=0;
  return output;
}

const char * decode (char * input){
  jwt.allocateJWTMemory();
  if(!jwt.decodeJWT(input))
  return getRGB(jwt,"SRGB-HEX-STRING");
  return NULL;
}

void setup()
{
  //Add LED CRGB Object, Data Pin and init strip as WS2852 Strip (Controller Name (See Examples of FastLED Lib for other Controllers))
  FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical

  //Start Serial Monitor for Debugging with a baud Rate of 115200
  Serial.begin(115200);
  Serial.println();

  //Setup Wi-Fi Network
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  //

  //Open UDP Socet to except Messages with LED Strip Colors
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

//receive and return udp package as String
char * receiv() {
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


//Takes A String with 7*6 Bytes and use every 6 Bytes as srgb color for an led. Unflexibel at the time, needs changing for less or more LEDs
void setLed(String Set){
  Set.trim();
  int len = Set.length();
  if(len==0) return;
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
char * rgb;
void loop()
{
  rgb=receiv();
  setLed(decode(rgb));
  //setLed(decode(receiv()) );
  //free(rgb);
  
}
