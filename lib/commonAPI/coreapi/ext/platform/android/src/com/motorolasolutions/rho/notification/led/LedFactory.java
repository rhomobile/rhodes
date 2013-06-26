package com.motorolasolutions.rho.notification.led;

import com.rho.notification.led.ILed;
import com.rho.notification.led.ILedFactory;
import com.rho.notification.led.ILedSingleton;

public class LedFactory implements ILedFactory
{
	private LedSingleton singleton;
	
	public LedFactory()
	{
		LedRhoListener.registerListener(this);
    }
    
	@Override
	public ILedSingleton getApiSingleton()
	{
		if(singleton == null) singleton = new LedSingleton();
		return singleton;
	}

	@Override
	public ILed getApiObject(String id)
	{
		return singleton.getLed(id);
	}

	public void onPause()
	{
		if(singleton != null)
		{
			singleton.onPause();
		}
	}

	public void onStop()
	{
		if(singleton != null)
		{
			singleton.onStop();
		}
	}

	public void onDestroy()
	{
		if(singleton != null)
		{
			singleton.onDestroy();
		}
	}
}