#ifndef MQTT_H
#define MQTT_H

#include "globals.h"
#include "constants.h"
#include "configuration.h"
#include "wifi.h"

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(String(topic) == String(espID)){
    char* p = (char*) payload; /// WTF????
    configure(p);
  }
  else {
    Serial.print("Message arrived ");
    Serial.println(topic);
    for(int i = 0; inChannelsList[i] != NULL; i++ ){
      if(*inChannelsList[i] == topic){
        switch ((char)payload[0]) {
          case '0':
            values[i] = 0;
            break;
          case '1':
            values[i] = 1;
            break;
          case '2':
            values[i] = 2;
            break;
          default:
            values[i] = 2;
        }
        //values[i] = (int)((char)payload[0] == '0');
        for(int j = 0; outDevices[j] != NULL; j++){
          Serial.println(j);
          outDevices[j]->update();
        }
        return;
      }
    }
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
        // but actually the LED is on; this is because
        // it is acive low on the ESP-01)
    } else {
      digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (espID[0] != '\0' && client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
      client.subscribe(espID);
    } else {
      if(espID[0] == '\0'){
        Serial.println("You need to configure ESPID");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
      }

      if(Serial.available() > 0){
        configurationMode();
        WiFi.begin(ssid, password);
      }
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

#endif //MQTT_H
