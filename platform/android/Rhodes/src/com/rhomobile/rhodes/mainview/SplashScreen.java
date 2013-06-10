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
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesApplication;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.webview.IRhoWebView;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.BitmapFactory;
import android.os.SystemClock;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;

public class SplashScreen implements MainView {
    
    public interface SplashScreenListener {
        void onSplashScreenGone(SplashScreen splashScreen);
        void onSplashScreenNavigateBack();
    }

	private static final String TAG = SplashScreen.class.getSimpleName();

	private static final boolean DEBUG = false;

	private static final String LOADING_ANDROID_PNG = "apps/app/loading.android.png";
	private static final String LOADING_PNG = "apps/app/loading.png";
	private static final String LOADING_PAGE = "apps/app/loading.html";

	private SplashScreenListener mSplashScreenListener;
	private FrameLayout mView;
	private IRhoWebView mWebView;
	private ImageView mImageView;
	private Activity mActivity;

	private native void nativeStart();
	private native void nativeHide();
	private native int howLongWaitMs();

	private boolean mFirstNavigate = true;
	private long mStartTimeMs = 0;
	private volatile List<String> mUrlToNavigate = null;
	private int mNavigateIndex = 0;
	private Thread mSleepThread;

    public SplashScreen(Activity activity, IRhoWebView webView, SplashScreenListener listener) {
        mSplashScreenListener = listener;
        mActivity = activity;
        mView = new FrameLayout(activity);
        mWebView = webView;
        loadContent();
    }

    public synchronized List<String> getUrlToNavigate() { return mUrlToNavigate; }
    private synchronized void setUrlToNavigate(String url) {
        if (mUrlToNavigate == null) {
            mUrlToNavigate = new ArrayList<String>();
        }
        mUrlToNavigate.add(url); 
    }

    private void loadContent() {
        AssetManager am = mActivity.getAssets();
        int type = 0;

        Logger.I(TAG, "Looking for start page source");
        final String[][] urls = {{LOADING_PNG}, {LOADING_PAGE}};
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

        mActivity.setContentView(mView, new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
        switch (type) {
        case 0:
            try {
                mImageView = new ImageView(mActivity);
                mImageView.setImageBitmap(BitmapFactory.decodeStream(am.open(fn[type])));
                mActivity.addContentView(mImageView, new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
                mImageView.setAdjustViewBounds(true);
                mImageView.setScaleType(ImageView.ScaleType.FIT_CENTER);
            } catch (IOException e) {
                Logger.E(TAG, e);
            }
            mView.addView(mWebView.getContainerView(), new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT)) ; 
            //mWebView.loadDataWithBaseURL("file:///android_asset/", "<html><body style=\"margin:0px\"><img src=\""+ fn[type] + "\" height=\"100%\" width=\"100%\" border=\"0\"/></body></html>", "text/html", "utf-8", null);
            break;
        case 1:
            mView.addView(mWebView.getContainerView(), new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT)) ; 
            mWebView.loadUrl("file:///android_asset/" + fn[type]);
            break;
        default:
            mView.addView(mWebView.getContainerView(), new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT)) ; 
            mWebView.loadData("<html><title>Loading</title><body text='white' bgcolor='black'>Loading...</body></html>", "text/html", "utf-8");
        }
        Logger.I(TAG, "Done");
    }
	
	public void start() {
	    mStartTimeMs = SystemClock.uptimeMillis();
		nativeStart();
	}
	
	@Override
	public View getView() {
		return mView;
	}
	
	@Override
	public IRhoWebView getWebView(int index) {
		return mWebView;
	}
	
	@Override
	public void goBack() {
		// Nothing here
	}
	
	@Override
	public void navigate(final String url, final int index) {

        Logger.I(TAG, "navigate: url=" + url + " ************************************************");

        long delay = howLongWaitMs();
        if (delay <= 0) {
            delay = 0;
        } else {
            delay = mStartTimeMs + delay - SystemClock.uptimeMillis();
            if (delay < 0)
                delay = 0;
        }

        Logger.D(TAG, "DELAY for SplashScreen = " + String.valueOf(delay));
        final SplashScreen curView = this;

        if (getUrlToNavigate() == null) {
            setUrlToNavigate(url);
            final long threadDelay = delay;
            mSleepThread = new Thread(new Runnable() {
                @Override
                public void run() {
                    SystemClock.sleep(threadDelay);
                    RhodesApplication.runWhen(
                        RhodesApplication.AppState.AppActivated,
                        new RhodesApplication.StateHandler(true) {
                             @Override public void run() {
                                 PerformOnUiThread.exec(new Runnable() {
                                     public void run() {
                                         Logger.I(TAG, "Dismiss splash screen now *******************************************");
                                        try {
                                            if (mNavigateIndex != 0) {
                                                throw new IllegalStateException("Non zero tab index(" + mNavigateIndex + ") to navigate from Splash Screen");
                                            }
                                            mSplashScreenListener.onSplashScreenGone(curView);
                                        } catch (Throwable e) {
                                            Logger.E(TAG, e);
                                        }
                                    }
                                });
                        }
                    });
                }
            });
            mSleepThread.start();
        } else {
            setUrlToNavigate(url);
        }
        
	}

    @Override
    public IRhoWebView detachWebView() {
        IRhoWebView v = null;
        if (mWebView != null) {
            mView.removeView(mWebView.getContainerView());
            v = mWebView;
            mWebView = null;
        }
        return v;
    }

    @Override
    public void back(int index) 
    {
        mSplashScreenListener.onSplashScreenNavigateBack();
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
    @Override
    public void executeJS(String js, int index) {
    }
    @Override
    public void stopNavigate(int index) {
    }
}
