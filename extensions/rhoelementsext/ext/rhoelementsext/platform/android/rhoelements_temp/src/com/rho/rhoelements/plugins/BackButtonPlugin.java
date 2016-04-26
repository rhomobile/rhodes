/**
 * 
 */
package com.rho.rhoelements.plugins;

import android.view.View;
import android.widget.AbsoluteLayout;
import android.widget.Button;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author FPC843 Implements the back button control
 */
@SuppressWarnings("deprecation")
public class BackButtonPlugin extends ButtonPlugin
{

	public BackButtonPlugin()
	{
		super();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		mButton = (Button) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","back_button"));
		mButton.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View v)
			{
				Common.elementsCore.browserBack();
			}
		});
		mButtonPanel = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","backbutton_panel"));

		mDrawableUp = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","backup"));
		mDrawableDown = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","backdown"));

		completeInit("com.rho.rhoelements.plugins.BackButtonPlugin.BACKBUTTON_PLUGIN_TRANSFER_COMPLETE", "BackButtonButtonUp", "BackButtonButtonDown");

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}

	protected void completeInit(String intentId, String resultIdUp, String resultIdDown)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));

		mIntentId = intentId;
		mResultIdUp = resultIdUp;
		mResultIdDown = resultIdDown;

		//By default it goes to the top-right corner of the screen
		mLeft = mDisplayMetrics.widthPixels - (mWidth + 1) * 5;
		mTop = 1;

		super.completeInit(intentId, resultIdUp, resultIdDown);

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}

	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("BackButton");
	}
}
