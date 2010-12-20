package com.rhomobile.rhodes;

import java.io.IOException;
import java.io.InputStream;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;
import android.widget.ImageView;

public class SplashScreenActivity extends BaseActivity {
	
	private static final String TAG = SplashScreenActivity.class.getName();
	
	private static final boolean VERBOSE = false;
	
	private static final String LOADING_ANDROID_PNG = "apps/app/loading.android.png";
	private static final String LOADING_PNG = "apps/app/loading.png";
	private static final String LOADING_PAGE = "apps/app/loading.html";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		AssetManager am = getResources().getAssets();
		
		View contentView = createImageView(am);
		if (contentView == null)
			contentView = createHtmlView(am);
		
		setContentView(contentView);
	}
	
	@Override
	public void onStop() {
		// Exit from application if we are go to background on splash screen
		getRhodesApplication().exit();
		super.onStop();
	}
	
	private View createImageView(AssetManager am) {
		String[] imageFiles = {LOADING_ANDROID_PNG, LOADING_PNG};
		for (String imageFile : imageFiles) {
			InputStream is = null;
			try {
				is = am.open(imageFile);
				Bitmap bitmap = BitmapFactory.decodeStream(is);
				
				ImageView view = new ImageView(this);
				view.setImageBitmap(bitmap);
				view.setAdjustViewBounds(false);
				
				return view;
			} catch (IOException e) {
				if (VERBOSE)
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
	
	private View createHtmlView(AssetManager am) {
		boolean hasNeededPage;

		String page = LOADING_PAGE;
		InputStream is = null;
		try {
			is = am.open(page);
			hasNeededPage = true;
		} catch (IOException e) {
			if (VERBOSE)
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
		WebView view = new WebView(this);
		
		if (hasNeededPage)
			view.loadUrl("file:///android_asset/" + page);
		else
			view.loadData("<html><title>Loading</title><body>Loading...</body></html>", "text/html", "utf-8");
		
		return view;
	}

}
