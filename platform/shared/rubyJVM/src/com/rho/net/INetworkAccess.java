package com.rho.net;

import java.io.IOException;

public interface INetworkAccess {

	public abstract void configure();
	public abstract IHttpConnection connect(String server) throws IOException;
	public abstract void close();
	
	public abstract String getHomeUrl();
}
