package com.rho.rhoelements.services;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class PluginService extends Service
{
	
	@Override
	public IBinder onBind(Intent arg0)
	{
		Log.v("Plugin", "Service onBind");
		return null;
	}
	
}
