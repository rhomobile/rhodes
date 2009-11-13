package com.rhomobile.rhodes;

public class RhoLogConf {

	public native String getEnabledCategories();
	public native void setEnabledCategories(String c);
	
	public native String getDisabledCategories();
	public native void setDisabledCategories(String c);
	
	public native int getMinSeverity();
	public native void setMinSeverity(int s);
	
	public native void saveToFile();
	public native void clearLog();
	
	public native String getLogText();
	public native int getLogTextPos();
}
