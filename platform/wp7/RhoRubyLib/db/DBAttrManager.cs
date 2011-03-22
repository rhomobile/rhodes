using System;
using rho.common;

namespace rho.db
{
    public class DBAttrManager
    {
    private Hashtable< int, Hashtable<String,int> > m_mapBlobAttrs = new Hashtable< int, Hashtable<String,int> >();
    private Hashtable< String, int> m_mapSrcNames = new Hashtable< String, int>();

    public boolean isBlobAttr(int nSrcID, String szAttr)
    {
        Hashtable<String,int> mapAttr = m_mapBlobAttrs.get(nSrcID);
        if ( mapAttr != null )
            return mapAttr.containsKey(szAttr);

        return false;
    }

    public boolean hasBlobAttrs()
    {
    	return m_mapBlobAttrs.size() > 0;
    }
    
    public int getSrcIDHasBlobsByName(String strName)
    {
        if (!m_mapSrcNames.ContainsKey(strName))
    		return 0;

        int nSrcID = m_mapSrcNames.get(strName);
        Hashtable<String,int> mapAttr = m_mapBlobAttrs.get(nSrcID);
        if ( mapAttr.size() == 0 )
        	return 0;
        
    	return nSrcID;
    }
    
    public boolean isOverwriteBlobFromServer(int nSrcID, String strAttr)
    {
        Hashtable<String,int> mapAttr = m_mapBlobAttrs.get(nSrcID);
        if ( mapAttr != null )
            return mapAttr.get(strAttr) != 0;

        return false;
    }

    public void loadBlobAttrs(DBAdapter db)
    {
        loadAttrs(db, m_mapBlobAttrs, "blob_attribs", m_mapSrcNames);
        //TODO: update/delete trigger for schema sources
    }
    
    static void loadAttrs(DBAdapter db, Hashtable< int, Hashtable<String,int> > mapAttrs, String strDBAttr,
        Hashtable<String, int> mapSrcNames)
    {
        mapAttrs.clear();
        String strSql = "SELECT source_id,";
        strSql += strDBAttr + ",name from sources";
    	
	    IDBResult res = db.executeSQL(strSql);
	    for ( ; !res.isEnd(); res.next() )
	    { 
	        int nSrcID = res.getIntByIdx(0);
	        String strAttribs = res.getStringByIdx(1);
	        if ( strAttribs.length() == 0 )
	            continue;
	
	        Tokenizer oTokenizer = new Tokenizer( strAttribs, "," );
	
	        Hashtable<String,int> mapAttr = new Hashtable<String,int>();
	        String strAttr = "";
			while (oTokenizer.hasMoreTokens()) 
	        {
				String tok = oTokenizer.nextToken();
				if (tok.length() == 0)
					continue;
	            
	            if ( strAttr.length() > 0 )
	            {
	                mapAttr.put(strAttr, int.Parse(tok) );
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
}
