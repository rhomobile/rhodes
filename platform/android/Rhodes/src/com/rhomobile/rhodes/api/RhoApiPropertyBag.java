package com.rhomobile.rhodes.api;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class RhoApiPropertyBag implements
        IRhoApiPropertyBag {

    private HashMap<String, String> mProperties;
    private List<String> mAllowedNames;
    
    public boolean isAllowed(String name) {
        return (mAllowedNames == null) || mAllowedNames.contains(name);
    }
    
    public Map<String, String> getPropertiesMap() { return mProperties; }

    public RhoApiPropertyBag() {
        mAllowedNames = null;
        mProperties = new HashMap<String, String>();
    }

    public RhoApiPropertyBag(List<String> allowedNames) {
        mAllowedNames = allowedNames;
        mProperties = new HashMap<String, String>();
    }

    @Override
    public void getAllProperties(IMethodResult result) {
        result.set(getPropertiesMap());
    }

    @Override
    public void getProperty(String name, IMethodResult result) {
        if (isAllowed(name)) {
            result.set(getPropertiesMap().get(name));
        } else {
            result.setArgError("Unknown property: '" + name + "'");
        }
    }

    @Override
    public void getProperties(List<String> names, IMethodResult result) {
        Map<String, String> subset = new HashMap<String, String>();
        for(String name: names) {
            if (isAllowed(name)) {
                subset.put(name, getPropertiesMap().get(name));
            } else {
                result.setArgError("Unknown property: '" + name + "'");
                return;
            }
        }
        result.set(subset);
    }

    @Override
    public void setProperty(String name, String val, IMethodResult result) {
        if (isAllowed(name)) {
            getPropertiesMap().put(name, val);
        } else {
            result.setArgError("Unknown property: '" + name + "'");
        }
    }

    @Override
    public void setProperties(Map<String, String> props, IMethodResult result) {
        Set<String> names = props.keySet();
        if (mAllowedNames.containsAll(names)) {
            getPropertiesMap().putAll(props);
        } else {
            names.removeAll(getPropertiesMap().keySet());
            StringBuffer strNames = new StringBuffer();
            for (String name: names) {
                strNames.append("'").append(name).append("', ");
            }
            strNames.setLength(strNames.length() - 2);
            result.setArgError("Unknown properties: " + strNames);
        }

    }

    @Override
    public void clearAllProperties(IMethodResult result) {
        getPropertiesMap().clear();
    }
}
