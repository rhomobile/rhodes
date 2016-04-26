/**
 * 
 */
package com.rho.rhoelements.plugins;

import android.view.View;
import android.widget.LinearLayout;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author FPC843
 * Implements the bottom command area control
 */
public class BottomCommandAreaPlugin extends CommandAreaPlugin
{
	
	public BottomCommandAreaPlugin()
	{
		super();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
				
		completeInit("com.rho.rhoelements.plugins.BottomCommandPlugin.BOTTOMCOMMANDAREA_PLUGIN_TRANSFER_COMPLETE", 
				"BottomCommandArea");
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	protected void completeInit(String intentId, String resultId)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		
		mCommandArea = (View) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","bottomcommand_area"));
		mCommandAreaPanel = (LinearLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","bottomcommand_area_panel"));
		
		mIntentId = intentId;
		mResultId = resultId;
		
		super.completeInit(intentId, resultId);
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("BottomCommandButton");
	}
}
