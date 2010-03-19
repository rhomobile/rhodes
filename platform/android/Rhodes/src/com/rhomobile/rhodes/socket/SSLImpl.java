package com.rhomobile.rhodes.socket;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;

import com.rhomobile.rhodes.Logger;

public class SSLImpl {
	
	private static final String TAG = "SSLImplJava";
	
	private SSLSocket sock;
	private int sockfd;
	
	private InputStream is;
	private OutputStream os;
	
	public native RhoSockAddr getRemoteSockAddr(int sockfd);
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	public boolean connect(int fd) {
		try {
			sockfd = fd;
			SSLSocketFactory factory = (SSLSocketFactory)SSLSocketFactory.getDefault();
			RhoSockAddr remote = getRemoteSockAddr(sockfd);
			Socket s = new RhoSocket(sockfd, remote);
			sock = (SSLSocket)factory.createSocket(s, remote.host.toString(), remote.port, true);
			sock.setUseClientMode(true);
			os = sock.getOutputStream();
			is = sock.getInputStream();
			return true;
		}
		catch (Exception e) {
			reportFail("connect", e);
			e.printStackTrace();
			return false;
		}
	}
	
	public void shutdown() {
		try {
			if (sock != null)
				sock.close();
		}
		catch (Exception e) {
			reportFail("shutdown", e);
		}
	}
	
	public boolean send(byte[] data) {
		try {
			if (os == null)
				return false;
			os.write(data);
			return true;
		}
		catch (Exception e) {
			reportFail("send", e);
			return false;
		}
	}
	
	public int recv(byte[] data) {
		try {
			if (is == null)
				return -1;
			
			int size = data.length;
			int n = is.read(data, 0, size);
			return n;
		}
		catch (Exception e) {
			reportFail("recv", e);
			return -1;
		}
	}
	
}
