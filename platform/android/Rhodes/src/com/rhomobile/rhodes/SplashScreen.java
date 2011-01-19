package com.rhomobile.rhodes;

import java.io.IOException;
import java.io.InputStream;
import java.util.Map;

import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.mainview.SimpleMainView;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;
import android.widget.ImageView;

public class SplashScreen implements MainView {
	
	private static final String TAG = SplashScreen.class.getSimpleName();
	
	private static final boolean DEBUG = false;
	
	private static final String LOADING_ANDROID_PNG = "apps/app/loading.android.png";
	private static final String LOADING_PNG = "apps/app/loading.png";
	private static final String LOADING_PAGE = "apps/app/loading.html";
	
	private View mContentView;
	
	private WebView mWebView;
	
	private native void nativeStart();
	private native void nativeHide();
	private native int howLongWaitMs();
	
	public SplashScreen(Context context) {
		AssetManager am = context.getResources().getAssets();
		mContentView = createImageView(context, am);
		if (mContentView == null)
			mContentView = createHtmlView(context, am);
	}
	
	private View createImageView(Context context, AssetManager am) {
		String[] imageFiles = {LOADING_ANDROID_PNG, LOADING_PNG};
		for (String imageFile : imageFiles) {
			InputStream is = null;
			try {
				is = am.open(imageFile);
				Bitmap bitmap = BitmapFactory.decodeStream(is);
				
				ImageView view = new ImageView(context);
				view.setImageBitmap(bitmap);
				view.setAdjustViewBounds(false);
				
				return view;
			} catch (IOException e) {
				if (DEBUG)
					Log.d(TAG, "Can't load " + imageFile, e);
			}
			finally {
				if (is != null)
					try {
						is.close();
					} catch (IOException e) {}
			}
		}
		
		return null;
	}
	
	private View createHtmlView(Context context, AssetManager am) {
		boolean hasNeededPage;

		String page = LOADING_PAGE;
		InputStream is = null;
		try {
			is = am.open(page);
			hasNeededPage = true;
		} catch (IOException e) {
			if (DEBUG)
				Log.d(TAG, "Can't load " + page, e);
			hasNeededPage = false;
		}
		finally {
			if (is != null)
				try {
					is.close();
				} catch (IOException e) {}
		}
		
		// Now create WebView and load appropriate content there
		WebView view = new WebView(context);
		
		if (hasNeededPage)
			view.loadUrl("file:///android_asset/" + page);
		else
			view.loadData("<html><title>Loading</title><body>Loading...</body></html>", "text/html", "utf-8");
		
		return view;
	}
	
	public void start() {
		nativeStart();
	}
	
	@Override
	public View getView() {
		return mContentView;
	}
	
	@Override
	public WebView getWebView(int index) {
		if (mWebView == null) {
			RhodesActivity ra = RhodesActivity.getInstance();
			mWebView = ra.createWebView();
		}
		return mWebView;
	}
	
	@Override
	public void goBack() {
		// Nothing here
	}
	
	@Override
	public void navigate(String url, int index) {
		if (DEBUG)
			Log.d(TAG, "navigate: url=" + url);

        RhodesService r = RhodesService.getInstance();
		MainView mainView = r.getMainView();
		SimpleMainView v = new SimpleMainView(mainView);
		r.setMainView(v);		
		//getWebView(0).loadUrl(url);
		v.navigate(url,0);
	}
	
	@Override
	public WebView detachWebView() {
		return getWebView(0);
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
		// TODO Auto-generated method stub
		
	}
	@Override
	public void reload(int index) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public String currentLocation(int index) {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public void switchTab(int index) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public int activeTab() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getTabsCount() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public void loadData(String data, int index) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void addNavBar(String title, Map<Object, Object> left,
			Map<Object, Object> right) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void removeNavBar() {
		// TODO Auto-generated method stub
		
	}

	/*
	public void start(ViewGroup outer) {
		outer.removeAllViews();
		if (view instanceof SplashImageView) {
			((SplashImageView)view).setupExecuted = false;
		}
		outer.addView(view);
		RhoService.platformLog(TAG, " view was showed on screen");
	}
	
	public void rho_start() {
		nativeStart();
		RhoService.platformLog(TAG, " rho native loading splash screen started");
	}
	
	public void hide(ViewGroup outer) {
		nativeHide();
		outer.removeView(view);
		RhoService.platformLog(TAG, " splash screen closed");
	}
	*/
}
