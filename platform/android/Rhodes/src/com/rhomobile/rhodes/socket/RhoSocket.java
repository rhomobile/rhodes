package com.rhomobile.rhodes.socket;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;

public class RhoSocket extends Socket {
	
	public RhoSocket(int sockfd, RhoSockAddr remote) throws IOException {
		super(new RhoSocketImpl(sockfd, remote));
		// Dummy line, need to get correctly initialized internal states ('created', 'bound', 'connected')
		connect(new InetSocketAddress(remote.host, remote.port));
	}
	
}
