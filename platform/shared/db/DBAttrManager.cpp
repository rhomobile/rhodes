#include "DBAttrManager.h"
#include "DBAdapter.h"
#include "common/StringConverter.h"
#include "common/Tokenizer.h"
#ifndef RHO_NO_RUBY
#include "ruby/ext/rho/rhoruby.h"
#endif //RHO_NO_RUBY

namespace rho{
namespace db{
using namespace rho::common;

void CDBAttrManager::add( int nSrcID, const char* szAttr )
{
    synchronized(m_mxSrcAttrs)
    {
        Hashtable<String,int>* pmapAttr = m_mapSrcAttrs.get(nSrcID);
        if ( pmapAttr != null )
        {
            Hashtable<String,int>& mapAttr = *pmapAttr;
            int nCount = 0;
            if ( mapAttr.containsKey(szAttr) )
                nCount = mapAttr.get(szAttr);

            mapAttr.put(szAttr, nCount+1);
        }else
        {
            pmapAttr = new Hashtable<String,int>();
            pmapAttr->put(szAttr, 1);
            m_mapSrcAttrs.put(nSrcID,pmapAttr);
        }
    }
}

void CDBAttrManager::remove( int nSrcID, const char* szAttr )
{
    synchronized(m_mxSrcAttrs)
    {
        Hashtable<String,int>* pmapAttr = m_mapSrcAttrs.get(nSrcID);
        if ( pmapAttr != null )
        {
            Hashtable<String,int>& mapAttr = *pmapAttr;
            if ( mapAttr.containsKey(szAttr) )
            {
                int nCount = mapAttr.get(szAttr);
                nCount--;
                if ( nCount <= 0 )
                    mapAttr.remove(szAttr);
                else
                    mapAttr.put(szAttr, nCount);
            }
        }
    }
}

void CDBAttrManager::save(CDBAdapter& db)
{
    synchronized(m_mxSrcAttrs)
    {
        for (HashtablePtr< int, Hashtable<String,int>* >::iterator it = m_mapSrcAttrs.begin();  it != m_mapSrcAttrs.end(); ++it )
        {
            int nSrcID = it->first;
            Hashtable<String,int>& mapAttr = *(it->second);
            String strAttribs = "";
            for ( Hashtable<String,int>::iterator itAttr = mapAttr.begin();  itAttr != mapAttr.end(); ++itAttr )
            {
                if ( strAttribs.length() > 0 )
                    strAttribs += ',';

                strAttribs += itAttr->first + ',' + convertToStringA(itAttr->second);
            }

            db.executeSQL("UPDATE sources SET source_attribs=? where source_id=?", strAttribs, nSrcID );
        }
    }
}

void CDBAttrManager::load(CDBAdapter& db)
{
    synchronized(m_mxSrcAttrs)
    {
        loadAttrs(db, m_mapSrcAttrs, "source_attribs");
    }
}

void CDBAttrManager::reset(CDBAdapter& db)
{
    synchronized(m_mxSrcAttrs)
    {
        m_mapSrcAttrs.clear();
        db.executeSQL("UPDATE sources SET source_attribs=?", "" );
    }
}

#ifndef RHO_NO_RUBY
unsigned long CDBAttrManager::getAttrsBySrc(int nSrcID)
{
    synchronized(m_mxSrcAttrs)
    {
        CHoldRubyValue arRes(rho_ruby_create_array());

        Hashtable<String,int>* pmapAttr = m_mapSrcAttrs.get(nSrcID);
        if ( pmapAttr != null )
        {
            Hashtable<String,int>& mapAttr = *pmapAttr;

            for ( Hashtable<String,int>::iterator itAttr = mapAttr.begin();  itAttr != mapAttr.end(); ++itAttr )
            {
                VALUE valStr = rho_ruby_create_string(itAttr->first.c_str());
                rho_ruby_add_to_array(arRes,valStr);
            }
        }

        return arRes;
    }
}
#endif //RHO_NO_RUBY

boolean CDBAttrManager::isBlobAttr(int nSrcID, const char* szAttr)
{
    Hashtable<String,int>* pmapAttr = m_mapBlobAttrs.get(nSrcID);
    if ( pmapAttr != null )
    {
        Hashtable<String,int>& mapAttr = *pmapAttr;
        return mapAttr.containsKey(szAttr);
    }

    return false;
}

boolean CDBAttrManager::isOverwriteBlobFromServer(int nSrcID, const String& strAttr)
{
    Hashtable<String,int>* pmapAttr = m_mapBlobAttrs.get(nSrcID);
    if ( pmapAttr != null )
    {
        Hashtable<String,int>& mapAttr = *pmapAttr;
        return mapAttr.get(strAttr) != 0;
    }

    return false;
}

void CDBAttrManager::loadBlobAttrs(CDBAdapter& db)
{
    loadAttrs(db, m_mapBlobAttrs, "blob_attribs");

    for ( HashtablePtr< int, Hashtable<String,int>* >::iterator it = m_mapBlobAttrs.begin();  it != m_mapBlobAttrs.end(); ++it )
    {
        int nSrcID = it->first;

        DBResult( res, db.executeSQL("SELECT name FROM sources WHERE source_id=?", nSrcID) );
        if ( res.isEnd() )
            continue;

        String strName = res.getStringByIdx(0);
        if ( !db.isTableExist(strName) )
            continue;

        db.createDeleteTrigger(strName);
    }
}

/*static*/void CDBAttrManager::loadAttrs(CDBAdapter& db, HashtablePtr< int, Hashtable<String,int>* >& mapAttrs, String strDBAttr)
{
    mapAttrs.clear();
    String strSql = "SELECT source_id,";
    strSql += strDBAttr + " from sources";

    DBResult( res, db.executeSQL(strSql.c_str()) );
    for ( ; !res.isEnd(); res.next() )
    { 
        int nSrcID = res.getIntByIdx(0);
        String strAttribs = res.getStringByIdx(1);
        if ( strAttribs.length() == 0 )
            continue;

        CTokenizer oTokenizer( strAttribs, "," );

        Hashtable<String,int>* pmapAttr = new Hashtable<String,int>();
        String strAttr = "";
	    while (oTokenizer.hasMoreTokens()) 
        {
		    String tok = oTokenizer.nextToken();
		    if (tok.length() == 0)
			    continue;
            
            if ( strAttr.length() > 0 )
            {
                int nCounter = 0;
                convertFromStringA(tok.c_str(),nCounter);
                pmapAttr->put(strAttr, nCounter);
                strAttr = "";
            }else
                strAttr = tok;
        }

        mapAttrs.put(nSrcID,pmapAttr);
    }
}

}
}
