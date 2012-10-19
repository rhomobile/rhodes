/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.osfunctionality;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.event.CalendarIDsProvider;

import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.view.WindowManager;
import android.webkit.WebSettings;
import android.webkit.WebView;

// Android 1.0
class AndroidFunctionality01 implements AndroidFunctionality {

	
	public String AccessOwnerInfo_getEmail(Context context) {
		return "";
	}
	
	public String AccessOwnerInfo_getUsername(Context context) {
		return "";
	}
	
	public int getDeviceRotation(Context context) {
		return -1;
	}

    @Override
    public void applyWebSettings(WebView view) {
        WebSettings settings = view.getSettings();

        settings.setSavePassword(false);
        settings.setSaveFormData(false);
        settings.setJavaScriptEnabled(true);
        settings.setJavaScriptCanOpenWindowsAutomatically(false);
        if (RhoConf.getBool("enable_screen_zoom")) {
            settings.setSupportZoom(true);
            Logger.I(TAG, "Enable WebView zoom");
        } else {
            settings.setSupportZoom(false);
            Logger.I(TAG, "Disable WebView zoom");
        }
        settings.setCacheMode(WebSettings.LOAD_CACHE_ELSE_NETWORK);
        settings.setSupportMultipleWindows(false);
        setWebPlugins(settings);
    }
    
    @SuppressWarnings("deprecation")
    protected void setWebPlugins(WebSettings settings) {
        if(RhoConf.getBool("enable_web_plugins")) {
            settings.setPluginsEnabled(true);
            Logger.I(TAG, "Set web plugins enabled");
        }
        else {
            settings.setPluginsEnabled(false);
            Logger.I(TAG, "Set web plugins disabled");
        }
    }

    @Override
    public void pauseWebView( WebView view, boolean doPause ) {
    }

    @SuppressWarnings("deprecation")
    @Override
    public int getScreenOrientation(Context context) {

        int configOrientation = context.getResources().getConfiguration().orientation;
        Logger.D(TAG, "Configuration orientation: " + context.getResources().getConfiguration().orientation);

        switch (configOrientation) {
        case Configuration.ORIENTATION_PORTRAIT:
            return ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
        case Configuration.ORIENTATION_LANDSCAPE:
            return ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
        default:
            {
                int displayOrientation = ((WindowManager)context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getOrientation();
                Logger.D(TAG, "Default display orientation: " + displayOrientation);

                switch(displayOrientation) {
                case 0:
                    return ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
                case 1:
                    return ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
                default:
                    return ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED;
                }
            }
        }
    }

	@Override
	public CalendarIDsProvider buildCalendarIDsProvider() {
		return new CalendarIDsProvider();
	}
}
