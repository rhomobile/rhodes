package com.rho.genpropbag;

import java.util.HashMap;
import java.util.Map;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class GenPropBagFactory
        extends RhoApiFactory< GenPropBag, GenPropBagSingleton>
        implements IGenPropBagFactory {

    GenPropBagSingleton mSingleton = new GenPropBagSingleton(this);
    Map<String, GenPropBag> mObjects = new HashMap<String, GenPropBag>();

    public GenPropBagFactory() {
        mObjects.put("ID1", new GenPropBag("ID1"));
        mObjects.put("ID2", new GenPropBag("ID2"));
    }
    
    @Override
    protected GenPropBagSingleton createSingleton() {
        return mSingleton; 
    }

    @Override
    protected GenPropBag createApiObject(String id) {
        return mObjects.get(id);
    }
}
