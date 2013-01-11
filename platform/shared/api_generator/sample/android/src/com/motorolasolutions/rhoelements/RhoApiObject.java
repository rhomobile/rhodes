package com.motorolasolutions.rhoelements;

public abstract class RhoApiObject implements IRhoApiObject {

    private String mId;
    
    public RhoApiObject(String id) {
        mId = id;
    }
    
    @Override
    public String getId() {
        return mId;
    }

}
