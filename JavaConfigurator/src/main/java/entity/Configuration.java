package entity;

import java.util.ArrayList;
import java.util.List;

public class Configuration {
    private List<OutDevice> OutDevices;
    private List<Sensor> Sensors;

    public Configuration() {
        OutDevices = new ArrayList<>();
        Sensors = new ArrayList<>();
    }

    public Configuration(ArrayList<OutDevice> outDevices, ArrayList<Sensor> sensors) {
        OutDevices = outDevices;
        Sensors = sensors;
    }

    public List<OutDevice> getOutDevices() {
        return OutDevices;
    }

    public void setOutDevices(List<OutDevice> outDevices) {
        OutDevices = outDevices;
    }

    public List<Sensor> getSensors() {
        return Sensors;
    }

    public void setSensors(List<Sensor> sensors) {
        Sensors = sensors;
    }
}
