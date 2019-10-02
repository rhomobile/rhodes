package com.rhomobile.rhodes;

import java.util.HashMap;

public class RhoRubySingleton
{

    public interface RhoRunnable {
        public void rhoRun();
    }


    private static RhoRubySingleton singleton = null;

    private static HashMap<Integer, RhoRunnable> ourRunnablesMap = new HashMap<Integer, RhoRunnable>();
    private static int ourRunnableIDCounter = 1;


    public static RhoRubySingleton instance()
    {
        if(singleton == null) {
            singleton = new RhoRubySingleton();
        }
        return singleton;
    }

    public void executeInRubyThread(RhoRunnable command)
    {
            int id = 0;
            synchronized (ourRunnablesMap) {
                id = ourRunnableIDCounter++;
                ourRunnablesMap.put(new Integer(id), command);
            }
            executeInRubyThreadNative(id);
    }

    private static void executeRunnableByID(int id) {
        RhoRunnable command = null;
        synchronized (ourRunnablesMap) {
            command = ourRunnablesMap.get(new Integer(id));
            ourRunnablesMap.remove(new Integer(id));
        }
        if (command != null) {
            command.rhoRun();
        }
    }

    public native String executeRubyMethodWithJSON(String full_class_name, String method_name, String parameters);
    public native String getRubyServerURL();

    private native void executeInRubyThreadNative(int id);
}
