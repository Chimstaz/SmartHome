/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/



#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "collections.h"

const String MVM = String("MVM");
const String LED = String("LED");
const String SERIALDEVICE = String("Serial");
// Update these with values suitable for your network.

const char* ssid = "esp";
const char* password = "haslo8266";
const char* mqtt_server = "192.168.0.104";

const char* esp_id = "ESP8266";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

Array<String*> inChannelsList(2);
Array<String*> outChannelsList(2);
Array<int> values(2);

class Sensor{
public:
  static void subscribe(JsonArray& channels){
    for(auto v: channels){
      client.subscribe(v.as<char*>());
    }
  }
  //virtual void update();
  //virtual String getValue();
  //virtual ~Sensor();
};

class OutDevice{
public:
  static void subscribe(JsonArray& channels){
    for(auto v: channels){
      // FIXME:
      client.subscribe(v.as<char*>());
    }
  }

  void update(){
      Serial.println("Update");
      for(int i = 0; i < channelsGroups; ++i){
        for(int j = 0; ; j++){
          int id = (*channels[i])[j];
          if(id != -1){
            if(values[id] == 0){
              break;
            }
          }
          else{
            on();
            return;
          }
        }
      }
      off();
  }

  virtual String getValue()=0;
  virtual void on()=0;
  virtual void off()=0;
  virtual ~OutDevice(){}
protected:
  int channelsGroups;
  Array<Array<int>*> channels;

  void registerChannels(JsonArray& channels){
    channelsGroups = 0;
    for(auto group = channels.begin(); group != channels.end(); ++group, ++channelsGroups){
      this->channels[channelsGroups] = new Array<int>();
      int j = 0;
      for(auto c = group->as<JsonArray>().begin(); c != group->as<JsonArray>().end(); ++c, ++j){
        this->channels[channelsGroups]->at(j) = c->as<JsonObject>()["ID"].as<int>();
      }
      (*(this->channels[channelsGroups]))[j] = -1;
    }
  }
};

Array<OutDevice*> outDevices(2);
Array<Sensor*> sensors(2);

class LedOutput: public OutDevice{
public:
  LedOutput(int Pin, JsonArray& channels){
    this->pin = Pin;
    pinMode(pin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    registerChannels(channels);
    value = 0;
    off();
  }

  void on(){
    value = 1;
    digitalWrite(pin, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is acive low on the ESP-01)
  }

  void off(){
    value = 0;
    digitalWrite(pin, HIGH);  // Turn the LED off by making the voltage HIGH
  }

  String getValue(){
    return String(value);
  }

  ~LedOutput(){
    for(int i = 0; i < channelsGroups; ++i){
      delete channels[i];
    }
    pinMode(pin, INPUT); // default settings
  }
private:
  int value;
  int pin;
};

class SerialPrinter: public OutDevice{
public:
  SerialPrinter(JsonArray& channels){
    Serial.println("Creating SerialPrinter");
    registerChannels(channels);
    value = 0;
    off();
  }

  void on(){
    value = 1;
    Serial.println("ON");
  }

  void off(){
    value = 0;
    Serial.println("OFF");
  }

  String getValue(){
    return String(value);
  }

  ~SerialPrinter(){
    for(int i = 0; i < channelsGroups; ++i){
      delete channels[i];
    }
  }
private:
  int value;
};


class MovementSensor: public Sensor{
public:
  MovementSensor(int Pin, JsonArray& channels){

  }

  void update(){

  }

  String getValue(){

  }
};

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


Sensor* createSensor(JsonObject &conf){
  String v = conf["Type"].as<String>(); // FIXME:
  if(v == String(MVM)){
    return new MovementSensor(conf["Pin"][0], conf["Channels"]);
  }
  //if(v == String(""))
}

OutDevice* createOutDevice(JsonObject &conf){
    String v = conf["Type"].as<String>();
    if(v == String("LED")){
      return new LedOutput(conf["Pin"][0], conf["Channels"]);
    }
    if(v == String("Serial")){
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
  // clear old config
  for(int i = 0; inChannelsList[i] != NULL; i++){
    client.unsubscribe(inChannelsList[i]->c_str());
    delete inChannelsList[i];
  }
  outChannelsList[0] = NULL;
  inChannelsList[0] = NULL;

  for(int i = 0; sensors[i] != NULL; i++){
    delete sensors[i];
  }

  for(int i = 0; outDevices[i] != NULL; i++){
    delete outDevices[i];
  }
  ///////////////////

  JsonArray& sensorsJsonArray = configuration["Sensors"];
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


  JsonArray& outDevicesJsonArray = configuration["OutDevices"];
  Serial.println("OutDevices");
  i = 0;
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

  for(int i = 0; inChannelsList[i] != NULL; i++){
    Serial.print("Subscribe topic: ");
    Serial.println(inChannelsList[i]->c_str());
    values[i] = 0;
    client.subscribe(inChannelsList[i]->c_str());
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(String(topic) == String(esp_id)){
    char* p = (char*) payload; /// WTF????
    configure(p);
  }
  else {
    Serial.print("Message arrived ");
    Serial.println(topic);
    for(int i = 0; inChannelsList[i] != NULL; i++ ){
      if(*inChannelsList[i] == topic){
        values[i] = (int)((char)payload[0] == '1');
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
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
      //Serial.print("1");
      //client.subscribe("inTopic");
      //Serial.print("2");
      //client.subscribe("inTopic");
      //Serial.print("3");
      //client.subscribe("inTopic");
      //Serial.print("4");
      client.subscribe(esp_id);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  outDevices[0] = NULL;
  sensors[0] = NULL;
  inChannelsList[0] = NULL;
  outChannelsList[0] = NULL;
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
