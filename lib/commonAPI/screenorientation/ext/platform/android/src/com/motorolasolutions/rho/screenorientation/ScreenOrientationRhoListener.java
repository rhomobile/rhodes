package com.motorolasolutions.rho.screenorientation;

import java.util.Iterator;
import java.util.LinkedList;

import android.app.Dialog;
import android.content.Intent;
import android.content.res.Configuration;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;

public class ScreenOrientationRhoListener extends AbstractRhoListener
{
    private static String TAG = ScreenOrientationRhoListener.class.getSimpleName();
    private static LinkedList<ScreenOrientationSingleton> mScreenOrientationInstances = null;
    
    public ScreenOrientationRhoListener()
    {
	if (mScreenOrientationInstances == null)
	    mScreenOrientationInstances = new LinkedList<ScreenOrientationSingleton>();
    }
    
    public static void addScreenOrientationInstance(ScreenOrientationSingleton screenOrientation)
    {
	try
	{
	    mScreenOrientationInstances.add(screenOrientation);
	}
	catch (NullPointerException e)
	{
	    Logger.E(TAG, "List containing the instances of ScreenOrientation is null");
	}
    }
    
    @Override
    public void onCreate(RhodesActivity activity, Intent intent)
    {
	Logger.D(TAG, "onCreate -- START");
    }

    @Override
    public void onStart(RhodesActivity activity)
    {
	Logger.D(TAG, "onStart -- START");
    }

    @Override
    public void onResume(RhodesActivity activity) {
	// TODO Auto-generated method stub

    }

    @Override
    public void onPause(RhodesActivity activity) {
	// TODO Auto-generated method stub

    }

    @Override
    public void onStop(RhodesActivity activity)
    {
	Iterator<ScreenOrientationSingleton> iterator = mScreenOrientationInstances.iterator();
	while (iterator.hasNext())
	{
	    ScreenOrientationSingleton screenOrientationInstance = iterator.next();
	    screenOrientationInstance.cleanUp();
	}
    }

    @Override
    public void onDestroy(RhodesActivity activity)
    {
	Iterator<ScreenOrientationSingleton> iterator = mScreenOrientationInstances.iterator();
	while (iterator.hasNext())
	{
	    ScreenOrientationSingleton screenOrientationInstance = iterator.next();
	    screenOrientationInstance.cleanUp();
	}
    }

    @Override
    public void onNewIntent(RhodesActivity activity, Intent intent) {
	// TODO Auto-generated method stub

    }

    @Override
    public void onActivityResult(RhodesActivity activity, int requestCode,
	    int resultCode, Intent intent) {
	// TODO Auto-generated method stub

    }

    @Override
    public Dialog onCreateDialog(RhodesActivity activity, int id) {
	// TODO Auto-generated method stub
	return null;
    }

    @Override
    public void onConfigurationChanged(RhodesActivity activity,
	    Configuration newConfig) {
	// TODO Auto-generated method stub

    }

    @Override
    public void onCreateApplication(IRhoExtManager extManager)
    {
	Logger.D(TAG, "onCreateApplication -- START");
	extManager.addRhoListener(this);
    }

}
