package com.rhomobile.rhodes;

public class RhoConf {

	public static native String getString(String name);
	public static native int getInt(String name);
	public static native boolean getBool(String name);
	
	public static native void setString(String name, String value);
	public static native void setInt(String name, int value);
	public static native void setBoolean(String name, boolean value);
	
	public static native boolean isExist(String name);
}
