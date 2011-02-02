package com.rhomobile.rhodes.mapview;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.util.PerformOnUiThread;

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
	
	private static class Touch {
		public float x;
		public float y;
		
		public Touch(float x, float y) {
			this.x = x;
			this.y = y;
		}
	};
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		mNativeDevice = getIntent().getLongExtra(INTENT_EXTRA_PREFIX + ".nativeDevice", 0);
		if (mNativeDevice == 0)
			throw new IllegalArgumentException();
		
		View view = new View(this) {
			
			private Touch[] mTouch = {null, null};
			
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
			
			private void dumpEvent(MotionEvent event) {
			   String names[] = { "DOWN" , "UP" , "MOVE" , "CANCEL" , "OUTSIDE" ,
			      "POINTER_DOWN" , "POINTER_UP" , "7?" , "8?" , "9?" };
			   StringBuilder sb = new StringBuilder();
			   int action = event.getAction();
			   int actionCode = action & MotionEvent.ACTION_MASK;
			   sb.append("event ACTION_" ).append(names[actionCode]);
			   if (actionCode == MotionEvent.ACTION_POINTER_DOWN
			         || actionCode == MotionEvent.ACTION_POINTER_UP) {
			      sb.append("(pid " ).append(
			      action >> MotionEvent.ACTION_POINTER_ID_SHIFT);
			      sb.append(")" );
			   }
			   sb.append("[" );
			   for (int i = 0; i < event.getPointerCount(); i++) {
			      sb.append("#" ).append(i);
			      sb.append("(pid " ).append(event.getPointerId(i));
			      sb.append(")=" ).append((int) event.getX(i));
			      sb.append("," ).append((int) event.getY(i));
			      if (i + 1 < event.getPointerCount())
			         sb.append(";" );
			   }
			   sb.append("]" );
			   Log.d(TAG, sb.toString());
			}
			
			@Override
			public boolean onTouchEvent (MotionEvent event) {
				dumpEvent(event);

				/*
				int action = event.getAction();
				int actionCode = action & MotionEvent.ACTION_MASK;
				
				if (actionCode == MotionEvent.ACTION_POINTER_DOWN ||
						actionCode == MotionEvent.ACTION_UP) {
					int pointerId = action >> MotionEvent.ACTION_POINTER_ID_SHIFT;
			      	if (actionCode == MotionEvent.ACTION_POINTER_UP)
			      		mTouch[pointerId] = null;
			      	if (actionCode == MotionEvent.ACTION_POINTER_DOWN)
			      		mTouch[pointerId] = new Touch(event.getX(idx));
				}
				
				switch (actionCode) {
				case MotionEvent.ACTION_DOWN:
					for (int i = 0; i < mTouch.length; ++i)
						mTouch[i] = null;
				case MotionEvent.ACTION_POINTER_DOWN:
					for (int i = 0; i < event.getPointerCount(); ++i) {
						int pointerId = event.getPointerId(i);
						float x = event.getX(i);
						float y = event.getY(i);
						mTouch[pointerId] = new Touch(x, y);
					}
					break;
				case MotionEvent.ACTION_POINTER_UP:
					for (int i = 0; i < event.getPointerCount(); ++i) {
						int pointerId = event.getPointerId(i);
						
					}
				}
				*/
				
				return true;
			}
		};
		setContentView(view);
	}
	
	@Override
	protected void onStop() {
		destroy(mNativeDevice);
		mNativeDevice = 0;
		finish();
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
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				getWindow().getDecorView().invalidate();
			}
		}, false);
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
