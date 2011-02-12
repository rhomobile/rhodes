package com.rhomobile.rhodes.mapview;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Build;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.animation.Animation;
import android.view.animation.ScaleAnimation;
import android.widget.FrameLayout;
import android.widget.ZoomButtonsController;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.mapview.MapTouch.Touch;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class MapView extends BaseActivity implements MapTouch {
	
	private static final String TAG = MapView.class.getSimpleName();
	
	private static final boolean ZOOM_ANIMATION_ENABLED = true;
	
	private static final int ZOOM_RESOLUTION = 50;
	
	private static final boolean ENABLE_MULTI_TOUCH = false;
	
	private static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + ".MapView";
	
	public native void setSize(MapView javaDevice, long nativeDevice, int width, int height);
	
	public native void setPinImage(long nativeDevice, Bitmap pin);
	public native void setPinCalloutImage(long nativeDevice, Bitmap pin);
	
	public native int minZoom(long nativeDevice);
	public native int maxZoom(long nativeDevice);
	public native int zoom(long nativeDevice);
	public native void setZoom(long nativeDevice, int zoom);
	
	public native void move(long nativeDevice, int dx, int dy);
	public native void click(long nativeDevice, int x, int y);
	
	public native void paint(long nativeDevice, Canvas canvas);
	public native void destroy(long nativeDevice);
	
	private TouchHandler mTouchHandler;
	
	private static MapView mc = null;
	
	private Touch mTouchFirst;
	private Touch mTouchSecond;
	private double mDistance;
	private float mScale;
	
	private View mSurface;
	private ZoomButtonsController mZoomController;
	
	private long mNativeDevice;
	
	private TouchHandler createTouchHandler() {
		String className;
		int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
		if (sdkVersion < Build.VERSION_CODES.ECLAIR)
			className = "OneTouchHandler";
		else
			className = "MultiTouchHandler";
		
		try {
			String pkgname = TouchHandler.class.getPackage().getName();
			String fullName = pkgname + "." + className;
			Class<? extends TouchHandler> klass =
				Class.forName(fullName).asSubclass(TouchHandler.class);
			return klass.newInstance();
		}
		catch (Exception e) {
			throw new IllegalStateException(e);
		}
	}

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
	
	public static void destroy() {
		if (mc != null) {
			mc.finish();
			mc = null;
		}
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mc = this;
		
		mNativeDevice = getIntent().getLongExtra(INTENT_EXTRA_PREFIX + ".nativeDevice", 0);
		if (mNativeDevice == 0)
			throw new IllegalArgumentException();
		
		Bitmap pin = BitmapFactory.decodeResource(getResources(), AndroidR.drawable.marker);
		setPinImage(mNativeDevice, pin);
		Bitmap pinCallout = BitmapFactory.decodeResource(getResources(), AndroidR.drawable.callout);
		setPinCalloutImage(mNativeDevice, pinCallout );
				
		mTouchHandler = createTouchHandler();
		mTouchHandler.setMapTouch(this);
		
		FrameLayout pv = new FrameLayout(this);
		setContentView(pv);
		
		mSurface = new View(this) {
			
			@Override
			protected void dispatchDraw(Canvas canvas) {
				//super.dispatchDraw(canvas);
				paint(mNativeDevice, canvas);
			}
			
			@Override
			protected void onSizeChanged(int w, int h, int oldW, int oldH) {
				super.onSizeChanged(w, h, oldW, oldH);
				if (w != oldW || h != oldH)
					setSize(MapView.this, mNativeDevice, w, h);
			}
			
			@Override
			protected void onDetachedFromWindow() {
				mZoomController.setVisible(false);
			}
			
			@Override
			public boolean onTouchEvent (MotionEvent event) {
				mZoomController.setVisible(true);
				
				if (mTouchHandler.handleTouch(event))
					return true;
				
				return super.onTouchEvent(event);
			}
		};
		
		pv.addView(mSurface, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		
		mZoomController = new ZoomButtonsController(mSurface);
		mZoomController.setAutoDismissed(false);
		mZoomController.setVisible(true);
		mZoomController.setOnZoomListener(new ZoomButtonsController.OnZoomListener() {
			
			@Override
			public void onZoom(boolean zoomIn) {
				zoomAnimated(zoomIn ? 1 : -1);
			}
			
			@Override
			public void onVisibilityChanged(boolean visible) {}
		});
	}
	
	@Override
	protected void onStop() {
		destroy(mNativeDevice);
		mNativeDevice = 0;
		mc = null;
		finish();
		super.onStop();
	}
	
	@Override
	public void onDestroy() {
		super.onDestroy();
	}
	
	public int zoom(int n) {
		int min = minZoom(mNativeDevice);
		int max = maxZoom(mNativeDevice);
		int zoom = zoom(mNativeDevice);
		zoom += n;
		if (zoom < min) zoom = min;
		if (zoom > max) zoom = max;
		
		mZoomController.setZoomOutEnabled(zoom > min);
		mZoomController.setZoomInEnabled(zoom < max);
		return zoom;
	}
	
	public void zoomAnimated(int n) {
		int zoom = zoom(n);
		
		if (!ZOOM_ANIMATION_ENABLED)
			setZoom(mNativeDevice, zoom);
		else {
			final int finalZoom = zoom;
			
			float from = 1f;
			float to = from * (n > 0 ? 2f : 0.5f);
			Animation anim = new ScaleAnimation(from, to, from, to,
					Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF, 0.5f);
			anim.setDuration(400);
			anim.setFillAfter(false);
			anim.setAnimationListener(new Animation.AnimationListener() {
				@Override
				public void onAnimationStart(Animation animation) {}
				@Override
				public void onAnimationRepeat(Animation animation) {}
				@Override
				public void onAnimationEnd(Animation animation) {
					setZoom(mNativeDevice, finalZoom);
				}
			});

			mSurface.startAnimation(anim);
		}
	}
	
	public void drawImage(Canvas canvas, int x, int y, Bitmap bm) {
		Paint paint = new Paint();
		canvas.drawBitmap(bm, x, y, paint);
	}
	
	public void drawText(Canvas canvas, int x, int y, String text, int color) 
	{
		Logger.I(TAG, "drawText: " + text);
		Paint paint = new Paint();
		//paint.setColor(color);
		canvas.drawText(text, x, y, paint);
	}
	
	public void redraw() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				mSurface.invalidate();
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

	@Override
	public void touchClick(Touch touch) {
		click(mNativeDevice, (int)touch.x, (int)touch.y);
	}
	
	@Override
	public void touchDown(Touch first, Touch second) {
		mTouchFirst = first;
		mTouchSecond = second;
		if (ENABLE_MULTI_TOUCH) {
			if (first != null && second != null) {
				mDistance = Math.sqrt(Math.pow(first.x - second.x, 2) +
						Math.pow(first.y - second.y, 2));
				mScale = 1f;
			}
		}
	}

	@Override
	public void touchUp(Touch first, Touch second) {
		mTouchFirst = first;
		mTouchSecond = second;
		if (ENABLE_MULTI_TOUCH) {
			if (ZOOM_ANIMATION_ENABLED) {
				if (first != null && second != null) {
					double newDistance = Math.sqrt(Math.pow(first.x - second.x, 2) +
							Math.pow(first.y - second.y, 2));
					int n = (int)((newDistance - mDistance)/ZOOM_RESOLUTION);
					int zoom = zoom(n);
					setZoom(mNativeDevice, zoom);
				}
			}
		}
	}

	@Override
	public void touchMove(Touch first, Touch second) {
		if (first == null || second == null) {
			// Move
			Touch t = first == null ? second : first;
			Touch prev = first == null ? mTouchSecond : mTouchFirst;
			
			int dx = (int)(prev.x - t.x);
			int dy = (int)(prev.y - t.y);
			prev.x = t.x;
			prev.y = t.y;
			
			if (dx != 0 || dy != 0)
				move(mNativeDevice, dx, dy);
		}
		
		if (ENABLE_MULTI_TOUCH) {
			if (first != null && second != null) {
				double oldDistance = Math.sqrt(Math.pow(mTouchFirst.x - mTouchSecond.x, 2) +
						Math.pow(mTouchFirst.y - mTouchSecond.y, 2));
				double newDistance = Math.sqrt(Math.pow(first.x - second.x, 2) +
						Math.pow(first.y - second.y, 2));
				
				if (ZOOM_ANIMATION_ENABLED) {
					float newScale = mScale * (float)Math.pow(2, newDistance - oldDistance);
					Animation anim = new ScaleAnimation(mScale, newScale, mScale, newScale,
							Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF, 0.5f);
					anim.setDuration(400);
					anim.setFillAfter(true);
					mSurface.startAnimation(anim);
				}
				else {
					int n = (int)((newDistance - oldDistance)/ZOOM_RESOLUTION);
					int zoom = zoom(n);
					setZoom(mNativeDevice, zoom);
				}
				
				mTouchFirst = first;
				mTouchSecond = second;
			}
		}
	}
}
