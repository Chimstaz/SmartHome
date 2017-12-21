#ifndef CONSTANSTS_H
#define CONSTANSTS_H

#include "globals.h"
#define BUILTIN_LED 2

const char* SENSORS_CONF = "Sensors";
  const char* SENSOR_TYPE = "Type";
  const char* SENSOR_DATA = "Data";
  const char* SENSOR_CHANNELS = "Channels";
const char* OUTDEVICES_CONF = "OutDevices";
  const char* OUTDEVICE_TYPE = SENSOR_TYPE;
  const char* OUTDEVICE_DATA = SENSOR_DATA;
  const char* OUTDEVICE_CHANNELS_GROUPS = SENSOR_CHANNELS;
    const char* PRIORITY_OF_GROUP = "Priority";
    const char* CHANNELS_IN_GROUP = "Channels";

      const char* CHANNEL_ID = "ID";
      const char* CHANNEL_NEGATION = "NegationFlag";
      const char* CHANNEL_VALUE_OFF = "ValueOFF";
      const char* CHANNEL_VALUE_ON = "ValueON";

const char* MVM = "MVM";
const char* BUTTONSENSOR = "ButtonSensor";
const char* LED = "LED";
const char* SERIALDEVICE = "Serial";
const char* PHOTOSENSOR = "PhotoSensor";

char ssid[256];
char password[256];
char mqtt_server[256];

const char* esp_id = "ESP82662";

#endif //CONSTANSTS_H
