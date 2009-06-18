package com.rhomobile.rhodes.datetime;

import android.app.Activity;
import android.util.Log;
import android.view.SurfaceHolder;

public class DateTimePickerScreen extends Activity implements SurfaceHolder.Callback {

	public void surfaceCreated(SurfaceHolder arg0) {
		Log.e(getClass().getSimpleName(), "surfaceCreated");
	}
	
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		Log.e(getClass().getSimpleName(), "surfaceChanged");
	}

	public void surfaceDestroyed(SurfaceHolder arg0) {
		Log.e(getClass().getSimpleName(), "surfaceDestroyed");
	}

}
