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

import android.webkit.WebView;

import android.app.Notification.Builder;
import android.content.Context;


//Android 3.0.x
class AndroidFunctionality11 extends AndroidFunctionality10 implements AndroidFunctionality {

	private static final int PRIORITY_HIGH = 0x00000001;

    @Override
    public void pauseWebView( WebView view, boolean doPause ) {
	if ( doPause ) {	    
	    view.onPause();
	    Logger.I(TAG, "Pause WebView");
	} else {	    
	    view.onResume();
	    Logger.I(TAG, "Resume WebView");
	}
    }

	@Override
	public Builder getNotificationBuilder( Context ctx, String channelID, String channelName ) {
		Builder builder = new android.app.Notification.Builder(ctx);
		builder.setPriority(PRIORITY_HIGH);
		return builder;
	}

	@Override
	public Builder getNotificationBuilderForService( Context ctx, String channelID, String channelName ) {
		Builder builder = new android.app.Notification.Builder(ctx);
		builder.setPriority(PRIORITY_HIGH);
		return builder;
	}

	
}