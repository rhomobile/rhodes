package com.rho.net;

import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.*;
import javax.microedition.io.SocketConnection;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

//import net.rim.device.api.io.SocketConnectionEnhanced;

public class TCPSocket extends RubyBasic {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("TCPSocket");

	private SocketConnection m_conn;
	//private SocketConnectionEnhanced m_extconn;
	private OutputStream     m_os;
	private InputStream      m_is;
	
	public TCPSocket(RubyClass c) {
		super(c);
	}

    public static TCPSocket alloc(RubyValue receiver) {
    	return new TCPSocket((RubyClass)receiver);
    }
	
    public void initialize(String strHost, int nPort)throws IOException
    {
    	m_conn = ((NetworkAccess)RhoClassFactory.getNetworkAccess()).socketConnect(strHost, nPort);
    	
    	/*
		try{
			m_extconn = (SocketConnectionEnhanced)m_conn;
		}catch(ClassCastException cce){
			LOG.ERROR("Cannot get SocketConnectionEnhanced connection.", cce);
		}    	
		
		if ( m_extconn != null )
			m_extconn.setSocketOptionEx(SocketConnectionEnhanced.READ_TIMEOUT, 1000);
		*/
    }
    
    public boolean is_closed()
    {
    	return m_conn == null;
    }

    public void close()throws IOException
    {
    	if ( m_conn != null )
    		m_conn.close();
    	if (m_os != null)
    		m_os.close();
    	if (m_is != null)
    		m_is.close();
    	
    	m_is = null;
    	m_os = null;
    	m_conn = null;
    }

    public int write(String strData)throws IOException
    {
    	if (m_os == null)
    		m_os = m_conn.openOutputStream();
    	
    	byte[] bytes = strData.getBytes(); 
    	m_os.write(bytes);
    	return bytes.length;
    }

    // Similar to #read, but raises +EOFError+ at end of string instead of
    // returning +nil+, as well as IO#sysread does.
    public String sysread(int nBytes)throws Exception
    {
    	if (m_os != null)
    		m_os.close();
    	m_os = null;

    	if (m_is==null)
    		m_is = m_conn.openInputStream();
    	
    	byte[] charBuffer = new byte[nBytes];
        bufferedReadByByte(charBuffer, m_is);
    	
        return new String(charBuffer);
    }
    
	private final int bufferedReadByByte(byte[] a, InputStream in) throws IOException {
		int bytesRead = 0;
		byte[] buf = new byte[1];
		while (bytesRead < (a.length)) 
		{
			int read = 0;
			try{
				read = in.read( buf, 0, buf.length );
				if (read < 0) {
					return bytesRead > 0 ? bytesRead : -1;
				}

				read = buf[0];
			}catch(IOException exc)
			{
				return bytesRead > 0 ? bytesRead : -1;
			}
			
			a[bytesRead] = (byte)read;
			bytesRead ++;
		}
		return bytesRead;
	}
    
	public static void initMethods(RubyClass klass) {
		
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return TCPSocket.alloc(receiver);}
		});
		
		klass.defineMethod( "initialize", new RubyTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block )
			{
		    	try{
					String strHost = arg1.toStr();
					int nPort = arg2.toInt();
					((TCPSocket)receiver).initialize(strHost, nPort);
			        return receiver;
					
				}catch(Exception e)
				{
					LOG.ERROR("initialize failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
		klass.getSingletonClass().defineMethod("open", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) 
			{
				try{
					TCPSocket res = TCPSocket.alloc(RubyRuntime.TCPSocketClass);
					String strHost = arg1.toStr();
					int nPort = arg2.toInt();
					res.initialize(strHost, nPort);
					
					return res;
				}catch(Exception e)
				{
					LOG.ERROR("open failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
		klass.defineMethod( "closed?", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
		    	try{
					boolean bRes = ((TCPSocket)receiver).is_closed();
					
					return ObjectFactory.createBoolean(bRes);
				}catch(Exception e)
				{
					LOG.ERROR("closed? failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});

		klass.defineMethod( "close", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
		    	try{
					((TCPSocket)receiver).close();
					
					return RubyConstant.QNIL;
				}catch(Exception e)
				{
					LOG.ERROR("close failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});

		klass.defineMethod( "write", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
		    	try{
					int nRes = ((TCPSocket)receiver).write(arg.toStr());
					
					return ObjectFactory.createInteger(nRes);
				}catch(Exception e)
				{
					LOG.ERROR("write failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});

		klass.defineMethod( "sysread", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
		    	try{
					String strRes = ((TCPSocket)receiver).sysread(arg.toInt());
					
					return ObjectFactory.createString(strRes);
				}catch(Exception e)
				{
					LOG.ERROR("sysread failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
	}	
}
