#ifndef SENSORS_H
#define SENSORS_H

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "collections.h"
#include "globals.h"
#include "constants.h"
#include "channelsUtils.h"


class Sensor{
public:
  void update(){
    update(true);
  }


  virtual String getValueWithUnits()=0;
  virtual int getValue()=0;
  virtual ~Sensor(){};
protected:
  int previousValue;

  void update(bool checkPervious){
    int v = this->getValue();
    for(int i = 0; channels[i] != ~0; i++){
      // if upBound >= downBound then send 1 if value is over upBound. If upBound < downBound then send 1 if value is below upBound
      if(upBound[i] >= downBound[i]){
        if(v > upBound[i] && (previousValue <= upBound[i] || !checkPervious)){
          client.publish(outChannelsList[channels[i]]->c_str(), "1", true); // publish retained message
        }
        else if(v < downBound[i] && (previousValue >= downBound[i] || !checkPervious)){
          client.publish(outChannelsList[channels[i]]->c_str(), "0", true); // publish retained message
        }
      }
      else {
        if(v < upBound[i] && (previousValue >= upBound[i]  || !checkPervious)){
          client.publish(outChannelsList[channels[i]]->c_str(), "1", true); // publish retained message
        }
        else if(v > downBound[i] && (previousValue <= downBound[i] || !checkPervious)){
          client.publish(outChannelsList[channels[i]]->c_str(), "0", true); // publish retained message
        }
      }
    }
    previousValue = v;
  }

  void registerChannels(JsonArray& channels){
    int i = 0;
    for(auto c = channels.begin(); c != channels.end(); ++c, ++i){
      this->channels[i] = findChannel((*c)[CHANNEL_ID].as<char*>(), outChannelsList ) ;
      Serial.print("Register out channel: ");
      Serial.print(outChannelsList[this->channels[i]]->c_str());
      Serial.print(" id: ");
      Serial.println(this->channels[i]);
      this->upBound[i] = (*c)[CHANNEL_VALUE_ON].as<int>();
      this->downBound[i] = (*c)[CHANNEL_VALUE_OFF].as<int>();
    }
    this->channels[i] = ~0;
  }
private:
  Array<unsigned int> channels;
  Array<int> upBound;
  Array<int> downBound;
};

Array<Sensor*> sensors(2);

// When getValue is called it try to get something from serial input.
// If there is nothing then retrun old value
// If there is something it try to get first inteager and set it as its new value
class ButtonSensor: public Sensor{
public:
  ButtonSensor(int pin, JsonArray& channels){
    Serial.print("Create button sensor on pin ");

    Serial.println(pin);
    this->pin = pin;
    pinMode(pin, INPUT);
    registerChannels(channels);
    value = 0;
    update(false);
  }

  String getValueWithUnits(){
    return String(value);
  }

  int getValue(){
    if(digitalRead(pin) == HIGH){
      value = 255;
    }
    else{
      value = 0;
    }
    return value;
  }

  ~ButtonSensor(){
    Serial.print("Button sensor on pin ");
    Serial.print(pin);
    Serial.println(" is dead.");
  }
private:
  int value;
  int pin;
};

class MovementSensor: public Sensor{
public:
  MovementSensor(int Pin, JsonArray& channels){

  }

  int getValue(){

  }

  String getValueWithUnits(){

  }

  ~MovementSensor(){}
};


#endif //SENSORS_H
