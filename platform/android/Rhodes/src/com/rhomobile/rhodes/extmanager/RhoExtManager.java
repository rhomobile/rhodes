package com.rhomobile.rhodes.extmanager;

import com.rhomobile.rhodes.Logger;

public class RhoExtManager extends Object {
    private static RhoExtManagerImpl ourRhoExtManager = null;

    public static RhoExtManagerImpl getImplementationInstance() {
        if (ourRhoExtManager == null) {
            ourRhoExtManager = new RhoExtManagerImpl();
        }
        return ourRhoExtManager;
    }

    /**
     * Gets resource id.
     * For example to get R.string.app_name value call getResourceId("string.app_name")
     * @param name - symbolic name of resource as it referenced at R class
     * @return resource id
     */
    public static int getResourceId(String name) {
        return RhoExtManagerImpl.getResId(name);
    }

    /**
     * @return IRhoExtManager instance
     */
    public static IRhoExtManager getInstance() {
        return getImplementationInstance();
    }

    /**
     * Convenient way to print trace message to rhodes log
     * @param tag - log source
     * @param msg - log message
     */
    public static void logT(String tag, String msg) {
        Logger.T(tag, msg);
    }

    /**
     * Convenient way to print info message to rhodes log
     * @param tag - log source
     * @param msg - log message
     */
    public static void logI(String tag, String msg) {
        Logger.I(tag, msg);
    }

    /**
     * Convenient way to print warning message to rhodes log
     * @param tag - log source
     * @param msg - log message
     */
    public static void logW(String tag, String msg) {
        Logger.W(tag, msg);
    }

    /**
     * Convenient way to print error message to rhodes log
     * @param tag - log source
     * @param msg - log message
     */
    public static void logE(String tag, String msg) {
        Logger.E(tag, msg);
    }

}