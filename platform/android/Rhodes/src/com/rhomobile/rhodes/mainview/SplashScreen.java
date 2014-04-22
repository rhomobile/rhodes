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
import java.util.Map;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.BitmapFactory;
import android.os.SystemClock;
import android.view.View;
import android.webkit.WebView;
import android.widget.ImageView;
import android.widget.LinearLayout;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesApplication;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class SplashScreen implements MainView{
    
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
	private View mView;
	private MainView mBackendView;

	private native void nativeStart();
	private native void nativeHide();
	private native int howLongWaitMs();

	private boolean mFirstNavigate = true;
	private long mStartTimeMs = 0;
	private volatile String mUrlToNavigate = null;
	private int mNavigateIndex = 0;
	private Thread mSleepThread;

    public SplashScreen(Activity activity, MainView mainView, SplashScreenListener listener) {
        mSplashScreenListener = listener;
        mBackendView = mainView;
        loadContent(activity);
    }

    public MainView getBackendView() {
        return mBackendView;
    }

    public View getSplashView() {
        return mView;
    }

    private void loadContent(Activity activity) {
        AssetManager am = activity.getAssets();
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

        switch (type) {
        case 0:
            try {
                Logger.I(TAG, "Loading image: " + fn[type]);
                
                ImageView imageView = new ImageView(activity);
                imageView.setImageBitmap(BitmapFactory.decodeStream(am.open(fn[type])));
                imageView.setAdjustViewBounds(true);
                imageView.setScaleType(ImageView.ScaleType.FIT_CENTER);
                mView = imageView;
            } catch (IOException e) {
                Logger.E(TAG, e);
            }
            break;
        case 1:
            {
                Logger.I(TAG, "Loading html: " + fn[type]);

                WebView webView = new WebView(activity);
                webView.setVerticalScrollBarEnabled(false);
                webView.setHorizontalScrollBarEnabled(false);
                webView.loadUrl("file:///android_asset/" + fn[type]);
                mView = webView;
            }
            break;
        default:
            {
                mSplashScreenListener.onSplashScreenGone(this);
            }
            return;
        }
        activity.addContentView(mView, new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
        Logger.I(TAG, "Done");
    }
	
	public void start() {
	    mStartTimeMs = SystemClock.uptimeMillis();
		nativeStart();
	}
	
	@Override
	public View getView() {
		return mBackendView.getView();
	}

    @Override
    public void setWebView(IRhoWebView view, int index) {
        mBackendView.setWebView(view, index);
    }

    @Override
    public IRhoWebView getWebView(int index) {
        return mBackendView.getWebView(index);
    }

    @Override
    public IRhoWebView getWebView(Object hdl) {
        return mBackendView.getWebView(hdl);
    }

    @Override
    public int getWebViewTab(Object hdl) {
        return mBackendView.getWebViewTab(hdl);
    }

	@Override
	public void goBack() {
		mBackendView.goBack();
	}
	
	@Override
	public void navigate(final String url, final int index) {

        Logger.D(TAG, "navigate: url=" + url);

        mBackendView.navigate(url, index);

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
	}

    @Override
    public IRhoWebView detachWebView() {
        return mBackendView.detachWebView();
    }

    @Override
    public void destroy() {
        mBackendView.destroy();
    }

    @Override
    public void back(int index) {
        mSplashScreenListener.onSplashScreenNavigateBack();
    }

	@Override
	public void forward(int index) {
	}
	@Override
	public void reload(int index) {
	    mBackendView.reload(index);
	}
	@Override
	public String currentLocation(int index) {
		return mBackendView.currentLocation(index);
	}
	@Override
	public void switchTab(int index) {
	    mBackendView.switchTab(index);
	}
	@Override
	public int activeTab() {
		return mBackendView.activeTab();
	}
	@Override
	public int getTabsCount() {
		return mBackendView.getTabsCount();
	}
	@Override
	public void loadData(String data, int index) {
	}
	@Override
	public void addNavBar(String title, Map<Object, Object> left, Map<Object, Object> right) {
	}
	@Override
	public void removeNavBar() {
	}
    @Override
    public void executeJS(String js, int index) {
        mBackendView.executeJS(js, index);
    }
    @Override
    public void stopNavigate(int index) {
        mBackendView.stopNavigate(index);
    }
    @Override
    public void saveCurrentPage(String format, String path, int index) {
        mBackendView.saveCurrentPage(format, path, index);
    }
    
	public String get_current_url(int tab_index) {
		return mBackendView.get_current_url(tab_index);
	}    
}
