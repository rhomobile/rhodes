package com.rho.notification;

import com.rho.notification.INotification;
import com.rho.notification.INotificationFactory;
import com.rho.notification.INotificationSingleton;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;

public class NotificationFactory extends AbstractRhoListener implements INotificationFactory, IRhoListener
{
	private NotificationSingleton singleton;
	
	public NotificationFactory()
	{
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

	@Override
	public void onPause(RhodesActivity activity)
	{
		if(singleton != null)
		{
			singleton.onPause();
		}
	}

	@Override
	public void onStop(RhodesActivity activity)
	{
		if(singleton != null)
		{
			singleton.onStop();
		}
	}

	@Override
	public void onDestroy(RhodesActivity activity)
	{
		if(singleton != null)
		{
			singleton.onDestroy();
		}
	}

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        NotificationFactorySingleton.setInstance(this);
        extManager.addRhoListener(this);
    }
}