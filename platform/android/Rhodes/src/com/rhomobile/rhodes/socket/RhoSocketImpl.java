package com.rhomobile.rhodes.socket;

import java.io.FileDescriptor;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.SocketAddress;
import java.net.SocketException;
import java.net.SocketImpl;
import java.net.SocketOptions;

import com.rhomobile.rhodes.Logger;

public class RhoSocketImpl extends SocketImpl {
	
	private static final String TAG = "RhoSocketImpl";
	
	private int sockfd = -1;
	
	private int soTimeout = 0;
	private int ipTos = 0;
	
	private RhoSockAddr remote;
	
	private native void initImpl(int sock);
	private native void closeImpl(int sock);
	private native Object getOptionImpl(int sock, int option);
	private native void setOptionImpl(int sock, int option, Object value);
	
	public RhoSocketImpl(int s, RhoSockAddr rem) {
		Logger.D(TAG, "New socket wrapper. fd: " + s + ", host: " + rem.host + ", port: " + rem.port);
		
		sockfd = s;
		remote = rem;
		initImpl(sockfd);
	}
	
	@Override
	public InetAddress getInetAddress() {
		return remote.host;
	}
	
	@Override
	public int getPort() {
		return remote.port;
	}
	
	@Override
	protected void close() throws IOException {
		Logger.T(TAG, "close");
		closeImpl(sockfd);
		fd = new FileDescriptor();
		sockfd = -1;
	}
	
	@Override
	public Object getOption(int option) throws SocketException {
		Logger.T(TAG, "getOption");
		switch (option) {
		case SocketOptions.SO_TIMEOUT:
			return Integer.valueOf(soTimeout);
		case SocketOptions.IP_TOS:
			return Integer.valueOf(ipTos);
		default:
			return getOptionImpl(sockfd, option);
		}
	}
	
	@Override
	public void setOption(int option, Object value) throws SocketException {
		Logger.T(TAG, "setOption");
		if (option == SocketOptions.SO_TIMEOUT)
			soTimeout = ((Integer)value).intValue();
		else {
			setOptionImpl(sockfd, option, value);
			if (option == SocketOptions.IP_TOS)
				ipTos = ((Integer)value).intValue();
		}
	}
	
	@Override
	protected int available() throws IOException {
		Logger.T(TAG, "available");
		// Ignore
		return 0;
	}
	
	@Override
	protected InputStream getInputStream() throws IOException {
		Logger.T(TAG, "getInputStream");
		// Ignore
		return null;
	}
	
	@Override
	protected OutputStream getOutputStream() throws IOException {
		Logger.T(TAG, "getOutputStream");
		// Ignore
		return null;
	}
	
	@Override
	protected void create(boolean arg0) throws IOException {
		Logger.T(TAG, "create");
		// Nothing
	}
	
	@Override
	protected void connect(String arg0, int arg1) throws IOException {
		Logger.T(TAG, "connect1");
		// Nothing
	}
	
	@Override
	protected void connect(InetAddress arg0, int arg1) throws IOException {
		Logger.T(TAG, "connect2");
		// Nothing
	}
	
	@Override
	protected void connect(SocketAddress arg0, int arg1) throws IOException {
		Logger.T(TAG, "connect3");
		// Nothing
	}
	
	@Override
	protected void listen(int arg0) throws IOException {
		Logger.T(TAG, "listen");
		// Nothing
	}
	
	@Override
	protected void accept(SocketImpl arg0) throws IOException {
		Logger.T(TAG, "accept");
		// Nothing
	}
	
	@Override
	protected void bind(InetAddress arg0, int arg1) throws IOException {
		Logger.T(TAG, "bind");
		// Nothing
	}
	
	@Override
	protected void sendUrgentData(int arg0) throws IOException {
		Logger.T(TAG, "sendUrgentData");
		// Nothing
	}
	
}
