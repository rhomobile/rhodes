package com.rho;

import java.io.IOException;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import com.rho.net.NetResponse;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;
import com.rho.net.NetRequest.MultipartItem;
import com.rho.file.SimpleFile;
import com.rho.file.RhoFile;

public class RhoConf {
    String      m_strConfFilePath = "";
    String      m_strRhoRootPath = "";
    private Hashtable m_mapValues = new Hashtable();
    Hashtable/*<String,String>*/ m_mapChangedValues = new Hashtable();
    Hashtable/*Ptr<String,Vector<String>* >*/ m_mapConflictedValues = new Hashtable();
    
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RhoConf");
    
    private static RhoConf m_Instance;
	private static final String CONF_FILENAME = "apps/rhoconfig.txt";
	private static final String CONF_CHANGES = ".changes";
	private static final String CONF_TIMESTAMP = ".timestamp";
	private static final String CONF_TIMESTAMP_PROP = "rho_conf_timestamp";
	
    public static RhoConf getInstance(){return m_Instance;}
    
	void setConfFilePath(String path){ m_strConfFilePath = path; }
	String getConfFilePath(){ return m_strConfFilePath; }
    
	void setRhoRootPath(String szRootPath){ m_strRhoRootPath = szRootPath;}
	public String getRhoRootPath(){ return m_strRhoRootPath;}
	
	Hashtable/*Ptr<String,Vector<String>* >&*/ getConflicts(){ return m_mapConflictedValues;}
	
    private void saveToFile(String szName)
    {
    	m_mapChangedValues.put(szName, getString(szName) );
    	
        String strData = saveChangesToString();
    	SimpleFile oFile = null;

    	try{
	        oFile = RhoClassFactory.createFile();
	        oFile.delete(getConfFilePath()+CONF_CHANGES);
	        
        	oFile.open( getConfFilePath()+CONF_CHANGES, false, false);
	        oFile.write( 0, strData.getBytes() );
	        oFile.close();
    	}catch(Exception exc){
    		if ( oFile != null )
    			try{ oFile.close(); }catch(IOException exc2){}
    	}
        
    }

    void checkConflicts()
    {
        m_mapConflictedValues.clear();
        
    	Enumeration enValues = m_mapChangedValues.elements();
    	Enumeration enKeys = m_mapChangedValues.keys();
		while (enValues.hasMoreElements()) 
		{
			String key = (String)enKeys.nextElement();
			String valueChanged = (String)enValues.nextElement();
			
			if ( !m_mapValues.containsKey(key) )
				continue;
			
            String strValue = (String)m_mapValues.get(key);
            if ( strValue.compareTo(valueChanged) != 0 )
            {
                Vector/*<String>* */ values = new Vector/*<String>*/();
                values.addElement(valueChanged);
                values.addElement(strValue);
                m_mapConflictedValues.put(key, values);
            }
        }
    }

    public void conflictsResolved()
    {
        if (m_mapConflictedValues.size() == 0 )
            return;

        String strTimestamp = RhoFile.readStringFromJarFile(CONF_FILENAME+CONF_TIMESTAMP, this);
        
        setString(CONF_TIMESTAMP_PROP, strTimestamp, true);
        m_mapConflictedValues.clear();
    }
    
    void readChanges()
    {
        String strTimestamp = RhoFile.readStringFromJarFile(CONF_FILENAME+CONF_TIMESTAMP, this);

        if ( RhoFile.isFileExist( getConfFilePath()+CONF_CHANGES) )
        {
            String strSettings = RhoFile.readStringFromFile(getConfFilePath()+CONF_CHANGES);
            loadFromString( strSettings, m_mapChangedValues );

            String strOldTimestamp = "";
            if ( m_mapChangedValues.containsKey(CONF_TIMESTAMP_PROP) )
                strOldTimestamp = (String)m_mapChangedValues.get(CONF_TIMESTAMP_PROP);
            
            if ( strTimestamp.compareTo(strOldTimestamp) != 0 )
                checkConflicts();
            
            loadFromString( strSettings, m_mapValues );            
        }else
        {
            m_mapChangedValues.put(CONF_TIMESTAMP_PROP,strTimestamp);
        }
    }
    
    void loadFromString(String szSettings, Hashtable/*<String,String>&*/ mapValues)
    {
		Tokenizer stringtokenizer = new Tokenizer(szSettings, "\n");
		while (stringtokenizer.hasMoreTokens()) {
			String tok = stringtokenizer.nextToken();
			tok = tok.trim();
			if (tok.length() == 0) {
				continue;
			}
			
			if ( tok.length() > 0 && tok.charAt(0) == '#' )
				continue;
			
			int i = tok.indexOf('=');
			String name;
			String value;
			if (i > 0) {
				name = tok.substring(0, i);
				value = tok.substring(i + 1);
			} else {
				name = tok;
				value = "";
			}
			name = name.trim();
			value = value.trim();
			
			if (value.startsWith("\'") && value.endsWith("\'")) {
				value = value.substring(1,value.length()-1);
			}
				
			setPropertyByName(name,value,mapValues);
		}
	}

    public void setPropertyByName(String name, String value, Hashtable/*<String,String>&*/ mapValues )
    {
    	mapValues.put(name,value);
    }
	
    String saveChangesToString()
    {
    	String strData = "";
    	Enumeration enValues = m_mapChangedValues.elements();
    	Enumeration enKeys = m_mapChangedValues.keys();
		while (enValues.hasMoreElements()) {
			String key = (String)enKeys.nextElement();
			String value = (String)enValues.nextElement();
			
            strData += key;
            strData += "=\'";
            strData += value;
            strData += "\'\n";
		}
		
    	return strData;
    }

    public String getString(String szName){
    	String value = (String)m_mapValues.get(szName);
    	if ( value != null )
    		return value;

        return "";
    }

    public String getPath(String szName)
    {
        String strPath = getString(szName);
        if ( strPath.length() == 0 )
            return strPath;

        return FilePath.join(strPath, "/");
    }
    
    public int getInt(String szName){
    	String value = (String)m_mapValues.get(szName);
    	if ( value != null && value.length() > 0 )
    		return Integer.parseInt(value);

        return 0;
    }

    public boolean  getBool(String szName){
        return getInt(szName) == 0 ? false : true;
    }

    public void setString(String szName, String str, boolean bSaveToFile){
    	m_mapValues.put(szName,str);
    	
    	if ( bSaveToFile )
    		saveToFile(szName);    	
    }

    public void setInt(String szName, int nVal, boolean bSaveToFile){
    	m_mapValues.put(szName,Integer.toString(nVal));
    	
    	if ( bSaveToFile )
    		saveToFile(szName);    	
    }

    public void setBool(String szName, boolean bVal, boolean bSaveToFile)
    {
        setInt(szName, bVal ? 1 : 0, bSaveToFile );
    }
    
    public boolean isExist(String szName){
    	return m_mapValues.containsKey(szName);
    }
    
    public static void InitRhoConf(){
        m_Instance = new RhoConf();
    	
    	String szRootPath = "";
    	try{
    		szRootPath = RhoClassFactory.createFile().getDirPath("");
    	}catch(Exception exc){}

    	
    	m_Instance.setConfFilePath(szRootPath + CONF_FILENAME);
    	m_Instance.setRhoRootPath(szRootPath);
    }

    public void loadConf()
    {
        m_mapValues.clear();
        m_mapChangedValues.clear();

    	loadFromJar();
        
    	readChanges();

    	loadFromJad();
    }

    private void loadFromJar()
    {
    	String strSettings = RhoFile.readStringFromJarFile(CONF_FILENAME, this);
		loadFromString(strSettings, m_mapValues);
   }
    
   void loadFromJad()
   {
		try{
			IRhoRubyHelper systemInfo = RhoClassFactory.createRhoRubyHelper();
			Enumeration keysEnum = m_mapValues.keys();
			while( keysEnum.hasMoreElements() )
			{
				String name = (String)keysEnum.nextElement(); 
				String strValue = systemInfo.getAppProperty(name);
				if ( strValue != null && strValue.length() > 0 )
					m_mapValues.put(name, strValue);
			}
		}catch(Exception exc){
			 
		}
   }
   
   private static long readToBuffer(java.io.InputStream is, byte[] buf, StringBuffer res, long limit)throws Exception
   {
	   long nTotal = 0;
	   while(nTotal < limit)
	   {
			long nRead = is.read(buf);
			if ( nRead < 0 )
				break;
			if ( nTotal + nRead > limit )
				nRead = limit - nTotal; 
					
			nTotal += nRead; 
			res.append(new String(buf,0,(int)nRead));
	   }
	   
	   return nTotal;
   }
   
   public RubyHash getRubyConflicts()
   {
	   RubyHash hashConflicts = ObjectFactory.createHash();
	   
		Hashtable/*Ptr<String,Vector<String>* >&*/ mapConflicts = RhoConf.getInstance().getConflicts();
    	Enumeration enValues = mapConflicts.elements();
    	Enumeration enKeys = mapConflicts.keys();
		while (enValues.hasMoreElements()) {
			String key = (String)enKeys.nextElement();
			Vector/*<String>&*/ values = (Vector)enValues.nextElement();
			
		    RubyArray arValues = new RubyArray(values.size());
		    for( int i = 0; i < (int)values.size(); i++)
		    	arValues.add(ObjectFactory.createString((String)values.elementAt(i)));
		
		    hashConflicts.add(ObjectFactory.createString(key), arValues);
		}
	   
	   return hashConflicts;
   }
   
   static RubyString getLogText_ruby(long limit)throws Exception
	{
		StringBuffer res = new StringBuffer(); 
    	SimpleFile oFile = null;
    	RhoLogConf logConf = RhoLogger.getLogConf();
	    boolean bOldSaveToFile = logConf.isLogToFile();
	    logConf.setLogToFile(false);
	    java.io.InputStream is = null;
    	try{
	        oFile = RhoClassFactory.createFile();
	        oFile.open( logConf.getLogFilePath(), true, false);
	        
	        if ( oFile.isOpened() )
	        {
	            long nFileSize = oFile.length();
	            long nPos = logConf.getLogTextPos();
	            long nMaxSize = nFileSize > nPos ? nFileSize : nPos; 
	            if ( limit <= 0 || limit > nMaxSize)
	                limit = nMaxSize;

	            res = new StringBuffer((int)limit);
	            is = oFile.getInputStream();
	            byte[] buf = new byte[8096];
	            if ( limit <= nPos )
	            {
	                is.skip(nPos-limit);
	                readToBuffer(is, buf, res, limit);
	            }else
	            {
	            	is.skip(nFileSize-(limit-nPos));
	                long nRead = readToBuffer(is, buf, res, limit);
	                
	                oFile.close();
	                oFile.open( logConf.getLogFilePath(), true, false);
	                is = oFile.getInputStream();
	                readToBuffer(is, buf, res, limit-nRead);
	            }
	            
	        }
	        
    	}finally
    	{
    		if ( oFile != null )
    			try{ oFile.close(); }catch(IOException exc2){}
    		
    		logConf.setLogToFile(bOldSaveToFile);
    	}
		
		return ObjectFactory.createString(res);
	}
   
   public static boolean sendLog()
   {
		com.rho.net.NetRequest nq = RhoClassFactory.createNetRequest();
		String strDevicePin = "";
		String strClientID = "";
		
		try{
			IRhoRubyHelper sysInfo = RhoClassFactory.createRhoRubyHelper();
			strDevicePin = sysInfo.getDeviceId();
		}catch(Exception exc)
		{
			LOG.ERROR("send_log:getDeviceId failed", exc);
		}

		try{
			strClientID = com.rho.sync.SyncThread.getSyncEngine().readClientID();
		}catch(Exception exc)
		{
			LOG.ERROR("send_log:readClientID failed", exc);
		}
		
	    String strLogUrl = RhoConf.getInstance().getPath("logserver");
	    if ( strLogUrl.length() == 0 )
	        strLogUrl = RhoConf.getInstance().getPath("syncserver");

		String strQuery = strLogUrl + "client_log?" +
		    "client_id=" + strClientID + "&device_pin=" + strDevicePin + "&log_name=" + RhoConf.getInstance().getString("logname");
		
	    MultipartItem oItem = new MultipartItem();
	    oItem.m_strFilePath = RhoLogger.getLogConf().getLogFilePath();
	    oItem.m_strContentType = "application/octet-stream";
		
	    boolean bOldSaveToFile = RhoLogger.getLogConf().isLogToFile();
	    RhoLogger.getLogConf().setLogToFile(false);
		
		NetResponse resp = null;
		try{
			resp = nq.pushMultipartData(strQuery, oItem, com.rho.sync.SyncThread.getSyncEngine(), null );
		}catch(Exception exc)
		{
			LOG.ERROR("send_log failed.", exc);
		}
		RhoLogger.getLogConf().setLogToFile(bOldSaveToFile);
		
		if ( resp == null || !resp.isOK() )
		{
	        LOG.ERROR("send_log failed : network error - " + resp.getRespCode() + "; Body - " + resp.getCharData());
			
			return false;
		}
		
		return true;
   }
   
   public static void initMethods(RubyClass klass) {
	   klass.getSingletonClass().defineMethod("set_property_by_name", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
				try {
					RhoConf.getInstance().setString(arg0.toString(), arg1.toString(), true);
					
					RhoLogger.getLogConf().loadFromConf(RhoConf.getInstance());
				} catch (Exception e) {
					LOG.ERROR("set_property_by_name failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});

	   klass.getSingletonClass().defineMethod("get_property_by_name", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				try {
					String res = RhoConf.getInstance().getString(arg0.toString());
					return ObjectFactory.createString(res);
				} catch (Exception e) {
					LOG.ERROR("get_property_by_name failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
	   
	   klass.getSingletonClass().defineMethod("is_property_exists", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				try {
					boolean bRes = RhoConf.getInstance().isExist(arg0.toString());
					
					return ObjectFactory.createBoolean(bRes);
				} catch (Exception e) {
					LOG.ERROR("is_property_exists failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
	   
		klass.getSingletonClass().defineMethod("show_log",	new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					IRhoRubyHelper systemInfo = RhoClassFactory.createRhoRubyHelper();
					systemInfo.showLog();
					return RubyConstant.QNIL;
				}catch(Exception e)
				{
					LOG.ERROR("show_log failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			    
			}
		});

		klass.getSingletonClass().defineMethod("send_log",	new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					sendLog();
					return RubyConstant.QNIL;
				}catch(Exception e)
				{
					LOG.ERROR("send_log failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			    
			}
		});

		klass.getSingletonClass().defineMethod("clean_log",	new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					RhoLogger.clearLog();
					return RubyConstant.QNIL;
				}catch(Exception e)
				{
					LOG.ERROR("clean_log failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			    
			}
		});

		klass.getSingletonClass().defineMethod("read_log",	new RubyNoOrOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					return getLogText_ruby(0);
				}catch(Exception e)
				{
					LOG.ERROR("read_log failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			    
			}
			
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block) {
				try{
					return getLogText_ruby(arg.toInt());
				}catch(Exception e)
				{
					LOG.ERROR("read_log failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			    
			}
		});
		
	}
}
