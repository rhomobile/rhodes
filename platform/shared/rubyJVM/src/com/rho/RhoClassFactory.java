package com.rho;

import com.rho.db.*;
import com.rho.net.*;
import java.io.IOException;
import com.rho.location.IGeoLocationImpl;

public class RhoClassFactory 
{ 
    public static SimpleFile createFile() throws Exception 
    {
        Class wrapperClass;
        try {
            wrapperClass = Class.forName("com.rho.Jsr75File");
        } catch (ClassNotFoundException exc) {  
            throw exc;
        }
        
        return (SimpleFile)wrapperClass.newInstance();
    }
    
    public static IDBStorage createDBStorage() throws Exception
    {
        Class wrapperClass;
        try {
            wrapperClass = Class.forName("com.rho.db.HsqlDBStorage");
        } catch (ClassNotFoundException exc) {  
            throw exc;
        }
        
        return (IDBStorage)wrapperClass.newInstance();
    }

    public static IRhoRubyHelper createRhoRubyHelper() throws Exception
    {
        Class wrapperClass;
        try {
            wrapperClass = Class.forName("com.rho.RhoRubyHelper");
        } catch (ClassNotFoundException exc) {  
            throw exc;
        }
        
        return (IRhoRubyHelper)wrapperClass.newInstance();
    }

    public static IGeoLocationImpl createGeoLocationImpl() throws Exception
    {
        Class wrapperClass;
        try {
            wrapperClass = Class.forName("com.rho.location.GeoLocationImpl");
        } catch (ClassNotFoundException exc) {  
            throw exc;
        }
        
        return (IGeoLocationImpl)wrapperClass.newInstance();
    }
    
    static INetworkAccess m_NAInstance;
    public static INetworkAccess getNetworkAccess() throws IOException
    {
    	try{
	    	if ( m_NAInstance == null )
	    	{    	
		        Class wrapperClass;
		        try {
		            wrapperClass = Class.forName("com.rho.net.NetworkAccess");
		        } catch (ClassNotFoundException exc) {  
		            throw exc;
		        }
		        
		        m_NAInstance = (INetworkAccess)wrapperClass.newInstance();
	    	}
    	}catch(Exception exc){
	    	throw new IOException(exc.getMessage());
	    }
	    return m_NAInstance;
    }
    
}
