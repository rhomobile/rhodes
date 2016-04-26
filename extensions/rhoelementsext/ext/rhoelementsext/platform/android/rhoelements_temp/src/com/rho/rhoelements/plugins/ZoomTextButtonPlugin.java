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
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.IRhoWebView.TextZoom;

/**
 * @author FPC843 This class implements the ZoomTextButton plugin
 */
@SuppressWarnings("deprecation")
public class ZoomTextButtonPlugin extends ButtonPlugin
{
	public ZoomTextButtonPlugin()
	{
		super();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		mButton = (Button) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","zoomtext_button"));
		mButton.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View v)
			{
				TextZoom textZoom = Common.elementsCore.getWebViewInstance().getTextZoom();
				if (textZoom == TextZoom.SMALLEST)
					textZoom = TextZoom.SMALLER;
				else if (textZoom == TextZoom.SMALLER)
					textZoom = TextZoom.NORMAL;
				else if (textZoom == TextZoom.NORMAL)
					textZoom = TextZoom.LARGER;
				else if (textZoom == TextZoom.LARGER)
					textZoom = TextZoom.LARGEST;
				else if (textZoom == TextZoom.LARGEST)
					textZoom = TextZoom.SMALLEST;
				
				Common.elementsCore.getWebViewInstance().setTextZoom(textZoom);
			}
		});
		mButtonPanel = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","zoomtextbutton_panel"));

		mDrawableUp = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","zoomtextup"));
		mDrawableDown = Common.mainActivity.getResources().getDrawable(RhoExtManager.getResourceId("drawable","zoomtextdown"));

		completeInit("com.rho.rhoelements.plugins.ZoomTextButtonPlugin.ZOOMTEXTBUTTON_PLUGIN_TRANSFER_COMPLETE", "ZoomTextButtonButtonUp", "ZoomTextButtonButtonDown");

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}

	protected void completeInit(String intentId, String resultIdUp, String resultIdDown)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));

		mIntentId = intentId;
		mResultIdUp = resultIdUp;
		mResultIdDown = resultIdDown;

		//By default it goes to the bottom-right corner of the screen
		//zoom text button will be always in bottom & right side in both landscape and portrait mode
		mLeft = mDisplayMetrics.widthPixels - (mWidth + 1) * 2; //To the left of the SIP button
		
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
		return new Version("ZoomTextButton");
	}
}
