package com.rhomobile.rhodes.extmanager;

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
        String[] nameComponents = name.split("\\.", 2);
        if (nameComponents.length == 2) {
            return RhoExtManagerImpl.getResId(nameComponents[0], nameComponents[1]);
        }
        throw new IllegalArgumentException(name);
    }

    /**
     * Gets resource id.
     * For example to get R.string.app_name value call getResourceId("string", "app_name")
     * @param className - symbolic name of R sub class containing an id
     * @param idName - symbolic name of field in the sub class
     * @return resource id
     */
    public static int getResourceId(String className, String idName) {
        return RhoExtManagerImpl.getResId(className, idName);
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
        getImplementationInstance().logT(tag, msg);
    }

    /**
     * Convenient way to print info message to rhodes log
     * @param tag - log source
     * @param msg - log message
     */
    public static void logI(String tag, String msg) {
        getImplementationInstance().logI(tag, msg);
    }

    /**
     * Convenient way to print warning message to rhodes log
     * @param tag - log source
     * @param msg - log message
     */
    public static void logW(String tag, String msg) {
        getImplementationInstance().logW(tag, msg);
    }

    /**
     * Convenient way to print error message to rhodes log
     * @param tag - log source
     * @param msg - log message
     */
    public static void logE(String tag, String msg) {
        getImplementationInstance().logE(tag, msg);
    }

}