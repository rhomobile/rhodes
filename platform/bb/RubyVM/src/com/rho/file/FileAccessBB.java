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
