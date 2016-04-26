package com.rho.rhoelements.transferprotocols;


import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.UnrecoverableKeyException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.List;
import java.net.Socket;
import java.net.UnknownHostException;

import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;
import javax.net.ssl.X509TrustManager;

import android.util.Base64;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.Config;
import com.rho.rhoelements.LogEntry;

class RESecureSocketFactory
{
    
    private static class MyTrustManager implements X509TrustManager {
		
        public void checkClientTrusted(X509Certificate[] chain, String authType)
        throws CertificateException {
            // Nothing
        }
        
        public void checkServerTrusted(X509Certificate[] chain, String authType)
        throws CertificateException {
            // Nothing
        }
        
        public X509Certificate[] getAcceptedIssuers() {
            return new X509Certificate[0];
        }
        
    };
	
	private static class MySecureTrustManager implements X509TrustManager {
		private X509TrustManager mSysTrustManager;
		private X509TrustManager mCustomTrustManager;
		
		public MySecureTrustManager( X509TrustManager sysTrustManager, X509TrustManager customTrustManager ) {
			mSysTrustManager = sysTrustManager;
			mCustomTrustManager = customTrustManager;
		}
        
		@Override
		public void checkClientTrusted(X509Certificate[] chain, String authType) throws CertificateException {
			try {
				if ( mCustomTrustManager != null ) {
					mCustomTrustManager.checkClientTrusted(chain, authType);
				}
			} catch ( CertificateException e ) {
				if ( mSysTrustManager != null ) {
					mSysTrustManager.checkClientTrusted(chain, authType);
				}
			}
		}
        
		@Override
		public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {
			try {
				if ( mCustomTrustManager != null ) {
					mCustomTrustManager.checkServerTrusted(chain, authType);
				}
			} catch ( CertificateException e ) {
				if ( mSysTrustManager != null ) {
					mSysTrustManager.checkServerTrusted(chain, authType);
				}
			}
			
		}
        
		@Override
		public X509Certificate[] getAcceptedIssuers() {
			X509Certificate ret[] = null;
			
			X509Certificate customAcceptedIssuers[] = (mCustomTrustManager!=null)?mCustomTrustManager.getAcceptedIssuers():new X509Certificate[0];
			X509Certificate sysAcceptedIssuers[] = (mSysTrustManager!=null)?mSysTrustManager.getAcceptedIssuers():new X509Certificate[0];
			
			if ( customAcceptedIssuers == null ) {
				customAcceptedIssuers = new X509Certificate[0];
			}
			
			if ( sysAcceptedIssuers == null ) {
				sysAcceptedIssuers = new X509Certificate[0];
			}
			
			int size = customAcceptedIssuers.length + sysAcceptedIssuers.length;
			
			if ( size > 0 ) {
				
		        ret = new X509Certificate[ size ];
		        
		        System.arraycopy(sysAcceptedIssuers, 0, ret, 0, sysAcceptedIssuers.length);
		        System.arraycopy(customAcceptedIssuers, 0, ret, sysAcceptedIssuers.length, customAcceptedIssuers.length);
			}
			
			return ret;
		}
		
	};
	
	private static void reportFail(String name, Exception e) {
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Call of \"" + name + "\" failed: " + e.getClass().getSimpleName() + ": " + e.getMessage()));
	}
    
    private static byte[] fileToBytes (File file) throws IOException {
        RandomAccessFile f = new RandomAccessFile(file, "r");
        
        try {
            long longlength = f.length();
            int length = (int) longlength;
            if (length != longlength) throw new IOException("File size >= 2 GB");
            
            byte[] data = new byte[length];
            f.readFully(data);
            return data;
        }
        finally {
            f.close();
        }
    }
	
	private static byte[] parseDERFromPEM(byte[] pem, String beginDelimiter, String endDelimiter) {
	    String data = new String(pem);
	    String[] tokens = data.split(beginDelimiter);
	    tokens = tokens[1].split(endDelimiter);
	    return Base64.decode(tokens[0],Base64.DEFAULT);
	}
    
	protected static X509Certificate generateCertificateFromDER(byte[] certBytes) throws CertificateException {
	    CertificateFactory factory = CertificateFactory.getInstance("X.509");
	    return (X509Certificate)factory.generateCertificate(new ByteArrayInputStream(certBytes));
	}
    
    private static Certificate loadCertificate( File f ) {
        X509Certificate cert = null;
        
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Loading SSL certificate from PEM file: " + f.getAbsolutePath()));
        
        try {
            
            byte[] fileBuf = fileToBytes( f );
            byte[] certBytes = parseDERFromPEM(fileBuf, "-----BEGIN CERTIFICATE-----", "-----END CERTIFICATE-----");
            cert = generateCertificateFromDER(certBytes);
            
            Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "SSL certificate loaded successfully"));
            
        } catch( IOException e ) {
            Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Reading certificate file failed: " + e.getClass().getSimpleName() + ": " + e.getMessage()));
        } catch ( CertificateException e ) {
            Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Certificate generation failed: " + e.getClass().getSimpleName() + ": " + e.getMessage()));
        }
        
        return cert;
    }
    
    private static List<Certificate> loadAllCertificates() {
        List<Certificate> certs = new ArrayList<Certificate>();
        
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Loading all SSL certificates from config"));
        
        
        if ( Common.config.getSetting( Config.SETTING_CAFILE ) != null ) {
            
            String caFilePath = Common.config.getSetting( Config.SETTING_CAFILE );
            
            Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "CAFile found in config: loading certificate: " + caFilePath));
            
            File caFile = new File( caFilePath );
            
            if ( caFile.exists() ) {
                Certificate c = loadCertificate(caFile);
                if ( c != null ) {
                    certs.add( c );
                }
            } else {
                Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "CAFile config parameter exists, but file " + caFilePath + " not found."));
            }
        }
        
        if ( Common.config.getSetting( Config.SETTING_CAPATH ) != null ) {
            String caFolderPath = Common.config.getSetting( Config.SETTING_CAPATH );
            
            Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "CAPath found in config: loading all certificates from " + caFolderPath));
            
            File caFolder = new File( caFolderPath );
            
            if ( caFolder.isDirectory() ) {
                File list[] = caFolder.listFiles();
                for ( File f : list ) {
                    Certificate c = loadCertificate(f);
                    if ( c != null ) {
                        certs.add( c );
                    }
                }
                
            } else {
                Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "CAPath config parameter exists, but folder " + caFolderPath + " not found."));
            }
        }
        
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "SSL certificates loaded: " + String.valueOf(certs.size())));
        
        return certs;
    }
    
    private static KeyStore getKeyStore() throws KeyStoreException, IOException, NoSuchAlgorithmException, CertificateException {
        // Create keystore for custom certificates
        KeyStore keystore = KeyStore.getInstance( KeyStore.getDefaultType() );
        keystore.load(null);
        
        List<Certificate> certs = loadAllCertificates();
        
        // Add loaded custom certificates to keystore
        if ( certs != null ) {
            for ( int i = 0; i < certs.size(); ++i ) {
                keystore.setCertificateEntry("cert-alias"+ String.valueOf(i),certs.get(i));
            }
        }
        
        return keystore;
    }
    
    private static SSLContext getSecureContext(KeyStore keystore) throws NoSuchAlgorithmException, KeyManagementException, CertificateException, KeyStoreException, IOException, UnrecoverableKeyException {
        SSLContext context = SSLContext.getInstance("TLS");
        
        // First, load all system installed certificates
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Creating TrustManager for system certificates"));
        
        TrustManagerFactory systemTmf = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
        systemTmf.init((KeyStore)null);
        X509TrustManager systemTrustManager = (X509TrustManager)systemTmf.getTrustManagers()[0];
        
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Creating TrustManager for custom certificates"));
        
        TrustManagerFactory tmf = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
        tmf.init(keystore);
        X509TrustManager customTrustManager = (X509TrustManager)tmf.getTrustManagers()[0];
        
        KeyManagerFactory kmf = null;
        /*
         if ( RhoConf.isExist("clientSSLCertificate")) {
         String clientCertPath = RhoConf.getString("clientSSLCertificate");
         
         Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "clientSSLCertificate is " + clientCertPath));
         
         if ( clientCertPath.length() > 0 ) {
         Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Creating KeyManager for client certificates"));
         
         kmf = KeyManagerFactory.getInstance( KeyManagerFactory.getDefaultAlgorithm() );
         
         String password = "";
         if (RhoConf.isExist("clientSSLCertificatePassword")) {
         password = RhoConf.getString("clientSSLCertificatePassword");
         }
         
         KeyStore clientKeystore = KeyStore.getInstance( "pkcs12" );
         clientKeystore.load( RhoFileApi.open(clientCertPath), password.toCharArray() );
         kmf.init(clientKeystore, password.toCharArray());
         }
         }
         */
        /*
         * this really works only with first provided TrustManager,
         * so we make our own wrapper which encapsulates both system installed and custom provided certificates
         */
        context.init(
                     (kmf==null)?null:kmf.getKeyManagers(),
                     new TrustManager[] { new MySecureTrustManager( systemTrustManager, customTrustManager ) },
                     new SecureRandom()
                     );
        
        return context;
    }
    
    private static SSLSocketFactory getSecureFactory() throws NoSuchAlgorithmException, KeyManagementException, CertificateException, KeyStoreException, IOException, UnrecoverableKeyException {
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Creating secure SSL factory"));
        
        SSLContext context = getSecureContext( getKeyStore() );
        
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Secure SSL factory initialization completed"));
        
        return (SSLSocketFactory)context.getSocketFactory();
        
    }
    
    public static javax.net.ssl.SSLSocketFactory createJavaXFactory()
    {
        javax.net.ssl.SSLSocketFactory factory = null;
        
        if (Common.config.getBooleanSetting(Config.SETTING_VERIFY_PEER))
        {
            try
            {
                factory = getSecureFactory();
            }
            catch(Exception e)
            {
                reportFail("RESecureSocketFactory.createJavaXFactory",e);
            }
        }
    
        if (null==factory)
        {
            try
            {
                SSLContext context = SSLContext.getInstance("TLS");
                TrustManager[] managers = {new MyTrustManager()};
                context.init(null, managers, new SecureRandom());
                factory = context.getSocketFactory();
            }
            catch(Exception e)
            {
                reportFail("RESecureSocketFactory.createJavaXFactory",e);
            }
        }
        
        return factory;
    }
    
    
    
    public static class ApacheSSLSocketFactory extends org.apache.http.conn.ssl.SSLSocketFactory {
        private SSLContext sslContext;
        
        public ApacheSSLSocketFactory(SSLContext context, KeyStore truststore) throws NoSuchAlgorithmException, KeyManagementException, KeyStoreException, UnrecoverableKeyException {
            super(truststore);
            
            sslContext = context;
        }
        
        @Override
        public Socket createSocket(Socket socket, String host, int port, boolean autoClose) throws IOException, UnknownHostException {
            return sslContext.getSocketFactory().createSocket(socket, host, port, autoClose);
        }
        
        @Override
        public Socket createSocket() throws IOException {
            return sslContext.getSocketFactory().createSocket();
        }
    }
    

    public static org.apache.http.conn.ssl.SSLSocketFactory createApacheFactory()
    {
        try
        {
            Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Creating secure SSL factory"));
        
            KeyStore keystore = getKeyStore();

            SSLContext context = getSecureContext(keystore);
        
            return new ApacheSSLSocketFactory(context,keystore);
        }
        catch (Exception e)
        {
            reportFail("RESecureSocketFactory.createApacheFactory",e);
            return org.apache.http.conn.ssl.SSLSocketFactory.getSocketFactory();
        }
    }
}