#ifndef SENSORS_H
#define SENSORS_H

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "collections.h"
#include "globals.h"

class Sensor{
public:
  static void subscribe(JsonArray& channels){
    for(auto v: channels){
      client.subscribe(v.as<char*>());
    }
  }
  //virtual void update();
  //virtual String getValue();
  //virtual ~Sensor();
};

Array<Sensor*> sensors(2);

class MovementSensor: public Sensor{
public:
  MovementSensor(int Pin, JsonArray& channels){

  }

  void update(){

  }

  String getValue(){

  }
};


#endif //SENSORS_H
