package com.rhomobile.rhodes.util;

import java.util.Collection;
import java.util.Map;
import java.util.Set;

import org.json.JSONException;
import org.json.JSONStringer;


import android.os.Bundle;

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
            //Logger.T(TAG, "Java toString(): " + mObject.toString());
            parse(mObject);
        } catch(Throwable exc) {
            throw new RuntimeException(exc);
        }
        
        return mStringer.toString();
    }
    
    protected void parse(Object obj) throws JSONException {
        Logger.T(TAG, "Parse " + obj.getClass().getSimpleName());
        if (Map.class.isInstance(obj)) {
            Logger.T(TAG, "Parsing Map instance >>>");
            parseMap((Map<String, ?>)obj);
            Logger.T(TAG, "Finishing Map instance <<<");
            return;
        }
        if (Collection.class.isInstance(obj)) {
            Logger.T(TAG, "Parsing Collection instance >>>");
            parseCollection((Collection<?>)obj);
            Logger.T(TAG, "Finishing Collection instance <<<");
            return;
        }
        if (Bundle.class.isInstance(obj)) {
            Logger.T(TAG, "Parsing Bundle instance >>>");
            parseBundle((Bundle)obj);
            Logger.T(TAG, "Finishing Bundle instance <<<");
            return;
        }
        
        mStringer.value(obj);
    }
    
    protected void parseMap(Map<String, ?> map) throws JSONException {
        Logger.D(TAG, map.toString());
        mStringer.object();
        for(Map.Entry<String, ?> entry: map.entrySet()) {
            mStringer.key(entry.getKey());
            parse(entry.getValue());
        }
        mStringer.endObject();
    }

    protected void parseCollection(Collection<?> list) throws JSONException {
        Logger.D(TAG, list.toString());
        mStringer.array();
        for(Object item: list) {
            parse(item);
        }
        mStringer.endArray();
    }

    protected void parseBundle(Bundle bundle) throws JSONException {
        Logger.D(TAG, bundle.toString());
        mStringer.object();

        Set<String> keys = bundle.keySet();
        for (String key : keys) {

            // Skip system related keys
            if(key.equals("from"))
                continue;
            if(key.equals("collapse_key"))
                continue;
            if(key.equals("phone_id")) {
                continue;
            }

            mStringer.key(key);
            parse(bundle.get(key));
        }
        mStringer.endObject();
    }

}
