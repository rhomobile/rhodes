package com.motorolasolutions.rho.screenorientation;

import java.util.LinkedList;
import java.util.List;

import com.rho.screenorientation.IScreenOrientationSingleton;
import com.rho.screenorientation.ScreenOrientationSingletonBase;
import com.rhomobile.rhodes.api.IMethodResult;

class ScreenOrientationSingleton extends ScreenOrientationSingletonBase implements IScreenOrientationSingleton
{
    
    public ScreenOrientationSingleton(ScreenOrientationFactory factory)
    {
        super(factory);
    }

    @Override
    protected String getInitialDefaultID() {
	// TODO Auto-generated method stub
	return null;
    }
}