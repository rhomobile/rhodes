package com.rho.rhoelements.barcode1;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class Barcode1Factory
        extends RhoApiFactory<Barcode1, Barcode1Singleton>
        implements IBarcode1Factory {

    @Override
    protected Barcode1Singleton createSingleton() {
        return new Barcode1Singleton(this);
    }

    @Override
    protected Barcode1 createApiObject(String id) {
        return new Barcode1(id);
    }
}
