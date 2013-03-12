package com.rho.examples.megamodule;

import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

public class Megamodule extends MegamoduleBase implements IMegamodule {
    
    public Megamodule(String id) {
        super(id);
    }

    @Override
    public void getIntegerProperty(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setIntegerProperty(int value, IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void produceArray(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void produceHash(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void produceComplicatedResult(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void showAlertFromUIThread(String message, IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setPeriodicallyCallback(int periodInMilliseconds,
            IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void isPeriodicallyCallbackSetted(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void stopPeriodicallyCallback(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void typesTest(String paramStr, boolean paramBool, int paramInt,
            double paramFloat, List<Integer> paramArray,
            Map<String, String> paramHash, IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void complicatedTypesTest1(List<Map<String, Object>> paramArray,
            IMethodResult result) {
        // TODO Auto-generated method stub
        
    }


}