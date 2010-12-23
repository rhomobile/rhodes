package com.rho;

import javax.microedition.io.HttpConnection;

public interface IBrowserAdapter {
	public void processConnection(HttpConnection connection, Object e);
	public boolean navigationMovement(int dx, int dy, int status, int time);
	public void executeJavascript(String strJavascript);
	public void setFullBrowser();
	public void setCookie(String url, String cookie);
}
