package com.rhomobile.rhodes.util;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;

import android.content.Context;

public class ContextFactory {
	public static Context getContext() { return RhodesService.getInstance(); }
	public static Context getUiContext() { return RhodesActivity.getContext(); }

}
