package com.rho.rhoelements.barcode1;

public class Barcode1FactorySingleton {
    private static IBarcode1Factory mFactory;
    public static void setInstance(IBarcode1Factory factory) {
        mFactory = factory;
    }
    public static IBarcode1Factory getInstance() {
        return mFactory;
    }
}
