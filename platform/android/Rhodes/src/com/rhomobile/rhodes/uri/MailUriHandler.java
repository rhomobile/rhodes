package com.rhomobile.rhodes.uri;

import android.app.Activity;
import android.content.Intent;
import android.net.MailTo;
import android.net.Uri;

public class MailUriHandler implements UriHandler {
	
	private Activity mainActivity;
	
	public MailUriHandler(Activity activity) {
		mainActivity = activity;
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
		
		mainActivity.startActivity(Intent.createChooser(intent, "Send e-mail..."));
	}

}
