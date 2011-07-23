package com.rhomobile.rhodes.util;

import android.content.Context;

public class ContextFactory {
    private static Context mContext;
    private static Context mUiContext;

    public static void setContext(Context ctx) { mContext = ctx; }
    public static void setUiContext(Context ctx) { mUiContext = ctx; }

	public static Context getContext() { return mContext; }
	public static Context getUiContext() { return mUiContext; }

}
