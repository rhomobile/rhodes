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
	
	private static final float CLICK_TOLERANCE = 8;
	private float mFirstTouchX;
	private float mFirstTouchY;
	private boolean mIsClickPossible;
	
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
			mIsClickPossible = true;
			mFirstTouchX = event.getX();
			mFirstTouchY = event.getY();
			mMapTouch.touchDown(new Touch(event.getX(), event.getY()), null);
			break;
		case MotionEvent.ACTION_UP:
			mMapTouch.touchUp(new Touch(event.getX(), event.getY()), null);
			// check for click
			if (checkDistance(event.getX(), event.getY(), mFirstTouchX, mFirstTouchY, CLICK_TOLERANCE) && mIsClickPossible) {
				mMapTouch.touchClick(new Touch(event.getX(), event.getY()));
			}
			break;
		case MotionEvent.ACTION_MOVE:
			if (!checkDistance(event.getX(), event.getY(), mFirstTouchX, mFirstTouchY, CLICK_TOLERANCE)) {
				mIsClickPossible = false;
			}
			mMapTouch.touchMove(new Touch(event.getX(), event.getY()), null);
			break;
		}
		
		return true;
	}
	
	private boolean checkDistance(float x1, float y1, float x2, float y2, float delta) {
		float dx = x1 - x2;
		if (dx < 0) dx = -dx;
		float dy = y1 - y2;
		if (dy < 0) dy = -dy;
		return ((dx <= delta) && (dy <= delta));
	}

}
