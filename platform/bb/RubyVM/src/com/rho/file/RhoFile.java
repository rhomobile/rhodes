package com.rho.file;

import com.rho.*;

public class RhoFile 
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RhoFile");

	public static String readStringFromFile(String szFilePath)
	{
		String strRes = "";
		IRAFile oFile = null; 
			
		try
		{
			oFile = RhoClassFactory.createRAFile();
			oFile.open( szFilePath);
			
			int nSize = (int)oFile.size();
		    byte[] data = new byte[nSize];
		    int len = oFile.read(data, 0, nSize);
		    if ( len > 0 )
		    	strRes = new String(data,0,len);
			
		}catch(Exception exc)
		{
			LOG.ERROR("readStringFromFile failed.", exc);
		}finally
		{
			if ( oFile != null )
				try{ oFile.close();}catch(Exception e){}
		}
		
		return strRes;
	}
	
	public static void writeStringToFile(String szFilePath, String strData)throws Exception
	{
		IRAFile file = null;
		try{
			file = RhoClassFactory.createRAFile();
			file.open(szFilePath, "w");
	        byte[] buf = strData.getBytes();
	        file.write(buf, 0, buf.length);
	    }finally
	    {
	    	if (file!=null)
	    		try{ file.close(); }catch(Exception exc){}
	    }
	}
	
	public static String readStringFromJarFile(String szFilePath, Object root)
	{
		java.io.InputStream fstream = null;
		String strFile = "";		
		try {
			fstream = RhoClassFactory.createFile().getResourceAsStream(root.getClass(),
				 "/" + szFilePath);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		if ( fstream == null )
			return strFile;
		 
		try{
			byte[] data = new byte[fstream.available()];
			int len = fstream.read(data);
			if ( len == 0 )
				return strFile;
			
			strFile = new String(data,0,len);
		}catch(java.io.IOException exc){
		}
		
		try{fstream.close();}catch(java.io.IOException exc){}
		
		return strFile;
	}
	
	public static boolean isFileExist(String szFilePath)
	{
		IRAFile oFile = null; 
		
		try
		{
			oFile = RhoClassFactory.createRAFile();
			oFile.open(szFilePath);
			return oFile.exists();
		}catch(Exception exc)
		{
			return false;
		}finally
		{
			if ( oFile != null )
				try{ oFile.close();}catch(Exception e){}
		}
	}
}
