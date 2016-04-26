/**
 * 
 */
package com.rho.rhoelements.plugins;

import android.content.Context;
import android.content.Intent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.AbsoluteLayout;
import android.widget.Button;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author FPC843
 * Implements the SIP button control
 */
@SuppressWarnings("deprecation")
public class SipButtonPlugin extends ButtonPlugin
{
	
	
	public SipButtonPlugin()
	{
		super();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		mButton = (Button) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","sip_button"));
		mButton.setOnClickListener(new View.OnClickListener() {
		    public void onClick(View v)
		    {	
		    	InputMethodManager imm = (InputMethodManager)Common.mainActivity.getSystemService(Context.INPUT_METHOD_SERVICE);
		    	imm.toggleSoftInput(0, 0);
		    }
		});
		mButtonPanel = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","sipbutton_panel"));
		
		mDrawableUp = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","sipup"));
		mDrawableDown = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","sipdown"));
		
		completeInit("com.rho.rhoelements.plugins.SipButtonPlugin.SIPBUTTON_PLUGIN_TRANSFER_COMPLETE", 
				"SipButtonButtonUp", 
				"SipButtonButtonDown");
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	protected void completeInit(String intentId, String resultIdUp, String resultIdDown)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		
		mIntentId = intentId;
		mResultIdUp = resultIdUp;
		mResultIdDown = resultIdDown;
		
		//By default it goes to the bottom-right corner of the screen
		//SIP button will be always in bottom & right side in both landscape and portrait mode
		mLeft = mDisplayMetrics.widthPixels - mWidth - 1;
		
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
		return new Version("SipButton");
	}
}
