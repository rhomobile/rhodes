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

package com.rhomobile.rhodes.alert;

import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;

import android.os.Bundle;
import android.os.Vibrator;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesApplication;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class Alert {

	private static final String TAG = "Alert";

	private static MediaPlayer currentMP = null;

	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}

    public static void showPopup(final Object params) {
        RhodesApplication.runWhen(
                RhodesApplication.AppState.AppActivated,
                new RhodesApplication.StateHandler(true) {
                    @Override
                    public void run() {
                        try {
                            Logger.T(TAG, "showPopup");
                            //PerformOnUiThread.exec(new Runnable() {
                            //    @Override
                            //    public void run() {
                                    PopupActivity.showDialog(params);
                            //    }
                            //});
                        }
                        catch (Exception e) {
                            reportFail("showPopup", e);
                            setError(e);
                        }
                    }
                });
    }

    public static void hidePopup() {
        RhodesApplication.runWhen(
                RhodesApplication.AppState.AppActivated,
                new RhodesApplication.StateHandler(true) {
                    @Override
                    public void run() {
                        try {
                            Logger.T(TAG, "hidePopup");
                            //PerformOnUiThread.exec(new Runnable() {
                            //    @Override
                            //    public void run() {
                                    PopupActivity.hidePopup();
                            //    }
                            //});
                        }
                        catch (Exception e) {
                            reportFail("hidePopup", e);
                            setError(e);
                        }
                    }
                });
    }

    public static void showStatusPopup(final String title, final String message, final String hide) {
        RhodesApplication.runWhen(
                RhodesApplication.AppState.AppActivated,
                new RhodesApplication.StateHandler(true) {
                    @Override
                    public void run() {
                        try {
                            Logger.I(TAG, "showStatusPopup");
                            PerformOnUiThread.exec(new Runnable() {
                                @Override
                                public void run() {
                                    PopupActivity.showStatusDialog(title, message, hide);
                                }
                            });
                        }
                        catch (Exception e) {
                            reportFail("showStatusPopup", e);
                            setError(e);
                        }
                    }
                });
    }

	public static void vibrate(int duration) {
		try {
			if (!Capabilities.VIBRATE_ENABLED)
				throw new IllegalAccessException("VIBRATE disabled");
			Logger.T(TAG, "vibrate: " + duration);
			Context ctx = RhodesService.getContext();
			Vibrator vibrator = (Vibrator)ctx.getSystemService(Context.VIBRATOR_SERVICE);
			vibrator.vibrate(duration);
		}
		catch (Exception e) {
			reportFail("vibrate", e);
		}
	}

	public static void playFile(String fileName, String mediaType) {
		try {
			Logger.T(TAG, "playFile: " + fileName + " (" + mediaType + ")");

			if (currentMP != null)
				currentMP.release();

			MediaPlayer mp = new MediaPlayer();
			currentMP = mp;
			mp.setOnErrorListener(new MediaPlayer.OnErrorListener() {
				public boolean onError(MediaPlayer mp, int what, int extra) {
					Logger.E(TAG, "Error when playing file : " + what + ", " + extra);
					return false;
				}
			});
			mp.setDataSource(RhoFileApi.openFd("apps/" + fileName));

			mp.prepare();
			mp.start();
		} catch (Exception e) {
			reportFail("playFile", e);
		}
	}
	public static void showNotification(Context ctx, int id,
								 String title, String text,
								 Bundle extras)
	{
		Intent notificationIntent = new Intent(ctx, RhodesService.class);
		if(extras != null)
			notificationIntent.putExtras(extras);
		StatusNotification.simpleNotification(TAG, id, ctx, notificationIntent, title, text);
	}

}
