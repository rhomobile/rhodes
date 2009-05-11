package j2me.lang;

import j2me.util.HashMap;

public class SystemMe {

    public static long nanoTime()
    {
        return System.currentTimeMillis()*1000000; 
    }
    
    public static j2me.util.Map getenv() {
        //TODO: getenv
        
        return new HashMap();
    }
    
    public static String getenv(String name) {
        //TODO: getenv
        return "";
    }
        
    public static String setProperty(String key, String value) {
        //TODO: setProperty
        return "";
    }
}
