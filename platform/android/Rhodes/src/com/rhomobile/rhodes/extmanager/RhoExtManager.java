package com.rhomobile.rhodes.extmanager;

public class RhoExtManager extends Object {

    private static RhoExtManagerImpl ourRhoExtManager = null;

    public static RhoExtManagerImpl getImplementationInstance() {
        if (ourRhoExtManager == null) {
            ourRhoExtManager = new RhoExtManagerImpl();
        }
        return ourRhoExtManager;
    }

    public static IRhoExtManager getInstance() {
        return getImplementationInstance();
    }

}