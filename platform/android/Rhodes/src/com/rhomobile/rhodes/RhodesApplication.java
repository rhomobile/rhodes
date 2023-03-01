/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes;

import java.io.File;
import java.io.IOException;
import java.util.Collection;
import java.util.Vector;

import android.app.KeyguardManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.app.Application;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.os.Environment;
import android.os.Handler;
import android.os.PowerManager;
import android.os.Process;
import android.util.Log;


import com.rhomobile.rhodes.ScreenReceiver.DeviceScreenEvent;
//import com.rhomobile.rhodes.camera.Camera;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Utils;
import com.rhomobile.rhodes.util.Utils.AssetsSource;
import com.rhomobile.rhodes.util.Utils.FileSource;

class ScreenReceiver extends BroadcastReceiver
{
	public enum DeviceScreenEvent
	{
		SCREEN_OFF,
		SCREEN_ON;
	}

	public ScreenReceiver() {
	}

	public native static void notifyDeviceScreenEvent(int event);

	@Override
	public void onReceive(Context context, Intent intent)
	{
		// do not handle anything if there are no activities yet
		if (BaseActivity.getActivitiesCount() < 1) {
			return;
		}

		// device is probably unlocked
		if (intent.getAction().equals(Intent.ACTION_SCREEN_ON))
		{
			// if keyguard is locked then unlock event should be called in ACTION_USER_PRESENT
			KeyguardManager keyguardManager = (KeyguardManager) context.getSystemService(Context.KEYGUARD_SERVICE);
			if (!keyguardManager.inKeyguardRestrictedInputMode())
			{
				notifyDeviceScreenEvent(DeviceScreenEvent.SCREEN_ON.ordinal());
			}
		}
		// device is unlocked
		else if(intent.getAction().equals(Intent.ACTION_USER_PRESENT))
		{
			notifyDeviceScreenEvent(DeviceScreenEvent.SCREEN_ON.ordinal());
		}
	}
}

public class RhodesApplication extends Application{
	private BroadcastReceiver mReceiver;
	
    private static final String TAG = RhodesApplication.class.getSimpleName();
    private static Handler mHandler;
    private static RhodesApplication curInstance;

    static AppEventObserver sRhodesAppActiveWatcher;
    static {
        NativeLibraries.load();
    }

    public static void handleAppStarted() {
        //sRhodesAppActiveWatcher.run();
    	stateChanged(AppState.AppStarted);
    }

    public static Context getContext() {
        return curInstance;
    }

    private boolean isAppHashChanged(String rootPath) {
        try {
            File hash = new File(rootPath, "hash");
            if (!hash.exists())
                return true;
            
            FileSource as = new AssetsSource(getResources().getAssets());
            FileSource fs = new FileSource();
            return !Utils.isContentsEquals(as, "hash", fs, hash.getPath());
        }
        catch (IOException e) {
            return true;
        }
    }

    private void registerStateHandlers() {
        sRhodesAppActiveWatcher = null;//AppState.AppStarted.addObserver("RhodesAppActiveObserver", true);
        
        RhodesApplication.runWhen(
                UiState.MainActivityPaused,
                new StateHandler(false) {
                    @Override
                    public void run() {
                        RhodesService.callUiDestroyedCallback();
                    }
                });
        RhodesApplication.runWhen(
                UiState.MainActivityPaused,
                new StateHandler(false) {
                    @Override
                    public void run() {
                        PowerManager powerMgr = (PowerManager)getSystemService(POWER_SERVICE);
                        if(!powerMgr.isScreenOn()) {
                            ScreenReceiver.notifyDeviceScreenEvent(DeviceScreenEvent.SCREEN_OFF.ordinal());
                        }
                    }
                });
        RhodesApplication.runWhen(
                AppState.AppActivated,
                new StateHandler(false) {
                    @Override
                    public void run() {
                        RhoExtManager.getImplementationInstance().onAppActivate(true);
                    }
                });
        RhodesApplication.runWhen(
                AppState.AppDeactivated,
                new StateHandler(false) {
                    @Override
                    public void run() {
                        RhoExtManager.getImplementationInstance().onAppActivate(false);
                    }
                });
     /*   RhodesApplication.runWhen(
                AppState.AppActivated,
                new StateHandler(true) {
                    @Override public void run() {
                        PerformOnUiThread.exec(new Runnable() {
                                @Override public void run() {
                                    Camera.init_from_UI_Thread();
                                }
                        });
                    }
                });
				*/
    }
    
    @Override
    public void onCreate(){
        super.onCreate();

        Log.i(TAG, "Initializing...");

        curInstance = this;

        registerStateHandlers();

        initClassLoader(getClassLoader());

        ApplicationInfo appInfo = getApplicationInfo();
        String rootPath;
        String sharedPath = null;

        String externalSharedPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Android/data/" + appInfo.packageName;
        String configPath = new File(externalSharedPath, "Config.xml").getAbsolutePath();

        File configXmlFile = null;
        File rhoconfigTxtFile = null;
        try
        {
            configXmlFile = new File(configPath);

            if (configXmlFile != null && configXmlFile.exists()) {
                rhoconfigTxtFile = new File(configXmlFile.getParent() + "/rhoconfig.txt");
            }
             
            if (rhoconfigTxtFile != null && rhoconfigTxtFile.exists()) {
                sharedPath = rhoconfigTxtFile.getParent();
            }
        
        } 
        catch (Throwable e)
        {
            Logger.E(TAG, e);
        }
        
        rootPath = RhoFileApi.initRootPath(appInfo.dataDir, appInfo.sourceDir, sharedPath);
        Log.d(TAG, "Root path: " + rootPath);

        try {
            boolean hashChanged = isAppHashChanged(rootPath);
            RhoFileApi.basicSetup(this, hashChanged);

            // Use paths for config and rholog
            setupRhodesApp();

            boolean emulateFileTree = !RhoConf.isExist("useAssetFS") || RhoConf.getBool("useAssetFS");
            RhoFileApi.finalSetup(this, hashChanged, emulateFileTree);
        } catch (IOException e) {
            Log.e(TAG, e.getMessage());
            stop();
            return;
        }

        Logger.T(TAG, "Root path: " + rootPath);

        //Signature.registerSignatureCaptureExtension();
        RhoExtManager.getImplementationInstance().createRhoListeners();

        IntentFilter filter = new IntentFilter(Intent.ACTION_SCREEN_ON);
        filter.addAction(Intent.ACTION_SCREEN_OFF);
        filter.addAction(Intent.ACTION_USER_PRESENT);

        mReceiver = new ScreenReceiver();
        registerReceiver(mReceiver, filter);

        Logger.I(TAG, "Initialized");
    }

    private native static void initClassLoader(ClassLoader c);
    private native static void setupRhodesApp();
    private native static void createRhodesApp();
    private native static void startRhodesApp();
    private native static void stopRhodesApp();
    private native static boolean canStartApp(String strCmdLine, String strSeparators);
    
    public static void create()
    {
        if (sAppState != AppState.Undefined) {
            Logger.E(TAG, "Cannot create application, it is already started!!!");
            return;
        }
        createRhodesApp();
    }

    public static void start()
    {
        if (sAppState != AppState.Undefined) {
            Logger.E(TAG, "Cannot start application it is already started!!!");
            return;
        }
        startRhodesApp();
    }

	public static boolean canStart(String strCmdLine)
	{
	    return canStartApp(strCmdLine, "&#");
	}
	
    public static void stop() {
        Logger.T(TAG, "Stopping application");
        mHandler = new Handler();
        mHandler.postDelayed(new Runnable() {
            public void run() {
                Logger.T(TAG, "do stopRhodesApp");
                stopRhodesApp();
                curInstance = null;
                try {
                    Logger.T(TAG, "do RhodesActivity.finish()");
                    RhodesActivity.safeGetInstance().finish();
                } catch (Throwable e) {
                    // Just postpone
                }
                Logger.T(TAG, "send quit signal");
                Process.sendSignal(Process.myPid(), Process.SIGNAL_QUIT);
                mHandler.postDelayed(new Runnable() {
                    public void run() {
                        Logger.T(TAG, "send kill signal");
                        Process.killProcess(Process.myPid());
                    }
                }, 500);
            }
        }, 500);
    }

    
    public static abstract class StateHandler implements Runnable
    {
        private Exception error;
        private boolean runOnce;
        
        public StateHandler(boolean once) { runOnce = once; }

        protected void setError(Exception err) { error = err; }
        public Exception getError() { return error; }
        public boolean isRunOnce() { return runOnce; }
        public abstract void run();
    }

    public static abstract class StateHandlerInUIThread extends StateHandler
    {
        public StateHandlerInUIThread(boolean once) { super(once); }

        public abstract void runInUI();

        final public void run()
        {
            PerformOnUiThread.exec(new Runnable() {
                @Override
                public void run() {
                    StateHandlerInUIThread.this.runInUI();
                } }
            );
        }
    }

    /**
     * 
     * @author lexis_tikh
     *
     */
    private static class EventObserver implements Runnable{
        private volatile boolean mReady = false;
        private boolean mCheckOnce;
        private String mTag;

        public EventObserver(String tag, boolean once) { mTag = tag; mCheckOnce = once; }
        public synchronized boolean isReady() { return mReady; }
        public String toString() { return mTag + " - " + (mReady?"ready":"not ready");}
        public synchronized void reset() { mReady = false; }
        public boolean isCheckOnce() { return mCheckOnce; }
        public void run() { // notify
            synchronized(this) {
                mReady = true;
            }
            Logger.I(mTag, "Ready");
        }
    }

    public static class AppEventObserver extends EventObserver {
        private AppState mState;

        public AppEventObserver(String tag, AppState state, boolean once) {
            super(tag, once);
            mState = state;
        }
        public void run() {
            super.run();
            
            if(mState == null) throw new IllegalStateException("AppEventObserver state is not initialized");
            
            RhodesApplication.stateChanged(mState);
        }
    }

    public static class UiEventObserver extends EventObserver {
        private UiState mState;

        public UiEventObserver(String tag, UiState state, boolean once) {
            super(tag, once);
            mState = state;
        }
        public void run() {
            super.run();
            
            if(mState == null) throw new IllegalStateException("UiEventObserver state is not initialized");
            
            RhodesApplication.stateChanged(mState);
        }
    }

    private static int runHandlers(Collection<StateHandler> handlers) {
        if (handlers == null) return 0;

        for(StateHandler handler: handlers) {
            handler.run();
            Exception error = handler.getError();
            if (error != null)
            {
                Logger.E(TAG, error);
                Thread.dumpStack();
            }
        }
        
        return handlers.size();
    }
    
    private static class StateImpl <Observer extends EventObserver> {
        private String TAG;
        private Vector<Observer> mObservers = new Vector<Observer>();
        private Vector<StateHandler> mHandlers = new Vector<StateHandler>();

        public StateImpl(String tag) { TAG = tag; }
        
        public void addObserver(Observer o) { mObservers.add(o); }
        public void addHandler(StateHandler h) { mHandlers.add(h); }

        public boolean isComplete() {
            boolean complete = true;
            for (Observer observer: mObservers) {
                complete &= observer.isReady();
                if (!complete) {
                    Logger.D(TAG, observer.toString());
                    break;
                }
            }
            return complete;
        }

        public Collection<StateHandler> commitHandlers() {
            Vector<StateHandler> handlers = new Vector<StateHandler>();
            Vector<StateHandler> doneHandlers = new Vector<StateHandler>();
            for (StateHandler handler: mHandlers) {
                handlers.add(handler);
                if (handler.isRunOnce()) {
                    doneHandlers.add(handler);
                }
            }
            Logger.T(TAG, "Handlers gone: " + doneHandlers.size());
            mHandlers.removeAll(doneHandlers);
            return handlers;
        }

        public void commitObservers() {
            Logger.T(TAG, "Commit observers");
            Vector<EventObserver> observers = new Vector<EventObserver>();
            for (EventObserver o: mObservers) {
                if (o.isCheckOnce()) {
                    Logger.T(TAG, o.mTag + " is gone");
                    observers.add(o);
                }
            }
            mObservers.removeAll(observers);
        }
    }
    
    
    public enum AppState
    {
        Undefined("Undefined") {
            @Override
            public boolean canHandle(AppState state) { return false; }
        },
        AppStarted("AppStarted") {
            @Override
            public boolean canHandle(AppState state) { return state == this; }
        },
        AppActivated("AppActivated") {
            @Override
            public boolean canHandle(AppState state) { return (state == this) || (state == AppStarted); }
        },
        AppDeactivated("AppDeactivated") {
            @Override
            public boolean canHandle(AppState state) { return (state == this) || (state == AppStarted); }
        };
        
        private static boolean appActivatedFlag = false;
        
        private String TAG;
        private StateImpl<AppEventObserver> mImpl;
        
        private AppState(String tag) {
            TAG = tag;
            mImpl = new StateImpl<AppEventObserver>(tag);
        }
        
        private synchronized Collection<StateHandler> commit()
        {
            Collection<StateHandler> handlers = null;// = new Vector<StateHandler>();

            Logger.T(TAG, "Starting commit. Current AppState: " + sAppState.TAG);
            if(mImpl.isComplete()) {
                if((this == AppActivated) && (sAppState == Undefined)) {
                    appActivatedFlag = true;
                    Logger.I(TAG, "Cannot commit now, application will be activated when started.");
                } else {
                    Logger.T(TAG, "Commiting AppState handlers.");
                    
                    handlers = mImpl.commitHandlers();
                    mImpl.commitObservers();
                    
                    sAppState = this;
                }
            }
            Logger.T(TAG, "After AppState commit: " + sAppState.TAG);
            return handlers;
        }
        
        public synchronized void addHandler(StateHandler handler) { mImpl.addHandler(handler); }
        public synchronized AppEventObserver addObserver(String tag, boolean once) {
            AppEventObserver observer = new AppEventObserver(tag, this, once);
            mImpl.addObserver(observer);
            return observer;
        }
        public abstract boolean canHandle(AppState state);

        static public void handleState(AppState state) {
            int cnt = runHandlers(state.commit());
            Logger.T(sAppState.TAG, "Handlers have completed: " + cnt);
            if((state == AppStarted) && appActivatedFlag) {
                cnt = runHandlers(AppActivated.commit());
                Logger.T(sAppState.TAG, "Handlers have completed: " + cnt);
            }
            return;
        }
    }
    
    public enum UiState
    {
        Undefined("Undefined") {
            @Override
            public boolean canHandle(UiState state) { return false; }
        },
        MainActivityCreated("MainActivityCreated") {
            @Override
            public boolean canHandle(UiState state) { return state == this; }
        },
        MainActivityStarted("MainActivityStarted") {
            @Override
            public boolean canHandle(UiState state) { return (state == this) || (state == MainActivityCreated); }
        },
        MainActivityResumed("MainActivitiResumed") {
            @Override
            public boolean canHandle(UiState state) { return (state == this) || (state == MainActivityStarted) || (state == MainActivityCreated); }
        },
        MainActivityPaused("MainActivityPaused") {
            @Override
            public boolean canHandle(UiState state) { return (state == this) || (state == MainActivityCreated); }
        };

        private StateImpl<UiEventObserver> mImpl;
        public String TAG;
        
        private UiState(String tag) {
            TAG = tag;
            mImpl = new StateImpl<UiEventObserver>(tag);
        }
        
        private synchronized Collection<StateHandler> commit()
        {
            Collection<StateHandler> handlers = null;

            Logger.T(TAG, "Starting commit. Current UiState: " + sUiState.TAG);

            if (mImpl.isComplete()) {
                if (!sUiState.canHandle(this)) {
                    handlers = mImpl.commitHandlers();
                    mImpl.commitObservers();
                    sUiState = this;
                }
            }
            Logger.T(TAG, "After UiState commit: " + sUiState.TAG);
            return handlers;
        }
        
        public synchronized void addHandler(StateHandler handler) { mImpl.addHandler(handler); }
        public synchronized UiEventObserver addObserver(String tag, boolean once) {
            UiEventObserver observer = new UiEventObserver(tag, this, once);
            mImpl.addObserver(observer);
            return observer;
        }
        public abstract boolean canHandle(UiState state);

        static public void handleState(UiState state) {
            int cnt = runHandlers(state.commit());
            Logger.T(sAppState.TAG, "Handlers have completed: " + cnt);
        }
    }

    private static AppState sAppState = AppState.Undefined;
    private static UiState sUiState = UiState.Undefined;
    
    public static boolean canHandleNow(AppState state) {
        boolean res = false;
        synchronized (AppState.class) {
            res = sAppState.canHandle(state);
        }
        Logger.T(sAppState.name(), (res?"Can":"Can not") + " handle App state: " + state.name());
        return res;
    }
    public static boolean canHandleNow(UiState state) {
        boolean res = false;
        synchronized (UiState.class) {
            res = sUiState.canHandle(state);
        }
        Logger.T(sUiState.name(), (res?"Can":"Can not") + " handle UI state: " + state.name());
        return res;
    }
    
    public static void runWhen(AppState state, StateHandler handler) {
        Logger.T(TAG, "Current AppState : " + sAppState.TAG);
        if (sAppState.canHandle(state)) {
            Logger.T(TAG, "Running AppState handler immediately: " + state.TAG);
            handler.run();
            if (handler.isRunOnce())
                return;
        }
        state.addHandler(handler);
        Logger.T(TAG, "AppState handler added: " + state.TAG);
        Logger.T(TAG, "Overall handlers: " + state.mImpl.mHandlers.size());
    }

    public static void runWhen(UiState state, StateHandler handler) {
        Logger.T(TAG, "Current UiState : " + sUiState.TAG);
        if (sUiState.canHandle(state)) {
            Logger.T(TAG, "Running UiState handler immediately: " + state.TAG);
            handler.run();
            if (handler.isRunOnce())
                {
                	Logger.I(TAG, "Inside runwhen before return");
                	return;
                }	
        }
        state.addHandler(handler);
        Logger.T(TAG, "UiState handler added: " + state.TAG);
    }

    public static void stateChanged(AppState state)
    {
        synchronized (AppState.class) {
            AppState.handleState(state);
        }
        Logger.I(TAG, "New AppState: " + sAppState.TAG);
    }
    public static void stateChanged(UiState state)
    {
        synchronized (UiState.class) {
            UiState.handleState(state);
        }
        Logger.I(TAG, "New UiState: " + sUiState.TAG);
    }

}
