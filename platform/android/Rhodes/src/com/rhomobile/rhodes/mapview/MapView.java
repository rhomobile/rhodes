package com.rhomobile.rhodes.mapview;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Bundle;
import android.view.View;

import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;

public class MapView extends BaseActivity {
	
	private static final String TAG = MapView.class.getSimpleName();
	
	private static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + ".MapView";
	
	public native void setSize(long nativeDevice, int width, int height);
	public native void paint(long nativeDevice, Canvas canvas);
	public native void destroy(long nativeDevice);
	
	private long mNativeDevice;

	public static void create(long nativeDevice) {
		RhodesActivity r = RhodesActivity.getInstance();
		if (r == null) {
			Logger.E(TAG, "Can't create map view because main activity is null");
			return;
		}
		
		Intent intent = new Intent(r, MapView.class);
		intent.putExtra(INTENT_EXTRA_PREFIX + ".nativeDevice", nativeDevice);
		r.startActivity(intent);
	}
	
	public static void destroy(MapView device) {
		device.finish();
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		mNativeDevice = getIntent().getLongExtra(INTENT_EXTRA_PREFIX + ".nativeDevice", 0);
		if (mNativeDevice == 0)
			throw new IllegalArgumentException();
		
		View view = new View(this) {
			@Override
			protected void dispatchDraw(Canvas canvas) {
				super.dispatchDraw(canvas);
				paint(mNativeDevice, canvas);
			}
			
			@Override
			protected void onSizeChanged(int w, int h, int oldW, int oldH) {
				super.onSizeChanged(w, h, oldW, oldH);
				if (w != oldW || h != oldH)
					setSize(mNativeDevice, w, h);
			}
		};
		setContentView(view);
	}
	
	@Override
	protected void onStop() {
		destroy(mNativeDevice);
		mNativeDevice = 0;
		super.onStop();
	}
	
	@Override
	public void onDestroy() {
		super.onDestroy();
	}
	
	public void drawImage(Canvas canvas, int x, int y, Bitmap bm) {
		Paint paint = new Paint();
		canvas.drawBitmap(bm, x, y, paint);
	}
	
	public void drawText(Canvas canvas, int x, int y, String text, int color) {
		Paint paint = new Paint();
		paint.setColor(color);
		canvas.drawText(text, x, y, paint);
	}
	
	public void redraw() {
		getWindow().getDecorView().invalidate();
	}
	
	public static Bitmap createImage(String path) {
		return BitmapFactory.decodeFile(path);
	}
	
	public static Bitmap createImage(byte[] data) {
		return BitmapFactory.decodeByteArray(data, 0, data.length);
	}
	
	public static void destroyImage(Bitmap bm) {
		bm.recycle();
	}
}
