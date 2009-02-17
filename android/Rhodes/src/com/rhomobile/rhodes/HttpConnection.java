package com.rhomobile.rhodes;

import java.io.IOException;

import com.rho.RhoConnection;

public class HttpConnection extends RhoConnection {

	public HttpConnection(String uri)
	{
		super(uri);
	}
	
	@Override
	protected void showGeoLocation() {
		// TODO Auto-generated method stub
		
	}

	public void processRequest() throws IOException
	{
		super.processRequest();
	}
	

}
