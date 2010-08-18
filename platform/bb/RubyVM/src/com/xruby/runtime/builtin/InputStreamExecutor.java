package com.xruby.runtime.builtin;

import com.rho.RhoClassFactory;
import com.rho.RhoRuby;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyValue;
import java.io.InputStream;
import java.io.IOException;

//import javolution.io.UTF8StreamReader;

public class InputStreamExecutor implements RubyIOExecutor {

	private InputStream m_is;
	//private boolean m_EOF = false;
	private String m_strInput;
	private int m_nPos = 0;
	
	public InputStreamExecutor(InputStream is) {
		m_is = is;
	}
	public InputStreamExecutor(String filename, String mode) {
		if ( !mode.equals("r") )
			throw new Error("Read Only!");
		
    	try {
//			m_is = RhoClassFactory.createFile().getResourceAsStream(filename.getClass(), filename);
    		m_is = RhoRuby.loadFile(filename);
		} catch (Exception e) {
			throw new Error( e.getMessage() );
		}
	}
	
	public void close() {
		try{
			if ( m_is != null )
				m_is.close();
		}catch(IOException exc){
		
		}
	}

    public InputStream getInputStream()
    {
    	return m_is;
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
				strLine = m_strInput.substring(m_nPos, nPos+chSep.length());
			else
				strLine = m_strInput.substring(m_nPos);
				
			if ( nPos > 0 )
				m_nPos = nPos + chSep.length(); 
			else
				m_nPos = m_strInput.length();
					
			return strLine;
		}catch(IOException exc){
			return null;
		}catch(Exception e){
			throw new RubyException(e.getMessage());
		}
	}

	public void print(String s) {
		throw new Error("should reach here");
	}

    //RHO_COMMENT
    /*private static char[] buffer = new char[1024];
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
	}*/
	public  byte[]  m_byteBuffer = new byte[1024];
	private final String readFully(InputStream in) throws IOException 
	{
		String strRes = "";
		synchronized (m_byteBuffer) {			
			int nRead = 0;
			do{
				nRead = in.read(m_byteBuffer);
				if (nRead>0)
				{
					String strTemp = new String(m_byteBuffer,0,nRead);
					strRes += strTemp;
				}
			}while( nRead > 0 );
		}
		
		return strRes;
	}
    
	
	public String read() {
		try{
			return readFully(m_is);
		}catch(IOException exc){
			return null;
		}
	}

	public String read(long length) 
	{
		try
		{
			byte[] buf = new byte[(int)length];
			int nRead = m_is.read(buf, 0, (int)length);
			
			return new String(buf,0,nRead);
		}catch(IOException exc){
			return null;
		}
	}

	public String read(int length, int offset) 
	{
		throw new RuntimeException("Read from offset does not supported");
	}

	public void seek(long pos) 
	{
		throw new RuntimeException("Seek does not supported");
	}

	public void truncate(int length) 
	{
		throw new RuntimeException("truncate does not supported");
	}

	public int write(String s) 
	{
		throw new RuntimeException("write does not supported");
	}

}
