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

import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.view.WindowManager;
import android.webkit.WebSettings;
import android.webkit.WebView;

import android.app.Activity;
import android.content.Intent;
import android.content.ComponentName;
import android.app.Notification.Builder;

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
    public Builder getNotificationBuilder( Context ctx, String channelID, String channelName ) {
        throw new UnsupportedOperationException();
    }

    @Override
    public Builder getNotificationBuilderForService( Context ctx, String channelID, String channelName ) {
        throw new UnsupportedOperationException();
    }

	@Override
	public ComponentName startForegroundService(Activity activity, Intent service) {
        Logger.D(TAG, "startService from AndroidFunctionality01");
		return activity.startService(service);
	}
}
