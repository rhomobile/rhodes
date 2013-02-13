package com.rho.barcode;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class BarcodeFactory
        extends RhoApiFactory<Barcode, BarcodeSingleton>
        implements IBarcodeFactory {

    @Override
    protected BarcodeSingleton createSingleton() {
        return new BarcodeSingleton(this);
    }

    @Override
    protected Barcode createApiObject(String id) {
        return new Barcode(id);
    }
}
