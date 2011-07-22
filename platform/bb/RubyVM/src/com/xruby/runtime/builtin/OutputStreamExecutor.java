package com.xruby.runtime.builtin;

import java.io.InputStream;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.lang.RubyValue;

public class OutputStreamExecutor implements RubyIOExecutor {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("APP");
	
	//private PrintStream ps;
	boolean m_bError = false;
	
	public OutputStreamExecutor(boolean bError) {
		m_bError = bError;
		/*if (os instanceof PrintStream) {
			this.ps = (PrintStream)os;
		} else {
			this.ps = new PrintStream(os);
		}*/
	}

    public InputStream getInputStream()
    {
    	return null;
    }
	
	public int write(String s) {
//		this.ps.print(s);
		if ( m_bError )
			LOG.ERROR(s);
		else
			LOG.INFO(s);
		
		return s.length();
	}
	
	public void print(String s) {
		write(s);
	}
	
	public void close() {
		//this.ps.close();
	}

	public boolean eof() {
		return true;
		//return this.eof();
	}

	public void flush() {
		//this.ps.flush();
	}

	public String gets(RubyValue seperator) {
		throw new Error("should reach here");
	}

	public String read() {
		throw new Error("should reach here");
	}

	public String read(long length) {
		throw new Error("should reach here");
	}

	public String read(int length, int offset) {
		throw new Error("should reach here");
	}

	public void seek(long pos) {
		throw new Error("should reach here");
	}

	public void truncate(int length) {
		throw new Error("should reach here");
	}
}
