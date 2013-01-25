package com.motorolasolutions.rhoelements.barcode1;

import com.motorolasolutions.rhoelements.IRhoApiFactory;
import com.motorolasolutions.rhoelements.IRhoApiSingletonFactory;

public interface IBarcode1Factory
    extends IRhoApiFactory<IBarcode1>,
            IRhoApiSingletonFactory<IBarcode1Singleton> {

    @Override
    IBarcode1Singleton getApiSingleton();

    @Override
    IBarcode1 getApiObject(String id);

}
