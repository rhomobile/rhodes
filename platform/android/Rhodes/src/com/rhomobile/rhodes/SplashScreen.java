package com.rhomobile.rhodes;

import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.ViewGroup;
import android.widget.ImageView;

public class SplashScreen {
	
	private ImageView view;
	
	private native void nativeStart();
	private native void nativeHide();
	
	public SplashScreen(Context ctx, String file) throws IOException {
		view = new ImageView(ctx);
		AssetManager am = ctx.getResources().getAssets();
		InputStream is = am.open(file);
		Bitmap bitmap = BitmapFactory.decodeStream(is);
		view.setImageBitmap(bitmap);
		view.setId(Rhodes.RHO_SPLASH_VIEW);
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
