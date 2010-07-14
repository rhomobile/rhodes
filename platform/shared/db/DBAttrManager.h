#pragma once

#include "common/RhoStd.h"
#include "common/RhoMutexLock.h"

namespace rho{
namespace db{
class CDBAdapter;

class CDBAttrManager
{
    common::CMutex m_mxSrcAttrs;
    HashtablePtr< int, Hashtable<String,int>* > m_mapSrcAttrs;

    HashtablePtr< int, Hashtable<String,int>* > m_mapBlobAttrs;

    static void loadAttrs(CDBAdapter& db, HashtablePtr< int, Hashtable<String,int>* >& mapAttrs, String strDBAttr);
public:
    
    void add( int nSrcID, const char* szAttr );
    void remove( int nSrcID, const char* szAttr );
    void save(CDBAdapter& db);
    void load(CDBAdapter& db);
    void reset(CDBAdapter& db);
#ifndef RHO_NO_RUBY
    unsigned long getAttrsBySrc(int nSrcID);
#endif //RHO_NO_RUBY
    boolean isBlobAttr(int nSrcID, const char* szAttr);
    boolean isOverwriteBlobFromServer(int nSrcID, const String& strAttr);
    void loadBlobAttrs(CDBAdapter& db);
};

}
}
