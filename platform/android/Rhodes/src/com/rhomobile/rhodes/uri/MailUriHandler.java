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

import java.util.Map;

import com.rhomobile.rhodes.Logger;

import android.content.Context;
import android.content.Intent;
import android.net.MailTo;
import android.net.Uri;

public class MailUriHandler implements UriHandler {
	
	private static final String TAG = "MailUriHandler";
	
	private Context ctx;
	
	public MailUriHandler(Context c) {
		ctx = c;
	}
	
	public boolean handle(String url) {
		if (!MailTo.isMailTo(url))
			return false;
	
		Logger.D(TAG, "This is 'mailto' uri, handle it");
		
		MailTo muri = MailTo.parse(url);
		
		Intent intent = new Intent(Intent.ACTION_SEND);
		intent.setType("message/rfc882");
		
		String s = muri.getTo();
		if (s != null) intent.putExtra(Intent.EXTRA_EMAIL, new String[]{s});
		
		s = muri.getCc();
		if (s != null) intent.putExtra(Intent.EXTRA_CC, new String[]{s});
		
		s = muri.getSubject();
		if (s != null) intent.putExtra(Intent.EXTRA_SUBJECT, s);
		
		s = muri.getBody();
		if (s != null) intent.putExtra(Intent.EXTRA_TEXT, s);
		
		Map<String,String> headers = muri.getHeaders();
		s = headers.get("bcc");
		if (s != null) intent.putExtra(Intent.EXTRA_BCC, new String[]{s});
		
		ctx.startActivity(Intent.createChooser(intent, "Send e-mail..."));
		return true;
	}

}
