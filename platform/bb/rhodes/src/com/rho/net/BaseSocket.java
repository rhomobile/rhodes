package com.rho.net;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Vector;

import javax.microedition.io.SocketConnection;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

import com.xruby.runtime.lang.*;

//import net.rim.device.api.io.SocketConnectionEnhanced;

public class BaseSocket extends RubyBasic {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("BaseSocket");

	// TODO: fix threads based implementation because now there are random fluctuations
	// when data does not returned even if it already readed from socket
	private static final boolean USE_THREADS_BASED_IMPLEMENTATION = false;
	
	private static final int READ_TIMEOUT = 1000;

	private SocketConnection m_conn;
	//private SocketConnectionEnhanced m_extconn;
	private OutputStream     m_os;
	private InputStream      m_is;
	
	private SocketThread     m_thr;
	private Vector           m_read_results = new Vector();
	private Vector           m_write_results = new Vector();
	
	private static class Result {
		
		int size;
		byte[] result;
		
		Result (int s) {
			size = s;
			result = null;
		}
		
		Result (byte[] b) {
			size = 0;
			result = b;
		}
		
		public void clear() {
			size = -1;
			result = null;
		}
		
	}
	
	private static class SocketCommand {
		
		private static final int READ = 0;
		private static final int WRITE = 1;
		
		public int cmd;
		public byte[] buffer;
		
		public SocketCommand(int c, byte[] b) {
			cmd = c;
			buffer = b;
		}
		
	};
	
	private class SocketThread extends Thread {
		
		private Vector queue = new Vector();
		private boolean active = true;
		
		public void stopSelf() {
			active = false;
			interrupt();
		}
		
		public void send_command(int cmd, byte[] b) {
			synchronized (queue) {
				queue.addElement(new SocketCommand(cmd, b));
				queue.notify();
			}
		}
		
		public void run() {
			Vector cmds = new Vector();
			for (;;) {
				if (!active)
					return;
				cmds.removeAllElements();
				synchronized (queue) {
					try {
						queue.wait();
					} catch (InterruptedException e) {
						if (!active)
							return;
						LOG.TRACE("Waiting on command queue interrupted", e);
						continue;
					}
					
					for (int i = 0; i != queue.size(); ++i)
						cmds.addElement(queue.elementAt(i));
					queue.removeAllElements();
				}
				
				for (int i = 0; i != cmds.size(); ++i) {
					SocketCommand cmd = (SocketCommand)cmds.elementAt(i);
					int size = 0;
					switch(cmd.cmd) {
					case SocketCommand.READ:
						try {
							LOG.TRACE("Processing READ command...");
							if (m_is == null)
								m_is = m_conn.openInputStream();
							while (size < cmd.buffer.length) {
								int n = m_is.read(cmd.buffer, size, cmd.buffer.length - size);
								if (n < 0)
									break;
								LOG.TRACE("Processing READ command: read " + n + " bytes...");
								size += n;
							}
						}
						catch (IOException e) {
							if (!active)
								return;
							LOG.TRACE("Error when reading from socket", e);
						}
						
						LOG.TRACE("Processing READ operation: notify about results");
						synchronized (m_read_results) {
							Result res = new Result(size);
							res.result = cmd.buffer;
							m_read_results.addElement(res);
							m_read_results.notify();
						}
						break;
					case SocketCommand.WRITE:
						try {
							LOG.TRACE("Processing WRITE command...");
							if (m_os == null)
								m_os = m_conn.openOutputStream();
							m_os.write(cmd.buffer);
						}
						catch (IOException e) {
							if (!active)
								return;
							LOG.TRACE("Error when writing to socket", e);
						}
						
						LOG.TRACE("Processing WRITE operation: notify about results");
						size = cmd.buffer.length;
						synchronized (m_write_results) {
							Result res = new Result(size);
							m_write_results.addElement(res);
							m_write_results.notify();
						}
						break;
					default:
						// Silently ignore
						LOG.TRACE("Unknown command, ignore it");
					}
				}
			}
		}
		
	};
	
	public BaseSocket(RubyClass c) {
		super(c);
	}
	
	protected void setConnection(SocketConnection c) {
		m_conn = c;
	}
	
	public void initialize(String strHost, int nPort) throws IOException
	{
		if (USE_THREADS_BASED_IMPLEMENTATION) {
	    	m_thr = new SocketThread();
	    	m_read_results.removeAllElements();
	    	m_write_results.removeAllElements();
	    	m_thr.start();
    	}
    	
		/*
		try{
			m_extconn = (SocketConnectionEnhanced)m_conn;
		}catch(ClassCastException cce){
			LOG.ERROR("Cannot get SocketConnectionEnhanced connection.", cce);
		}    	
		
		if ( m_extconn != null )
			m_extconn.setSocketOptionEx(SocketConnectionEnhanced.READ_TIMEOUT, READ_TIMEOUT);
		*/
	}
	
	public boolean is_closed()
    {
    	return m_conn == null;
    }
	
	public void flush()throws IOException
    {
    	if (m_os != null)
    		m_os.flush();
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
    	if (USE_THREADS_BASED_IMPLEMENTATION) {
    		m_thr.stopSelf();
    		m_thr = null;
    		m_read_results.removeAllElements();
    		m_write_results.removeAllElements();
    	}
    }
	
	public int write(String strData)throws IOException
    {
    	if (USE_THREADS_BASED_IMPLEMENTATION) {
    		LOG.TRACE("Send WRITE command");
        	m_thr.send_command(SocketCommand.WRITE, strData.getBytes());
        	LOG.TRACE("Waiting for WRITE results...");
        	synchronized (m_write_results) {
        		for (;;) {
    				try {
    					m_write_results.wait();
    				} catch (InterruptedException e) {
    					continue;
    				}
    				if (m_write_results.isEmpty())
    					return 0;
    				Result res = (Result)m_write_results.elementAt(0);
    				m_write_results.removeElementAt(0);
    				
    				if (res.size != 0)
    					LOG.TRACE("WRITE success: " + res.size + " bytes written");
    				return res.size;
        		}
    		}
    	}
    	else {
	    	if (m_os == null)
	    		m_os = m_conn.openOutputStream();
	    	
	    	byte[] bytes = strData.getBytes(); 
	    	m_os.write(bytes);
	    	return bytes.length;
    	}
    }

    // Similar to #read, but raises +EOFError+ at end of string instead of
    // returning +nil+, as well as IO#sysread does.
    public String sysread(int nBytes)throws Exception
    {
    	if (USE_THREADS_BASED_IMPLEMENTATION) {
    		synchronized (m_read_results) {
    			if (m_read_results.isEmpty()) {
    				// Send request only if there is no cached results
		    		LOG.TRACE("Send READ command");
		        	m_thr.send_command(SocketCommand.READ, new byte[nBytes]);
		        	LOG.TRACE("Waiting for READ results...");
		        	m_read_results.wait(READ_TIMEOUT);
	    		}
				
    			if (m_read_results.isEmpty()) {
					// Timeout
					LOG.TRACE("Timeout on READ operation");
					m_thr.interrupt();
					return new String();
				}
				Result res = (Result)m_read_results.elementAt(0);
				m_read_results.removeElementAt(0);
				
				if (res.size == 0)
					throw new RubyException(RubyRuntime.EOFErrorClass, "");					
					//return new String();
				
				LOG.TRACE("READ success: " + res.size + " bytes readed");
				return new String(res.result, 0, res.size);
    		}
    	}
    	else {
	    	if (m_os != null)
	    		m_os.close();
	    	m_os = null;
	
	    	if (m_is==null)
	    		m_is = m_conn.openInputStream();
	    	
	    	byte[] charBuffer = new byte[nBytes];
	    	int n = m_is.read(charBuffer);
	        //int n = bufferedReadByByte(charBuffer, m_is);
	    	
	        if ( n < 0 )
	        	throw new RubyException(RubyRuntime.EOFErrorClass, "");
	        
	        return new String(charBuffer, 0, n == -1 ? 0 : n);
    	}
    }
    
	private final int bufferedReadByByte(byte[] a, InputStream in) throws IOException {
		int bytesRead = 0;
		//byte[] buf = new byte[1];
		while (bytesRead < (a.length)) 
		{
			int read = 0;
			try{
				// Implement read timeout
				// WARNING!!! Does not work because in.available() always return 0
				/*
				long started = System.currentTimeMillis();
				for (;;) {
					int avail = in.available();
					if (avail > 0)
						break;
					
					long now = System.currentTimeMillis();
					if (now - started > READ_TIMEOUT) {
						if (bytesRead > 0)
							break;
						return -1;
					}
			
					try {
						Thread.sleep(10);
					}
					catch (InterruptedException e) {}
				}
				*/
				// Actual read
				//read = in.read( buf, 0, buf.length );
				read = in.read();
				if (read < 0) {
					return bytesRead > 0 ? bytesRead : -1;
				}

				//read = buf[0];
			}catch(IOException exc)
			{
				return bytesRead > 0 ? bytesRead : -1;
			}
			
			a[bytesRead] = (byte)read;
			bytesRead ++;
		}
		return bytesRead;
	}
	
}
