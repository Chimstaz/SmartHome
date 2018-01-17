package entity;

import java.util.ArrayList;
import java.util.List;

public class Sensor {
    private String Type;
    private List<Channel> Channels;
    private List<Object> Data;


    public Sensor(String type) {
        Channels = new ArrayList<>();
        Data = new ArrayList<>();
        this.Type = type;
    }

    public Sensor(String type, List<Channel> channels, List<Object> data) {
        Type = type;
        Channels = channels;
        Data = data;
    }

    public String getType() {
        return Type;
    }

    public void setType(String type) {
        Type = type;
    }

    public List<Channel> getChannels() {
        return Channels;
    }

    public void setChannels(List<Channel> channels) {
        Channels = channels;
    }

    public List<Object> getData() {
        return Data;
    }

    public void setData(List<Object> data) {
        Data = data;
    }
}
