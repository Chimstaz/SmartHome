#ifndef CONSTANSTS_H
#define CONSTANSTS_H

#include "globals.h"
#define BUILTIN_LED 2

const char* SENSORS_CONF = "Sensors";
  const char* SENSOR_TYPE = "Type";
  const char* SENSOR_PINS = "Pin";
  const char* SENSOR_CHANNELS = "Channels";
const char* OUTDEVICES_CONF = "OutDevices";
  const char* OUTDEVICE_TYPE = SENSOR_TYPE;
  const char* OUTDEVICE_PINS = SENSOR_PINS;
  const char* OUTDEVICE_CHANNELS_GROUPS = SENSOR_CHANNELS;

    const char* CHANNEL_ID = "ID";
    const char* CHANNEL_NEGATION = "NegationFlag";

const char* MVM = "MVM";
const char* LED = "LED";
const char* SERIALDEVICE = "Serial";

// Update these with values suitable for your network.
char ssid[256];//= "Maxxnet";
char password[256];// = "1111";
char mqtt_server[256];// = "192.168.43.153";
// char *ssid = "esp";
// char *password = "haslo8266";

const char* esp_id = "ESP8266";

#endif //CONSTANSTS_H
