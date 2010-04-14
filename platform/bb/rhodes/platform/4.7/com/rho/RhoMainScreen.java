package com.rho;

import net.rim.device.api.system.Display;
import net.rim.device.api.ui.TouchEvent;
import net.rim.device.api.ui.container.MainScreen;

public abstract class RhoMainScreen extends MainScreen {
	
	protected abstract void onTouchUnclick();
	
	protected boolean touchEvent(TouchEvent message) {
		int nEvent = message.getEvent();
		
		if ( nEvent == TouchEvent.UNCLICK )
		{
			onTouchUnclick();
			return true;
		}
		
		return super.touchEvent(message);
	}
	
	public boolean isTouchScreen()
	{
	    //DeviceInfo.getDevicename()
	    return net.rim.device.api.ui.Touchscreen.isSupported();
	}
	
	protected abstract void onChangeOrientation(int x, int y, int nOrientation);
	protected void sublayout(int arg0, int arg1) 
	{
		onChangeOrientation(arg0, arg1, Display.getOrientation()== Display.ORIENTATION_LANDSCAPE ? 1 : 2);
		super.sublayout(arg0, arg1);
	}
	
};
