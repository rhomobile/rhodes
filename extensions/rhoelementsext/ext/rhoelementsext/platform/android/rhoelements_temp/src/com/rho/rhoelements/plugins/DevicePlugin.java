/**
 * 
 */
package com.rho.rhoelements.plugins;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;

/**
 * @author pxb743
 *
 */
public class DevicePlugin extends Plugin {

	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	@Override
	public void onSetting(PluginSetting setting) {
		
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));
		
		if (setting.getName().equalsIgnoreCase("wake"))
		{
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{
				//Wake the device if sleeping
				Common.elementsCore.setWakeLock(true);
				//Disable the Lock Screen
				//Not sure if this is needed as once disabled there is no way to enable again
				//Common.elementsCore.setKeyguradLock(false);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot wake the device from background application"));
			}
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + setting.getName()));
		}
	}

	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown() {
		// TODO Auto-generated method stub

	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version ("Device"); //$NON-NLS-1$
	}

}
