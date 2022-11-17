#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <base64.h>
#include <Crypto.h>

#define NUM_LEDS 7
#define DATA_PIN 14

#define SSID "HaRaPlaSi"
#define PASSWORT "1WonderfulLlamaAlpacaNet"
#define KEY "test"

CRGB leds[NUM_LEDS];


WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[190];  // buffer for incoming packets
//char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back
#include <CustomJWT.h>
#include <time.h>
#include <string.h>
#define MAX_FIELD_SIZE 44

CustomJWT jwt(KEY, 260);
int timestart=0;
_Bool passivSleep(int time){
  int milli=millis();
  if(timestart==0){
    timestart=milli;
    return 0;
  }
  if(timestart>milli){
    timestart=milli;
    return 1;
  }
  if(milli-timestart>time){
    timestart=0;
    return 1;
  }
  return 0;
}

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
  if(!jwt.decodeJWT(input)){
    Serial.printf("F");
    return getRGB(jwt,"SRGB-HEX-STRING");
  }
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
  Serial.printf("Connecting to %s ", SSID);
  WiFi.begin(SSID, PASSWORT);
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
  //while(true){
    packetSize = Udp.parsePacket();
    if (!packetSize) {
      return 0;
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
  //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  //Udp.write(replyPacket);
  //Udp.endPacket();
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
_Bool remote=0;
int t=0;
int diff=0;
int sum;
static char LEDstates[3][47f]{
  "ff0000ff0000ff0000ff0000ff0000ff0000ff0000",
  "00ff0000ff0000ff0000ff0000ff0000ff0000ff00",
  "0000ff0000ff0000ff0000ff0000ff0000ff0000ff"
};
int i=0;
void loop()
{
  rgb=receiv();
  if(rgb){
    remote=1;
    setLed(decode(rgb));
  }
  if(!remote){
    diff=t;
    t=millis();
    diff=diff-t;
    sum+=diff;
    if(passivSleep(1000)){
      setLed(LEDstates[i]);
      i++;
      i=i%3;
      sum=0;
    }
  }
  //setLed(decode(receiv()) );
  //free(rgb);
  
}
