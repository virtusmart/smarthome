/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Modified and corrected by Przemyslaw Myk
 * Poland
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * DESCRIPTION
 *
 * Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
 * http://www.mysensors.org/build/temp
 */

#define MY_GATEWAY_SERIAL

#include <MySensors.h>  
#include <SPI.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define COMPARE_TEMP 0 // Send temperature only if changed? 1 = Yes 0 = No

#define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 
#define MAX_ATTACHED_DS18B20 16
#define ATTACHED_DS18B20 3
unsigned long SLEEP_TIME = 3000; // Sleep time between reads (in milliseconds)
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 

byte D[ATTACHED_DS18B20][8] = {
{ 0x28, 0x08, 0xB4, 0x16, 0xA8, 0x01, 0x3C, 0xD4 },
{ 0x28, 0xFF, 0xFE, 0xA7, 0x8A, 0x16, 0x03, 0xCF },
{ 0x28, 0xFF, 0x75, 0x05, 0x93, 0x16, 0x04, 0x9D }
};
//MySensor gw;
float lastTemperature[MAX_ATTACHED_DS18B20];
int numSensors=0;
boolean receivedConfig = false;
boolean metric = true; 
// Initialize temperature message
MyMessage msg(0,V_TEMP);

void before()  
{ 
  // Startup up the OneWire library
  sensors.begin();
}

void setup() 
{
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);

  // Startup and initialize MySensors library. Set callback for incoming messages. 
//  begin();
}
void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("ArduNano1_rozdzielnia_dol", "1.1");

  // Fetch the number of attached temperature sensors  
  numSensors = sensors.getDeviceCount();

  // Present all sensors to controller
  for (int i=0; i<ATTACHED_DS18B20 && i<MAX_ATTACHED_DS18B20; i++) {   
     present(i, S_TEMP);
  }
}


void loop()     
{     
  
//Serial.println(millis()); 
  // Process incoming messages (like config from server)
  //process(); 

  // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();

  // query conversion time and sleep until conversion completed
  int16_t conversionTime = sensors.millisToWaitForConversion(sensors.getResolution());
  // sleep() call can be replaced by wait() call if node need to process incoming messages (or if node is repeater)
  sleep(conversionTime);

  // Read temperatures and send them to controller 
  for (int i=0; i<3 && i<MAX_ATTACHED_DS18B20; i++) {
 
    // Fetch and round temperature to one decimal
 //   float temperature = static_cast<float>(static_cast<int>((sensors.requestTemperaturesByAddress(D[i])) * 10.)) / 10.;
      float temperature = sensors.getTempC(D[i]);
      
    // Only send data if temperature has changed and no error
    if (lastTemperature[i] != temperature && temperature != -127.00 && temperature != 85.00 && abs(lastTemperature[i] - temperature)>0.5) {
      

   
        // Send in the new temperature
        send(msg.setSensor(i).set(temperature,1).set(temperature,1));
        // Save new temperatures for next compare
        lastTemperature[i]=temperature;
    }
  
  }
  sleep(2000);
}
