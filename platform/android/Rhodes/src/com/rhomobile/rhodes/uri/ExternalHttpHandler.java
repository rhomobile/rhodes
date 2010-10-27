package com.rhomobile.rhodes.uri;

import com.rhomobile.rhodes.Logger;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

public class ExternalHttpHandler implements UriHandler {
	
	private static final String TAG = "ExternalHttpHandler";
	private Context ctx;
	
	public ExternalHttpHandler(Context c) {
		ctx = c;
	}

	public boolean handle(String url) {
		Uri uri = Uri.parse(url);
		String scheme = uri.getScheme();
		if (!scheme.equals("http") && !scheme.equals("https"))
			return false;
		
		String target = uri.getQueryParameter("rho_open_target");
		if (target == null || !target.equals("_blank"))
			return false;
		
		Logger.D(TAG, "This is external 'http' uri, handle it");
		
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setData(uri);
		
		ctx.startActivity(Intent.createChooser(intent, "Open in..."));
		return true;
	}

}
