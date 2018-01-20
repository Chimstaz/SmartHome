import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import entity.*;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Configurator {
    public static void main(String[] args){
        Configuration configuration = ExampleConfigurations.getExamplePriorityGroup();

        Gson gson = new GsonBuilder().create();
        gson.toJson(configuration, System.out);
    }

    private static Configuration getShowConfigurationTom() {
//        Movement Led
        AndChannels ledAndChannels1 = new AndChannels(2, Collections.singletonList(new Channel("MVM", true)));
        AndChannels ledAndChannels2 = new AndChannels(1, Collections.singletonList(new Channel("YLed", true)));
        List<Object> ledData = Collections.singletonList(5);
        List<AndChannels> ledAndChannelsList = new ArrayList<>();
        ledAndChannelsList.add(ledAndChannels1);
        ledAndChannelsList.add(ledAndChannels2);
        OutDevice mvmLed = new OutDevice(DeviceType.LED.name(), ledAndChannelsList, ledData);

//        Button Sensor
        List<Channel> sb1ChannelList = Collections.singletonList(new Channel("SB1", 126, 125));
        Sensor buttonSensor = new Sensor(DeviceType.BTN.name(), sb1ChannelList, Collections.singletonList(14));

//        DisableSwitch
        List<Channel> DBChannelList = Collections.singletonList(new Channel("DB1", 126,125));
        List<Object> dbData = new ArrayList<>();
        dbData.add(13);
        dbData.add(12);
        Sensor dbSwitch = new Sensor(DeviceType.DISABLESWITCH.name(), DBChannelList, dbData);


        List<Sensor> sensors = new ArrayList<>();
        sensors.add(dbSwitch);
        sensors.add(buttonSensor);

        return new Configuration(Collections.singletonList(mvmLed), sensors);
    }

    private static Configuration getShowConfigurationMax() {
        //        MOVE Sensor
        List<Channel> mvmChannels = Collections.singletonList(new Channel("MVM", 125,127));
        List<Object> mvmData = Collections.singletonList(13);
        Sensor moveSensor = new Sensor(DeviceType.MVM.name(), mvmChannels, mvmData);

//        Blinking LED
        List<AndChannels> blinkingLedChannels = Collections
                .singletonList(new AndChannels(0, Collections.singletonList(new Channel("TIME6"))));
        List<Object> blinkingLEDData = Collections.singletonList(2);
        OutDevice blinkingLED = new OutDevice(DeviceType.LED.name(), blinkingLedChannels, blinkingLEDData);

//      Most complicated LED
        AndChannels ledAndChannels1 = new AndChannels(1, Collections.singletonList(new Channel("DB1")));
        AndChannels ledAndChannels2 = new AndChannels(2, Collections.singletonList(new Channel("SB1")));
        List<Object> ledData = Collections.singletonList(12);
        List<AndChannels> ledAndChannelsList = new ArrayList<>();
        ledAndChannelsList.add(ledAndChannels1);
        ledAndChannelsList.add(ledAndChannels2);
        OutDevice complicatedLed = new OutDevice(DeviceType.LED.name(), ledAndChannelsList, ledData);

//        TIME Sensor
        List<Object> timeSensorData = Collections.singletonList(6);
        List<Channel> timerChannels = Collections.singletonList(new Channel("TIME6", 2,3));
        Sensor timeSensor = new Sensor(DeviceType.TIME.name(), timerChannels, timeSensorData);


        ArrayList<OutDevice> outDevices = new ArrayList<>();
        ArrayList<Sensor> sensors = new ArrayList<>();
        outDevices.add(blinkingLED);
        outDevices.add(complicatedLed);
        sensors.add(moveSensor);
        sensors.add(timeSensor);
        return new Configuration(outDevices, sensors);
    }

    private static Configuration sampleConfiguration() {
        ArrayList<OutDevice> outDevices = new ArrayList<>();
        ArrayList<Sensor> sensors = new ArrayList<>();


//        LED --------------------------------------------------
        OutDevice led = new OutDevice(DeviceType.LED.name());

        Channel sb1 = new Channel("SB1", true);
        Channel sb2 = new Channel("SB2");

        ArrayList<Channel> sb1ChannelList = new ArrayList<>();
        sb1ChannelList.add(sb1);
        ArrayList<Channel> sb2ChannelList = new ArrayList<>();
        sb2ChannelList.add(sb2);

        ArrayList<AndChannels> ledOrChannels = new ArrayList<>();
        ledOrChannels.add(new AndChannels(1, sb1ChannelList));
        ledOrChannels.add(new AndChannels(2, sb2ChannelList));

        led.setChannels(ledOrChannels);

        ArrayList<Object> ledData = new ArrayList<>();
        ledData.add(16);
        led.setData(ledData);

//        SERIAL ---------------------------------------------------
        OutDevice serial = new OutDevice(DeviceType.SERIAL.name());
        serial.setData(ledData);
        serial.setChannels(ledOrChannels);

//        SENSOR Button -------------------------------------------------
        Sensor button = new Sensor(DeviceType.BTN.name());
        Channel sb1In = new Channel("SB1", 126, 125);

//        button Channels
        List<Channel> sensorChannelList = new ArrayList<>();
        sensorChannelList.add(sb1In);
        button.setChannels(sensorChannelList);
//        button data
        ArrayList<Object> sensorData = new ArrayList<>();
        sensorData.add(5);
        button.setData(sensorData);


        Configuration configuration = new Configuration();
        outDevices.add(led);
        outDevices.add(serial);
        configuration.setOutDevices(outDevices);

        sensors.add(button);
        configuration.setSensors(sensors);
        return configuration;
    }
}


