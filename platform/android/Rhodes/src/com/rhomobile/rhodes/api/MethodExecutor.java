package com.rhomobile.rhodes.api;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class MethodExecutor {
    private static final String TAG = MethodExecutor.class.getSimpleName();
    
    public static void run(Runnable task) {
        Logger.T(TAG, "Run in current thread");
        task.run();
    }

    public static void runWithSeparateThread(Runnable task) {
        Logger.T(TAG, "Run in new thread");
        //try {
            Thread thr = new Thread(task);
            thr.start();
            //thr.join();
        //} catch (InterruptedException e) {
        //    Logger.E(TAG, e);
        //}
    }

    public static void runWithUiThread(Runnable task) {
        Logger.T(TAG, "Run in UI thread");
        PerformOnUiThread.exec(task);
    }
}
