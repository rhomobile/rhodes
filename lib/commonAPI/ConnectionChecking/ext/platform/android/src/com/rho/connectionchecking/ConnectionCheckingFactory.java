package com.rho.connectionchecking;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class ConnectionCheckingFactory implements IConnectionCheckingFactory {

	private ConnectionCheckingSingleton singleton;

	@Override
	public
	IConnectionCheckingSingleton getApiSingleton() {
		if(singleton == null) singleton = new ConnectionCheckingSingleton(this);
		return singleton;
	}

	@Override
	public
	IConnectionChecking getApiObject(String id) {
		
		return null;
	}
}
