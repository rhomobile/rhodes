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
        return "0";
    }
    @Override
    public void enumerate(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

}