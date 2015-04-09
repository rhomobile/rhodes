package com.rhomobile.rhodes.event;

import android.net.Uri;

public class CalendarIDsProviderFroyo extends CalendarIDsProviderBase {
	public Uri.Builder getUriBuilder() { return new Uri.Builder().scheme("content").authority("com.android.calendar"); }
}