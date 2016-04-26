package com.rho.rhoelements.plugins;


import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;


/**
 * @author hpkx48
 * Reboot Plug-in Simply provides Reboot functionality 
 */
public class RebootPlugin extends Plugin
{	
	public static Version getVersion()
	{
		return new Version ("Reboot");
	}

	@Override
	public void onPageStarted(String url)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));    
	}
	
	@Override
	public void onShutdown()
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
	    if (setting.getValue().length() > 0)
	    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
	    else
	    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));
	    if (setting.getName().equalsIgnoreCase("boottype"))
	    {
			if (setting.getValue().equalsIgnoreCase("warm"))
			{
			    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Reboot: Warm"));
			    
			}
			else if(setting.getValue().equalsIgnoreCase("cold"))
			{
			    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Reboot: Cold"));
			}	 
			else if(setting.getValue().equalsIgnoreCase("ColdCAD"))
			{
			    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Reboot: ColdCAD"));
			}
	    }
	}	
}
	
	
	

