package entity;

import java.util.List;

public class Sensor {
    private String Type;
    private List<List<Channel>> Channels;
    private List<Object> Data;

    public Sensor(String type, List<List<Channel>> channels, List<Object> data) {
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

    public List<List<Channel>> getChannels() {
        return Channels;
    }

    public void setChannels(List<List<Channel>> channels) {
        Channels = channels;
    }

    public List<Object> getData() {
        return Data;
    }

    public void setData(List<Object> data) {
        Data = data;
    }
}
