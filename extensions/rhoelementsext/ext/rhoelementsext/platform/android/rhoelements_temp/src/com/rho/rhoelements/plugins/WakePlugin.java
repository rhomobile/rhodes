package com.rho.rhoelements.plugins;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;

public class WakePlugin extends Plugin
{

	//private boolean wakeLockEnabled = false;
	//private boolean wifiLockEnabled = false;
	
	public WakePlugin()
	{
		
	}

	/** Method called when the App gets foreground focus event */
	/*@Override
	public void onForeground(String url)
	{		
		//  An Application focus change is received 
		
		//  Application has become active. 
					
		restoreSavedEMMLAttributes();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,"Application foreground event received by wake plugin"));
		
	}*/
	
	/** Method called when the App gets Background focus event */
	/*@Override
	public void onBackground(String url)
	{		
		//  An Application focus change is received 

		//  Application has gone into the background.					
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,"Application background event received by wake plugin"));
		
		// Release any locks that were required by this application
		if (wakeLockEnabled)
		{
			Common.elementsCore.setWakeLock(false);
		}
		if (wifiLockEnabled)
		{
			Common.elementsCore.setWifiLock(false);
		}
	}*/
	
	/** 
	 * Method to apply the saved plugin attributes 
	 * */
	/*public void restoreSavedEMMLAttributes()
	{
		// The data is already saved in the member variables so apply all the settings	
		// Acquire any locks if they were set in this application
		if (wakeLockEnabled)
		{
			Common.elementsCore.setWakeLock(true);
		}
		if (wifiLockEnabled)
		{
			Common.elementsCore.setWifiLock(true);
		}
		
	}*/
	
	@Override
	public void onSetting(PluginSetting setting)
	{
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'",
					setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));

		// Audio Device Control
		if (setting.getName().equalsIgnoreCase("wakeLock"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				//wakeLockEnabled = true;
				//if (Common.elementsCore.bLaunchingAppHasFocus)
				//{
					Common.elementsCore.setWakeLock(true);
				//}
				//else
				//{
					//Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot set the lock from a background application"));
				//}
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				//wakeLockEnabled = false;
				//if (Common.elementsCore.bLaunchingAppHasFocus)
				//{
					Common.elementsCore.setWakeLock(false);
				//}
				//else
				//{
					//Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot set the lock from a background application"));
				//}
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if (setting.getName().equalsIgnoreCase("wifiLock"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				//wifiLockEnabled = true;
				//if (Common.elementsCore.bLaunchingAppHasFocus)
				//{
					Common.elementsCore.setWifiLock(true);
				//}
				//else
				//{
					//Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot set the lock from a background application"));
				//}
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				//wifiLockEnabled = false;
				//if (Common.elementsCore.bLaunchingAppHasFocus)
				//{
					Common.elementsCore.setWifiLock(false);
				//}
				//else
				//{
					//Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot set the lock from a background application"));
				//}
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}		
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + setting.getName()));
		}
	}

	@Override
	public void onShutdown()
	{
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
	    return new Version ("Wake"); //$NON-NLS-1$
	}
}
