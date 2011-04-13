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
