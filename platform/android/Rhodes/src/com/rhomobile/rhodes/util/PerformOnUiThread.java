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

package com.rhomobile.rhodes.util;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesApplication;

public class PerformOnUiThread implements Runnable {

	private static final String TAG = PerformOnUiThread.class.getSimpleName();

	private Runnable runnable;

	public PerformOnUiThread(Runnable r) {
		runnable = r;
	}

	public void run() {
		try {
			runnable.run();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
		finally {
			synchronized (runnable) {
				runnable.notify();
			}
		}
	}

	public static void exec(final Runnable r, final long delay) {
        RhodesApplication.runWhen(
                RhodesApplication.UiState.MainActivityCreated,
                new RhodesApplication.StateHandler(true) {
                    @Override
                    public void run() {
                        try {
                            RhodesActivity.safeGetInstance().post(r, delay);
                        }
                        catch (Exception e) {
                            Logger.E(TAG, e);
                            setError(e);
                        }
                    }
                });
    }

    @Deprecated
	public static void exec(final Runnable r, final boolean wait) {
        if (wait)
            sync_exec(r);
        else
            exec(r);
    }

    public static void exec(final Runnable r) {
        RhodesApplication.runWhen(
                RhodesApplication.UiState.MainActivityCreated,
                new RhodesApplication.StateHandler(true) {
                    @Override
                    public void run() {
                        try {
                            RhodesActivity.safeGetInstance().post(r);
                        } catch (Exception e) {
                            Logger.E(TAG, e);
                            setError(e);
                        }
                    }
                });
    }


    // Special exec edition for RhoBluetoothManager
    //TODO: Use future pattern to return result and wait
    @Deprecated
    public static void sync_exec(final Runnable r)
    {
        try {
            RhodesActivity ra = RhodesActivity.safeGetInstance();

            long thrId = Thread.currentThread().getId();
            if (ra.getUiThreadId() == thrId) {
                // We are already in UI thread
                r.run();
            }
            else {
                // Post request to UI thread and wait when it would be done
                synchronized (r) {
                    ra.post(new PerformOnUiThread(r));
                    r.wait();
                }
            }
        } catch (Exception e) {
            Logger.E(TAG, e);
        }
    }
};
