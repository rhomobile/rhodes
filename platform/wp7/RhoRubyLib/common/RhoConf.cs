using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace rho.common
{
    public class RhoConf
    {
    String      m_strConfFilePath = "";
    String      m_strRhoRootPath = "";
    Hashtable<String, String> m_mapValues = new Hashtable<String, String>();
    Hashtable<String,String> m_mapChangedValues = new Hashtable<String,String>();
    Hashtable<String,Vector<String> > m_mapConflictedValues = new Hashtable<String,Vector<String> >();
    
	private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RhoConf");
    
    private static RhoConf m_Instance;
	private static String CONF_FILENAME = "apps/rhoconfig.txt";
	private static String CONF_CHANGES = ".changes";
	private static String CONF_TIMESTAMP = ".timestamp";
	private static String CONF_TIMESTAMP_PROP = "rho_conf_timestamp";
	
    public static RhoConf getInstance(){return m_Instance;}
    
	void setConfFilePath(String path){ m_strConfFilePath = path; }
	String getConfFilePath(){ return m_strConfFilePath; }
    
	void setRhoRootPath(String szRootPath){ m_strRhoRootPath = szRootPath;}
	public String getRhoRootPath(){ return m_strRhoRootPath;}
	
	Hashtable<String,Vector<String> > getConflicts(){ return m_mapConflictedValues;}
	
    private void saveToFile(String szName)
    {
    	m_mapChangedValues.put(szName, getString(szName) );
    	
        String strData = saveChangesToString();
        CRhoFile.writeStringToFile(getConfFilePath()+CONF_CHANGES, strData);
    }

    void checkConflicts()
    {
        m_mapConflictedValues.clear();
        Hashtable<String, String>.Enumerator hashEnum = m_mapChangedValues.GetEnumerator();
		while( hashEnum.MoveNext() )
		{
			String key = hashEnum.Current.Key;
			String valueChanged = hashEnum.Current.Value;
			
			if ( !m_mapValues.containsKey(key) )
				continue;
			
            String strValue = (String)m_mapValues.get(key);
            if ( strValue.compareTo(valueChanged) != 0 )
            {
                Vector<String> values = new Vector<String>();
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

        String strTimestamp = CRhoFile.readStringFromResourceFile(CONF_FILENAME+CONF_TIMESTAMP);
        
        setString(CONF_TIMESTAMP_PROP, strTimestamp, true);
        m_mapConflictedValues.clear();
    }
    
    void readChanges()
    {
        String strTimestamp = CRhoFile.readStringFromResourceFile(CONF_FILENAME+CONF_TIMESTAMP);

        if ( CRhoFile.isFileExist( getConfFilePath()+CONF_CHANGES) )
        {
            String strSettings = CRhoFile.readStringFromFile(getConfFilePath()+CONF_CHANGES);
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
    
    void loadFromString(String szSettings, Hashtable<String,String> mapValues)
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

    public void setPropertyByName(String name, String value, Hashtable<String,String> mapValues )
    {
    	mapValues.put(name,value);
    }
	
    String saveChangesToString()
    {
    	String strData = "";
        Hashtable<String, String>.Enumerator hashEnum = m_mapChangedValues.GetEnumerator();
		while( hashEnum.MoveNext() )
        {
            String key = hashEnum.Current.Key;
			String value = hashEnum.Current.Value;
			
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

        return CFilePath.join(strPath, "/");
    }
    
    public int getInt(String szName){
    	String value = (String)m_mapValues.get(szName);
    	if ( value != null && value.length() > 0 )
    		return int.Parse(value);

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
    	m_mapValues.put(szName,nVal.ToString());
    	
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
            CRhoFile.recursiveCreateDir(CRhodesApp.getRhoRootPath());
    		szRootPath = CRhodesApp.getRhoRootPath();
    	}catch(Exception ){}

    	
    	m_Instance.setConfFilePath(szRootPath + CONF_FILENAME);
    	m_Instance.setRhoRootPath(szRootPath);
    }

    public void loadConf()
    {
        m_mapValues.clear();
        m_mapChangedValues.clear();

    	loadFromResource();
        
    	readChanges();
    }

    private void loadFromResource()
    {
    	String strSettings = CRhoFile.readStringFromResourceFile(CONF_FILENAME);
		loadFromString(strSettings, m_mapValues);
   }
/*    
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
	   
		Hashtable<String,Vector<String> > mapConflicts = RhoConf.getInstance().getConflicts();
    	Enumeration enValues = mapConflicts.elements();
    	Enumeration enKeys = mapConflicts.keys();
		while (enValues.hasMoreElements()) {
			String key = (String)enKeys.nextElement();
			Vector<String> values = (Vector)enValues.nextElement();
			
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
	        LOG.ERROR("send_log failed : network error - " +  ( resp == null ? "" : resp.getRespCode() + "; Body - " + resp.getCharData()));
			
			return false;
		}
		
		return true;
   }*/
   }
}
