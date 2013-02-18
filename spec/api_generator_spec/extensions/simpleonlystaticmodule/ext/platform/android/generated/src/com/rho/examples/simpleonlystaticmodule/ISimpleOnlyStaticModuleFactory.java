package com.rho.examples.simpleonlystaticmodule;

import com.rhomobile.rhodes.api.IRhoApiFactory;
import com.rhomobile.rhodes.api.IRhoApiSingletonFactory;

public interface ISimpleOnlyStaticModuleFactory
    extends IRhoApiFactory<ISimpleOnlyStaticModule>,
            IRhoApiSingletonFactory<ISimpleOnlyStaticModuleSingleton> {

    @Override
    ISimpleOnlyStaticModuleSingleton getApiSingleton();

    @Override
    SimpleOnlyStaticModule getApiObject(String id);

}
