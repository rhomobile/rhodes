package com.rho.barcode;

import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

public class Barcode extends BarcodeBase implements IBarcode {

    
    public Barcode(String id) {
        super(id);
        
        setProperty("display", "LCD", new MethodResult());
        setProperty("sound", "Dolby", new MethodResult());
    }

    @Override
    public void start(IMethodResult result) {
        result.set("1234567890");
    }

    @Override
    public void stop(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void disable(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void take(Map<String, String> propertyMap, IMethodResult result) {
        result.set("1234567890");
    }

    @Override
    public void enable(Map<String, String> propertyMap, IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void getIlluminationMode(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setIlluminationMode(String value, IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

}