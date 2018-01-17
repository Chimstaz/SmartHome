package entity;

import java.util.List;

public class AndChannels {
    int Priority;
    List<Channel> Channels;


    public AndChannels(int priority, List<Channel> channels) {
        Priority = priority;
        Channels = channels;
    }

    public int getPriority() {
        return Priority;
    }

    public void setPriority(int priority) {
        Priority = priority;
    }

    public List<Channel> getChannels() {
        return Channels;
    }

    public void setChannels(List<Channel> channels) {
        Channels = channels;
    }
}
