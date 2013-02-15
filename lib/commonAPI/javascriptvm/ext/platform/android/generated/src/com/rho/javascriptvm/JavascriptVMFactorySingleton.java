package com.rho.javascriptvm;

public class JavascriptVMFactorySingleton {
    private static IJavascriptVMFactory mFactory;
    public static void setInstance(IJavascriptVMFactory factory) {
        mFactory = factory;
    }
    public static IJavascriptVMFactory getInstance() {
        return mFactory;
    }
}
