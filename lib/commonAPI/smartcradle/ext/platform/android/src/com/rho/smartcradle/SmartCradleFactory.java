package com.rho.smartcradle;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class SmartCradleFactory implements ISmartCradleFactory{
	
	private SmartCradleSingleton singleton=null;

	@Override
	public
	ISmartCradleSingleton getApiSingleton() {
		if(singleton==null)
		{
			singleton=new SmartCradleSingleton();
		}
		return singleton;
		
	}

	@Override
	public ISmartCradle getApiObject(String id) {
		
		return null;
	}

 /*   @Override
    protected SmartcradleSingleton createSingleton() {
        return new SmartcradleSingleton(this);
    }

    @Override
    protected Smartcradle createApiObject(String id) {
        return new Smartcradle(id);
    }
    */
}
