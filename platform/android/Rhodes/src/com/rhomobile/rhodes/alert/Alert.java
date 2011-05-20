/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
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
                            //PerformOnUiThread.exec(new Runnable() {
                            //    @Override
                            //    public void run() {
                                    PopupActivity.showStatusDialog(title, message, hide);
                            //    }
                            //});
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
