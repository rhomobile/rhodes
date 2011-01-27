package com.rho;

import java.util.HashMap;

public class RhoConf {
    String      m_strConfFilePath = "";
    String      m_strRhoRootPath = "";
    
    private HashMap m_mapValues = new HashMap();
    HashMap/*<String,String>*/ m_mapChangedValues = new HashMap();
    HashMap/*Ptr<String,Vector<String>* >*/ m_mapConflictedValues = new HashMap();
    
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
	
	HashMap/*Ptr<String,Vector<String>* >&*/ getConflicts(){ return m_mapConflictedValues;}
	
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
    }

    public void setInt(String szName, int nVal, boolean bSaveToFile){
    	m_mapValues.put(szName,Integer.toString(nVal));
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

    	m_Instance.setConfFilePath(szRootPath + CONF_FILENAME);
    	m_Instance.setRhoRootPath(szRootPath);
    }

    public void loadConf()
    {
        m_mapValues.clear();
        m_mapChangedValues.clear();

    	loadFromJar();
    }

    private void loadFromJar()
    {
    	setString("syncserver", "http://rhodes-store-server.heroku.com/application", false);
    }
    
}
