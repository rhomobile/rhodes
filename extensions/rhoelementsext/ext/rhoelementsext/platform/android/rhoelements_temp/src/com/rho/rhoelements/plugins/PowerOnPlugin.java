/**
 * 
 */
package com.rho.rhoelements.plugins;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.RhodesService;

/**
 * @author FPC843
 * This class implements the PowerOn plugin. 
 * The PowerOn META Tag is used to register to receive an event when the device is resumed from suspend mode.
 */
public class PowerOnPlugin extends Plugin
{
	private String mPowerOnEventUrl = null;
	private BroadcastReceiver mReceiver = null;
	Handler mHandler = null;
	
	/**
	 * Constructor 
	 */
	public PowerOnPlugin()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));		mHandler = new Handler();
		mReceiver = new BroadcastReceiver()
		{	
			@Override
			public void onReceive(Context arg0, Intent arg1)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "PowerOn event received"));
				//TODO this is a dirty hack because fixing properly was too risky for the 4.0 release
				//Please remove this hack with a way of making sure RhodesActivity is ready before navigate is sent (see the Runnable class below)
				//mHandler.post(new PowerOnEventRunnable());
				mHandler.postDelayed(new PowerOnEventRunnable(), 25);
			}
		};
		//IntentFilter intentFilter = new IntentFilter(Intent.ACTION_USER_PRESENT);
		IntentFilter intentFilter = new IntentFilter(Intent.ACTION_SCREEN_ON);
		Common.mainActivity.registerReceiver(mReceiver, intentFilter);
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}

	/**
	 * @see com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	@Override
	public void onSetting(PluginSetting setting)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		if (setting == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "setting is null"));
			return;
		}
		
		if (setting.getName().equalsIgnoreCase("PowerOnEvent"))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "PowerOnEvent"));
			if (setting.getValue().length() > 0)
				mPowerOnEventUrl = setting.getValue();
			else
				mPowerOnEventUrl = null;
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}

	/**
	 * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		Common.mainActivity.unregisterReceiver(mReceiver);
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		return new Version("PowerOn");
	}
	
	@Override
	public void onPageStarted(String url)
	{
		mPowerOnEventUrl = "";
	}
	
	private class PowerOnEventRunnable implements Runnable
	{
		public void run()
		{
			if ( (mPowerOnEventUrl != null) && (mPowerOnEventUrl.length() > 0) )
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
				if(mPowerOnEventUrl.startsWith("/app"))
				{
					//TODO Temporary fix for Rho4.0. Need to move this fix to Plugin.navigate(string);
					RhodesService.doRequest(mPowerOnEventUrl);
				}
				else
				{
					navigate(mPowerOnEventUrl);
				}
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
			}
		}
	}
}
