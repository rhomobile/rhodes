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
package com.rhomobile.rhodes;

import android.os.SystemClock;
import android.util.Config;

public class HttpLog {
	private final static String LOGTAG = "http";

	//private static final boolean DEBUG = false;
	//public static final boolean LOGV = DEBUG ? Config.LOGD : Config.LOGV;
	public static final boolean LOGV = Config.LOGV;

	static void v(String logMe) {
		Logger.T(LOGTAG, SystemClock.uptimeMillis() + " "
				+ Thread.currentThread().getName() + " " + logMe);
	}

	static void e(String logMe) {
		Logger.E(LOGTAG, logMe);
	}
}

