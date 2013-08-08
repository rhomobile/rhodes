package com.rho.mediaplayer;

import android.view.KeyEvent;

import com.motorolasolutions.rho.keycapture.KeyCaptureSingleton;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;

public class MediaplayerRhoListener extends AbstractRhoListener
{
	private static final String LOGTAG = "MediaplayerRhoListener";
	private static MediaplayerFactory factory;
	
	public static void registerListener(MediaplayerFactory inputFactory)
	{
		factory = inputFactory;
	}
	
	public MediaplayerRhoListener()
	{
		
	}
	
	@Override
	public void onResume(RhodesActivity activity)
	{
		Logger.D(LOGTAG, "onResume");
		if(factory != null) factory.onResume();
	}

	@Override
	public void onPause(RhodesActivity activity)
	{
		Logger.D(LOGTAG, "onPause");
		if(factory != null) factory.onPause();
	}

	@Override
	public void onStop(RhodesActivity activity)
	{
		Logger.D(LOGTAG, "onStop");
		if(factory != null) factory.onStop();
	}

	@Override
	public void onDestroy(RhodesActivity activity)
	{
		Logger.D(LOGTAG, "onDestroy");
		if(factory != null) factory.onDestroy();
	}

	@Override
	public void onCreateApplication(IRhoExtManager extManager)
	{
		Logger.D(LOGTAG, "onCreateApplication");
		extManager.addRhoListener(this);
		extManager.startKeyEventUpdates(this);
		
	}
	
	@Override
	public boolean onKey(int keyCode, KeyEvent event)
	{
		if(factory != null)
		{
			return factory.onKey(keyCode, event);
		}
		return false;
	}
}