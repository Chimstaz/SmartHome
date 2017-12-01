#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Sensors.h"
#include "OutDevices.h"
#include "constants.h"

Sensor* createSensor(JsonObject &conf){
  String v = conf["Type"].as<String>(); // FIXME:
  if(v == MVM){
    return new MovementSensor(conf["Pin"][0], conf["Channels"]);
  }
  //if(v == String(""))
}


OutDevice* createOutDevice(JsonObject &conf){
    String v = conf["Type"].as<String>();
    if(v == LED){
      return new LedOutput(conf["Pin"][0], conf["Channels"]);
    }
    if(v == SERIALDEVICE){
      return new SerialPrinter((JsonArray&)conf["Channels"]);
    }
}


void addChannels(Array<String*> &channelsList, JsonArray &channels){
  for(auto c: channels){
    const char* id = c["ID"].as<char*>();
    for(int j = 0; ; j++){
      if(channelsList[j] == NULL){
        channelsList[j] = new String(id);
        channelsList[j+1] = NULL;
        c["ID"] = j;
        return;
      }
      if(channelsList[j]->equals(id)){
        c["ID"] = j;
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
    addChannels(outChannelsList, (*it)["Channels"]);
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
    JsonArray& channels = (*it)["Channels"];
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

  addSensors(configuration["Sensors"]);

  addOutDevices(configuration["OutDevices"]);

  subscribeOnTopics(inChannelsList);
}


#endif //CONFIGURATION_H
