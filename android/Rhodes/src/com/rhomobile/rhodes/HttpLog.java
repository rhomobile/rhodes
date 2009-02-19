package com.rhomobile.rhodes;

import android.os.SystemClock;
import android.util.Config;
import android.util.Log;

public class HttpLog {
	private final static String LOGTAG = "http";

	private static final boolean DEBUG = false;
	static final boolean LOGV = DEBUG ? Config.LOGD : Config.LOGV;

	static void v(String logMe) {
		Log.v(LOGTAG, SystemClock.uptimeMillis() + " "
				+ Thread.currentThread().getName() + " " + logMe);
	}

	static void e(String logMe) {
		Log.e(LOGTAG, logMe);
	}
}

