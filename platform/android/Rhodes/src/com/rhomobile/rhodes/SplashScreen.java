package com.rhomobile.rhodes;

import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.ImageView;

public class SplashScreen {
	
	private static final String LOADING_ANDROID_PNG = "apps/app/loading.android.png";
	private static final String LOADING_PNG = "apps/app/loading.png";
	private static final String LOADING_PAGE = "apps/app/loading.html";
	
	private static final String TAG = "RhoSplashScreen";	
	
	private View view;
	
	private native void nativeStart();
	private native void nativeHide();
	
	public class SplashImageView extends ImageView {
		private boolean setupExecuted = false;
		
		public SplashImageView(Context ctx) {
			super(ctx);
		}
		
		protected void onDraw (Canvas canvas) {
			RhodesService.platformLog(TAG, "Splash Screen Image REAL DRAWING !");
			super.onDraw(canvas);
			if (!setupExecuted) {
				Rhodes.runPostponedSetup();
				setupExecuted = true;
			}
		}
		
		
	}
	

	public SplashScreen(Context ctx) {
		RhodesService.platformLog(TAG, "SplashScreen()");
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
			v.setId(RhodesService.RHO_SPLASH_VIEW);
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
				v.setId(RhodesService.RHO_SPLASH_VIEW);
				view = v;
			}
			catch (IOException e) {
				
				WebView v = RhodesService.createLoadingWebView(ctx);
				
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
				
				v.setId(RhodesService.RHO_SPLASH_VIEW);
				
				view = v;
			}
		}
	}
	
	public void start(ViewGroup outer) {
		outer.removeAllViews();
		outer.addView(view);
		RhodesService.platformLog(TAG, " view was showed on screen");
	}
	
	public void rho_start() {
		nativeStart();
		RhodesService.platformLog(TAG, " rho native loading splash screen started");
	}
	
	public void hide(ViewGroup outer) {
		nativeHide();
		outer.removeView(view);
		RhodesService.platformLog(TAG, " splash screen closed");
	}
}
