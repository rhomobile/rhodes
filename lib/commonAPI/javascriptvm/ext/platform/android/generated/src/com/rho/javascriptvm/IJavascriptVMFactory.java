package com.rho.javascriptvm;

import com.rhomobile.rhodes.api.IRhoApiFactory;
import com.rhomobile.rhodes.api.IRhoApiSingletonFactory;

public interface IJavascriptVMFactory
    extends IRhoApiFactory<IJavascriptVM>,
            IRhoApiSingletonFactory<IJavascriptVMSingleton> {

    @Override
    IJavascriptVMSingleton getApiSingleton();

    @Override
    JavascriptVM getApiObject(String id);

}
