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
