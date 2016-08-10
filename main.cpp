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

byte postPage(char* domainBuffer,int thisPort,char* page,char* thisData);

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

    Serial.print(F("Starting ethernet..."));
    if(!Ethernet.begin(mac)) {
        Serial.println(F("failed"));
    } else {
        Serial.println(Ethernet.localIP());
    }

    delay(2000);
    Serial.println(F("Ready"));

}//END: init_io

int main(void) {

    init_io();

    while (1) {

        // If using a static IP, comment out the next line
        Ethernet.maintain();

        thisMillis = millis();
        if(thisMillis - lastMillis > delayMillis) {
            lastMillis = thisMillis;

            // params must be url encoded.
            sprintf(params, "temp1=%i", totalCount);
            if(!postPage(serverName, serverPort, pageName, params)) {
                Serial.print(F("Fail "));
            } else {
                Serial.print(F("Pass "));
            }

            totalCount++;
            Serial.println(totalCount, DEC);
        }

    }//END: loop

    return 0;
}//END: main


byte postPage(char* domainBuffer, int thisPort,char* page, char* thisData) {

    int inChar;
    char outBuf[64];

    Serial.print(F("connecting..."));

    if(client.connect(domainBuffer,thisPort) == 1) {
        Serial.println(F("connected"));

        // send the header
        sprintf(outBuf, "POST %s HTTP/1.1", page);
        client.println(outBuf);
        sprintf(outBuf, "Host: %s", domainBuffer);
        client.println(outBuf);
        client.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));
        sprintf(outBuf, "Content-Length: %u\r\n", strlen(thisData));
        client.println(outBuf);

        // send the body (variables)
        client.print(thisData);
    } else {
        Serial.println(F("failed"));
        return 0;
    }

    int connectLoop = 0;

    while(client.connected()) {
        while(client.available()) {
            inChar = client.read();
            Serial.write(inChar);
            connectLoop = 0;
        }

        delay(1);
        connectLoop++;
        if(connectLoop > 10000) {
            Serial.println();
            Serial.println(F("Timeout"));
            client.stop();
        }
    }

    Serial.println();
    Serial.println(F("disconnecting."));
    client.stop();

    return 1;
}
