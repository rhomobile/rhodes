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

public class RhoSocketImpl extends SocketImpl {
	
	private int sockfd = -1;
	
	private int soTimeout = 0;
	private int ipTos = 0;
	
	private native void initImpl(int sock);
	private native InetAddress getRemoteHostImpl(int sock);
	private native int getRemotePortImpl(int sock);
	private native void closeImpl(int sock);
	private native Object getOptionImpl(int sock, int option);
	private native void setOptionImpl(int sock, int option, Object value);
	
	public RhoSocketImpl(int s) {
		sockfd = s;
		initImpl(sockfd);
	}
	
	@Override
	public InetAddress getInetAddress() {
		InetAddress addr = getRemoteHostImpl(sockfd);
		return addr;
	}
	
	@Override
	public int getPort() {
		return getRemotePortImpl(sockfd);
	}
	
	@Override
	protected void close() throws IOException {
		closeImpl(sockfd);
		fd = new FileDescriptor();
		sockfd = -1;
	}
	
	@Override
	public Object getOption(int option) throws SocketException {
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
		// Ignore
		return 0;
	}
	
	@Override
	protected InputStream getInputStream() throws IOException {
		// Ignore
		return null;
	}
	
	@Override
	protected OutputStream getOutputStream() throws IOException {
		// Ignore
		return null;
	}
	
	@Override
	protected void create(boolean arg0) throws IOException {
		// Nothing
	}
	
	@Override
	protected void connect(String arg0, int arg1) throws IOException {
		// Nothing
	}
	
	@Override
	protected void connect(InetAddress arg0, int arg1) throws IOException {
		// Nothing
	}
	
	@Override
	protected void connect(SocketAddress arg0, int arg1) throws IOException {
		// Nothing
	}
	
	@Override
	protected void listen(int arg0) throws IOException {
		// Nothing
	}
	
	@Override
	protected void accept(SocketImpl arg0) throws IOException {
		// Nothing
	}
	
	@Override
	protected void bind(InetAddress arg0, int arg1) throws IOException {
		// Nothing
	}
	
	@Override
	protected void sendUrgentData(int arg0) throws IOException {
		// Nothing
	}
	
}
