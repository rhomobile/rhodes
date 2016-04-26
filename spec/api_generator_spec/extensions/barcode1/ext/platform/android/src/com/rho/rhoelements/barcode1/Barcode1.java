package com.rho.rhoelements.barcode1;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.RhoApiObject;

public class Barcode1 extends RhoApiObject implements IBarcode1 {

    private Map<String, String> mProps = new HashMap<String, String>();
    
    public Barcode1(String id) {
        super(id);
        mProps.put("display", "LCD");
        mProps.put("sound", "Dolby");
    }

    @Override
    public void getAllProperties(IMethodResult result) {
        result.set(mProps);
    }

    @Override
    public void getProperty(String name, IMethodResult result) {
        result.set(mProps.get(name));
    }

    @Override
    public void getProperties(List<String> names, IMethodResult result) {
        List<String> props = new ArrayList<String>();
        for (String name: names) {
            props.add(mProps.get(name));
        }
        result.set(props);
    }

    @Override
    public void setProperty(String name, String val, IMethodResult result) {
        mProps.put(name, val);
    }

    @Override
    public void setProperties(Map<String, String> props, IMethodResult result) {
        mProps.putAll(props);
    }

    @Override
    public void clearAllProperties(IMethodResult result) {
        mProps.clear();
    }

    @Override
    public void takeBarcode(IMethodResult result) {
        result.set("0123456789");
    }

}