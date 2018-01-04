#ifndef OUTDEVICES_H
#define OUTDEVICES_H


#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "collections.h"
#include "constants.h"
#include "globals.h"
#include "channelsUtils.h"


int compareChannelsGroupsByPriority(Array<unsigned int>* const *a, Array<unsigned int>* const *b){
  return (*a)->at(0) - (*b)->at(0);
}


class OutDevice{
public:
  // called when message came on subscribed channel, Check if OutDevice should change state.
  void update(){
      Serial.println("Update");
      bool wasFalseBefore = false;
      unsigned int currentPriority = 0;
      for(int i = 0; i < channelsGroups; ++i){
        if(currentPriority < (*channels[i])[0] && wasFalseBefore){
          off();
          return;
        }
        currentPriority = (*channels[i])[0];
        bool isUndefined = false;
        for(int j = 1; ; j++){
          unsigned int id = (*channels[i])[j];
          if(id != ~0){
            if(values[id>>1] == 2){
              isUndefined = true;
              continue;
            }
            else if(values[id>>1] == (id&1)){
              wasFalseBefore = true;
              break;
            }
          }
          else if(!isUndefined){
            on();
            return;
          }
          else{
            break;
          }
        }
      }
      off();
  }

  // return string that will be return on request channel
  virtual String getValue()=0;

  // turn on OutDevice
  virtual void on()=0;

  // turn off OutDevice
  virtual void off()=0;
  virtual ~OutDevice(){}
protected:
  int channelsGroups;
  Array<Array<unsigned int>*> channels;

  void registerChannels(JsonArray& channels){
    channelsGroups = 0;
    for(auto group = channels.begin(); group != channels.end(); ++group, ++channelsGroups){
      this->channels[channelsGroups] = new Array<unsigned int>();
      this->channels[channelsGroups]->at(0) = group->as<JsonObject>()[PRIORITY_OF_GROUP];
      int j = 1;
      for(auto c = group->as<JsonObject>()[CHANNELS_IN_GROUP].as<JsonArray>().begin(); c != group->as<JsonObject>()[CHANNELS_IN_GROUP].as<JsonArray>().end(); ++c, ++j){
        this->channels[channelsGroups]->at(j) = findChannel((*c)[CHANNEL_ID].as<char*>(), inChannelsList ) << 1;
        if((*c)[CHANNEL_NEGATION].as<int>()){
          this->channels[channelsGroups]->at(j) += 1;
        }
      }
      (*(this->channels[channelsGroups]))[j] = ~0;
    }
    // sort channels by priority
    this->channels.sort(*compareChannelsGroupsByPriority, channelsGroups);
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
  SerialPrinter(const char* name, JsonArray& channels){
    this->name = name;
    Serial.println("Creating SerialPrinter with name: " + this->name);
    registerChannels(channels);
    value = 0;
    off();
  }

  void on(){
    value = 1;
    Serial.println("ON " + name);
  }

  void off(){
    value = 0;
    Serial.println("OFF " + name);
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
  String name;
  int value;
};



#endif //OUTDEVICES_H
