import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import entity.*;

import java.lang.reflect.Array;
import java.util.ArrayList;

public class Configurator {
    public static void main(String[] args){
        ArrayList<OutDevice> outDevices = new ArrayList<>();
        ArrayList<Sensor> sensors = new ArrayList<>();

        OutDevice led = new OutDevice(DeviceType.LED.name());

        Channel sb1 = new Channel("SB1");
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



        OutDevice serial = new OutDevice(DeviceType.Serial.name());
        serial.setData(ledData);
        serial.setChannels(ledOrChannels);



        Configuration configuration = new Configuration();
        outDevices.add(led);
        outDevices.add(serial);
        configuration.setOutDevices(outDevices);



        Gson gson = new GsonBuilder().create();
        gson.toJson(configuration, System.out);
    }
}


