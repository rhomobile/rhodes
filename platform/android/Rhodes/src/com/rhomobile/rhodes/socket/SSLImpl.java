package com.rhomobile.rhodes.socket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

import com.rhomobile.rhodes.Logger;

public class SSLImpl {
	
	private static final String TAG = "SSLImplJava";
	
	private static SSLSocketFactory factory = null;
	
	private SSLSocket sock;
	private int sockfd;
	
	private InputStream is;
	private OutputStream os;
	
	public native RhoSockAddr getRemoteSockAddr(int sockfd);
	
	private static class MyTrustManager implements X509TrustManager {
		
		public void checkClientTrusted(X509Certificate[] chain, String authType)
				throws CertificateException {
			// Nothing
			Logger.T(TAG, "checkClientTrusted");
		}

		public void checkServerTrusted(X509Certificate[] chain, String authType)
				throws CertificateException {
			// Nothing
			Logger.T(TAG, "checkServerTrusted");
		}

		public X509Certificate[] getAcceptedIssuers() {
			Logger.T(TAG, "getAcceptedIssuers");
			return new X509Certificate[0];
		}
		
	};
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getClass().getSimpleName() + ": " + e.getMessage());
	}
	
	private static SSLSocketFactory getFactory(boolean verify) throws NoSuchAlgorithmException, KeyManagementException {
		if (verify)
			return (SSLSocketFactory)SSLSocketFactory.getDefault();
		
		if (factory == null) {
			SSLContext context = SSLContext.getInstance("TLS");
			TrustManager[] managers = {new MyTrustManager()};
			context.init(null, managers, new SecureRandom());
			factory = context.getSocketFactory();
		}
		return factory;
	}
	
	public boolean connect(int fd, boolean sslVerifyPeer) {
		try {
			sockfd = fd;
			RhoSockAddr remote = getRemoteSockAddr(sockfd);
			Socket s = new RhoSocket(sockfd, remote);
			SSLSocketFactory f = getFactory(sslVerifyPeer);
			sock = (SSLSocket)f.createSocket(s, remote.host.toString(), remote.port, true);
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
		catch (IOException e) {
		    Logger.I(TAG, "shutdown fails: IOException: " + e.getMessage());
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
