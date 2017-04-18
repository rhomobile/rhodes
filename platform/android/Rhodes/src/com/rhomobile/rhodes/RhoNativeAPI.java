package com.rhomobile.rhodes;

import android.webkit.WebResourceResponse;

/*
	Expose all native JNI methods here
*/

public class RhoNativeAPI {	
    //call this before calling any other native RHO API;
	public static native void initJNIThread();

	public static native WebResourceResponse directLocalRequest( String url );
}