package com.rho;

import rhomobile.RhodesApplication;
import net.rim.device.api.system.Display;
import net.rim.device.api.ui.TouchEvent;
import net.rim.device.api.ui.container.MainScreen;

public abstract class RhoMainScreen extends MainScreen 
{
	public RhoMainScreen()
	{
		super();
	}

	public RhoMainScreen(long style)
	{
		super(style);
	}
	
	protected boolean onTouchUnclick(int x, int y){ return false; }
	protected boolean onTouchClick(int x, int y){ return false; }
	protected boolean onTouchDown(int x, int y){ return false; }
	protected boolean onTouchUp(int x, int y){ return false; }
	protected boolean onTouchMove(int x, int y){ return false; }
	
	protected boolean touchEvent(TouchEvent message) {
		if ( RhodesApplication.getInstance().isInputDisabled() )
			return true;
		
		int nEvent = message.getEvent();
		switch( nEvent )
		{
		case TouchEvent.CLICK:
			if ( onTouchClick(message.getX(1), message.getY(1)) )
				return true;
			break;
		case TouchEvent.UNCLICK:
			if ( onTouchUnclick(message.getX(1), message.getY(1)) )
				return true;
			break;
			
		case TouchEvent.DOWN:
			if ( onTouchDown(message.getX(1), message.getY(1)) )
				return true;
			break;
		case TouchEvent.UP:
			if ( onTouchUp(message.getX(1), message.getY(1)) )
				return true;
			break;
		case TouchEvent.MOVE:
			if ( onTouchMove(message.getX(1), message.getY(1)) )
				return true;
			break;
		}
		
		return super.touchEvent(message);
	}
	
	public boolean isTouchScreen()
	{
	    //DeviceInfo.getDevicename()
	    return net.rim.device.api.ui.Touchscreen.isSupported();
	}
	
	protected void onChangeOrientation(int x, int y, int nOrientation){}
	protected void sublayout(int arg0, int arg1) 
	{
		onChangeOrientation(arg0, arg1, Display.getOrientation()== Display.ORIENTATION_LANDSCAPE ? 1 : 2);
		super.sublayout(arg0, arg1);
	}
	
};
