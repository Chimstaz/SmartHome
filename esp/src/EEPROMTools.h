#ifndef EEPROMTOOLS_H
#define EEPROMTOOLS_H

#include <EEPROM.h>
#include <Arduino.h>  // for type definitions
#include "constants.h"

int mqtt_broker_EEPROM_offset = 0;
int brokerIPSize = 128;
int espIdOffset = 128;
int espIdSize = 32;

template <class T> int EEPROM_writeAnything(int ee, const T& value){
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value){
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}

int EEPROM_writeCharacters(int offset, const char* value, int maxSize){
  unsigned int i = 0;
  const char *curr_value = value;
  for(; i < maxSize && *curr_value != '\0'; i++, curr_value++){
    EEPROM.write(offset + i, *curr_value);
  }
  if(i == maxSize){
    return -1;
  }
  EEPROM.write(offset + i, *curr_value); //save ending char
  EEPROM.commit();
  return ++i;
}

int EEPROM_readCharacters(int offset, char *buffer, int maxSize){
  unsigned int i = 0;
  char value = 'a';
  for(; i < maxSize && value != '\0'; i++){
    value = EEPROM.read(offset + i);
    buffer[i] = value;
  }
  if(i == maxSize and value != '\0'){
    return -1;
  }
  buffer[i] = '\0';
  return i;
}

String EEPROM_readString(int offset, int size){
  char* read = new char[size];
  EEPROM_readCharacters(offset, read, size);
  return String(read);
}

int EEPROM_writeString(int offset, String msg, int size){
  return EEPROM_writeCharacters(offset, msg.c_str(), size);
}

void readConfigFromEEPROM(){
  EEPROM_readCharacters(mqtt_broker_EEPROM_offset, mqtt_server, brokerIPSize); //read broker IP
  EEPROM_readCharacters(espIdOffset, espID, espIdSize); // read espID
}

void initializeEEPROM(){
  EEPROM.begin(brokerIPSize + espIdSize); //size of broker IP address
  readConfigFromEEPROM();
}

#endif
