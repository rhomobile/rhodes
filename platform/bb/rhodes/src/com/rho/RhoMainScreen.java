package com.rho;

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
	
};
