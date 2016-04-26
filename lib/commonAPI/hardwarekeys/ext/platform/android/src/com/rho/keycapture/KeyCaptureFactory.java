package com.rho.keycapture;

import com.rho.keycapture.IKeyCapture;
import com.rho.keycapture.IKeyCaptureFactory;
import com.rho.keycapture.IKeyCaptureSingleton;

public class KeyCaptureFactory implements IKeyCaptureFactory
{
	private KeyCaptureSingleton singleton;
	
	public KeyCaptureFactory()
	{
		super();
		KeyCaptureRhoListener.registerListener(this);
	}

	@Override
	public IKeyCaptureSingleton getApiSingleton()
	{
		if(singleton == null) singleton = new KeyCaptureSingleton();
		return singleton;
	}

	@Override
	public IKeyCapture getApiObject(String id)
	{
		return null;
	}

}
