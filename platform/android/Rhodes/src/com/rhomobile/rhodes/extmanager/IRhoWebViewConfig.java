package com.rhomobile.rhodes.extmanager;

public interface IRhoWebViewConfig {
//    void set(String name, boolean val);
//    void set(String name, int val);
//    void set(String name, double val);
//    void set(String name, String val);

    boolean getBool(String name);
    int getInt(String name);
    double getDouble(String name);
    String getString(String name);
}
