package com.motorolasolutions.rho.notification;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;

public class NotificationRhoListener extends AbstractRhoListener
{
	private static String TAG = "NotificationRhoListener"; 
	private static NotificationFactory factory;
	
	public static void registerListener(NotificationFactory inputFactory)
	{
		factory = inputFactory;
	}
	
	@Override
	public void onCreateApplication(IRhoExtManager extManager)
	{
		Logger.D(TAG, "BarcodeFactoryRhoListener: onCreateApplication");
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
