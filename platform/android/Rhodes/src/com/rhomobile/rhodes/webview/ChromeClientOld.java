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

package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.RhodesActivity;

import android.app.Activity;
import android.view.Window;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebView;

public class ChromeClientOld extends WebChromeClient {
	
	private Activity mRhodesActivity;
	
	public ChromeClientOld(RhodesActivity rhodesActivity) {
		mRhodesActivity = rhodesActivity;
	}
	
	@Override
	public void onProgressChanged(WebView view, int newProgress) {
		if (RhodesActivity.ENABLE_LOADING_INDICATION) {
			newProgress *= 100;
			if (newProgress < 0)
				newProgress = 0;
			if (newProgress > RhodesActivity.MAX_PROGRESS)
				newProgress = RhodesActivity.MAX_PROGRESS;
			mRhodesActivity.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, newProgress);
		} else {
		    newProgress = RhodesActivity.MAX_PROGRESS;
		}
		super.onProgressChanged(view, newProgress);
	}
	
	@Override
	public boolean onJsAlert(WebView view, String url, String message, JsResult result) {
		return false;
	}
	
	@Override
	public boolean onJsConfirm(WebView view, String url, String message, JsResult result) {
		return false;
	}
	
}
