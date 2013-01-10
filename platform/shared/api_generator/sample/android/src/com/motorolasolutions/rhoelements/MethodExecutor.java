package com.motorolasolutions.rhoelements;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class MethodExecutor {
    private static final String TAG = MethodExecutor.class.getSimpleName();
    
    public void run(Runnable task) {
        task.run();
    }

    public void runWithSeparateThread(Runnable task) {
        //try {
            Thread thr = new Thread(task);
            thr.start();
            //thr.join();
        //} catch (InterruptedException e) {
        //    Logger.E(TAG, e);
        //}
    }

    public void runWithUiThread(Runnable task) {
        PerformOnUiThread.exec(task);
    }
}
