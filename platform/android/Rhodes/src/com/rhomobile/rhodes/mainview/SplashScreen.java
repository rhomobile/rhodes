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

package com.rhomobile.rhodes.mainview;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.util.Map;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesApplication;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.webview.GoogleWebView;
import com.rhomobile.rhodes.webview.WebView;

import android.content.Context;
import android.content.res.AssetManager;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

public class SplashScreen implements MainView {
	
	private static final String TAG = SplashScreen.class.getSimpleName();
	
	private static final boolean DEBUG = false;
	
	private static final String LOADING_ANDROID_PNG = "apps/app/loading.android.png";
	private static final String LOADING_PNG = "apps/app/loading.png";
	private static final String LOADING_PAGE = "apps/app/loading.html";
	
	private FrameLayout mView;
	
	private WebView mWebView;
	
	private native void nativeStart();
	private native void nativeHide();
	private native int howLongWaitMs();
	
	private boolean mFirstNavigate = true;
	
    public SplashScreen(RhodesActivity context) {
        mView = new FrameLayout(context);
        mView.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
		AssetManager am = context.getResources().getAssets();
		mWebView = createHtmlView(context, am);
		mView.addView(mWebView.getView());
		mFirstNavigate = true;
	}

    private WebView createHtmlView(RhodesActivity context, AssetManager am) {

        int type = 0;
        final String[][] urls = {{LOADING_ANDROID_PNG, LOADING_PNG}, {LOADING_PAGE}};
        String[] fn = new String[2];
        types: for (String[] X: urls) {
            for (String url:X) {
                InputStream is = null;
                try {
                    is = am.open(url);
                    fn[type] = url;
                    break types;
                }
                catch (IOException e) {
                    if (DEBUG)
                        Logger.D(TAG, "Can't load " + url + ": " + e.getMessage());
                    continue;
                }
                finally {
                    if (is != null)
                        try {
                            is.close();
                        } catch (IOException e) {}
                }
            }
            type++;
        }

        // Now create WebView and load appropriate content there
        WebView view = null;//new GoogleWebView(context);
        if (Capabilities.WEBKIT_BROWSER_ENABLED) {
            Logger.D(TAG, "Creating Motorola WebKIT view");
            try {
                Class<? extends WebView> viewClass = (Class<? extends WebView>)Class.forName("com.rhomobile.rhodes.webview.EkiohWebView");
                Constructor<? extends WebView> viewCtor = viewClass.getConstructor(Context.class, Runnable.class);
                view = viewCtor.newInstance(context, RhodesApplication.AppState.AppStarted.addObserver("MotorolaStartEngineObserver", true));
            } catch (Throwable e) {
                Logger.E(TAG, e);
                RhodesApplication.stop();
            }
        } else {
            Logger.D(TAG, "Creating Google web view");
            final GoogleWebView googleWebView = new GoogleWebView(context);
            view = googleWebView;
            RhodesApplication.runWhen(RhodesApplication.AppState.AppStarted, new RhodesApplication.StateHandler(true) {
                @Override
                public void run()
                {
                    googleWebView.applyWebSettings();
                }
            });
            switch (type) {
            case 0:
              view.loadDataWithBaseURL("file:///android_asset/", "<html><body style=\"margin:0px\"><img src=\""+ fn[type] + "\" height=\"100%\" width=\"100%\" border=\"0\"/></body></html>", "text/html", "utf-8", null);
              break;
            case 1:
              view.loadUrl("file:///android_asset/" + fn[type]);
              break;
            default:
              view.loadData("<html><title>Loading</title><body text='white' bgcolor='black'>Loading...</body></html>", "text/html", "utf-8");
          }
        }


        return view;
    }
	
	public void start() {
		nativeStart();
	}
	
	@Override
	public View getView() {
		return mView;
	}
	
	@Override
	public WebView getWebView(int index) {
		return mWebView;
	}
	
	@Override
	public void goBack() {
		// Nothing here
	}
	
	@Override
	public void navigate(final String url, final int index) {

		Logger.D(TAG, "navigate: url=" + url);
		
		int delay = howLongWaitMs();
		if (delay < 0) {
			delay = 0;
		}
		
		Logger.D(TAG, "DELAY for SplashScreen = " + String.valueOf(delay));
		final SplashScreen curView = this;

        PerformOnUiThread.exec(new Runnable() {
            private String mUrl = url;
            private int mIndex = index;
            public void run() {
                try {
                    RhodesActivity activity = RhodesActivity.safeGetInstance();
                    if (mFirstNavigate) {
                        mFirstNavigate = false;
                        activity.switchToSimpleMainView(curView).navigate(mUrl, mIndex);
                    }
                    else {
                        // Recover navigate in case of race conditions
                        activity.getMainView().navigate(mUrl, mIndex);
                    }
                } catch (Throwable e) {
                    Logger.E(TAG, e);
                }
            }
        }, delay);
	}

    @Override
    public WebView detachWebView() {
        WebView v = null;
        if (mWebView != null) {
            mView.removeView(mWebView.getView());
            v = mWebView;
            mWebView = null;
        }
        return v;
    }

	@Override
	public void back(int index) 
	{
        RhodesActivity ra = RhodesActivity.getInstance();
        if ( ra != null )
            ra.moveTaskToBack(true);
	}
	
	@Override
	public void forward(int index) {
	}
	@Override
	public void reload(int index) {
	}
	@Override
	public String currentLocation(int index) {
		return null;
	}
	@Override
	public void switchTab(int index) {
	}
	@Override
	public int activeTab() {
		return 0;
	}
	@Override
	public int getTabsCount() {
		return 1;
	}
	@Override
	public void loadData(String data, int index) {
	}
	@Override
	public void addNavBar(String title, Map<Object, Object> left,
			Map<Object, Object> right) {
	}
	@Override
	public void removeNavBar() {
	}
}
