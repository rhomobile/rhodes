package com.motorolasolutions.rhoelements.barcode1;

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
    public void getProps(IMethodResult result) {
        result.set(mProps);
    }

    @Override
    public void getProps(String name, IMethodResult result) {
        result.set(mProps.get(name));
    }

    @Override
    public void getProps(List<String> names, IMethodResult result) {
        List<String> props = new ArrayList<String>();
        for (String name: names) {
            props.add(mProps.get(name));
        }
        result.set(props);
    }

    @Override
    public void takeBarcode(IMethodResult result) {
        result.set("0123456789");
    }

}