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
