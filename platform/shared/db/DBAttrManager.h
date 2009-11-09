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

public:
    
    void add( int nSrcID, const char* szAttr );
    void remove( int nSrcID, const char* szAttr );
    void save(CDBAdapter& db);
    void load(CDBAdapter& db);
    void reset(CDBAdapter& db);
    unsigned long getAttrsBySrc(int nSrcID);

};

}
}
