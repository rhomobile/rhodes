package com.rhomobile.rhodes;

import java.util.HashMap;

public class RhoRubySingleton
{

    private static final String TAG = "RhoRubySingletonJava";

    public interface RhoRunnable {
        public void rhoRun();
    }

    public interface IRubyNativeCallback {
        public void onRubyNative(String json_with_param);
    }

    private static RhoRubySingleton singleton = null;

    private static HashMap<Integer, RhoRunnable> ourRunnablesMap = new HashMap<Integer, RhoRunnable>();
    private static int ourRunnableIDCounter = 1;

    private static HashMap<String, IRubyNativeCallback> ourNativeCallbacsMap = new HashMap<String, IRubyNativeCallback>();


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

    public void addRubyNativeCallback(String callback_id, IRubyNativeCallback callback) {
        Logger.I(TAG,"add RubyNativeCallback ["+callback_id+"]");
        ourNativeCallbacsMap.put(callback_id, callback);
        addRubyNativeCallbackNative(callback_id);
    }

    public void removeRubyNativeCallback(String callback_id) {
        Logger.I(TAG,"remove RubyNativeCallback ["+callback_id+"]");
        ourNativeCallbacsMap.remove(callback_id);
        removeRubyNativeCallbackNative(callback_id);
    }

    private static void callRubyNativeCallback(String callback_id, String json_with_param){
        IRubyNativeCallback callback = ourNativeCallbacsMap.get(callback_id);
        if (callback != null) {
            Logger.I(TAG,"call RubyNativeCallback ["+callback_id+"] with param ["+json_with_param+"]");
            callback.onRubyNative(json_with_param);
        }
        else {
            Logger.E(TAG,"Error: RubyNativeCallback ["+callback_id+"] NOT FOUND !");
        }
    }

    private native void addRubyNativeCallbackNative(String callback_id);
    private native void removeRubyNativeCallbackNative(String callback_id);

    private native void executeInRubyThreadNative(int id);
}
