package com.motorolasolutions.rhoelements.barcode1;

import com.motorolasolutions.rhoelements.RhoApiFactoryBase;
import com.motorolasolutions.rhoelements.IRhoApiFactory;
import com.motorolasolutions.rhoelements.IRhoApiSingleton;
import com.motorolasolutions.rhoelements.IRhoApiSingletonFactory;

public class Barcode1Factory
        extends RhoApiFactoryBase<Barcode1, Barcode1Singleton>
        implements IRhoApiFactory<IBarcode1>, IRhoApiSingletonFactory<IRhoApiSingleton> {

    @Override
    protected Barcode1Singleton createSingleton() {
        return new Barcode1Singleton(this);
    }

    @Override
    protected Barcode1 createApiObject(String id) {
        return new Barcode1(id);
    }
}
