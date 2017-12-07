#ifndef WIFI_H
#define WIFI_H

#include "globals.h"
#include "constants.h"

void configurationMode();
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  Serial.setDebugOutput(true); //TODO:

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(Serial.available() > 0){
      configurationMode();
      WiFi.begin(ssid, password);
      WiFi.printDiag(Serial);
    }
    Serial.printf("Connection status: %d\n", WiFi.status());
    // Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void printConfiguration(){
  Serial.println("\nEntering configuration mode. Type enter to exit");
  Serial.println("Current ssid: ");
  Serial.println(ssid);
  Serial.println("current password: ");
  Serial.println(password);
  Serial.println("current mqtt_broker: ");
  Serial.println(mqtt_server);
}

void configurationMode(){
  Serial.setTimeout(10000);
  printConfiguration();

  String configure = Serial.readStringUntil('\n');
  if(configure.startsWith("ssid:")){
    configure = configure.substring(5);
    Serial.print("Before copy: ");
    Serial.println(ssid);
    configure = configure.substring(0, configure.length() - 1);
    strcpy(ssid, configure.c_str());

    Serial.print("Updated ssid: ");
    Serial.println(ssid);

  } else if(configure.startsWith("pass:")){
    configure = configure.substring(5);
    configure = configure.substring(0, configure.length() - 1);
    strcpy(password, configure.c_str());
    Serial.print("Updated password: ");
    Serial.println(password);

  } else if(configure.startsWith("mqttServer:")){

    configure = configure.substring(11);
    configure = configure.substring(0, configure.length() - 1);

    strcpy(mqtt_server, configure.c_str());
    Serial.print("Updated mqtt_server IP: ");
    Serial.println(mqtt_server);
  }

  Serial.setTimeout(1000);
}

#endif //WIFI_H
