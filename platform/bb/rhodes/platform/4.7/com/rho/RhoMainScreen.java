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
