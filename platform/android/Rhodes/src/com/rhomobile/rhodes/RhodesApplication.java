package com.rhomobile.rhodes;

import java.util.Vector;

import android.app.Application;
import android.os.Handler;
import android.os.Process;

public class RhodesApplication extends Application{
	
    private static final String TAG = RhodesApplication.class.getSimpleName();
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

    
    public static abstract class StateHandler
    {
        private Exception error;
        protected void setError(Exception err) { error = err; }
        public Exception getError() { return error; }
        public abstract boolean run();
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
        
        private Vector<StateHandler> mHandlers = new Vector<StateHandler>();
        private String TAG;
        
        private AppState(String tag) { TAG = tag; }
        
        private synchronized void handle()
        {
            Logger.T(TAG, "Running AppState handlers: " + TAG);
            Vector<StateHandler> doneHandlers = new Vector<StateHandler>();
            for (StateHandler handler: mHandlers) {
                if (handler.run()) {
                    doneHandlers.add(handler);
                }
                Exception error = handler.getError();
                if (error != null)
                {
                    Logger.E(TAG, error);
                    Thread.dumpStack();
                }
            }
            mHandlers.removeAll(doneHandlers);
        }
        
        public synchronized void addHandler(StateHandler handler) { mHandlers.add(handler); }
        public abstract boolean canHandle(AppState state);
        
        static public AppState handleState(AppState state, AppState prev) {
            if((state == AppActivated) && (prev != Undefined)) {
                appActivatedFlag = true;
                return prev;
            }
            state.handle();
            if((state == AppStarted) && appActivatedFlag) {
                AppActivated.handle();
            }
            return AppActivated;
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
        MainActivityPaused("MainActivityPaused") {
            @Override
            public boolean canHandle(UiState state) { return (state == this) || (state == MainActivityCreated); }
        };

        private Vector<StateHandler> mHandlers = new Vector<StateHandler>();
        public String TAG;
        
        private UiState(String tag) { TAG = tag; }
        
        private synchronized void handle()
        {
            Logger.T(TAG, "Running AppState handlers: " + TAG);
            Vector<StateHandler> doneHandlers = new Vector<StateHandler>();
            for (StateHandler handler: mHandlers) {
                if (handler.run()) {
                    doneHandlers.add(handler);
                }
                Exception error = handler.getError();
                if (error != null)
                {
                    Logger.E(TAG, error);
                    Thread.dumpStack();
                }
            }
            mHandlers.removeAll(doneHandlers);
        }
        
        public synchronized void addHandler(StateHandler handler) { mHandlers.add(handler); }
        public abstract boolean canHandle(UiState state);
        
        static public UiState handleState(UiState state, UiState prev) {
            if(!prev.canHandle(state))
            {
                state.handle();
                return state;
            }
            return prev;
        }
    }

    private static AppState sAppState = AppState.Undefined;
    private static UiState sUiState = UiState.Undefined;
    
    public static void runWhen(AppState state, StateHandler handler) {
        Logger.T(TAG, "Current AppState : " + sAppState.TAG);
        if (sAppState.canHandle(state)) {
            Logger.T(TAG, "Running AppState handler immediately: " + state.TAG);
            if (handler.run())
                return;
        }
        state.addHandler(handler);
        Logger.T(TAG, "AppState handler added: " + state.TAG);
    }

    public static void runWhen(UiState state, StateHandler handler) {
        Logger.T(TAG, "Current UiState : " + sUiState.TAG);
        if (sUiState.canHandle(state)) {
            Logger.T(TAG, "Running UiState handler immediately: " + state.TAG);
            if (handler.run())
                return;
        }
        state.addHandler(handler);
        Logger.T(TAG, "UiState handler added: " + state.TAG);
    }

    public static void stateChanged(AppState state)
    {
        sAppState = AppState.handleState(state, sAppState);
        Logger.I(TAG, "New AppState: " + sAppState.TAG);
    }
    public static void stateChanged(UiState state)
    {
        sUiState = UiState.handleState(state, sUiState);
        Logger.I(TAG, "New UiState: " + sUiState.TAG);
    }

}
