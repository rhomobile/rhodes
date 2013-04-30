package com.motorolasolutions.rho.screenorientation;

import com.rho.screenorientation.IScreenOrientationFactory;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.RhoApiFactory;

public class ScreenOrientationFactory
        extends RhoApiFactory< ScreenOrientation, ScreenOrientationSingleton>
        implements IScreenOrientationFactory
{
    private static String TAG = ScreenOrientationFactory.class.getSimpleName(); 
    
    @Override
    protected ScreenOrientationSingleton createSingleton() {
	Logger.D(TAG, "createSingleton()");
        return new ScreenOrientationSingleton(this);
    }

    @Override
    protected ScreenOrientation createApiObject(String id) {
	Logger.D(TAG, "createApiObject() -- id: " + id);
        return new ScreenOrientation(id);
    }
}
