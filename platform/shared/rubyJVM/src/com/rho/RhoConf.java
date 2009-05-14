package com.rho;

import java.util.Enumeration;
import java.io.IOException;

public class RhoConf {
    String      m_strConfFilePath = "";
    String      m_strRhoRootPath = "";
    private java.util.Hashtable m_mapValues = new java.util.Hashtable();
    
    private static RhoConf m_Instance;
	private static String CONF_FILENAME = "apps/rhoconfig.txt";
    
    public static RhoConf getInstance(){return m_Instance;}
    
	void setConfFilePath(String path){ m_strConfFilePath = path; }
	String getConfFilePath(){ return m_strConfFilePath; }
    
	void setRhoRootPath(String szRootPath){ m_strRhoRootPath = szRootPath;}
	String getRhoRootPath(){ return m_strRhoRootPath;}
	
    public void saveToFile(){
        String strData = saveToString();
    	SimpleFile oFile = null;

    	try{
	        oFile = RhoClassFactory.createFile();
	        
        	oFile.open( getConfFilePath(), false, false);
        	oFile.truncate(0);
	        oFile.write( 0, strData.getBytes() );
	        oFile.close();
    	}catch(Exception exc){
    		if ( oFile != null )
    			try{ oFile.close(); }catch(IOException exc2){}
    	}
        
    }
    
    void loadFromFile(){
    	SimpleFile oFile = null;
    	try{
	        oFile = RhoClassFactory.createFile();
	        oFile.open( getConfFilePath(), true, false);
	        
	        if ( oFile.isOpened() ){
	            String strSettings = oFile.readString();
	            oFile.close();
	            loadFromString( strSettings );
	        }
	        
    	}catch(Exception exc){
    		if ( oFile != null )
    			try{ oFile.close(); }catch(IOException exc2){}
    	}
    }
    
    void loadFromString(String szSettings){
		Tokenizer stringtokenizer = new Tokenizer(szSettings, "\n");
		while (stringtokenizer.hasMoreTokens()) {
			String tok = stringtokenizer.nextToken();
			tok = tok.trim();
			if (tok.length() == 0) {
				continue;
			}
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
			
			setPropertyByName(name,value);
		}
	}

    void setPropertyByName(String name, String value ){
    	m_mapValues.put(name,value);
    }
	
    String saveToString(){
    	String strData = "";
    	Enumeration enValues = m_mapValues.elements();
    	Enumeration enKeys = m_mapValues.keys();
		while (enValues.hasMoreElements()) {
			String key = (String)enKeys.nextElement();
			String value = (String)enValues.nextElement();
			
            strData += key;
            strData += "=";
            strData += value;
            strData += "\n";
		}
		
    	return strData;
    }

    public String getString(String szName){
    	String value = (String)m_mapValues.get(szName);
    	if ( value != null )
    		return value;

        return "";
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

    public void   setString(String szName, String str){
    	m_mapValues.put(szName,str);
    }

    public void   setInt(String szName, int nVal){
    	m_mapValues.put(szName,Integer.toString(nVal));
    }

    public void   setBool(String szName, boolean bVal){
        setInt(szName, bVal ? 1 : 0 );
    }

    boolean  isExist(String szName){
    	return m_mapValues.containsKey(szName);
    }
    
    public static void InitRhoConf(){
        m_Instance = new RhoConf();
    	
    	String szRootPath = "";
    	try{
    		szRootPath = RhoClassFactory.createFile().getDirPath("");
    		//TODO: implement recursive dir creation
    		RhoClassFactory.createFile().getDirPath("apps");
    	}catch(Exception exc){}

    	
    	m_Instance.setConfFilePath(szRootPath + CONF_FILENAME);
    	m_Instance.setRhoRootPath(szRootPath);
    }

    public void loadConf(){
    	loadFromConfFromJar();
    	loadFromFile();
    }
    
    private void loadFromConfFromJar()
    {
		java.io.InputStream fstream = getClass().getResourceAsStream(
				 "/" + CONF_FILENAME);
		if ( fstream == null )
			return;
		 
		try{
			byte[] data = new byte[fstream.available()];
			int len = fstream.read(data);
			if ( len == 0 )
				return;
			
			String strSettings = new String(data,0,len);
			loadFromString(strSettings);
		}catch(java.io.IOException exc){
			 
		}
   }
    
}
