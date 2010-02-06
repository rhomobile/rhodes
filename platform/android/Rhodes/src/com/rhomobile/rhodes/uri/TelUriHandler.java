package com.rhomobile.rhodes.uri;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;

public class TelUriHandler implements UriHandler {
	
	private Activity mainActivity;
	
	public TelUriHandler(Activity activity) {
		mainActivity = activity;
	}

	public void handle(Uri uri) {
		Intent intent = new Intent(Intent.ACTION_DIAL);
		intent.setData(uri);
		
		mainActivity.startActivity(Intent.createChooser(intent, "Dial..."));
	}

}
