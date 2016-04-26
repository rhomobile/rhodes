package com.rho.rhoelements.barcode1;

import com.rhomobile.rhodes.api.IRhoApiFactory;
import com.rhomobile.rhodes.api.IRhoApiSingletonFactory;

public interface IBarcode1Factory
    extends IRhoApiFactory<IBarcode1>,
            IRhoApiSingletonFactory<IBarcode1Singleton> {

    @Override
    IBarcode1Singleton getApiSingleton();

    @Override
    IBarcode1 getApiObject(String id);

}
