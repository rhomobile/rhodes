package com.rhomobile.rhodes;

import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;
import android.widget.ImageView;

public class SplashScreen {
	
	private static final String TAG = SplashScreen.class.getName();
	
	private static final boolean DEBUG = false;
	
	private static final String LOADING_ANDROID_PNG = "apps/app/loading.android.png";
	private static final String LOADING_PNG = "apps/app/loading.png";
	private static final String LOADING_PAGE = "apps/app/loading.html";
	
	private View mContentView;
	
	private native void nativeStart();
	private native void nativeHide();
	
	/*
	public class SplashImageView extends ImageView {
		public boolean setupExecuted = false;
		
		public SplashImageView(Context ctx) {
			super(ctx);
		}
		
		protected void onDraw (Canvas canvas) {
			RhoService.platformLog(TAG, "Splash Screen Image REAL DRAWING !");
			super.onDraw(canvas);
			if (!setupExecuted) {
				Rhodes.runPostponedSetup();
				setupExecuted = true;
			}
		}
		
		
	}
	*/
	
	public SplashScreen(Context context) {
		AssetManager am = context.getResources().getAssets();
		mContentView = createImageView(context, am);
		if (mContentView == null)
			mContentView = createHtmlView(context, am);
	}
	
	public View getContentView() {
		return mContentView;
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

	/*
	public SplashScreen(Context ctx) {
		RhoService.platformLog(TAG, "SplashScreen()");
		AssetManager am = ctx.getResources().getAssets();
		//RhodesService r = RhodesService.getInstance();
		//boolean bc = r.isBundleChanged();
		
		String file = LOADING_ANDROID_PNG;
		
		boolean loading_android_ok = false;
		
		Bitmap bitmap = null;
		try {
			InputStream is = am.open(file);
			bitmap = BitmapFactory.decodeStream(is);
			is.close();
			
			ImageView v = new SplashImageView(ctx);
			v.setImageBitmap(bitmap);
			v.setAdjustViewBounds(false);
			v.setId(RhoService.RHO_SPLASH_VIEW);
			view = v;
			loading_android_ok = true;
		}
		catch (IOException e) {
		}
		if (!loading_android_ok) { 
			try {
				file = LOADING_PNG;
				InputStream is = am.open(file);
				bitmap = BitmapFactory.decodeStream(is);
				is.close();
				
				ImageView v = new SplashImageView(ctx);
				v.setImageBitmap(bitmap);
				v.setAdjustViewBounds(false);
				v.setId(RhoService.RHO_SPLASH_VIEW);
				view = v;
			}
			catch (IOException e) {
				
				WebView v = RhoService.createLoadingWebView(ctx);
				
				String page = LOADING_PAGE;
				
				boolean hasNeededPage;
				try {
					InputStream is1 = am.open(page);
					if (is1 != null)
						is1.close();
					hasNeededPage = true;
				}
				catch (IOException e1) {
					hasNeededPage = false;
				}
				
				if (hasNeededPage) {
					v.loadUrl("file:///android_asset/" + page);
				}
				else {
					v.loadData("<html><title>Loading</title><body>Loading...</body></html>", "text/html", "utf-8");
				}
				
				v.setId(RhoService.RHO_SPLASH_VIEW);
				
				view = v;
			}
		}
	}
	*/
	
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
