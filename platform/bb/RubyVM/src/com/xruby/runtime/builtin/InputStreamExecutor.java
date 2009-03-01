package com.xruby.runtime.builtin;

import java.io.OutputStream;
import java.io.PrintStream;

import com.xruby.runtime.lang.RubyValue;
import java.io.InputStream;
import java.io.IOException;

import javolution.io.UTF8StreamReader;

public class InputStreamExecutor implements RubyIOExecutor {

	private InputStream m_is;
	//private boolean m_EOF = false;
	private String m_strInput;
	private int m_nPos = 0;
	
	public InputStreamExecutor(InputStream is) {
		m_is = is;
	}
	public InputStreamExecutor(String filename, String mode) {
		if ( mode != "r" )
			throw new Error("Read Only!");
		
    	m_is = filename.getClass().getResourceAsStream(filename);
	}
	
	public void close() {
		try{
			if ( m_is != null )
				m_is.close();
		}catch(IOException exc){
		
		}
	}

	public boolean eof() {
		if ( m_strInput == null )
			return false;
		
		return m_nPos >= m_strInput.length();
	}

	public void flush() {
		throw new Error("should reach here");
	}

	public String gets(RubyValue seperator) 
	{
		try{
			if( m_strInput == null )
			{
				m_nPos = 0;
				m_strInput = readFully(m_is);
			}
			
			String strLine = "";	
			String chSep = seperator.toStr();	
		
			int nPos = m_strInput.indexOf(chSep, m_nPos );
			if ( nPos > 0 )
				strLine = m_strInput.substring(m_nPos, nPos);
			else
				strLine = m_strInput.substring(m_nPos);
				
			m_nPos = nPos + chSep.length(); 
					
			return strLine;
		}catch(IOException exc){
			return "";
		}
	}

	public void print(String s) {
		throw new Error("should reach here");
	}

    //RHO_COMMENT
    private static char[] buffer = new char[1024];
	public static final String readFully(InputStream in) throws IOException {
		int size = in.available();
		if (size < 0)
			size = 1024;
		RubyString str = ObjectFactory.createString();	
		UTF8StreamReader reader = new UTF8StreamReader();
		reader.setInput(in);
		while (true) {
			synchronized (buffer) {
				int len = reader.read(buffer);
				if (len < 0) {
					break;
				}
				str.appendChars(buffer,len);
			}
		}
		return str.toStr();
	}
	
	public String read() {
		try{
			return readFully(m_is);
		}catch(IOException exc){
			return "";
		}
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

	public int write(String s) {
		throw new Error("should reach here");
	}

}
