package com.motorolasolutions.rho.screenorientation;

import com.rho.screenorientation.IScreenOrientationFactory;
import com.rhomobile.rhodes.api.RhoApiFactory;

public class ScreenOrientationFactory
        extends RhoApiFactory< ScreenOrientation, ScreenOrientationSingleton>
        implements IScreenOrientationFactory
{

    @Override
    protected ScreenOrientationSingleton createSingleton() {
        return new ScreenOrientationSingleton(this);
    }

    @Override
    protected ScreenOrientation createApiObject(String id) {
        return new ScreenOrientation(id);
    }
}
