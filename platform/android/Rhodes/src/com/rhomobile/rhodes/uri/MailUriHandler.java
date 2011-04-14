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

import java.net.URISyntaxException;

import com.rhomobile.rhodes.Logger;

import android.content.Context;
import android.content.Intent;
import android.net.MailTo;

public class MailUriHandler implements UriHandler {
	
	private static final String TAG = "MailUriHandler";
	
	private Context ctx;
	
	public MailUriHandler(Context c) {
		ctx = c;
	}
	
	public boolean handle(String url) throws URISyntaxException {
		if (!MailTo.isMailTo(url))
			return false;
	
		Logger.D(TAG, "This is 'mailto' uri, handle it");
		
		ctx.startActivity(Intent.createChooser(Intent.parseUri(url, 0), "Open in..."));
		return true;

	}

}
