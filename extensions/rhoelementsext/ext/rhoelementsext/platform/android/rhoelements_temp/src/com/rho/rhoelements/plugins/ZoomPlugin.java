/**
 * 
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
public class ZoomPlugin extends Plugin
{

	/**
	 * @see com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	@Override
	public void onSetting(PluginSetting setting)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		
		if ( (setting == null) || (setting.getValue() == null) )
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "setting is null"));
			return;
		}
		try
		{
			if (setting.getName().equalsIgnoreCase("text"))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "text"));
				
				/* There is no need to check that the int value is within the valid zoom range as this check is carried out
				 * by browserSetTextZoom()
				 */
				Common.elementsCore.browserSetTextZoom(Integer.parseInt(setting.getValue()));
			}
			else if (setting.getName().equalsIgnoreCase("page"))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "page"));
				Common.elementsCore.browserSetZoom(Double.parseDouble(setting.getValue()));
			}
		}
		catch (NumberFormatException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Passed parameter has wrong format"));
		}
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
		return new Version("ZoomPlugin");
	}

}
