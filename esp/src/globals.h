#ifndef GLOBALS_H
#define GLOBALS_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "collections.h"

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

Array<String*> inChannelsList(2);
Array<String*> outChannelsList(2);
Array<int> values(2);

#endif //GLOBALS_H
