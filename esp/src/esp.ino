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

const String MVM = String("MVM");
const String LED = String("LED");
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
  //virtual void update();
  //virtual String getValue();
  //virtual ~OutDevice();
};

OutDevice* outDevices = NULL;
Sensor* sensors = NULL;


class LedOutput: public OutDevice{
public:
  LedOutput(int Pin, JsonArray& channels){

  }

  void update(){

  }

  String getValue(){

  }
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


Sensor createSensor(JsonObject &conf){
  String v = conf["Type"].as<String>(); // FIXME:
  if(v == String(MVM)){
    return MovementSensor(conf["Pin"][0], conf["Channels"]);
  }
  //if(v == String(""))
}

OutDevice createOutDevice(JsonObject &conf){
    String v = conf["Type"].as<String>();
    if(v == String("LED")){
      return LedOutput(conf["Pin"][0], conf["Channels"]);
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if(String(topic) == String(esp_id)){
    // TODO: move to configure()
    StaticJsonBuffer<1000> configurationBuffer;
    char* p = (char*) payload; /// WTF????
    JsonObject& configuration = configurationBuffer.parseObject(p);
    if(!configuration.success()){
      // TODO: ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    if(sensors != NULL){
      delete [] sensors;
    }
    JsonArray& sensorsJsonArray = configuration["Sensors"];
    sensors = new Sensor[sensorsJsonArray.size()];
    int i = 0;
    for(JsonArray::iterator it = sensorsJsonArray.begin(); it != sensorsJsonArray.end(); ++it, ++i)
    {
      sensors[i] = createSensor(*it);
    }

    if(outDevices != NULL){
      delete [] outDevices;
    }
    JsonArray& outDevicesJsonArray = configuration["OutDevices"];
    outDevices = new OutDevice[outDevicesJsonArray.size()];
    i = 0;
    for(JsonArray::iterator it=outDevicesJsonArray.begin(); it!=outDevicesJsonArray.end(); ++it, ++i)
    {
      outDevices[i] = createOutDevice(*it);
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
      Serial.print("1");
      client.subscribe("inTopic");
      Serial.print("2");
      client.subscribe("inTopic");
      Serial.print("3");
      client.subscribe("inTopic");
      Serial.print("4");
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
