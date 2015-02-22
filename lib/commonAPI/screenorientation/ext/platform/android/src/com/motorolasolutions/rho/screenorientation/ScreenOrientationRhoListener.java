package com.motorolasolutions.rho.screenorientation;

import java.util.Iterator;
import java.util.LinkedList;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;

public class ScreenOrientationRhoListener extends AbstractRhoListener {
    private static String TAG = ScreenOrientationRhoListener.class.getSimpleName();
    private static LinkedList<ScreenOrientationSingleton> mScreenOrientationInstances = null;

    public ScreenOrientationRhoListener() {
        if (mScreenOrientationInstances == null)
            mScreenOrientationInstances = new LinkedList<ScreenOrientationSingleton>();
    }

    public static void addScreenOrientationInstance(ScreenOrientationSingleton screenOrientation) {
        try {
            mScreenOrientationInstances.add(screenOrientation);
        } catch (NullPointerException e) {
            Logger.E(TAG, "List containing the instances of ScreenOrientation is null");
        }
    }



	@Override
    public void onPause(RhodesActivity activity) {
        Iterator<ScreenOrientationSingleton> iterator = mScreenOrientationInstances.iterator();
        while (iterator.hasNext()) {
            ScreenOrientationSingleton screenOrientationInstance = iterator.next();
            screenOrientationInstance.m_Paused =true;
        }
    }
	
		@Override
    public void onResume(RhodesActivity activity) {
        Iterator<ScreenOrientationSingleton> iterator = mScreenOrientationInstances.iterator();
        while (iterator.hasNext()) {
            ScreenOrientationSingleton screenOrientationInstance = iterator.next();
            screenOrientationInstance.m_Paused =false;
        }
    }


    @Override
    public void onStop(RhodesActivity activity) {
        Iterator<ScreenOrientationSingleton> iterator = mScreenOrientationInstances.iterator();
        while (iterator.hasNext()) {
            ScreenOrientationSingleton screenOrientationInstance = iterator.next();
            screenOrientationInstance.cleanUp();
        }
    }

    @Override
    public void onDestroy(RhodesActivity activity) {
        Iterator<ScreenOrientationSingleton> iterator = mScreenOrientationInstances.iterator();
        while (iterator.hasNext()) {
            ScreenOrientationSingleton screenOrientationInstance = iterator.next();
            screenOrientationInstance.cleanUp();
        }
    }

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        Logger.D(TAG, "onCreateApplication -- START");
        extManager.addRhoListener(this);
    }

}
