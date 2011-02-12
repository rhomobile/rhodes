package com.rhomobile.rhodes.mapview;

import android.util.Log;
import android.view.MotionEvent;

import com.rhomobile.rhodes.mapview.MapTouch.Touch;

public class MultiTouchHandler implements TouchHandler {
	
	private static final String TAG = MultiTouchHandler.class.getSimpleName();
	
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
				"POINTER_DOWN", "POINTER_UP", "7?", "8?", "9?" };
		StringBuilder sb = new StringBuilder();
		int action = event.getAction();
		int actionCode = action & MotionEvent.ACTION_MASK;
		sb.append("event ACTION_").append(names[actionCode]);
		if (actionCode == MotionEvent.ACTION_POINTER_DOWN
				|| actionCode == MotionEvent.ACTION_POINTER_UP) {
			sb.append("(pid ").append(
					action >> MotionEvent.ACTION_POINTER_ID_SHIFT);
			sb.append(")");
		}
		sb.append("[");
		for (int i = 0; i < event.getPointerCount(); i++) {
			sb.append("#").append(i);
			sb.append("(pid ").append(event.getPointerId(i));
			sb.append(")=").append((int) event.getX(i));
			sb.append(",").append((int) event.getY(i));
			if (i + 1 < event.getPointerCount())
				sb.append(";");
		}
		sb.append("]");
		Log.d(TAG, sb.toString());
	}
	
	@Override
	public boolean handleTouch(MotionEvent event) {
		if (DEBUG)
			dumpEvent(event);
		
		int action = event.getAction();
		int actionCode = action & MotionEvent.ACTION_MASK;
		
		Touch[] data = new Touch[Math.max(2, event.getPointerCount())];
		int i, lim;
		for (i = 0, lim = Math.min(data.length, event.getPointerCount()); i < lim; ++i)
			data[i] = new Touch(event.getX(i), event.getY(i));
		for (; i < data.length; ++i)
			data[i] = null;
		
		boolean isSingleTouch = (event.getPointerCount() < 2);
		
		switch (actionCode) {
		case MotionEvent.ACTION_DOWN:
		case MotionEvent.ACTION_POINTER_DOWN:
			mIsClickPossible = isSingleTouch;
			mFirstTouchX = data[0].x;
			mFirstTouchY = data[0].y;
			mMapTouch.touchDown(data[0], data[1]);
			break;
		case MotionEvent.ACTION_UP:
		case MotionEvent.ACTION_POINTER_UP:
			mMapTouch.touchUp(data[0], data[1]);
			if (isSingleTouch) {
				if (checkDistance(data[0].x, data[0].y, mFirstTouchX, mFirstTouchY, CLICK_TOLERANCE)) {
					mMapTouch.touchClick(data[0]);
				}
			}
			break;
		case MotionEvent.ACTION_MOVE:
			if (isSingleTouch) {
				if (!checkDistance(data[0].x, data[0].y, mFirstTouchX, mFirstTouchY, CLICK_TOLERANCE)) {
					mIsClickPossible = false;
				}
			}
			else {
				mIsClickPossible = false;
			}
			mMapTouch.touchMove(data[0], data[1]);
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
