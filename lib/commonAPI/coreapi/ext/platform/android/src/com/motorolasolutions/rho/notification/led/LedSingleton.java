package com.motorolasolutions.rho.notification.led;

import java.nio.ShortBuffer;
import java.util.Map;

import android.app.Activity;
import android.content.Context;

import com.rho.notification.led.ILedSingleton;
import com.rho.notification.led.ILed;
import com.motorolasolutions.rho.notification.led.LedImpl;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesApplication;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


public class LedSingleton implements ILedSingleton
{
	protected static String TAG = "LedSingleton";
    
    private ILed mRed   = new LedImpl("red", 0xFFFF0000/*, 1*/);
    private ILed mGreen = new LedImpl("green", 0xFF00FF00/*, 2*/);
    private ILed mBlue  = new LedImpl("blue", 0xFF0000FF/*, 3*/);
    
    public void enumerate(IMethodResult result)
    {
        List<Object> list = new ArrayList<Object>();
        list.add("red");
        list.add("green");
        list.add("blue");
        result.set(list);
    }
    
    public ILed getLed(String id)
    {
        if ("red".equals(id)) {
            return mRed;
        } else if ("green".equals(id)) {
            return mGreen;
        } else if ("blue".equals(id)) {
            return mBlue;
        }

        return null;
    }

	public void onPause()
	{
	}

	public void onStop()
	{
	}

	public void onDestroy()
	{
	}
}