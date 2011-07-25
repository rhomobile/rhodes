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
	
	public static void deleteFile(String szFilePath)
	{
		try{
			RhoClassFactory.createFile().delete(szFilePath);
		}catch(Exception exc)
		{
			LOG.ERROR("deleteFile failed.", exc);			
		}
	}
}
