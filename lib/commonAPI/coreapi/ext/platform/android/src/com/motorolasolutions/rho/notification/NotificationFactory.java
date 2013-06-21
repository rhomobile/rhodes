package com.motorolasolutions.rho.notification;

import com.rho.notification.INotification;
import com.rho.notification.INotificationFactory;
import com.rho.notification.INotificationSingleton;

public class NotificationFactory implements INotificationFactory
{
	private NotificationSingleton singleton;
	
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
}