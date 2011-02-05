package com.rhomobile.rhodes.mapview;

import android.view.MotionEvent;

public interface TouchHandler {

	public void setMapTouch(MapTouch mapTouch);
	public boolean handleTouch(MotionEvent event);
	
}
