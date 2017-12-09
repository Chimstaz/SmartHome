#ifndef WIFI_H
#define WIFI_H

#include "globals.h"
#include "configurationMode.h"

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println("Connecting to last remembered network:");
  WiFi.printDiag(Serial);

  WiFi.begin(ssid, password);



  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(Serial.available() > 0){
      configurationMode();
      WiFi.begin(ssid, password);
    }
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

#endif //WIFI_H
