#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 3


OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
float PiwnicaDsKlatkaVal;
float PiwnicaDSOgrzewZasilVal;
float PiwnicaDSOgrzewPowrotVal;
DeviceAddress PiwnicaDsKlatka = { 0x28, 0x08, 0xB4, 0x16, 0xA8, 0x01, 0x3C, 0xD4 };
DeviceAddress PiwnicaDSOgrzewZasil = { 0x28, 0xFF, 0xFE, 0xA7, 0x8A, 0x16, 0x03, 0xCF };
DeviceAddress PiwnicaDSOgrzewPowrot = { 0x28, 0xFF, 0x75, 0x05, 0x93, 0x16, 0x04, 0x9D };

void setup(void)
{
  // start serial port
  Serial.begin(115200);
  // Start up the library
  sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(PiwnicaDsKlatka, 10);
  sensors.setResolution(PiwnicaDSOgrzewZasil, 10);
  sensors.setResolution(PiwnicaDSOgrzewPowrot, 10);
}


void loop(void)
{ 

  delay(2000);

  sensors.requestTemperatures();

  StaticJsonDocument<100> jsonBufferA;

  if(abs(float(PiwnicaDsKlatkaVal -  sensors.getTempC(PiwnicaDsKlatka)))>0.5){
    PiwnicaDsKlatkaVal =  sensors.getTempC(PiwnicaDsKlatka);
    jsonBufferA["0P_DS_Klatka"] = sensors.getTempC(PiwnicaDsKlatka);
    serializeJson(jsonBufferA, Serial);
    Serial.println();
  }

  if(abs(float(PiwnicaDSOgrzewZasilVal -  sensors.getTempC(PiwnicaDSOgrzewZasil)))>0.5){
    PiwnicaDSOgrzewZasilVal = sensors.getTempC(PiwnicaDSOgrzewZasil);
    StaticJsonDocument<100> jsonBufferB;
    jsonBufferB["0P_DS_Ogrzew_Zasil"] = sensors.getTempC(PiwnicaDSOgrzewZasil);
    serializeJson(jsonBufferB, Serial);
    Serial.println();
  }
  
  if(abs(float(PiwnicaDSOgrzewPowrotVal -  sensors.getTempC(PiwnicaDSOgrzewPowrot)))>0.5){
    PiwnicaDSOgrzewPowrotVal = sensors.getTempC(PiwnicaDSOgrzewPowrot);
    StaticJsonDocument<100> jsonBufferC;
    jsonBufferC["0P_DS_Ogrzew_Powrot"] = sensors.getTempC(PiwnicaDSOgrzewPowrot);
    serializeJson(jsonBufferC, Serial);
    Serial.println();
  }
  
  delay(4000);
}
