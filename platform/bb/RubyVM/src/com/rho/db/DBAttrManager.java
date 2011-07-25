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

package com.rho.db;

import java.util.Enumeration;
import java.util.Hashtable;
import com.rho.Mutex;
import com.rho.Tokenizer;
import com.xruby.runtime.lang.RubyValue;
import com.rho.RhoRuby;

public class DBAttrManager {
    //private Mutex m_mxSrcAttrs = new Mutex();
    //private Hashtable/*< int, Hashtable<String,int>* >*/ m_mapSrcAttrs = new Hashtable();
    private Hashtable/*Ptr< int, Hashtable<String,int>* >*/ m_mapBlobAttrs = new Hashtable();
    private Hashtable/*Ptr< String, int>*/ m_mapSrcNames = new Hashtable();

    public boolean isBlobAttr(Integer nSrcID, String szAttr)
    {
        Hashtable/*<String,int>**/ mapAttr = (Hashtable)m_mapBlobAttrs.get(nSrcID);
        if ( mapAttr != null )
            return mapAttr.containsKey(szAttr);

        return false;
    }

    public boolean hasBlobAttrs()
    {
    	return m_mapBlobAttrs.size() > 0;
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

    public void loadBlobAttrs(DBAdapter db)throws DBException
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
