/**
 * 
 */
package com.rho.rhoelements.plugins;

import java.net.MalformedURLException;
import java.net.URL;

import android.view.View;
import android.widget.AbsoluteLayout;
import android.widget.Button;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author FPC843
 * Implements the Go button plugin
 */
@SuppressWarnings("deprecation")
public class GoButtonPlugin extends ButtonPlugin
{
	protected boolean isVisible = false;
	
	public GoButtonPlugin()
	{
		super();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		mButton = (Button) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","go_button"));
		mButton.setOnClickListener(new View.OnClickListener() {
		    public void onClick(View v) {
		    	AddressBarPlugin addressBarPlugin = (AddressBarPlugin) Common.pluginManager.getPlugin("AddressBar");
		    	if(addressBarPlugin != null)
		    	{
		    		addressBarPlugin.go();
		    	}
		    }
		});
		mButtonPanel = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","gobutton_panel"));
		
		mDrawableUp = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","goup"));
		mDrawableDown = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","godown"));
		
		completeInit("com.rho.rhoelements.plugins.GoButtonPlugin.GOBUTTON_PLUGIN_TRANSFER_COMPLETE", 
				"GoButtonButtonUp", 
				"GoButtonButtonDown");
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	protected void completeInit(String intentId, String resultIdUp, String resultIdDown)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		
		mIntentId = intentId;
		mResultIdUp = resultIdUp;
		mResultIdDown = resultIdDown;
		
		//default position
		mLeft = mDisplayMetrics.widthPixels - (mWidth + 1) * 6;
		mTop = 1;
		
		super.completeInit(intentId, resultIdUp, resultIdDown);
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.ButtonPlugin#onPageStarted(java.lang.String)
	 */
	@Override
	public void onPageStarted(String url)
	{
		if (isVisible)
			mButtonPanel.setVisibility(View.GONE);
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onPageFinished(java.lang.String)
	 */
	@Override
	public void onPageFinished(String url)
	{
		if (isVisible)
			mButtonPanel.setVisibility(View.VISIBLE);
	}
	
	
	protected void setVisibility(String visibilityValue)
	{
		if (visibilityValue.compareToIgnoreCase("visible") == 0)
		{
			isVisible = true;
			mButtonPanel.setVisibility(View.VISIBLE);
		}
		else if (visibilityValue.compareToIgnoreCase("hidden") == 0)
		{
			isVisible = false;
			mButtonPanel.setVisibility(View.GONE);
		}
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("GoButton");
	}
}
