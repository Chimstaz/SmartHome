#ifndef CONSTANSTS_H
#define CONSTANSTS_H

#include "globals.h"

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
    const char* CHANNEL_VALUE_OFF = "ValueOFF";
    const char* CHANNEL_VALUE_ON = "ValueON";

const char* MVM = "MVM";
const char* BUTTONSENSOR = "ButtonSensor";
const char* LED = "LED";
const char* SERIALDEVICE = "Serial";

// Update these with values suitable for your network.
const char* ssid = "esp";
const char* password = "haslo8266";
const char* mqtt_server = "192.168.0.105";

const char* esp_id = "ESP82662";

#endif //CONSTANSTS_H
