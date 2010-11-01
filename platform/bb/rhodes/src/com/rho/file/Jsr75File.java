package com.rho.file;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import javax.microedition.io.*;
import javax.microedition.io.file.*;
import java.util.Vector;

import com.rho.*;

public class Jsr75File implements SimpleFile 
{
    private FileConnection fconn;
    private InputStream    in;
    private OutputStream   out;
    private long           inPos;
    private long           outPos = 0;
    //private long           currPos = 0;
    private long           fileSize = 0;
    private boolean        noFlush;
    private boolean        m_bOpened = false;
    
    private static final int ZERO_BUF_SIZE = 4096;
    
    private static Vector m_arRoots;
    private static String m_strRhoPath;
    
	static private void log(String txt) {
		System.out.println("FileSystem: " + txt);
	}

	static void recursiveCreateDir( String strPath  )throws IOException
	{
		int nSlash = strPath.indexOf('/', getRhoPath().length() );
		while (nSlash >=0 )
		{
			String strDir = strPath.substring(0, nSlash+1);
			createDir(strDir);
			nSlash = strPath.indexOf('/', nSlash+1 );
		}
	}
	
	static void createDir( String strDir  )throws IOException
	{
		log("Dir:"+strDir);
		FileConnection fdir = null;
		try{
			fdir = (FileConnection)Connector.open(strDir);//,Connector.READ_WRITE);
			if ( fdir == null )
			{
				log("Dir Connection opened failed : null.");
				throw new RuntimeException("Dir Connection opened failed : null.");
			}
			log("Dir Connection opened.");
	        // If no exception is thrown, then the URI is valid, but the file may or may not exist.
	        if (!fdir.exists()) { 
	        	log("Create directory.");
	        	fdir.mkdir();  // create the dir if it doesn't exist
	        }else{
	        	log("Dir exists.");	        	
	        }
		}catch(IOException exc){
			log("Create directory failed." + exc.getMessage());
			throw exc;
		}finally{
			if ( fdir != null )
				fdir.close();
		}
	}

    static private Vector getRoots(){
    	if ( m_arRoots != null )
    		return m_arRoots;
    	
    	m_arRoots = new Vector();
    	Enumeration e = FileSystemRegistry.listRoots();
        while (e.hasMoreElements()) {
        	String strRoot = (String)e.nextElement();
        	log("Root: " + strRoot);
        	
        	m_arRoots.addElement(strRoot);
        }    	
        
        return m_arRoots;
    }
	
    static private String findRoot(String strRoot){
    	Vector arRoots = getRoots();
    	for( int i = 0; i < arRoots.size(); i++ ){
    		String strRoot1 = (String)arRoots.elementAt(i);
        	if ( strRoot1.equalsIgnoreCase(strRoot) ){
        		return strRoot1;
        	}
    	}
    	
    	return null;
    }
    
    static String makeRootPath(){
    	//SoftVersion ver = Version.getSoftVersion();    	
		
    	String strRoot = findRoot("SDCard/");
    	
    	if ( strRoot != null )
    		return strRoot;
    	/*
    	if ( ver.nMajor == 0 )//emulator
			strRoot = findRoot("store/");
		else if ( ver.nMajor >= 4 && ver.nMinor >= 3 )
			strRoot = findRoot("store/");*/
    	
    	strRoot = findRoot("store/");
    	return strRoot;
    }
    
	//http://supportforums.blackberry.com/rim/board/message?board.id=java_dev&thread.id=6553            	
    public static String getRhoPath() throws IOException{
    	if ( m_strRhoPath != null )
    		return m_strRhoPath;
    	
    	String strRoot = makeRootPath();
    	if ( strRoot == null )
    		throw new IOException("Could not find storage");
    	
    	m_strRhoPath = "file:///" + strRoot;
    	
    	if ( strRoot.equalsIgnoreCase("store/") )
    		m_strRhoPath += "home/user/";
    	
    	m_strRhoPath += "Rho/";
    	createDir( m_strRhoPath );

    	RhoRubyHelper helper = new RhoRubyHelper();
    	String appName = helper.getAppProperty("MIDlet-Name");
    	if ( appName == null || appName.length() == 0 )
    		appName = helper.getModuleName();
    	if ( appName == null || appName.length() == 0 )
    		appName = "rhodesApp";
    	
    	m_strRhoPath += appName + "/";
    	createDir( m_strRhoPath );
    	
    	return m_strRhoPath;
    }

    public void copyJarFileToMemory(String strFileName, InputStream jarStream)throws IOException
    {
    	//create target directories
    	String strTargetPath = FilePath.join(getRhoPath(), strFileName);
    	recursiveCreateDir(strTargetPath);
    	
    	FileConnection fMem = null;
    	OutputStream out = null;
    	
    	try{
	    	fMem = (FileConnection)Connector.open(strTargetPath,Connector.READ_WRITE);
			if ( !fMem.exists() )
				fMem.create();
	    	
    		int nSize = jarStream.available();
    		if ( nSize > 0 )
    		{
    			out = fMem.openOutputStream();
	    		byte[] buf = new byte[nSize];
	    		jarStream.read(buf);
	    		out.write(buf);
    		}			
    	}finally{
			if ( out != null )
				out.close();
			if ( fMem != null )
				fMem.close();
    	}
    }
    
    public static void copyRhoFileFromDeviceMemory(String strFileName)
    {
    	InputStream in = null;
    	OutputStream out = null;
    	FileConnection fSDCard = null;
    	FileConnection fMem = null;
		try{
	    	String strSDCardPath = getRhoPath() + strFileName;
	    	int nSDcardPos = strSDCardPath.indexOf("SDCard"); 
	    	String strMemoryPath = strSDCardPath.substring(0,nSDcardPos) + "store/home/user" + 
	    		strSDCardPath.substring(nSDcardPos+6);
	    	strSDCardPath += "_mem";
	    		
	    	fMem = (FileConnection)Connector.open(strMemoryPath);
    		in = fMem.openInputStream();
    		
    		fSDCard = (FileConnection)Connector.open(strSDCardPath,Connector.READ_WRITE);
    		if ( !fSDCard.exists() )
    			fSDCard.create();
    		out = fSDCard.openOutputStream();
    		int nSize = (int)fMem.fileSize();
    		byte[] buf = new byte[nSize];
    		in.read(buf);
    		out.write(buf);
    		
		}catch(IOException exc){
			log("copyRhoFileFromDeviceMemory failed." + exc.getMessage());
		}finally{
			try{
				if ( in != null )
					in.close();
				if ( out != null )
					out.close();
				if ( fMem != null )
					fMem.close();
				if ( fSDCard != null )
					fSDCard.close();
				
			}catch(Exception exc){
				log("copyRhoFileFromDeviceMemory failed." + exc.getMessage());
			}
					
		}
    	
    }
    
    public static boolean isRhoFolderExist()
    {
    	String strSdCardPath = "file:///SDCard/Rho/";
    	String strMemoryPath = "file:///store/home/user/Rho/";
    	
		FileConnection fdir = null;
		try{
			fdir = (FileConnection)Connector.open(strSdCardPath);
			if ( fdir != null && fdir.exists() )
				return true;
			
			if (fdir != null)
				fdir.close();
			
			fdir = (FileConnection)Connector.open(strMemoryPath);
			if ( fdir != null && fdir.exists() )
				return true;
		}catch(IOException exc){
			log("isRhoFolderExist failed." + exc.getMessage());
		}finally{
			if ( fdir != null )
				try{ fdir.close(); }catch(Exception exc){}
		}

		return false;
    }
    
    public static boolean isSDCardExist()
    {
    	String strSdCardPath = "file:///SDCard/";
    	
		FileConnection fdir = null;
		try{
			fdir = (FileConnection)Connector.open(strSdCardPath);
			if ( fdir != null && fdir.exists() )
				return true;
		}catch(IOException exc){
			log("isSDCardExist failed." + exc.getMessage());
		}finally{
			if ( fdir != null )
				try{ fdir.close(); }catch(Exception exc){}
		}

		return false;
    }
    
    public String getDirPath(String strDir) throws IOException{
    	String strRoot = getRhoPath();
    	if ( strRoot == null )
    		throw new IOException("Could not find storage");
    	
    	if ( strDir != null && strDir.length() > 0 )
    	{
    		String strDirPath = strDir.startsWith(strRoot) ? strDir : FilePath.join(strRoot, strDir);
    		strDirPath = FilePath.join(strDirPath, "/");
    		createDir( strDirPath );
    		return strDirPath;
    	}
    	
    	return strRoot;
    }
    
    public static void listFolder(String uri, Vector result) {
    	FileConnection fc = null;
    	try {
    		fc = (FileConnection)Connector.open(uri,Connector.READ);
    		Enumeration enumFiles = fc.list("*",true);
    		while ( enumFiles.hasMoreElements() ) {
    			result.addElement(uri+"/"+enumFiles.nextElement());
    		}   		
    	} catch (IOException x) { 
        	log("Exception in listFolder: " + x.getMessage());
        } finally {
        	if (fc !=null) {
        		try {
        			fc.close();
        		} catch(Exception exc) {}
        	}
        }
    }
    
    private static void deleteFilesInFolder(FileConnection fcFolder){
    	
    	try{
			Enumeration enumFiles = fcFolder.list();
			
	        while ( enumFiles.hasMoreElements() ) {
	            String fileName = (String)enumFiles.nextElement();
	            String fullFileName = fcFolder.getURL() + fileName;
	            	
	            try{ 
					FileConnection fc2 = (FileConnection) Connector.open(fullFileName);
					if ( fc2.isDirectory() )
						deleteFilesInFolder(fc2);
					
					try{fc2.delete();}finally{ fc2.close();};
				}catch( IOException exc) {
					log("deleteFilesInFolder exception: " + exc.getMessage());
	        	}
	        }
    	}catch(IOException exc){
    		log(exc.getMessage());
    	}
    }
    
    public void delete(String path) 
    {
        String url = path;
        if (!url.startsWith("file:")) { 
            if (url.startsWith("/")) { 
                url = "file:///" + path;
            } else { 
           
            	try{
	            	String strRhoPath = getRhoPath();
	            	url = strRhoPath + path;
            	} catch (IOException x) { 
                 	log("getRhoPath exception: " + x.getMessage());
                     //throw new IOException(StorageError.FILE_ACCESS_ERROR, x);
                 }              	
            }
        }
        
        FileConnection fc = null;
        try { 
        	fc = (FileConnection)Connector.open(url,Connector.READ_WRITE);
			if ( fc.isDirectory() )
				deleteFilesInFolder(fc);
        	
        	fc.delete();
        } catch (IOException x) { 
        	log("delete Exception: " + x.getMessage());
            //throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        }finally{
        	if (fc !=null)
        		try{
        			fc.close();
        		}catch(IOException exc){
        			
        		}
        }
    }

    public void renameOverwrite(String oldname, String newname)
    {
        delete(newname);

        FileConnection fconn = null;
    	try{
        	fconn = (FileConnection)Connector.open(oldname, Connector.READ_WRITE);
        	
        	String name = newname;
        	int nSlash = newname.lastIndexOf('/');
        	if ( nSlash >= 0 )
        		name = newname.substring(nSlash+1);
        	
        	if ( fconn.exists() ){
        		fconn.rename(name);
        	}
    	}catch(IOException exc){
    		log("renameOverwrite from '" + oldname + "' to '" + newname + "' Exception: " + exc.getMessage());
    	}finally{
    		if ( fconn != null )
    			try{fconn.close();}catch(IOException exc){
    				log("renameOverwrite close Exception: " + exc.getMessage());
			    }
    	}
    }
   
    public boolean isOpened(){
    	return m_bOpened;
    }

    public void open(String path, boolean readOnly, boolean noFlush)throws IOException 
    {
        String url = path;
        this.noFlush = noFlush;
        log("Open file:" + path);
        if (!url.startsWith("file:")) { 
            /*if (url.startsWith("/")) { 
                url = "file:///" + path;
            } else {*/ 
           
            	try{
	            	url = FilePath.join(getRhoPath(), path);
	            	
            	} catch (IOException x) { 
                 	log("getRhoPath Exception: " + x.getMessage());
                    throw x;
                 }              	
           // }
        }
        
        try { 
            if (fconn == null) {
                fconn = (FileConnection)Connector.open(url,Connector.READ_WRITE);
                // If no exception is thrown, then the URI is valid, but the file may or may not exist.
                if (!fconn.exists() && !readOnly ) { 
                    fconn.create();  // create the file if it doesn't exist
                }
            }
            if ( fconn.exists() )
            {
            	fileSize = fconn.fileSize();
            	m_bOpened = true;
	            if (!readOnly) {
	                //fconn.setWritable(true);	                    
	            	
	                out = fconn.openOutputStream();
	                outPos = 0;
	            }
            }else
            {
            	fileSize = 0;
            	m_bOpened = false;
            	
            }
        } catch (IOException x) { 
        	log("Exception: " + x.getMessage());
            throw x;
        }        
        in = null;
        inPos = 0;
    }

    public String readString()throws IOException{
    	if ( !isOpened() || fileSize == 0 )
    		return "";
    	
	    byte[] data = new byte[(int)fileSize];
	    int len = read(0,data);
	    if ( len == 0 )
	        return "";

	    return new String(data,0,len);
    }
    
    public int read(long pos, byte[] b)throws IOException
    {
        int len = b.length;

        if (pos >= fileSize) { 
            return 0;
        }

        try {
            if (in == null || inPos > pos) { 
                sync(); 
                if (in != null) { 
                    in.close();
                }
                in = fconn.openInputStream();
                inPos = in.skip(pos);
            } else if (inPos < pos) { 
                inPos += in.skip(pos - inPos);
            }
            if (inPos != pos) { 
                return 0;
            }        
            int off = 0;
            while (len > 0) { 
                int rc = in.read(b, off, len);
                if (rc > 0) { 
                    inPos += rc;
                    off += rc;
                    len -= rc;
                } else { 
                    break;
                }
            }
            return off;
        } catch(IOException x) { 
            throw x;
        } 
    }

    public void truncate(int nSize)throws IOException{
    	if ( fconn != null ){
    		fconn.truncate(nSize);
    		fileSize = nSize;
    	}
    }
    
    public OutputStream getOutStream(){ return out; }
    
    public OutputStream getOutStreamEx(long pos)throws IOException
    { 
        if (outPos != pos) {                         
            out.close();
            out = fconn.openOutputStream(pos);
            if (pos > fileSize) { 
                byte[] zeroBuf = new byte[ZERO_BUF_SIZE];
                do { 
                    int size = pos - fileSize > ZERO_BUF_SIZE ? ZERO_BUF_SIZE : (int)(pos - fileSize);
                    out.write(zeroBuf, 0, size);
                    fileSize += size;
                    
                    //BB
                    //fconn.truncate(fileSize);
                } while (pos != fileSize);
            }
            outPos = pos;
        }
    	
    	return out; 
    }
    
    public InputStream getInputStream()throws IOException{
    	if ( in  == null )
    		in = fconn.openInputStream();
    	
    	return in; 
   }
    
    public void write(long pos, byte[] b)throws IOException
    {
        int len = b.length;
        if (out == null) { 
            throw new IOException("Illegal mode");
        }
        int nTry = 0;
        while (nTry <= 1)
        {
	        try 
	        {
	        	getOutStreamEx(pos);
	        	
	            out.write(b, 0, len);
	            outPos += len;
	            if (outPos > fileSize) { 
	                fileSize = outPos;
	            }
	            //BB
	            //fconn.truncate(fileSize);
	            if (in != null) { 
	                in.close();
	                in = null;
	            }
	            break;
	        }catch(IOException exc){
	        	nTry++;
	        	if ( nTry > 1 )
	        		throw exc;
	        	else{
	                outPos = -pos;
	        	}
	        		
	        }
        }
    }

    public long length() { 
        return fileSize;
    }

    public void close() throws IOException{ 
        try {
        	m_bOpened = false;
            if (in != null) { 
                in.close();
                in = null;
            }
            if (out != null) { 
                out.close();
                out = null;
            }
            
            if ( fconn != null )
            {
	        	log("File close: " + fconn.getName());
	            
	            fconn.close();
	            fconn = null;
            }
        } catch(IOException x) { 
            throw x;
        }
    }
    
    public void sync() throws IOException{
        if (!noFlush && out != null) { 
            try { 
                out.flush();
            } catch(IOException x) { 
                throw x;
            }
        }
    }

    public InputStream getResourceAsStream(Class fromClass, String path){
	 if ( fromClass == null )
          return null;

       return fromClass.getResourceAsStream(path);
    }
}
