package com.rhomobile.rhodes.extmanager;

public interface IRhoConfig {
    boolean isExist(String name);
    
    String getString(String name);

    boolean getBool(String name, boolean defValue);
    int getInt(String name, int defValue);
    double getDouble(String name, double defValue);

}
