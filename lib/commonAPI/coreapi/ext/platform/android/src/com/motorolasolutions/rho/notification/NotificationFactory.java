package com.motorolasolutions.rho.notification;

import com.rho.notification.INotification;
import com.rho.notification.INotificationFactory;
import com.rho.notification.INotificationSingleton;

public class NotificationFactory implements INotificationFactory
{
	private NotificationSingleton singleton;
	
	public NotificationFactory()
	{
		NotificationRhoListener.registerListener(this);
	}
	
	@Override
	public INotificationSingleton getApiSingleton()
	{
		if(singleton == null) singleton = new NotificationSingleton();
		return singleton;
	}

	@Override
	public INotification getApiObject(String id)
	{
		return null;
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