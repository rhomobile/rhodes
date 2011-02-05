package com.rhomobile.rhodes.mapview;

import android.util.Log;
import android.view.MotionEvent;

import com.rhomobile.rhodes.mapview.MapTouch.Touch;

public class OneTouchHandler implements TouchHandler {
	
	private static final String TAG = OneTouchHandler.class.getSimpleName();
	
	private static final boolean DEBUG = false;
	
	private MapTouch mMapTouch;
	
	public void setMapTouch(MapTouch mapTouch) {
		mMapTouch = mapTouch;
	}
	
	private void dumpEvent(MotionEvent event) {
		String names[] = { "DOWN", "UP", "MOVE", "CANCEL", "OUTSIDE",
				"5?", "6?", "7?", "8?", "9?" };
		StringBuilder sb = new StringBuilder();
		int action = event.getAction();
		sb.append("event ACTION_").append(names[action]);
		sb.append("[");
		sb.append((int)event.getX());
		sb.append((int)event.getY());
		sb.append("]");
		Log.d(TAG, sb.toString());
	}

	@Override
	public boolean handleTouch(MotionEvent event) {
		if (DEBUG)
			dumpEvent(event);
		
		int action = event.getAction();
		switch (action) {
		case MotionEvent.ACTION_DOWN:
			mMapTouch.touchDown(new Touch(event.getX(), event.getY()), null);
			break;
		case MotionEvent.ACTION_UP:
			mMapTouch.touchUp(new Touch(event.getX(), event.getY()), null);
			break;
		case MotionEvent.ACTION_MOVE:
			mMapTouch.touchMove(new Touch(event.getX(), event.getY()), null);
			break;
		}
		
		return true;
	}

}
