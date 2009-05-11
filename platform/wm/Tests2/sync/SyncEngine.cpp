#include "SyncEngine.h"
#include "SyncSource.h"

namespace rho {
namespace sync {

using namespace rho::net;

void CSyncEngine::doSyncAllSources()
{
    setState(esSyncAllSources);

    loadAllSources();
    loadClientID();
    syncAllSources();

    setState(esNone);
}

void CSyncEngine::doSyncSource()
{
    //TODO:doSyncSource
}

void CSyncEngine::loadAllSources()
{
    m_sources.clear();

    DBResult( res, getDB().executeSQL("SELECT source_id,source_url,token from sources order by source_id") );
    for ( ; !res.isEnd(); res.next() ) 
        m_sources.add( new CSyncSource(res, *this) );
}

void CSyncEngine::loadClientID()
{
    if ( m_clientID.length() > 0 )
        return;

    DBResult( res , getDB().executeSQL("SELECT client_id from client_info limit 1") );
    if ( !res.isEnd() )
        m_clientID = res.getStringByIdx(0);
    else
    {
        m_clientID = requestClientIDByNet();

        getDB().executeSQL("INSERT INTO client_info (client_id) values (?)", m_clientID);
    }
}

String CSyncEngine::requestClientIDByNet()
{
    if ( m_sources.size() == 0 )
        return "";

    CSyncSource& src = *m_sources.get(0);
    String strUrl = src.getUrl();
    String strQuery = "clientcreate";
    strQuery += SYNC_SOURCE_FORMAT();

    CNetResponse oResp;
    getNet().pullData(strUrl,strQuery,oResp);

    return oResp.m_strResponse;
}

int CSyncEngine::getStartSource()
{
    for( int i = 0; i < m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.get(i);
        if ( !src.isEmptyToken() )
            return i;
    }

    return 0;
}

void CSyncEngine::syncAllSources()
{
    for( int i = getStartSource(); i < m_sources.size() && getState() != esExit; i++ )
    {
        CSyncSource& src = *m_sources.get(i);
        src.sync();
    }
}

}
}