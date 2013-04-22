package com.rho.genpropbag;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;

class GenPropBagSingleton extends GenPropBagSingletonBase implements IGenPropBagSingleton {
    public GenPropBagSingleton(GenPropBagFactory factory) {
        super(factory);
    }
    @Override
    protected String getInitialDefaultID() {
        return "ID1";
    }
    @Override
    public void enumerate(IMethodResult result) {
        result.collect("ID1");
        result.collect("ID2");
        result.set();
        
    }

}