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
