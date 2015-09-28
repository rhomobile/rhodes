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

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Map;
import java.lang.Thread;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Environment;
import android.os.SystemClock;
import android.view.View;
import android.webkit.WebView;
import android.widget.ImageView;
import android.widget.LinearLayout;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesApplication;
import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.webview.WebViewConfig;

public class SplashScreen implements MainView{
    
    public interface SplashScreenListener {
        void onSplashScreenGone(SplashScreen splashScreen);
        void onSplashScreenNavigateBack();
    }
    
    private class SplashScreenExtension extends AbstractRhoExtension {
        private boolean mIsActive = true;
        @Override
        public boolean onNavigateComplete(IRhoExtManager extManager, String urlOfDocument, IRhoWebView ext, boolean res) {
            
            if (mIsActive) {
			long  mSplashScreenDurationValue = 0; //By default the splash screen duration is set to 0
			IRhoConfig rhoelementsGetConfig=  RhoExtManager.getInstance().getConfig("rhoelementsext");
			//Get Duration of Splash Screen from Config.xml in milli seconds.
			String mGetLoadingPNGDurationValue = null;
			if(rhoelementsGetConfig != null){
			 mGetLoadingPNGDurationValue = rhoelementsGetConfig.getString(WebViewConfig.SETTING_SPLASHSCREEN_DURATION);
			}
			if(mGetLoadingPNGDurationValue != null)
			{
				try {
					mSplashScreenDurationValue = Long.parseLong(mGetLoadingPNGDurationValue, 10); 	
					if(mSplashScreenDurationValue<0)
						mSplashScreenDurationValue=0;
				}catch (NumberFormatException nfe) {
					Logger.I(TAG, "NumberFormatException: " + nfe.getMessage());
			    }
			}
	                activateHideTimer(mSplashScreenDurationValue);
			mIsActive = false;
            }
            return res;
        }
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
    
    private SplashScreenExtension mDocCompleteListener;
	private ImageView.ScaleType mScaleType = ImageView.ScaleType.FIT_CENTER;
    private int mBackgroundColor = Color.BLACK;

    public SplashScreen(Activity activity, MainView mainView, SplashScreenListener listener) {
        mSplashScreenListener = listener;
        mBackendView = mainView;
        readConfig();
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
                imageView.setBackgroundColor(mBackgroundColor);
                
                String actualSplashScreenPathValue="";
                String appendSDCardPathToActualSplashScreenPathValue="";
                String mGetSplashScreenPathValue=null;
                boolean isDefaultSplashScreenLoading = true;                
                IRhoConfig rhoelementsConfig=  RhoExtManager.getInstance().getConfig("rhoelementsext");
                if(rhoelementsConfig!=null)
                	mGetSplashScreenPathValue = rhoelementsConfig.getString(WebViewConfig.SETTING_SPLASHSCREEN_PATH); 
                
                if(mGetSplashScreenPathValue != null && !(mGetSplashScreenPathValue.isEmpty())){
	                if(mGetSplashScreenPathValue.startsWith("file://", 0)){
	                	actualSplashScreenPathValue = mGetSplashScreenPathValue.substring(7);
	                	appendSDCardPathToActualSplashScreenPathValue = Environment.getExternalStorageDirectory() + actualSplashScreenPathValue;
	                	isDefaultSplashScreenLoading = false;
	                }
	                else if(mGetSplashScreenPathValue.startsWith("http://", 0) || mGetSplashScreenPathValue.startsWith("https://", 0)){
	                	isDefaultSplashScreenLoading = true;
	                }
	                else if(mGetSplashScreenPathValue.startsWith("/", 0)){
	                	actualSplashScreenPathValue = mGetSplashScreenPathValue;
	                	appendSDCardPathToActualSplashScreenPathValue = Environment.getExternalStorageDirectory() + actualSplashScreenPathValue;
	                	isDefaultSplashScreenLoading = false;
	                }
	                else if(!mGetSplashScreenPathValue.startsWith("/", 0)){
	                	actualSplashScreenPathValue = "/" + mGetSplashScreenPathValue;
	                	appendSDCardPathToActualSplashScreenPathValue = Environment.getExternalStorageDirectory() + actualSplashScreenPathValue;
	                	isDefaultSplashScreenLoading = false;
	                }	
	                
	                if(!actualSplashScreenPathValue.startsWith("/", 0) && !isDefaultSplashScreenLoading){
	                	actualSplashScreenPathValue = "/" + actualSplashScreenPathValue;
	                	appendSDCardPathToActualSplashScreenPathValue = Environment.getExternalStorageDirectory() + actualSplashScreenPathValue;
	                }
	                
	                if(!isDefaultSplashScreenLoading){ 
	                	File file = new File(appendSDCardPathToActualSplashScreenPathValue.toString());	                 
	                	boolean featureDemoFileExists = file.exists();	                 
		                if (featureDemoFileExists)              
		                {
		                 	InputStream fileInputStream = new FileInputStream(file);                 
		                 	imageView.setImageBitmap(BitmapFactory.decodeStream(fileInputStream));	                 
		                }
		                else
		                {
					isDefaultSplashScreenLoading = true;
		                }
	                }
                }                 
                
                if(isDefaultSplashScreenLoading){
                	Logger.I(TAG, "Specified splashscreen image is not found or supported. Setting the default splashscreen image.");    
                	imageView.setImageBitmap(BitmapFactory.decodeStream(am.open(fn[type])));                 
                } 
                
                imageView.setAdjustViewBounds(true);
                imageView.setScaleType(mScaleType);
                mView = imageView;
                RhoExtManager.getInstance().registerExtension("SplashScreen", mDocCompleteListener = new SplashScreenExtension());
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
    
    private void readConfig() {
        String configString = RhoConf.getString("splash_screen");
        if (configString != null) {
            String [] configParams = configString.split(";");
            
            for(String param: configParams) {
                
                Logger.I(TAG, "Process config: " + param);
                
                if (param.equals("zoom")) {
                    mScaleType = ImageView.ScaleType.FIT_CENTER;
                    Logger.I(TAG, "Scale type: zoom");
                } else
                if (param.equals("stretch")) {
                    mScaleType = ImageView.ScaleType.FIT_XY;
                    Logger.I(TAG, "Scale type: stretch");
                } else
                if (param.startsWith("bgcolor=#")) {
                    String colorString = param.substring(9);
                    boolean hasAlpha = colorString.length() == 8;
                    if (colorString.length() == 6 || hasAlpha) {
                        int alpha = hasAlpha ? Integer.parseInt(colorString.substring(0, 2), 16) : 0xff;
                        int red   = Integer.parseInt(hasAlpha ? colorString.substring(2, 4) : colorString.substring(0, 2), 16);
                        int green = Integer.parseInt(hasAlpha ? colorString.substring(4, 6) : colorString.substring(2, 4), 16);
                        int blue  = Integer.parseInt(hasAlpha ? colorString.substring(6) : colorString.substring(4), 16);
                        
                        mBackgroundColor = Color.argb(alpha, red, green, blue);
                        
                        Logger.I(TAG, "Background color: " + mBackgroundColor);
                    }
                }
            }
        }
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
        Logger.T(TAG, "navigate: " + url);

        mBackendView.navigate(url, index);
        
        if (mDocCompleteListener == null) {
            activateHideTimer(0);
        }
	}

    private void activateHideTimer(long mSplashScreenDurationValue) {
        long delay = mSplashScreenDurationValue;
        if(delay == 0){
        	delay = howLongWaitMs();
        }
        if (delay < 0) {
            return;
        }
        else if (delay > 0){
            delay = mStartTimeMs + delay - SystemClock.uptimeMillis();
            if (delay < 0)
                delay = 0;
        }

        Logger.T(TAG, "Delay to remove SplashScreen = " + String.valueOf(delay));

        final long threadDelay = delay;
        mSleepThread = new Thread(new Runnable() {
            @Override
            public void run() {
                SystemClock.sleep(threadDelay);
                removeSplashScreen();
            }

        });
        mSleepThread.start();
    }

    @Override
    public void removeSplashScreen() {
        RhodesApplication.runWhen(
            RhodesApplication.AppState.AppActivated,
            new RhodesApplication.StateHandler(true) {
                @Override public void run() {
                    PerformOnUiThread.exec(new Runnable() {
                        public void run() {
                            
                            Logger.T(TAG, "Do removing Splash Screen");
                            try {
                                if (mNavigateIndex != 0) {
                                    throw new IllegalStateException("Non zero tab index(" + mNavigateIndex + ") to navigate from Splash Screen");
                                }
                                mSplashScreenListener.onSplashScreenGone(SplashScreen.this);
                            } catch (Throwable e) {
                                Logger.E(TAG, e);
                            }
                        }
                    });
                }
            });
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
	
	@Override
	public String getTabDefaultUrl() {
		// TODO Auto-generated method stub
		return "";
	}
}
