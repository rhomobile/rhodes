package com.rho.signalindicators;

import android.content.res.Configuration;

import com.rho.signalindicators.ISignalIndicators;
import com.rho.signalindicators.ISignalIndicatorsFactory;
import com.rho.signalindicators.ISignalIndicatorsSingleton;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;

public class SignalIndicatorsFactory implements ISignalIndicatorsFactory
{
	protected static String TAG = "SignalIndicatorsFactory";
	private SignalIndicatorsSingleton singleton;
	
	public SignalIndicatorsFactory()
	{
		super();
		SignalIndicatorsRhoListener.registerListener(this);
	}

	@Override
	public ISignalIndicatorsSingleton getApiSingleton()
	{
		if(singleton == null) singleton = new SignalIndicatorsSingleton(this);
		return singleton;
	}

	@Override
	public ISignalIndicators getApiObject(String id)
	{
		Logger.D(TAG, "SignalIndicators extension does not use API objects. Use the singleton");
		return null;
	}

	public void onConfigurationChanged(RhodesActivity activity, Configuration newConfig)
	{
		if(singleton != null) singleton.onConfigurationChanged();
	}
}
