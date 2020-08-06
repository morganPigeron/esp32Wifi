/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

#include <SPI.h>
#include "Led.h"

#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11

#define USE_SERIAL Serial

#define NBRLED 100
#define LEDAMP 40

const uint8_t nullByte = 0;

Led ledArray[NBRLED];


WiFiMulti wifiMulti;

DHT dht(DHTPIN, DHTTYPE);


void setup() {

    USE_SERIAL.begin(115200);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    wifiMulti.addAP("SFR_0800", "AlessiaMorgan1!");

    dht.begin();

    SPI.begin();
    initLedArray();

}

void loop() {
    int i;

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);
    
    // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin("http://192.168.1.38:1880/api/temp?name=esp32&temp=" + String(t) + "&humidity=" + String(h) + "&heatIndex=" +String(hic)); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    redPatern();
    sendStartFrame();
    for(i=0; i<NBRLED; i++)
    {
        sendLedFrame(i);
    }
    sendEndFrame();
    delay(500);

    greenPatern();
    sendStartFrame();
    for(i=0; i<NBRLED; i++)
    {
        sendLedFrame(i);
    }
    sendEndFrame();
    delay(500);

    bluePatern();
    sendStartFrame();
    for(i=0; i<NBRLED; i++)
    {
        sendLedFrame(i);
    }
    sendEndFrame();
    delay(500);

    
    delay(5000);
}

void sendStartFrame()
{
    SPI.transfer(nullByte);
    SPI.transfer(nullByte);
    SPI.transfer(nullByte);
    SPI.transfer(nullByte);
}

void sendLedFrame(int id)
{
    SPI.transfer(ledArray[id].getBrightness());
    SPI.transfer(ledArray[id].getBlue());
    SPI.transfer(ledArray[id].getGreen());
    SPI.transfer(ledArray[id].getRed());
}

void sendEndFrame()
{
    int i;
    int nbr = (NBRLED-1)/16;

    for(i=0; i<nbr; i++)
    {
        SPI.transfer(nullByte);
    }
    SPI.transfer(nullByte);
}

void initLedArray()
{
    int i;

    for(i=0; i<NBRLED; i++)
    {
        ledArray[i].setId(i);
    }
}

void setLed(int id, int r, int g, int b, int brightness)
{
    ledArray[id].settings(r,g,b,brightness);
}

void randomPatern()
{
    int i;

    for(i=0; i<NBRLED; i++)
    {
        setLed(i,random(255),random(255),random(255),random(255));
    }  
}

void redPatern()
{
    int i;

    for(i=0; i<NBRLED; i++)
    {
        setLed(i, LEDAMP, 0, 0, LEDAMP);
    }  
}

void bluePatern()
{
    int i;

    for(i=0; i<NBRLED; i++)
    {
        setLed(i, 0, LEDAMP, 0, LEDAMP);
    }  
}

void greenPatern()
{
    int i;

    for(i=0; i<NBRLED; i++)
    {
        setLed(i, 0, 0, LEDAMP, LEDAMP);
    }  
}
