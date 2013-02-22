package com.rho.<%= namefixed %>;

import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

public class <%= namecamelcase %> extends <%= namecamelcase %>Base implements I<%= namecamelcase %> {
    
    public <%= namecamelcase %>(String id) {
        super(id);
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

}