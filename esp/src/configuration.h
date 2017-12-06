#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Sensors.h"
#include "OutDevices.h"
#include "constants.h"

Sensor* createSensor(JsonObject &conf){
  const char* v = conf[SENSOR_TYPE].as<char*>(); // FIXME:
  if(strcmp(v, MVM) == 0){
    return new MovementSensor(conf[SENSOR_PINS][0], conf[SENSOR_CHANNELS]);
  }
  if(strcmp(v, SERIALSENSOR) == 0){
    return new SerialSensor((JsonArray&)conf[SENSOR_CHANNELS]);
  }
}


OutDevice* createOutDevice(JsonObject &conf){
    const char* v = conf[OUTDEVICE_TYPE].as<char*>();
    if(strcmp(v, LED) == 0){
      return new LedOutput(conf[OUTDEVICE_PINS][0], conf[OUTDEVICE_CHANNELS_GROUPS]);
    }
    if(strcmp(v, SERIALDEVICE) == 0){
      return new SerialPrinter((JsonArray&)conf[OUTDEVICE_CHANNELS_GROUPS]);
    }
}


void addChannels(Array<String*> &channelsList, JsonArray &channels){
  for(auto c: channels){
    const char* id = c[CHANNEL_ID].as<char*>();
    for(int j = 0; ; j++){
      if(channelsList[j] == NULL){
        channelsList[j] = new String(id);
        channelsList[j+1] = NULL;
        c[CHANNEL_ID] = j;
        return;
      }
      if(channelsList[j]->equals(id)){
        c[CHANNEL_ID] = j;
        return;
      }
    }
  }
}


void clear_config(){
  for(int i = 0; inChannelsList[i] != NULL; i++){
    client.unsubscribe(inChannelsList[i]->c_str());
    delete inChannelsList[i];
  }
  outChannelsList[0] = NULL;
  inChannelsList[0] = NULL;

  for(int i = 0; sensors[i] != NULL; i++){
    delete sensors[i];
  }
  sensors[0] = NULL;

  for(int i = 0; outDevices[i] != NULL; i++){
    delete outDevices[i];
  }
  outDevices[0] = NULL;
}


void subscribeOnTopics(Array<String*> &channelsList){
  for(int i = 0; channelsList[i] != NULL; i++){
    Serial.print("Subscribe topic: ");
    Serial.println(channelsList[i]->c_str());
    values[i] = 0;
    client.subscribe(channelsList[i]->c_str());
  }
}


void addSensors(JsonArray& sensorsJsonArray){
  Serial.println("Sensors");
  int i = 0;
  for(JsonArray::iterator it = sensorsJsonArray.begin(); it != sensorsJsonArray.end(); ++it, ++i)
  {
    Serial.println("   Add Sensor");
    addChannels(outChannelsList, (*it)[SENSOR_CHANNELS]);
    sensors[i] = createSensor(*it);
  }
  sensors[i] = NULL;
  sensors.trim(sensorsJsonArray.size()+1);

}


void addOutDevices(JsonArray& outDevicesJsonArray){
  Serial.println("OutDevices");
  int i = 0;
  for(JsonArray::iterator it=outDevicesJsonArray.begin(); it!=outDevicesJsonArray.end(); ++it, ++i)
  {
    Serial.println("   Add OutDevice");
    JsonArray& channels = (*it)[OUTDEVICE_CHANNELS_GROUPS];
    for(auto group: channels){
      addChannels(inChannelsList, group);
    }
    outDevices[i] = createOutDevice(*it);
  }
  outDevices[i] = NULL;
  outDevices.trim(outDevicesJsonArray.size()+1);
}


void configure(char* payload){
  Serial.println(payload);
  StaticJsonBuffer<1000> configurationBuffer;
  JsonObject& configuration = configurationBuffer.parseObject(payload);
  if(!configuration.success()){
    // TODO: ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // do nothing??
    Serial.println("Error reading configuration");
    return;
  }
  Serial.println("Parsed");

  clear_config();

  addSensors(configuration[SENSORS_CONF]);

  addOutDevices(configuration[OUTDEVICES_CONF]);

  subscribeOnTopics(inChannelsList);
}


#endif //CONFIGURATION_H
