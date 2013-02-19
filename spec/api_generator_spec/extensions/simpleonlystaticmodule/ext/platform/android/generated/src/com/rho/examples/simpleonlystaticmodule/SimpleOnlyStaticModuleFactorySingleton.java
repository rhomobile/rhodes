package com.rho.examples.simpleonlystaticmodule;

public class SimpleOnlyStaticModuleFactorySingleton {
    private static ISimpleOnlyStaticModuleFactory mFactory;
    public static void setInstance(ISimpleOnlyStaticModuleFactory factory) {
        mFactory = factory;
    }
    public static ISimpleOnlyStaticModuleFactory getInstance() {
        return mFactory;
    }
}
