package com.rho.examples.megamodule;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;

class MegamoduleSingleton extends MegamoduleSingletonBase implements IMegamoduleSingleton {
    public MegamoduleSingleton(MegamoduleFactory factory) {
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

    @Override
    public void getObjectsCount(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void getObjectByIndex(int index, IMethodResult result) {
        // TODO Auto-generated method stub
        
    }
}