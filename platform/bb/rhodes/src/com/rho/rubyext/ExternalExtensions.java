package com.rho.rubyext;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

public class ExternalExtensions
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("ExternalExtensions");

	public static void registerExtensions() {
        Class wrapperClass = null;
        try {
            wrapperClass = Class.forName("com.rho.rubyext.Extensions");
        } catch (ClassNotFoundException exc) {  
        	LOG.ERROR("com.rho.rubyext.Extensions not exist !",exc);    	
        }
        if (wrapperClass != null) {
        	Runnable extensions_run = null;
        	try {
        		extensions_run = (Runnable)wrapperClass.newInstance();
        	} catch (Exception e) {
            	LOG.ERROR("com.rho.rubyext.Extensions not instantiated !",e);    	
      		
        	}
	        if (extensions_run != null ) {
	        	extensions_run.run();
	        }
	        else {
            	LOG.ERROR("com.rho.rubyext.Extensions instantiated but not supported Runnable !");    	
	        }
        }
		
	}
}