
/* *******************************
 *  yasmin samy 
   29/6/2022
   *******************************
 */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "ITI_Students";
const char* password = "ITI@2022";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char  incomingPacket[UDP_TX_PACKET_MAX_SIZE];  // buffer for incoming packets
//char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back
int incomingByte = 0; // for incoming serial data

void setup()
{
  Serial.begin(9600);
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


void loop()
{
   int packetSize = Udp.parsePacket();

   /*if recevied from UDP, send UART*/
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
   int len = Udp.read(incomingPacket,UDP_TX_PACKET_MAX_SIZE);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
  //  sending to UART
    for(int i=0;i<len;++i)
    {
      Serial.write(atoi(&incomingPacket[i]));
    }
   
  }

/*if received from UART, send on UDP*/
  if (Serial.available() > 0) {
    
    incomingByte = Serial.read();
   Udp.beginPacket("10.145.3.230", 4444);
    Udp.print(incomingByte);
    Udp.endPacket();
    
  }
  
}
