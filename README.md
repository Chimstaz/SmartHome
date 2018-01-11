# SmartHome
SmartHome is a project for creating and customising Intelligent Home, based on MQTT protocol and ESP8266. 

## Purpose
The idea of the project is to create a simple, configurable IoT network. User is provided with tool that allows to remotly configure the behaviour of the controlled devices. User can choose what type of device or sensor is connected to the ESP module. Each device can be controlled by set of sensors.

#### Currently supported devices and sensors:
- OutDevice that is controlled by binary signal (ON and OFF - Example: Led Strip).
- Button Sensor - sensor value depending on the external button

## Getting Started
These instructions will get you a copy of the project up and running in your local environment for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites
ESP8266 platform (ex. with NodeMCU)
Project is being developed with PlatofrmIO for Atom
MQTT Broker (may be local - ex. mosquitto or remote one)

### Configuration
After uploading the project to the ESP. It is required to set up connection to the wifi network and the broker address. In the Serial input type:  

```ssid:NameOfTheNetwork```  
where NameOfTheNetwork is the name of the network ESP is to connect to.  

To set up the password type:  
```pass:PasswordToNetwork```  

To set up the broker IP:  
```brokerIP:BrokerIP```  

And to set the esp ID:
```espID:ESPID```

Now the ESP should connect to the network and to the broker. Note, that you will need to set this only once (The ESP will remember all of the provided configurations).

## Usage
To set up the devices and sensors on the ESP you need to send a configuration via MQTT. Configuration is in json and contains:

Array of:
**SensorConf:**
- TYPE {MVM, DIST, TEMP, CLK, BTN}   - Type of device that this object describes
- DATA []                            - where the sensor is plugged in (and other sensor-specific information)
- Channels: []                       - list of channels that this device publishes its state (Binary)
  - ID                                  - id of the channel 
  - ValueON                             - value of the sensor that triggers the 'ON' state 
  - ValueOFF                            - value of the sensor that triggers the 'OFF' state

**OutConf:**
- TYPE: {LED}                        
- DATA []
- Channels: []                       - list of channels that trigger the Device (logical 'or')
  - ChannelIDs: []                   - list of channels (logical 'and')
    - ID                                - id of the channel
    - negationFlag                      - if the value on the channel should be interpreted as negated

For the explanation of list of channelIDs see **Example** section.

## Example
Example configuration could look like:

```JSON
{
  "Sensors": [
    {
      "Type": "ButtonSensor",
      "Channels": [
          {
            "ID": "SensorButton",
            "ValueOFF": 125,
            "ValueON": 126
          }
      ],
      "Pin": [
        3
      ]
    }
  ],
  "OutDevices": [
    {
      "Type": "Serial",
      "Channels": [
        [
          {
            "ID": "SensorButton",
            "NegationFlag": true
          },
          {
            "ID": "ID2"
          }
        ], 
        [
          {
            "ID": "SensorButton"
          },
          {
            "ID": "ID3"
          }
        ]
      ]
    }
  ]
}
```
The Out Device's state will correspond with the logical statement:

```(~SensorButton && ID2) || (SensorButton && ID3)```

### Default Values
If some of those fields are not provided the default value will be taken:
- NegationFlag = false


## Contributing
TBD
## Deployment
TBD

## Built with
- [Arduino](https://www.arduino.cc/)
- [PlatformIO](http://platformio.org/)

## Authors
- **Maksymilian Wojczuk**
- **Tomasz Michalec**

## License
This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/Chimstaz/SmartHome/blob/master/LICENSE) file for details.

## Acknowledgements
- This is a student project
- don't hesitate to let us know about possible improvements or issues
