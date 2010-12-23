package com.rho;

import javax.microedition.io.HttpConnection;
import rhomobile.RhodesApplication;

public class BrowserAdapter5 implements IBrowserAdapter 
{
	public BrowserAdapter5(RhoMainScreen oMainScreen, RhodesApplication app) 
	{
	}

	public void setFullBrowser()
	{
	}
	
    public void processConnection(HttpConnection connection, Object e) 
    {
    }

    public void executeJavascript(String strJavascript)
    {
    }
    public void setCookie(String url, String cookie)
    {
    }
    
    public boolean navigationMovement(int dx, int dy, int status, int time)
    {
    	return false;
    }
}
