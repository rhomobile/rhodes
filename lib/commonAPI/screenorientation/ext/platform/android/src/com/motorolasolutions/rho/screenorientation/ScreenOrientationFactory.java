package com.motorolasolutions.rho.screenorientation;

import com.rho.screenorientation.IScreenOrientation;
import com.rho.screenorientation.IScreenOrientationFactory;
import com.rho.screenorientation.IScreenOrientationSingleton;

public class ScreenOrientationFactory implements IScreenOrientationFactory
{
    private IScreenOrientationSingleton mSingleton;

    @Override
    public IScreenOrientationSingleton getApiSingleton() {
        if (mSingleton == null) {
            mSingleton = new ScreenOrientationSingleton();
        }
        return mSingleton;
    }

    @Override
    public IScreenOrientation getApiObject(String id) {
        return null;
    } 
    
}
