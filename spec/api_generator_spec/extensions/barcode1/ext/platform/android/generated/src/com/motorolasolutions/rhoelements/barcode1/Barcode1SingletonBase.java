package com.motorolasolutions.rhoelements.barcode1;

import com.rhomobile.rhodes.api.RhoApiSingleton;

public abstract class Barcode1SingletonBase extends RhoApiSingleton<IBarcode1, Barcode1Factory> {

    public Barcode1SingletonBase(Barcode1Factory factory) {
        super(factory);
    }
}
