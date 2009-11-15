package com.rhomobile.rhodes;

public class Logger {

	// Errors
	public native static void E(String tag, String msg);
	// Warnings
	public native static void W(String tag, String msg);
	// Info
	public native static void I(String tag, String msg);
	// Debug
	public native static void D(String tag, String msg);
	// Trace
	public native static void T(String tag, String msg);
	
}
