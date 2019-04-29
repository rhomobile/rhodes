package com.rhomobile.rhodes;

public class RhoRubySingleton
{
    private static RhoRubySingleton singleton = null;
    public static RhoRubySingleton instance()
    {
        if(singleton == null) {
            singleton = new RhoRubySingleton();
        }
        return singleton;
    }

    public native String executeRubyMethodWithJSON(String full_class_name, String method_name, String parameters);
}
