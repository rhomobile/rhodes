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

package com.rhomobile.rhodes.mapview;

import java.util.Hashtable;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.animation.Animation;
import android.view.animation.ScaleAnimation;
import android.widget.FrameLayout;
import android.widget.ZoomButtonsController;

import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Utils;

public class MapView extends BaseActivity implements MapTouch {
	
	private static final String TAG = MapView.class.getSimpleName();
	
	private static final boolean ZOOM_ANIMATION_ENABLED = true;
	
	private static final int ZOOM_RESOLUTION = 50;
	
	private static final boolean ENABLE_MULTI_TOUCH = false;
	
	private static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + ".MapView";
	
	public native void setSize(MapView javaDevice, long nativeDevice, int width, int height);
	
	public native void setPinImage(long nativeDevice, int pin);
	public native void setPinCalloutImage(long nativeDevice, int pin);
	public native void setPinCalloutLinkImage(long nativeDevice, int pin);
	public native void setESRILogoImage(long nativeDevice, int esriLogo);
	public native void setGoogleLogoImage(long nativeDevice, int googleLogo);
	public native void setMyLocationImage(long nativeDevice, int pin);
	
	public native int minZoom(long nativeDevice);
	public native int maxZoom(long nativeDevice);
	public native int zoom(long nativeDevice);
	public native void setZoom(long nativeDevice, int zoom);
	
	public native void move(long nativeDevice, int dx, int dy);
	public native int click(long nativeDevice, int x, int y);
	
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
	
	public boolean mIsInExitProcess;
	
	private static int ourDensity = Bitmap.DENSITY_NONE;//DisplayMetrics.DENSITY_DEFAULT;
	
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
		Logger.I(TAG, "create()");
		resetRequestSet();
		
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
		Logger.I(TAG, "destroy()");
		final MapView mv = mc;
		mc = null;
		if (mv != null) {
			mv.mIsInExitProcess = true;
			Logger.I(TAG, "destroy() mc != null - process destroy");
			PerformOnUiThread.exec(new Runnable() {
				public void run() {
					mv.finish();
					MapBitmapManager.getSharedInstance().totalClean();
				}
			});
		}
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		Logger.I(TAG, "onCreate()");
		mIsInExitProcess = false;
		super.onCreate(savedInstanceState);

		mc = this;
		
		MapBitmapManager.getSharedInstance().totalClean();
		
		mNativeDevice = getIntent().getLongExtra(INTENT_EXTRA_PREFIX + ".nativeDevice", 0);
		if (mNativeDevice == 0)
			throw new IllegalArgumentException();
		
		BitmapFactory.Options opt = new BitmapFactory.Options();
		opt.inScaled = false;
		Bitmap pin = BitmapFactory.decodeResource(getResources(), R.drawable.marker, opt);

		//ourDensity = pin.getDensity();
		pin.setDensity(Bitmap.DENSITY_NONE);
		setPinImage(mNativeDevice, MapBitmapManager.getSharedInstance().addBitmap(pin));
		
		
		Bitmap pinCallout = BitmapFactory.decodeResource(getResources(), R.drawable.callout);
		setPinCalloutImage(mNativeDevice, MapBitmapManager.getSharedInstance().addBitmap(pinCallout) );
		Bitmap pinCalloutLink = BitmapFactory.decodeResource(getResources(), R.drawable.callout_link);
		setPinCalloutLinkImage(mNativeDevice, MapBitmapManager.getSharedInstance().addBitmap(pinCalloutLink) );
		
		Bitmap esriLogo = BitmapFactory.decodeResource(getResources(), R.drawable.esri);
		setESRILogoImage(mNativeDevice, MapBitmapManager.getSharedInstance().addBitmap(esriLogo) );

		Bitmap googleLogo = BitmapFactory.decodeResource(getResources(), R.drawable.google);
		setGoogleLogoImage(mNativeDevice, MapBitmapManager.getSharedInstance().addBitmap(googleLogo) );
		
		Bitmap pinMyLocation = BitmapFactory.decodeResource(getResources(), R.drawable.location, opt);
		pinMyLocation.setDensity(Bitmap.DENSITY_NONE);
		setMyLocationImage(mNativeDevice, MapBitmapManager.getSharedInstance().addBitmap(pinMyLocation) );
		
		mTouchHandler = createTouchHandler();
		mTouchHandler.setMapTouch(this);
		
		FrameLayout pv = new FrameLayout(this);
		setContentView(pv);
		
		mSurface = new View(this) {
			
			@Override
			protected void dispatchDraw(Canvas canvas) {
				//super.dispatchDraw(canvas);
				if (mNativeDevice != 0) {
					paint(mNativeDevice, canvas);
				}
			}
			
			@Override
			protected void onSizeChanged(int w, int h, int oldW, int oldH) {
				super.onSizeChanged(w, h, oldW, oldH);
				if (w != oldW || h != oldH)
					setSize(MapView.this, mNativeDevice, w, h);
				setZoom(mNativeDevice, zoom(mNativeDevice));
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
	public boolean onKeyDown(int keyCode, KeyEvent event) {
	    if (keyCode == KeyEvent.KEYCODE_BACK) {
			Logger.I(TAG, "onBack button pressed()");
			mIsInExitProcess = true;
			mc = null;
			mNativeDevice = 0;
			destroy(mNativeDevice);
			PerformOnUiThread.exec(new Runnable() {
				public void run() {
					 finish();
				}
			});
	    }
	    return super.onKeyDown(keyCode, event);
	}
	
	@Override
	protected void onStop() {
		Logger.I(TAG, "onStop() 1");
		super.onStop();
		Logger.I(TAG, "onStop() 2");
		//mNativeDevice = 0;
		//destroy(mNativeDevice);
		//mc = null;
		//PerformOnUiThread.exec(new Runnable() {
		//	public void run() {
		//		 finish();
		//	}
		//});
	}
	
	@Override
	public void onPause() {
		Logger.I(TAG, "onPause() 1");
		super.onDestroy();
		Logger.I(TAG, "onPause() 2");
	}
	
	@Override
	public void onDestroy() {
		Logger.I(TAG, "onDestroy() 1");
		MapBitmapManager.getSharedInstance().totalClean();
		Logger.I(TAG, "onDestroy() 2");
		super.onDestroy();
		Logger.I(TAG, "onDestroy() 3");
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
					if (mc != null) {
						setZoom(mNativeDevice, finalZoom);
					}
				}
			});

			mSurface.startAnimation(anim);
		}
	}
	
	public void drawImage(Canvas canvas, int x, int y, int bm) {
		//Logger.I(TAG, "drawImage(img_id="+String.valueOf(bm)+") start");
		Bitmap b = MapBitmapManager.getSharedInstance().getBitmap(bm);
		if (b != null) {
			//Logger.I(TAG, "drawImage() image exist");
			Paint paint = new Paint();
			canvas.drawBitmap(b, x, y, paint);
		}
		//Logger.I(TAG, "drawImage() finish");
	}

	public void drawText(Canvas canvas, int x, int y, int width, int height, String text, int color) 
	{
		//Logger.I(TAG, "drawText: " + text);

		String [] lines = text.split("\n");

		canvas.save();
		Rect rcClip = new Rect( x, y, x+width, y+height);
		canvas.clipRect(rcClip);

		Paint paint = new Paint();
		paint.setColor(color);
		paint.setAntiAlias(true);
		paint.setTextSize(28);

		for( int i = 0; i < lines.length; i++ )
		{
		    Rect rcLine = new Rect();
			paint.getTextBounds( lines[i], 0, lines[i].length()-1, rcLine );
			int nTextHeight = rcLine.height();
			y += nTextHeight + nTextHeight/3;
			canvas.drawText(lines[i], x, y, paint);

			if (i == 0) {
				y += nTextHeight;
			}
			paint.setTextSize(18);
		}

		canvas.restore();
	}
	
	public void redraw() {
		if (mIsInExitProcess) {
			return;
		}
		if (mc != null) {
			PerformOnUiThread.exec(new Runnable() {
				public void run() {
					if ((mc != null) && (mSurface != null)) {
						mSurface.invalidate();
					}
				}
			});
		}
	}
	
	public static int createImage(String path) {
		//Logger.I(TAG, "createImage(img_path="+path+") start");
		Bitmap b = BitmapFactory.decodeFile(path);
		if (b == null) {
			b = BitmapFactory.decodeStream(RhoFileApi.open(RhoFileApi.normalizePath("apps/" + path)));
			if (b != null) {
				//Logger.I(TAG, "createImage() OK!");
			}
			else {
				//Logger.I(TAG, "createImage() FAIL!");
			}
		}
		else {
			//Logger.I(TAG, "createImage() OK!");
		}
		if (b != null) {
			b.setDensity(ourDensity);
			return MapBitmapManager.getSharedInstance().addBitmap(b);
		}
		return 0;
	}
	
	public static int createImage(byte[] data) {
		Bitmap b = BitmapFactory.decodeByteArray(data, 0, data.length);
		if (b != null) {
			return MapBitmapManager.getSharedInstance().addBitmap(b);
		}
		return 0;
	}
	
	public static int createImageEx(byte[] data, int x, int y, int w, int h) {
		Bitmap b = BitmapFactory.decodeByteArray(data, 0, data.length);
		if (b == null) {
			return 0;
		}
		return MapBitmapManager.getSharedInstance().addBitmap(Bitmap.createBitmap(b, x, y, w, h));
	}
	
	public static void destroyImage(int bm) {
		//Logger.I(TAG, "destroyImage("+String.valueOf(bm)+")");
		MapBitmapManager.getSharedInstance().releaseBitmap(bm);
	}
	
	public static void addRefImage(int bm) {
		//Logger.I(TAG, "addRefImage("+String.valueOf(bm)+")");
		MapBitmapManager.getSharedInstance().addRef(bm);
	}

	public void destroyDevice() {
		mNativeDevice = 0;
		mIsInExitProcess = true;
	}
	
	@Override
	public void touchClick(Touch touch) {
		if (mIsInExitProcess) {
			return;
		}
		if (0 != click(mNativeDevice, (int)touch.x, (int)touch.y) ) {
			// block all UI because we in exit process from now !
			mIsInExitProcess = true;
		}
	}
	
	@Override
	public void touchDown(Touch first, Touch second) {
		if (mIsInExitProcess) {
			return;
		}
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
		if (mIsInExitProcess) {
			return;
		}
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
		if (mIsInExitProcess) {
			return;
		}
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
	
	public static int getBitmapWidth(int id) {
		Bitmap b = MapBitmapManager.getSharedInstance().getBitmap(id);
		if (b != null) {
			return b.getWidth();
		}
		return 0;
	}
	
	public static int getBitmapHeight(int id) {
		Bitmap b = MapBitmapManager.getSharedInstance().getBitmap(id);
		if (b != null) {
			return b.getHeight();
		}
		return 0;
	}
	


	
	private static Hashtable<Integer,Request> ourRequests = null;
	
	public static void resetRequestSet() {
		getRequests().clear();
	}
	
	public static Hashtable<Integer,Request> getRequests() {
		if (ourRequests == null) {
			ourRequests = new Hashtable<Integer,Request>();
		}
		return ourRequests;
	}
	
	public static int mapengine_request_make() {
		Request r = new Request();
		getRequests().put(new Integer(r.getId()), r);
		return r.getId();
	}
	
	public static int mapengine_request_data(int request_id, String url) {
		//Utils.platformLog("MapengineRequest", "mapengine_request_data("+String.valueOf(request_id)+", "+url+");");
		Request r = getRequests().get(new Integer(request_id));
		if (r != null) {
			r.requestData(url);
			int res = r.getDataSize();
			//Utils.platformLog("MapengineRequest", "mapengine_request_data() return "+String.valueOf(res));
			return res;
		}
		//Utils.platformLog("MapengineRequest", "mapengine_request_data() return 0");
		return 0;
	}
	
	public static byte[] mapengine_get_requested_data(int request_id) {
		Request r = getRequests().get(new Integer(request_id));
		if (r != null) {
			byte[] ar = r.getData();
			r.cancel();
			getRequests().remove(new Integer(request_id));
			return ar;
		}
		return null;
	}
	
	public static void mapengine_request_cancel(int request_id) {
		Request r = getRequests().get(new Integer(request_id));
		if (r != null) {
			r.cancel();
			getRequests().remove(new Integer(request_id));
		}
	}
	
	public static native void deletemapviewpointer(int p);
	
	public static void mapengine_delete_mapview_in_ui_thread(int p) {
		final int pp = p;
		PerformOnUiThread.exec(new Runnable() {
			private int mP = pp;
			public void run() {
				try {
					deletemapviewpointer(mP);
				}
				catch (Throwable e) {
					Utils.platformLog("MapengineDeleteInUIThread", "Exception during perform delete !");
					e.printStackTrace();
				}
			}
		});
	}
	
	
}
