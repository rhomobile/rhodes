package com.rho.examples.megamodule;

public class MegamoduleFactorySingleton {
    private static IMegamoduleFactory mFactory;
    public static void setInstance(IMegamoduleFactory factory) {
        mFactory = factory;
    }
    public static IMegamoduleFactory getInstance() {
        return mFactory;
    }
}
