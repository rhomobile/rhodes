package com.rho.rhoelements.plugins;

import android.content.Intent;
import android.view.View;
import android.widget.AbsoluteLayout;
import android.widget.Button;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author FPC843
 * Implements the minimize button control
 */
@SuppressWarnings("deprecation")
public class MinimizeButtonPlugin extends ButtonPlugin
{
	public MinimizeButtonPlugin()
	{
		super();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		mButton = (Button) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","minimize_button"));
		mButton.setOnClickListener(new View.OnClickListener() {
		    public void onClick(View v) {
		    	Common.mainActivity.moveTaskToBack(true);
		    	//Send the minimize event to the Application plugin
			Intent msgIntent = new Intent(ApplicationPlugin.APPLICATION_ACTION);
			msgIntent.putExtra(ApplicationPlugin.APPLICATION_METHOD, ApplicationPlugin.ApplicationEvents.MINIMIZED.ordinal());
			Common.mainActivity.sendBroadcast(msgIntent);
		    }
		});
		mButtonPanel = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","minimizebutton_panel"));
		
		mDrawableUp = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","minbutton"));
		mDrawableDown = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","minbuttondown"));
		
		completeInit("com.rho.rhoelements.plugins.MinimizeButtonPlugin.MINIMIZE_PLUGIN_TRANSFER_COMPLETE", 
				"MinimizeButtonButtonUp", 
				"MinimizeButtonButtonDown");
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	protected void completeInit(String intentId, String resultIdUp, String resultIdDown)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		
		mIntentId = intentId;
		mResultIdUp = resultIdUp;
		mResultIdDown = resultIdDown;
		
		//By default it goes to the top-right corner of the screen
		mLeft = mDisplayMetrics.widthPixels - (mWidth + 1) * 2;
		mTop = 1;
		
		super.completeInit(intentId, resultIdUp, resultIdDown);
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("MinimizeButton");
	}
}
