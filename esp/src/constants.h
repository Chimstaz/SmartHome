#ifndef CONSTANSTS_H
#define CONSTANSTS_H

#include "globals.h"

const char* SENSORS_CONF = "Sensors";
  const char* SENSOR_TYPE = "Type";
  const char* SENSOR_DATA = "Data";
  const char* SENSOR_CHANNELS = "Channels";
const char* OUTDEVICES_CONF = "OutDevices";
  const char* OUTDEVICE_TYPE = SENSOR_TYPE;
  const char* OUTDEVICE_DATA = SENSOR_DATA;
  const char* OUTDEVICE_CHANNELS_GROUPS = SENSOR_CHANNELS;

    const char* CHANNEL_ID = "ID";
    const char* CHANNEL_NEGATION = "NegationFlag";
    const char* CHANNEL_VALUE_OFF = "ValueOFF";
    const char* CHANNEL_VALUE_ON = "ValueON";

const char* MVM = "MVM";
const char* BUTTONSENSOR = "ButtonSensor";
const char* LED = "LED";
const char* SERIALDEVICE = "Serial";

// Update these with values suitable for your network.
const char* ssid = "agatom";
const char* password = "xxx";
const char* mqtt_server = "192.168.0.102";

const char* esp_id = "ESP82662";

#endif //CONSTANSTS_H
