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
 * @author FPC843
 * 
 */
@SuppressWarnings("deprecation")
public class StopButtonPlugin extends ButtonPlugin
{
	protected boolean isVisible = false;

	public StopButtonPlugin()
	{
		super();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		mButton = (Button) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","stop_button"));
		mButton.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View v)
			{
				Common.elementsCore.browserStop();
			}
		});
		mButtonPanel = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","stopbutton_panel"));

		mDrawableUp = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","stopbutton"));
		mDrawableDown = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","stopbuttondown"));

		completeInit("com.rho.rhoelements.plugins.StopButtonPlugin.STOPBUTTON_PLUGIN_TRANSFER_COMPLETE", "StopButtonButtonUp", "StopButtonButtonDown");

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

	protected void setVisibility(String visibilityValue)
	{
		if (visibilityValue.compareToIgnoreCase("visible") == 0)
		{
			isVisible = true;
			//mButtonPanel.setVisibility(View.VISIBLE);
		}
		else if (visibilityValue.compareToIgnoreCase("hidden") == 0)
		{
			isVisible = false;
			mButtonPanel.setVisibility(View.GONE);
		}
	}

	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.ButtonPlugin#onPageStarted(java.lang.String)
	 */
	@Override
	public void onPageStarted(String url)
	{
		if (isVisible)
			mButtonPanel.setVisibility(View.VISIBLE);
	}

	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onPageFinished(java.lang.String)
	 */
	@Override
	public void onPageFinished(String url)
	{
		if (isVisible)
			mButtonPanel.setVisibility(View.GONE);
	}

	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("StopButton");
	}
}
