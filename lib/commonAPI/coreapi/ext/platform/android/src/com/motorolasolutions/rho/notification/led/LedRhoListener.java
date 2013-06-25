package com.motorolasolutions.rho.notification.led;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;

public class LedRhoListener extends AbstractRhoListener
{
	private static String TAG = "LedRhoListener";
	private static LedFactory factory;
	
	public static void registerListener(LedFactory inputFactory)
	{
		factory = inputFactory;
	}
	
	@Override
	public void onCreateApplication(IRhoExtManager extManager)
	{
		Logger.D(TAG, "LedFactoryRhoListener: onCreateApplication");
		extManager.addRhoListener(this);
	}
	
	@Override
	public void onResume(RhodesActivity activity)
	{
		
	}

	@Override
	public void onPause(RhodesActivity activity)
	{
		Logger.D(TAG, "onPause");
		if(factory != null) factory.onPause();
	}

	@Override
	public void onStop(RhodesActivity activity)
	{
		Logger.D(TAG, "onStop");
		if(factory != null) factory.onStop();
	}

	@Override
	public void onDestroy(RhodesActivity activity)
	{
		Logger.D(TAG, "onDestroy");
		if(factory != null) factory.onDestroy();
	}

}
