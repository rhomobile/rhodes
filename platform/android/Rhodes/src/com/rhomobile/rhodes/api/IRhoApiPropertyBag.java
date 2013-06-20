package com.rhomobile.rhodes.api;

import java.util.List;
import java.util.Map;

public interface IRhoApiPropertyBag {
    void getAllProperties(IMethodResult result);
    void getProperty(String name, IMethodResult result);
    void getProperties(List<String> names, IMethodResult result);
    void setProperty(String name, String val, IMethodResult result);
    void setProperties(Map<String, String> props, IMethodResult result);
    //void clearAllProperties(IMethodResult result);
}
