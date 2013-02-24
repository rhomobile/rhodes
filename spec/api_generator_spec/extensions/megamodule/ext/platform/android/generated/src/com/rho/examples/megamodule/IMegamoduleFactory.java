package com.rho.examples.megamodule;

import com.rhomobile.rhodes.api.IRhoApiFactory;
import com.rhomobile.rhodes.api.IRhoApiSingletonFactory;

public interface IMegamoduleFactory
    extends IRhoApiFactory<IMegamodule>,
            IRhoApiSingletonFactory<IMegamoduleSingleton> {

    @Override
    IMegamoduleSingleton getApiSingleton();

    @Override
    IMegamodule getApiObject(String id);

}
