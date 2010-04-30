package com.rho.db;

import java.util.Enumeration;
import java.util.Hashtable;
import com.rho.Mutex;
import com.rho.Tokenizer;
import com.xruby.runtime.lang.RubyValue;
import com.rho.RhoRuby;

public class DBAttrManager {
    private Mutex m_mxSrcAttrs = new Mutex();
    private Hashtable/*< int, Hashtable<String,int>* >*/ m_mapSrcAttrs = new Hashtable();
    private Hashtable/*Ptr< int, Hashtable<String,int>* >*/ m_mapBlobAttrs = new Hashtable();
    private Hashtable/*Ptr< String, int>*/ m_mapSrcNames = new Hashtable();

    public void add( Integer nSrcID, String szAttr )
    {
    	synchronized(m_mxSrcAttrs)
    	{
	        Hashtable/*<String,int>*/ mapAttr = (Hashtable)m_mapSrcAttrs.get(nSrcID);
	        if ( mapAttr != null )
	        {
	            int nCount = 0;
	            if ( mapAttr.containsKey(szAttr) )
	                nCount = ((Integer)mapAttr.get(szAttr)).intValue();
	
	            mapAttr.put(szAttr, new Integer(nCount+1));
	        }else
	        {
	            mapAttr = new Hashtable/*<String,int>*/();
	            mapAttr.put(szAttr, new Integer(1));
	            m_mapSrcAttrs.put(nSrcID,mapAttr);
	        }
    	}
    }

    public void remove( Integer nSrcID, String szAttr )
    {
    	synchronized(m_mxSrcAttrs)
    	{
	        Hashtable/*<String,int>*/ mapAttr = (Hashtable)m_mapSrcAttrs.get(nSrcID);
	        if ( mapAttr != null )
	        {
	            if ( mapAttr.containsKey(szAttr) )
	            {
	                int nCount = ((Integer)mapAttr.get(szAttr)).intValue();
	                nCount--;
	                if ( nCount <= 0 )
	                    mapAttr.remove(szAttr);
	                else
	                    mapAttr.put(szAttr, new Integer(nCount));
	            }
	        }
    	}
    }

    public void save(DBAdapter db)throws DBException
    {
    	synchronized(m_mxSrcAttrs)
    	{
        	Enumeration valsSrc = m_mapSrcAttrs.elements();
        	Enumeration keysSrc = m_mapSrcAttrs.keys();
    		while (valsSrc.hasMoreElements()) 
    		{
    			Integer nSrcID = (Integer)keysSrc.nextElement();
    			Hashtable mapAttr = (Hashtable)valsSrc.nextElement();
    			
	            String strAttribs = "";
	            Enumeration valsAttrs = mapAttr.elements();
	        	Enumeration keysAttrs = mapAttr.keys();
	            
	        	while (valsAttrs.hasMoreElements())
	            {
	        		String strAttr = (String)keysAttrs.nextElement();
	        		Integer nCounter = (Integer)valsAttrs.nextElement();
	                if ( strAttribs.length() > 0 )
	                    strAttribs += ',';
	
	                strAttribs += strAttr + ',' + nCounter;
	            }
	
	            db.executeSQL("UPDATE sources SET source_attribs=? where source_id=?", strAttribs, nSrcID );
	        }
    	}
    }

    public void load(DBAdapter db)throws DBException
    {
    	synchronized(m_mxSrcAttrs)
    	{
    		loadAttrs(db, m_mapSrcAttrs, "source_attribs", null);
    	}
    }

    public void reset(DBAdapter db)throws DBException
    {
    	synchronized(m_mxSrcAttrs)
    	{
	        m_mapSrcAttrs.clear();
	        db.executeSQL("UPDATE sources SET source_attribs=?", "" );
    	}
    }

    public RubyValue getAttrsBySrc(int iSrcID)
    {
    	synchronized(m_mxSrcAttrs)
    	{
    		Integer nSrcID = new Integer(iSrcID);
    		RubyValue arRes = RhoRuby.create_array();
    		
	        Hashtable/*<String,int>*/ mapAttr = (Hashtable)m_mapSrcAttrs.get(nSrcID);
	        if ( mapAttr != null )
	        {
	        	Enumeration keysAttrs = mapAttr.keys();
	        	while (keysAttrs.hasMoreElements())
	            {
	        		String strAttr = (String)keysAttrs.nextElement();
	            	RubyValue valStr = RhoRuby.create_string(strAttr);
	            	RhoRuby.add_to_array(arRes,valStr);
	            }
	        }

	        return arRes;
    	}
    }

    public boolean isBlobAttr(Integer nSrcID, String szAttr)
    {
        Hashtable/*<String,int>**/ mapAttr = (Hashtable)m_mapBlobAttrs.get(nSrcID);
        if ( mapAttr != null )
            return mapAttr.containsKey(szAttr);

        return false;
    }

    public Integer getSrcIDHasBlobsByName(String strName)
    {
    	Integer nSrcID = (Integer)m_mapSrcNames.get(strName);
    	if ( nSrcID == null )
    		return null;

        Hashtable/*<String,int>**/ mapAttr = (Hashtable)m_mapBlobAttrs.get(nSrcID);
        if ( mapAttr.size() == 0 )
        	return null;
        
    	return nSrcID;
    }
    
    public boolean isOverwriteBlobFromServer(Integer nSrcID, String strAttr)
    {
        Hashtable/*<String,int>**/ mapAttr = (Hashtable)m_mapBlobAttrs.get(nSrcID);
        if ( mapAttr != null )
            return ((Integer)mapAttr.get(strAttr)).intValue() != 0;

        return false;
    }

    void loadBlobAttrs(DBAdapter db)throws DBException
    {
        loadAttrs(db, m_mapBlobAttrs, "blob_attribs", m_mapSrcNames);
        //TODO: update/delete trigger for schema sources
    }
    
    static void loadAttrs(DBAdapter db, Hashtable/*Ptr< int, Hashtable<String,int>* >&*/ mapAttrs, String strDBAttr, Hashtable mapSrcNames)throws DBException
    {
        mapAttrs.clear();
        String strSql = "SELECT source_id,";
        strSql += strDBAttr + ",name from sources";
    	
	    IDBResult res = db.executeSQL(strSql);
	    for ( ; !res.isEnd(); res.next() )
	    { 
	        Integer nSrcID = new Integer(res.getIntByIdx(0));
	        String strAttribs = res.getStringByIdx(1);
	        if ( strAttribs.length() == 0 )
	            continue;
	
	        Tokenizer oTokenizer = new Tokenizer( strAttribs, "," );
	
	        Hashtable/*<String,int>*/ mapAttr = new Hashtable/*<String,int>*/();
	        String strAttr = "";
			while (oTokenizer.hasMoreTokens()) 
	        {
				String tok = oTokenizer.nextToken();
				if (tok.length() == 0)
					continue;
	            
	            if ( strAttr.length() > 0 )
	            {
	                mapAttr.put(strAttr, new Integer(Integer.parseInt(tok)) );
	                strAttr = "";
	            }else
	                strAttr = tok;
	        }
	
			mapAttrs.put( nSrcID, mapAttr );
			if ( mapSrcNames != null )
				mapSrcNames.put(res.getStringByIdx(2).toUpperCase(), nSrcID);
	    }
    }
}
