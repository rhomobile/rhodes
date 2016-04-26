package com.rho.battery;

import com.rho.battery.IBattery;
import com.rho.battery.IBatteryFactory;
import com.rho.battery.IBatterySingleton;
import com.rhomobile.rhodes.Logger;

public class BatteryFactory implements IBatteryFactory
{
	protected static String TAG = "BatteryFactory";
	private BatterySingleton singleton;
	
	@Override
	public IBatterySingleton getApiSingleton()
	{
		if(singleton == null) singleton = new BatterySingleton(this);
		return singleton;
	}

	@Override
	public IBattery getApiObject(String id)
	{
		Logger.D(TAG, "Battery extension does not use API objects. Use the singleton");
		return null;
	}
}
