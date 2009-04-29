package com.rho;

import com.rho.db.*;

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
}
