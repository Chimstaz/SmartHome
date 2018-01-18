package entity;

public class Channel {
    private String ID;
    private Boolean negationFlag;
    private Integer ValueON;
    private Integer ValueOFF;

    public Channel(String ID, Boolean negationFlag) {
        this.ID = ID;
        this.negationFlag = negationFlag;
    }

    public Channel(String ID) {
        this.ID = ID;
    }

    public Channel(String ID, Integer valueON, Integer valueOFF) {
        this.ID = ID;
        ValueON = valueON;
        ValueOFF = valueOFF;
    }

    public Boolean getNegationFlag() {
        return negationFlag;
    }

    public void setNegationFlag(Boolean negationFlag) {
        this.negationFlag = negationFlag;
    }

    public Integer getValueON() {
        return ValueON;
    }

    public void setValueON(Integer valueON) {
        ValueON = valueON;
    }

    public Integer getValueOFF() {
        return ValueOFF;
    }

    public void setValueOFF(Integer valueOFF) {
        ValueOFF = valueOFF;
    }

    public String getID() {
        return ID;
    }

    public void setID(String ID) {
        this.ID = ID;
    }
}
