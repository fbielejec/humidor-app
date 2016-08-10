/*
 *@author: fbielejec
 */

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

#include <SPI.h>
#include <Ethernet.h>

// set this to the number of milliseconds delay
#define delayMillis 30000UL // 30 seconds

////////////////////
//---PROTOTYPES---//
////////////////////



/////////////////////
//---GLOBAL VARS---//
/////////////////////

// this must be unique across all devices on the network
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Change to your domain name for virtual servers
// char serverName[] = "www.mydomain.com";
// If no domain name, use the ip address above
char serverName[] = "127.0.0.1";

// change to your server's port
int serverPort = 8080;

// change to the page on that server
char pageName[] = "/arduinotest";

EthernetClient client;
int totalCount = 0;
// insure params is big enough to hold your variables
char params[32];

unsigned long thisMillis = 0;
unsigned long lastMillis = 0;

/////////////////////
//---ENTRY POINT---//
/////////////////////

void init_io(void) {

     init();

    // open serial port
    Serial.begin(9600);
      while (!Serial);
    Serial.println("\t Comm-link online");

// disable SD SPI
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);

  // Start ethernet
  Serial.println(F("Starting ethernet..."));
  Ethernet.begin(mac, ip, gateway, gateway, subnet);

  // If using dhcp, comment out the line above
  // and uncomment the next 2 lines plus the Ethernet.maintain call in loop

  // if(!Ethernet.begin(mac)) Serial.println(F("failed"));
  // else Serial.println(F("ok"));

  Serial.println(Ethernet.localIP());

  delay(2000);
  Serial.println(F("Ready"));



}//END: init_io

int main(void) {

    init_io();



    while (1) {


    }//END: loop

    return 0;
}//END: main

