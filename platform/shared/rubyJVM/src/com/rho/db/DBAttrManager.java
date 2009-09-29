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

    public void add( int iSrcID, String szAttr )
    {
    	synchronized(m_mxSrcAttrs)
    	{
    		Integer nSrcID = new Integer(iSrcID);
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

    public void remove( int iSrcID, String szAttr )
    {
    	synchronized(m_mxSrcAttrs)
    	{
    		Integer nSrcID = new Integer(iSrcID);
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
	        IDBResult res = db.executeSQL("SELECT source_id,source_attribs from sources");
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
	
	            m_mapSrcAttrs.put( nSrcID, mapAttr );
	        }
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
}
