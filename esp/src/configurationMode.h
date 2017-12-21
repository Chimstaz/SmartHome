#ifndef CONFIGURATIONMODE_H
#define CONFIGURATIONMODE_H

#include "constants.h"
#include "EEPROMTools.h"

void printConfiguration(){
  Serial.println("\nEntering configuration mode. Type enter to exit");
  Serial.print("Current ssid: ");
  Serial.print(ssid);
  Serial.print(" current password: ");
  Serial.print(password);
  Serial.print(" current mqtt_broker: ");
  Serial.print(mqtt_server);
  Serial.print(" current esp ID: ");
  Serial.println(espID);
}

void configurationMode(){
  Serial.setTimeout(10000);
  printConfiguration();

  String configure = Serial.readStringUntil('\n');
  if(configure.startsWith("ssid:")){
    configure = configure.substring(5);
    configure = configure.substring(0, configure.length() - 1); //skip the newLine on the end of user input
    strcpy(ssid, configure.c_str());
    Serial.print("Updated ssid: ");
    Serial.println(ssid);

  } else if(configure.startsWith("pass:")){
    configure = configure.substring(5);
    configure = configure.substring(0, configure.length() - 1);
    strcpy(password, configure.c_str());
    Serial.print("Updated password: ");
    Serial.println(password);

  } else if(configure.startsWith("brokerIP:")){
    configure = configure.substring(9);
    configure = configure.substring(0, configure.length() - 1);
    strcpy(mqtt_server, configure.c_str());
    EEPROM_writeCharacters(mqtt_broker_EEPROM_offset, mqtt_server, brokerIPSize);
    Serial.print("Updated mqtt_server IP: ");
    Serial.println(mqtt_server);

  } else if(configure.startsWith("espID:")){
    configure = configure.substring(6,configure.length() - 1);
    strcpy(espID, configure.c_str());
    EEPROM_writeCharacters(espIdOffset, espID, espIdSize);
    Serial.print("Updated esp ID: ");
    Serial.println(espID);
  } else {
    Serial.print("Unknown field update: ");
    Serial.println(configure);
  }

  Serial.setTimeout(1000);
}

#endif //CONFIGURATIONMODE_H
