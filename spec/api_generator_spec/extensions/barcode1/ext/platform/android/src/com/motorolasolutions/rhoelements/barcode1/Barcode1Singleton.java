package com.motorolasolutions.rhoelements.barcode1;

import java.util.LinkedList;
import java.util.List;

import com.motorolasolutions.rhoelements.IMethodResult;
import com.motorolasolutions.rhoelements.IRhoApiSingleton;

class Barcode1Singleton extends Barcode1SingletonBase implements IRhoApiSingleton {
    public Barcode1Singleton(Barcode1Factory factory) {
        super(factory);
    }

    List<String> getIDs() {
        List<String> ids = new LinkedList<String>();
        ids.add("SCN1");
        ids.add("SCN2");
        return ids;
    }
    
    @Override
    protected String getInitialDefaultID() {
        return getIDs().get(0);
    }

    @Override
    public void enumerate(IMethodResult res) {
        res.set(getIDs());
    }
}