package com.rho;

import net.rim.device.api.ui.container.MainScreen;

public abstract class RhoMainScreen extends MainScreen {
	
	protected boolean onTouchUnclick(){ return false; }
	
	public boolean isTouchScreen()
	{
		return false;
	}	
};
