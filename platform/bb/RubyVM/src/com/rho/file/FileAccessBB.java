package com.rho.file;

import java.io.IOException;

import com.rho.RhoClassFactory;

public class FileAccessBB implements IFileAccess {
	
	private static IRAFile impl = null;

    private void InitImpl() {
    	if (impl == null) {
    		try {
    			impl = RhoClassFactory.createRAFile();
    		}
    		catch (Exception e) {
    			System.out.println("FileAccessBB:InitImpl Exception: " + e.getMessage());
    		}
    	}
    }
	
    public boolean exists(String filename) 
    {
    	InitImpl();
    	synchronized (impl) {
	    	try{
	    		impl.open(filename);
	    		return impl.exists();
	    	}
	    	catch (Exception exc) {
	    		return false;
	    	}
	    	finally {
	    		try {
					impl.close();
				} catch (Exception e) {
					System.out.println("FileAccessBB:exists Exception on close: " + e.getMessage() + ";File: " + filename);
				}
	    	}
    	}
    }
	
    public long size(String filename) {
    	InitImpl();
    	synchronized (impl) {
			try {
				impl.open(filename);
				return impl.size();
			}
			catch (Exception exc) {
				return 0;
			}
			finally {
				try {
					impl.close();
				}
				catch (Exception e) {
					System.out.println("FileAccessBB:size Exception on close: " + e.getMessage() + ";File: " + filename);
				}
			}
		}
    }
	
    public void delete(String filename)
    {
    	InitImpl();
    	synchronized (impl) {
	    	try {
	    		impl.open(filename, "rw");
	    		impl.delete();
	    	}catch(Exception exc){
	    		System.out.println("FileAccessBB:delete '" + filename + "' Exception: " + exc.getMessage());
	    	}finally{
	    		try {
					impl.close();
				} catch (IOException e) {
					System.out.println("FileAccessBB:delete Exception on close: " + e.getMessage() + ";File: " + filename);
				}
	    	}
    	}
    }

    public void renameOverwrite(String oldname, String newname) {

    	InitImpl();
    	synchronized (impl) {
	        delete(newname);
	    	try{
	        	String name = newname;
	        	int nSlash = newname.lastIndexOf('/');
	        	if ( nSlash >= 0 )
	        		name = newname.substring(nSlash+1);
	        	
	        	InitImpl();
	        	impl.open(oldname, "rw");
	        	impl.rename(name);
	    	}catch(IOException exc){
	    		System.out.println("FileAccessBB:renameOverwrite from '" + oldname + "' to '" + newname + "' Exception: " + exc.getMessage());
	    	}finally{
	    		try {
					impl.close();
				} catch (Exception e) {
					System.out.println("FileAccessBB:rename Exception on close: " + e.getMessage() + ";From '" + oldname + "' to '" + newname + "'");
				}
	    	}
    	}
    }

}
