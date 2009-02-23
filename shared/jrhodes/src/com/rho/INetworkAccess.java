package com.rho;

import java.io.IOException;
import java.net.HttpURLConnection;

public interface INetworkAccess {

	public abstract String getSuffix();
	
	public abstract void autoConfigure();
	
	public abstract boolean isNetworkAvailable();
	
	public abstract HttpURLConnection connect(String server) throws IOException;
	
	public abstract void log(String txt);
	
}
