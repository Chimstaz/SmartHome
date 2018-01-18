package entity;

import java.util.ArrayList;
import java.util.List;

public class OutDevice {
    private String Type;
    private List<AndChannels> Channels;
    private List<Object> Data;

    public OutDevice(String type) {
        Channels = new ArrayList<>();
        Data = new ArrayList<>();
        this.Type = type;
    }

    public OutDevice(String type, List<AndChannels> channels, List<Object> data) {
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

    public List<AndChannels> getChannels() {
        return Channels;
    }

    public void setChannels(List<AndChannels> channels) {
        Channels = channels;
    }

    public List<Object> getData() {
        return Data;
    }

    public void setData(List<Object> data) {
        Data = data;
    }
}
