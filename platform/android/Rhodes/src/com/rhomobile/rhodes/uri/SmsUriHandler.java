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
package com.rhomobile.rhodes.uri;

import com.rhomobile.rhodes.Logger;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

public class SmsUriHandler implements UriHandler {
	
	private static final String TAG = "SmsUriHandler";
	private Context ctx;
	
	public SmsUriHandler(Context c) {
		ctx = c;
	}

	public boolean handle(String url) {
		Uri uri = Uri.parse(url);
		if (!uri.getScheme().equals("sms"))
			return false;
		
		Logger.D(TAG, "This is 'sms' uri, handle it");
		
		Intent intent = new Intent(Intent.ACTION_SENDTO);
		intent.setData(uri);
		
		ctx.startActivity(Intent.createChooser(intent, "Send SMS..."));
		return true;
	}

}
