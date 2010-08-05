package com.rho;

import com.rho.db.IDBStorage;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyProgram;
import com.rho.net.NetResponse;
import com.rho.file.*;

public interface IRhoRubyHelper {
	public abstract void initRubyExtensions();
	public abstract RubyProgram createMainObject() throws Exception;
	public abstract String getPlatform();
	
	public abstract IDBStorage createDBStorage();
	public abstract IFileAccess createFileAccess();
	public abstract IRAFile createRAFile();
	public abstract IRAFile createFSRAFile();
	
	public void loadBackTrace(RubyArray backtrace);
	public String getDeviceId();
	
	public String getAppProperty(String name);
	public String getModuleName();
	public boolean isSimulator();
	
	public void showLog();
	
	public String getGeoLocationText();
	public void wakeUpGeoLocation();
	
	public NetResponse postUrl(String url, String body);
	public void navigateUrl(String url);
	public void navigateBack();
}
