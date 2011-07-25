/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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

    //Used from jni
    @SuppressWarnings("unused")
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
            RhoSockAddr remote = getRemoteSockAddr(fd);
            Socket s = new RhoSocket(fd, remote);
            SSLSocketFactory f = getFactory(sslVerifyPeer);

            SSLSocket aSock = (SSLSocket)f.createSocket(s, remote.host.toString(), remote.port, true);
            aSock.setUseClientMode(true);

            synchronized (this) {
                sock = aSock;
                os = sock.getOutputStream();
                is = sock.getInputStream();
                sockfd = fd;
            }
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
			if (sock != null) {
                synchronized (this) {
                    if (sock != null) {
                        sock.close();
                        sock = null;
                        os = null;
                        is = null;

                        //TODO: check file descriptor is closed gracefully
                        sockfd = -1;
                    }
                }
			}
		}
		catch (Exception e) {
			reportFail("shutdown", e);
		}
	}
	
	public boolean send(byte[] data) {
		try {
			if (os != null) {
                OutputStream aOs = null;
                synchronized (this) {
                    if (os != null)
                        aOs = os;
                }
                if (aOs != null) {
                    aOs.write(data);
                    return true;
                }
			}
		}
		catch (Exception e) {
			reportFail("send", e);
		}
        return false;
	}
	
	public int recv(byte[] data) {
		try {
			if (is != null) {
                InputStream aIs = null;
                synchronized (this) {
                    if (is != null)
                        aIs = is;
                }
                if (aIs != null) {
                    int size = data.length;
                    int n = is.read(data, 0, size);
                    return n;
                }
			}
		}
		catch (Exception e) {
			reportFail("recv", e);
		}
        return -1;
    }

}
