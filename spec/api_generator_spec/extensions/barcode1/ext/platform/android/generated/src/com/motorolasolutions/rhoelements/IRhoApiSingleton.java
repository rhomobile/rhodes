package com.motorolasolutions.rhoelements;

public interface IRhoApiSingleton {
    String getDefaultID();
    void setDefaultID(final String id);
    void enumerate(IMethodResult res);
}
