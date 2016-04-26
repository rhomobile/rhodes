/**
 * 
 */
package com.rho.rhoelements.plugins;

import android.view.View;
import android.widget.AbsoluteLayout;
import android.widget.Button;
import android.view.inputmethod.InputMethodManager;
import android.content.Context;
import android.content.Intent;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author FPC843
 * implements the quit button control
 */
@SuppressWarnings("deprecation")
public class SettingsButtonPlugin extends ButtonPlugin
{
	public SettingsButtonPlugin()
	{
		super();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		mButton = (Button) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","settings_button"));
		mButton.setOnClickListener(new View.OnClickListener() {
		    public void onClick(View v)
		    {
		    	 Common.elementsCore.loadSettingsPage();
		    }
		});
		mButtonPanel = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","settingsbutton_panel"));

		mDrawableUp = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","settingsup"));
		mDrawableDown = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","settingsdown"));
		
		completeInit("com.rho.rhoelements.plugins.SettingsButtonPlugin.SETTINGSBUTTON_PLUGIN_TRANSFER_COMPLETE", 
				"SettingsButtonButtonUp", 
				"SettingsButtonButtonDown");
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	protected void completeInit(String intentId, String resultIdUp, String resultIdDown)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		
		mIntentId = intentId;
		mResultIdUp = resultIdUp;
		mResultIdDown = resultIdDown;

		//By default it goes to the top-right corner of the screen
		mLeft = mDisplayMetrics.widthPixels - (3*mWidth) - 3;
		
		View mWebView = Common.getWebView();
		int webViewAbsoluteCoord[] = new int[2];
		mWebView.getLocationOnScreen(webViewAbsoluteCoord);
		int webViewBottom = mWebView.getHeight();
		mTop = webViewBottom - mHeight - 1;
		
		
		super.completeInit(intentId, resultIdUp, resultIdDown);
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("SettingsButton");
	}
}
