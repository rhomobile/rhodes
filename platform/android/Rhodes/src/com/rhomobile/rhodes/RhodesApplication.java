package com.rhomobile.rhodes;

import android.app.Application;
import android.os.Handler;
import android.os.Process;

public class RhodesApplication extends Application{
	
//    private static final String TAG = NativeApplication.class.getSimpleName();
    private static Handler mHandler;
	static {
		NativeLibraries.load();
	}

    private native static void createRhodesApp();
    private native static void startRhodesApp();
    private native static void stopRhodesApp();
    private native static boolean canStartApp(String strCmdLine, String strSeparators);
	
    public static void create()
    {
        createRhodesApp();
    }

    public static void start()
	{
	    startRhodesApp();
	}

	public static boolean canStart(String strCmdLine)
	{
	    return canStartApp(strCmdLine, "&#");
	}
	
    public static void stop() {
        
        mHandler = new Handler();
        mHandler.postDelayed(new Runnable() {
            public void run() {
                stopRhodesApp();
                Process.sendSignal(Process.myPid(), Process.SIGNAL_QUIT);
                mHandler.postDelayed(new Runnable() {
                    public void run() {
                        Process.killProcess(Process.myPid());
                    }
                }, 500);
            }
        }, 500);
    }


}
