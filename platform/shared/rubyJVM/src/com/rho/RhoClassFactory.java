package com.rho;

import com.rho.db.*;
import com.rho.net.*;
import com.rho.file.*;
import java.io.IOException;

public class RhoClassFactory 
{ 
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("ClassFactory");
	
    public static SimpleFile createFile() throws Exception 
    {
        Class wrapperClass;
        try {
            wrapperClass = Class.forName("com.rho.file.Jsr75File");
        } catch (ClassNotFoundException exc) {  
        	try {
                wrapperClass = Class.forName("com.rhomobile.rhodes.AndroidFile"); //android
            } catch (ClassNotFoundException e) {
	        	LOG.ERROR_OUT("createFile - Class not found",e);    	

                throw e;
            }
        }
        
        try {
        	return (SimpleFile)wrapperClass.newInstance();
        }catch(Exception e)
        {
        	LOG.ERROR_OUT("createFile - newInstance failed",e);    	
        	
        	throw e;
        }
        
    }
    
    public static IFileAccess createFileAccess() throws Exception
    {
    	return RhoClassFactory.createRhoRubyHelper().createFileAccess();
    }
    
    public static IRAFile createRAFile() throws Exception
    {
    	return RhoClassFactory.createRhoRubyHelper().createRAFile();
    }

    public static IRAFile createFSRAFile() throws Exception
    {
    	return RhoClassFactory.createRhoRubyHelper().createFSRAFile();
    }

    public static IDBStorage createDBStorage() throws Exception
    {
    	return RhoClassFactory.createRhoRubyHelper().createDBStorage();
    }

    public static IRhoRubyHelper createRhoRubyHelper() throws Exception
    {
    	LOG.TRACE("createRhoRubyHelper");    	
    	
        Class wrapperClass;
        try {
            wrapperClass = Class.forName("com.rho.RhoRubyHelper"); //bb
        } catch (ClassNotFoundException exc) {  
        	try {
                wrapperClass = Class.forName("com.rhomobile.rhodes.RhoRubyHelper"); //android
            } catch (ClassNotFoundException e) {
	        	LOG.ERROR("createRhoRubyHelper- Class not found",e);    	
            	
                throw e;
            }
        }
        
        try{
        	return (IRhoRubyHelper)wrapperClass.newInstance();
        }catch(Exception e)
        {
        	LOG.ERROR("createRhoRubyHelper - newInstance failed",e);    	
        	
        	throw e;
        }
        	
    }

    static INetworkAccess m_NAInstance;
    public static INetworkAccess getNetworkAccess() throws IOException
    {
    	try{
	    	if ( m_NAInstance == null )
	    	{    	
	        	LOG.TRACE("getNetworkAccess");    	
	    		
		        Class wrapperClass;
		        try {
		            wrapperClass = Class.forName("com.rho.net.NetworkAccess");
		        } catch (ClassNotFoundException exc) {  
		        	try {
			            wrapperClass = Class.forName("com.rhomobile.rhodes.NetworkAccessImpl"); //android 
			        } catch (ClassNotFoundException e) {
			        	LOG.ERROR("getNetworkAccess- Class not found",e);    	
			        	
			            throw e;
			        }
		        }
		        
		        m_NAInstance = (INetworkAccess)wrapperClass.newInstance();
	    	}
    	}catch(Exception exc){
        	LOG.ERROR("getNetworkAccess - newInstance failed",exc);    	
    		
	    	throw new IOException(exc.getMessage());
	    }
	    return m_NAInstance;
    }
    
    public static NetRequest createNetRequest()
    {
    	return new NetRequest();
    }
}
