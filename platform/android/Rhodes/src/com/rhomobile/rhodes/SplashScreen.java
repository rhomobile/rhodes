package com.rhomobile.rhodes;

import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.ImageView;

public class SplashScreen {
	
	private static final String INSTALLING_PNG = "apps/app/installing.png";
	private static final String LOADING_PNG = "apps/app/loading.png";
	
	private static final String INSTALLING_PAGE = "apps/app/installing.html";
	private static final String LOADING_PAGE = "apps/app/loading.html";
	
	private View view;
	
	private native void nativeStart();
	private native void nativeHide();
	
	public SplashScreen(Context ctx) {
		AssetManager am = ctx.getResources().getAssets();
		Rhodes r = RhodesInstance.getInstance();
		//boolean bc = r.isBundleChanged();
		
		String file = LOADING_PNG;
		
		Bitmap bitmap = null;
		try {
			InputStream is = am.open(file);
			bitmap = BitmapFactory.decodeStream(is);
			is.close();
			
			ImageView v = new ImageView(ctx);
			v.setImageBitmap(bitmap);
			v.setId(Rhodes.RHO_SPLASH_VIEW);
			view = v;
		}
		catch (IOException e) {
			WebView v = r.createWebView();
			
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
			
			v.setId(Rhodes.RHO_SPLASH_VIEW);
			
			view = v;
		}
	}
	
	public void start(ViewGroup outer) {
		outer.removeAllViews();
		outer.addView(view);
		nativeStart();
	}
	
	public void hide(ViewGroup outer) {
		nativeHide();
		outer.removeView(view);
	}
}
