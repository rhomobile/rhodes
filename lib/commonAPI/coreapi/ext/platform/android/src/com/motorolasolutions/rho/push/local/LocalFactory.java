package com.motorolasolutions.rho.push.local;

import com.rho.push.local.ILocal;
import com.rho.push.local.ILocalFactory;
import com.rho.push.local.ILocalSingleton;
import com.rhomobile.rhodes.Logger;

public class LocalFactory implements ILocalFactory
{
	protected static String TAG = "Push.LocalFactory";
	protected LocalSingleton singleton;
	
	public LocalFactory()
	{
		singleton = new LocalSingleton(this);
	}

	@Override
	public ILocalSingleton getApiSingleton()
	{
		Logger.D(TAG, "getApiSingleton");
		return singleton;
	}

	@Override
	public ILocal getApiObject(String id)
	{
		Logger.D(TAG, "getApiObject");
		return singleton.getLocal(id);
	}
}
