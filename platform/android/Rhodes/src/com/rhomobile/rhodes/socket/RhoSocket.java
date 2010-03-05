package com.rhomobile.rhodes.socket;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;

public class RhoSocket extends Socket {
	
	public RhoSocket(int sockfd) throws IOException {
		super(new RhoSocketImpl(sockfd));
		// Dummy string, just to set 'connected' state
		connect(new InetSocketAddress("localhost", 0));
	}
	
}
