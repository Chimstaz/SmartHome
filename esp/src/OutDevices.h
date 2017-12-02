#ifndef OUTDEVICES_H
#define OUTDEVICES_H


#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "collections.h"
#include "globals.h"

class OutDevice{
public:
  void update(){
      Serial.println("Update");
      for(int i = 0; i < channelsGroups; ++i){
        for(int j = 0; ; j++){
          unsigned int id = (*channels[i])[j];
          if(id != ~0){
            if(values[id>>1] == id&1){
                break;
            }
          }
          else{
            on();
            return;
          }
        }
      }
      off();
  }

  virtual String getValue()=0;
  virtual void on()=0;
  virtual void off()=0;
  virtual ~OutDevice(){}
protected:
  int channelsGroups;
  Array<Array<unsigned int>*> channels;

  void registerChannels(JsonArray& channels){
    channelsGroups = 0;
    for(auto group = channels.begin(); group != channels.end(); ++group, ++channelsGroups){
      this->channels[channelsGroups] = new Array<unsigned int>();
      int j = 0;
      for(auto c = group->as<JsonArray>().begin(); c != group->as<JsonArray>().end(); ++c, ++j){
        this->channels[channelsGroups]->at(j) = (*c)["ID"].as<int>() << 1;
        if((*c)["NegationFlag"].as<int>()){
          this->channels[channelsGroups]->at(j) += 1;
        }
      }
      (*(this->channels[channelsGroups]))[j] = ~0;
    }
  }
};

Array<OutDevice*> outDevices(2);


class LedOutput: public OutDevice{
public:
  LedOutput(int Pin, JsonArray& channels){
    this->pin = Pin;
    pinMode(pin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    registerChannels(channels);
    value = 0;
    off();
  }

  void on(){
    value = 1;
    digitalWrite(pin, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is acive low on the ESP-01)
  }

  void off(){
    value = 0;
    digitalWrite(pin, HIGH);  // Turn the LED off by making the voltage HIGH
  }

  String getValue(){
    return String(value);
  }

  ~LedOutput(){
    for(int i = 0; i < channelsGroups; ++i){
      delete channels[i];
    }
    pinMode(pin, INPUT); // default settings
  }
private:
  int value;
  int pin;
};


class SerialPrinter: public OutDevice{
public:
  SerialPrinter(JsonArray& channels){
    Serial.println("Creating SerialPrinter");
    registerChannels(channels);
    value = 0;
    off();
  }

  void on(){
    value = 1;
    Serial.println("ON");
  }

  void off(){
    value = 0;
    Serial.println("OFF");
  }

  String getValue(){
    return String(value);
  }

  ~SerialPrinter(){
    for(int i = 0; i < channelsGroups; ++i){
      delete channels[i];
    }
  }
private:
  int value;
};



#endif //OUTDEVICES_H
