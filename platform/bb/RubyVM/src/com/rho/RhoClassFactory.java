package com.rho;

import com.rho.db.*;
import com.rho.net.*;
import java.io.IOException;

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
    
    static INetworkAccess m_NAInstance;
    public static INetworkAccess getNetworkAccess() throws IOException
    {
    	try{
	    	if ( m_NAInstance == null )
	    	{    	
		        Class wrapperClass;
		        try {
		            wrapperClass = Class.forName("com.rho.net.bb.NetworkAccess");
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
