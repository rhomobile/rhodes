package com.rho.mediaplayer;

import android.view.KeyEvent;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class MediaplayerFactory extends RhoApiFactory<Mediaplayer, MediaplayerSingleton> implements IMediaplayerFactory
{
	private MediaplayerSingleton singleton;

	public MediaplayerFactory()
	{
		super();
		MediaplayerRhoListener.registerListener(this);
	}

	@Override
	protected MediaplayerSingleton createSingleton()
	{
		if(singleton == null) singleton = new MediaplayerSingleton(this); 
		return singleton;
	}

	@Override
	protected Mediaplayer createApiObject(String id)
	{
		return new Mediaplayer(id);
	}

	public void onResume()
	{
		if(singleton != null) singleton.onResume();
	}

	public void onPause()
	{
		if(singleton != null) singleton.onPause();
	}

	public void onDestroy()
	{
		if(singleton != null) singleton.onDestroy();
	}

	public void onStop()
	{
		if(singleton != null) singleton.onStop();
	}

	public boolean onKey(int keyCode, KeyEvent event)
	{
		if(singleton != null) return singleton.onKey(keyCode, event);
		return false;
	}
}
