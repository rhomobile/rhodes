package com.rhomobile.rhodes.extmanager;

public interface IRhoConfig {
    String getValue(String name);
    String getValue(String name, String defaultValue);
}
