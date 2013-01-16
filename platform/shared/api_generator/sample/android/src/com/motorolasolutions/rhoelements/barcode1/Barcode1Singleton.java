package com.motorolasolutions.rhoelements.barcode1;

import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import com.motorolasolutions.rhoelements.Barcode1SingletonBase;
import com.motorolasolutions.rhoelements.IBarcode1;
import com.motorolasolutions.rhoelements.IMethodResult;

class Barcode1Singleton extends Barcode1SingletonBase {

    private Map<String, IBarcode1> sActiveBarcodes = new LinkedHashMap<String, IBarcode1>();
    
    public static void init() { setInstance(new Barcode1Singleton()); }

    @Override
    public IBarcode1 getApiObject(final String id) {
        IBarcode1 res = sActiveBarcodes.get(id);
        if (res == null) {
            res = new Barcode1(id);
            sActiveBarcodes.put(id, res);
        }
        return res;
    }

    List<String> getIds() {
        List<String> ids = new LinkedList<String>();
        ids.add("SCN1");
        ids.add("SCN2");
        return ids;
    }
    
    protected String createDefaultId() {
        return getIds().get(0);
    }

    @Override
    public void enumerate(IMethodResult res) {
        res.set(getIds());
    }
}