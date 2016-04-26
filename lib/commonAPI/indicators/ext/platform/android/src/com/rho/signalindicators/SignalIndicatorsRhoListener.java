package com.rho.signalindicators;

import android.content.res.Configuration;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;

public class SignalIndicatorsRhoListener extends AbstractRhoListener
{
	private static String TAG = "SignalIndicatorListener";
	private static SignalIndicatorsFactory factory;
	
	public static void registerListener(SignalIndicatorsFactory inputFactory)
	{
		factory = inputFactory;
	}
	
	@Override
	public void onCreateApplication(IRhoExtManager extManager)
	{
		Logger.D(TAG, "onCreateApplication");
		extManager.addRhoListener(this);
	}

    @Override
    public void onConfigurationChanged(RhodesActivity activity, Configuration newConfig)
    {
		Logger.D(TAG, "onConfigurationChanged");
		if(factory != null) factory.onConfigurationChanged(activity, newConfig);
    }
}
