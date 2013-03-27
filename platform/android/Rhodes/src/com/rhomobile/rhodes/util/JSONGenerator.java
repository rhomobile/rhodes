package com.rhomobile.rhodes.util;

import java.util.Collection;
import java.util.Map;

import org.json.JSONException;
import org.json.JSONStringer;


import com.rhomobile.rhodes.Logger;

public class JSONGenerator {
    private static final String TAG = JSONGenerator.class.getSimpleName();
    private JSONStringer mStringer;
    private Object mObject;
    
    protected JSONStringer getStringer() { return mStringer; }

    public JSONGenerator(Object obj) {
        mObject = obj;
    }
    
    public String toString() {
        try {
            if(mStringer == null) {
                mStringer = new JSONStringer();
            }
            parse(mObject);
        } catch(Throwable exc) {
            throw new RuntimeException(exc);
        }
        
        return mStringer.toString();
    }
    
    protected void parse(Object obj) throws JSONException {
        if(Map.class.isInstance(mObject)) {
            Logger.T(TAG, "Parsing Map instance >>>");
            parse((Map<String, ?>)mObject);
            Logger.T(TAG, "Finishing Map instance <<<");
            return;
        }
        if(Collection.class.isInstance(mObject)) {
            Logger.T(TAG, "Parsing Collection instance >>>");
            parse((Collection<?>)mObject);
            Logger.T(TAG, "Finishing Collection instance <<<");
            return;
        }
        
        mStringer.value(obj);
    }
    
    protected void parse(Map<String, ?> map) throws JSONException {
        mStringer.object();
        for(Map.Entry<String, ?> entry: map.entrySet()) {
            mStringer.key(entry.getKey());
            parse(entry.getValue());
        }
        mStringer.endObject();
    }

    protected void parse(Collection<?> list) throws JSONException {
        mStringer.array();
        for(Object item: list) {
            parse(item);
        }
        mStringer.endArray();
    }

}
