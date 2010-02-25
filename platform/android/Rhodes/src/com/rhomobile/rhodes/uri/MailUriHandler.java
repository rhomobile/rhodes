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

import android.content.Context;
import android.content.Intent;
import android.net.MailTo;
import android.net.Uri;

public class MailUriHandler implements UriHandler {
	
	private Context ctx;
	
	public MailUriHandler(Context c) {
		ctx = c;
	}
	
	public String scheme() {
		return "mailto";
	}

	public void handle(Uri uri) {
		MailTo muri = MailTo.parse(uri.toString());
		
		Intent intent = new Intent(Intent.ACTION_SEND);
		intent.setType("message/rfc882");
		
		String s = muri.getTo();
		if (s != null) intent.putExtra(Intent.EXTRA_EMAIL, new String[]{s});
		
		s = muri.getSubject();
		if (s != null) intent.putExtra(Intent.EXTRA_SUBJECT, s);
		
		s = muri.getBody();
		if (s != null) intent.putExtra(Intent.EXTRA_TEXT, s);
		
		ctx.startActivity(Intent.createChooser(intent, "Send e-mail..."));
	}

}
