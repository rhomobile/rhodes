/**
 * Implementation of Sip module
 */
package com.rho.rhoelements.plugins;


import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;

/**
 * @author FPC843
 *
 */
public class SipPlugin extends Plugin
{
	private SipMode savedSipMode = SipMode.AUTOMATIC;
	
	/**
	 * 
	 * @author FPC843
	 * Enumeration describing current SIP mode.
	 * MANUAL when shown by the SIP button; it will still hide 
	 */
	public enum SipMode
	{
		MANUAL,
		AUTOMATIC
	}
	
	/**
	 * Stores the current SIP mode
	 */
	static public SipMode currentSipMode = SipMode.AUTOMATIC;

   
    /** Method called when the App gets background event */
    @Override
    public void onBackground(String url)
    {
        //  An Application focus change is received 
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Application background event received by SIP plugin"));
    
        savedSipMode = currentSipMode;
    }
    

	/** Method called when the App gets foreground focus event */
	@Override
	public void onForeground(String url)
	{		
		//  An Application focus change is received 
		
		//  Application has become active. 
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,"Application foreground event received by SIP plugin"));
		
		currentSipMode = savedSipMode;
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
		
		if (setting.getName().equalsIgnoreCase("Left"))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Left"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Parameter not supported"));
		}
		else if (setting.getName().equalsIgnoreCase("Top"))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Top"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Parameter not supported"));
		}
		if (setting.getName().equalsIgnoreCase("Manual"))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Manual"));
			 if (Common.elementsCore.bLaunchingAppHasFocus)
				 currentSipMode = SipMode.MANUAL;
			 else
				 savedSipMode = SipMode.MANUAL;
			
		}
		else if (setting.getName().equalsIgnoreCase("Automatic"))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Automatic"));
			if (Common.elementsCore.bLaunchingAppHasFocus)
				currentSipMode = SipMode.AUTOMATIC;
			else
				savedSipMode = SipMode.AUTOMATIC;
		}

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}

	/**
	 * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown()
	{
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("SipPlugin");
	}
}
