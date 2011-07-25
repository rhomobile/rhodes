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

import java.net.URISyntaxException;

import com.rhomobile.rhodes.Logger;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.webkit.URLUtil;

public class ExternalHttpHandler implements UriHandler {
	
	private static final String TAG = "ExternalHttpHandler";
	private Context ctx;
	
	public ExternalHttpHandler(Context c) {
		ctx = c;
	}

	public boolean handle(String url) throws URISyntaxException {
		Uri uri = Uri.parse(url);

		if (!URLUtil.isHttpUrl(url) && !URLUtil.isHttpsUrl(url))
			return false;
		
		String target = uri.getQueryParameter("rho_open_target");
		if (target == null || !target.equals("_blank"))
			return false;
		
		Logger.D(TAG, "This is external 'http' uri, handle it");
		
		//Remove 'rho_open_target' parameter from URL
		final String rhoParam = "rho_open_target=_blank";
		int pos = url.indexOf(rhoParam);
		if (pos + rhoParam.length() == url.length())
		    url = url.substring(0, pos - 1);
		else
		    url = url.substring(0, pos) + url.substring(pos + rhoParam.length() + 1 , url.length());
		
		ctx.startActivity(Intent.createChooser(Intent.parseUri(url, 0), "Open in..."));
		return true;
	}

}
