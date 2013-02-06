package com.rhomobile.rhodes.api;

public interface IRhoApiSingleton {
    String getDefaultID();
    void setDefaultID(String id);
    void enumerate(IMethodResult res);
}
