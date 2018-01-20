import com.sun.tools.corba.se.idl.constExpr.And;
import entity.*;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class ExampleConfigurations {

    static Configuration getExampleConfigurationWithPhotoSensAndDisableSwitch() {
        //        DisableSwitch
        List<Channel> DBChannelList = Collections.singletonList(new Channel("ManualSwitch", 126, 125));
        List<Object> dbData = new ArrayList<>();
        dbData.add(13);
        dbData.add(12);
        Sensor dbSwitch = new Sensor(DeviceType.DISABLESWITCH.name(), DBChannelList, dbData);

//        Photo Sensor
        List<Channel> photoChannelList = Collections.singletonList(new Channel("LightSensor", 350, 700));
        Sensor photoSensor = new Sensor(DeviceType.PHOTO.name(), photoChannelList, null);

        Configuration configuration = new Configuration();
        List<Sensor> sensors = new ArrayList<>();
        sensors.add(photoSensor);
        sensors.add(dbSwitch);
        configuration.setSensors(sensors);
        return configuration;
    }

    static Configuration getExampleLEDForPhotoAndManual() {

        AndChannels photoChannel = new AndChannels(2, Collections.singletonList(new Channel("LightSensor")));
        AndChannels manualChannel = new AndChannels(1, Collections.singletonList(new Channel("ManualSwitch")));

        List<AndChannels> andChannels = new ArrayList<>();
        andChannels.add(photoChannel);
        andChannels.add(manualChannel);
        OutDevice led = new OutDevice(DeviceType.LED.name(), andChannels, Collections.singletonList(16));

        return new Configuration(Collections.singletonList(led), new ArrayList<>());
    }

    static Configuration getExamplePriorityGroup() {
        AndChannels bedroomGroupChannel = new AndChannels(2, Collections.singletonList(new Channel("Sypialnia")));

//        Bedroom LED1
        AndChannels manualBedroomLed1 = new AndChannels(1, Collections.singletonList(new Channel("SypialniaLED1")));
        List<AndChannels> led1Channels = new ArrayList<>();
        led1Channels.add(manualBedroomLed1);
        led1Channels.add(bedroomGroupChannel);
        OutDevice bedroomLed1 = new OutDevice(DeviceType.LED.name(), led1Channels, Collections.singletonList(13));


//        Bedroom LED2
        AndChannels manualBedroomLed2 = new AndChannels(1, Collections.singletonList(new Channel("SypialniaLED1")));
        List<AndChannels> led2Channels = new ArrayList<>();
        led2Channels.add(manualBedroomLed2);
        led2Channels.add(bedroomGroupChannel);
        OutDevice bedroomLed2 = new OutDevice(DeviceType.LED.name(), led2Channels, Collections.singletonList(14));

        List<OutDevice> outDevices = new ArrayList<>();
        outDevices.add(bedroomLed1);
        outDevices.add(bedroomLed2);

        return new Configuration(outDevices, new ArrayList<>());

    }
}
