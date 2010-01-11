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
import java.util.Vector;

//import net.rim.device.api.io.SocketConnectionEnhanced;

public class TCPSocket extends RubyBasic {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("TCPSocket");

	private SocketConnection m_conn;
	//private SocketConnectionEnhanced m_extconn;
	private OutputStream     m_os;
	private InputStream      m_is;
	
	private SocketThread     m_thr;
	
	private static class Result {
		
		int size;
		byte[] result;
		
		Result (int s) {
			size = s;
			result = null;
		}
		
		Result (byte[] b) {
			result = b;
		}
		
	}
	
	private static class SocketCommand {
		
		private static final int READ = 0;
		private static final int WRITE = 1;
		
		public int cmd;
		public byte[] buffer;
		public Result notifier;
		
		public SocketCommand(int c, byte[] b, Result n) {
			cmd = c;
			buffer = b;
			notifier = n;
		}
		
	};
	
	private class SocketThread extends Thread {
		
		private Vector queue = new Vector();
		
		public void send_command(int cmd, byte[] b, Result n) {
			synchronized (queue) {
				queue.addElement(new SocketCommand(cmd, b, n));
				queue.notify();
			}
		}
		
		public void run() {
			Vector cmds = new Vector();
			for (;;) {
				cmds.removeAllElements();
				synchronized (queue) {
					try {
						queue.wait();
					} catch (InterruptedException e) {
						continue;
					}
					
					for (int i = 0; i != queue.size(); ++i)
						cmds.addElement(queue.elementAt(i));
					queue.removeAllElements();
				}
				
				for (int i = 0; i != cmds.size(); ++i) {
					SocketCommand cmd = (SocketCommand)cmds.elementAt(i);
					try {
						switch(cmd.cmd) {
						case SocketCommand.READ:
							if (m_is == null)
								m_is = m_conn.openInputStream();
							int readed;
							for (readed = 0; readed < cmd.buffer.length;) {
								int n = m_is.read(cmd.buffer, readed, cmd.buffer.length - readed);
								if (n < 0)
									break;
								readed += n;
							}
							synchronized (cmd.notifier) {
								cmd.notifier.size = readed;
								cmd.notifier.result = cmd.buffer;
								cmd.notifier.notify();
							}
							break;
						case SocketCommand.WRITE:
							if (m_os == null)
								m_os = m_conn.openOutputStream();
							m_os.write(cmd.buffer);
							synchronized (cmd.notifier) {
								cmd.notifier.size = cmd.buffer.length;
								cmd.notifier.notify();
							}
							break;
						default:
							// Silently ignore
						}
					}
					catch (IOException e) {
						// TODO:
					}
				}
			}
		}
		
	};
	
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
    	m_thr = new SocketThread();
    	m_thr.start();
    	*/
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
    	//m_thr.interrupt();
    	//m_thr = null;
    }

    public int write(String strData)throws IOException
    {
    	if (m_os == null)
    		m_os = m_conn.openOutputStream();
    	
    	byte[] bytes = strData.getBytes(); 
    	m_os.write(bytes);
    	return bytes.length;
    	
    	/*
    	Result res = new Result(0);
    	m_thr.send_command(SocketCommand.WRITE, strData.getBytes(), res);
    	synchronized (res) {
    		for (;;) {
				try {
					res.wait();
				} catch (InterruptedException e) {
					continue;
				}
				return res.size;
    		}
		}
		*/
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
        int n = bufferedReadByByte(charBuffer, m_is);
    	
        return new String(charBuffer, 0, n == -1 ? 0 : n);

        /*
    	Result res = new Result(0);
    	m_thr.send_command(SocketCommand.READ, new byte[nBytes], res);
    	synchronized (res) {
			for (;;) {
				try {
					res.wait();
				}
				catch (InterruptedException e) {
					continue;
				}
				return new String(res.result, 0, res.size);
			}
		}
		*/
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
