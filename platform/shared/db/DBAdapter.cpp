#include "DBAdapter.h"

#include "common/RhoFile.h"

namespace rho{
namespace db{
IMPLEMENT_LOGCLASS(CDBAdapter,"DB");

using namespace rho::common;

static int onDBBusy(void* data,int nTry)
{
    LOGC(ERROR,CDBAdapter::getLogCategory())+"Database BUSY";
    return 0;
}

void SyncBlob_DeleteCallback(sqlite3_context* dbContext, int nArgs, sqlite3_value** ppArgs)
{
    char* type = NULL;
    if ( nArgs < 2 )
        return;

    type = (char*)sqlite3_value_text(*(ppArgs+1));
    if ( type && strcmp(type,"blob.file") == 0 ){
        char* pFilePath = (char*)sqlite3_value_text(*(ppArgs));
        CRhoFile::deleteFile(pFilePath);
    }
}

void CDBAdapter::open (String strDbPath, String strVer)
{
    if ( strcasecmp(strDbPath.c_str(),m_strDbPath.c_str() ) == 0 )
        return;
    close();

    m_strDbPath = strDbPath;

    checkVersion(strVer);

    boolean bExist = CRhoFile::isFileExist(strDbPath.c_str());
    int nRes = sqlite3_open(strDbPath.c_str(),&m_dbHandle);
    //TODO: raise exception if error
    if ( !bExist )
        createSchema();

    sqlite3_create_function( m_dbHandle, "rhoOnDeleteObjectRecord", 3, SQLITE_ANY, 0,
	    SyncBlob_DeleteCallback, 0, 0 );
    sqlite3_busy_handler(m_dbHandle, onDBBusy, 0 );
}

void CDBAdapter::checkVersion(String& strVer)
{
    //TODO: checkVersion
}

void CDBAdapter::createSchema()
{
    //TODO: createSchema
}

void CDBAdapter::close()
{
    for (Hashtable<String,sqlite3_stmt*>::iterator it = m_mapStatements.begin();  it != m_mapStatements.end(); ++it )
        sqlite3_finalize( it->second );

    m_mapStatements.clear();

    if ( m_dbHandle != 0 )
        sqlite3_close(m_dbHandle);

    m_dbHandle = 0;
    m_strDbPath = String();
}

DBResultPtr CDBAdapter::prepareStatement( const char* szSt )
{
    if ( m_dbHandle == null )
        return new CDBResult(m_mxDB);

	DBResultPtr res = new CDBResult(0,m_bInsideTransaction ? m_mxTransDB : m_mxDB);
    sqlite3_stmt* st = m_mapStatements.get(szSt);
    if ( st != null )
	{
		res->setStatement(st);
        return res;
	}
	
    int rc = sqlite3_prepare_v2(m_dbHandle, szSt, -1, &st, NULL);
    if ( rc != SQLITE_OK )
    {
        //TODO: raise exception
        return res;
    }

    res->setStatement(st);
    m_mapStatements.put(szSt, st);

    return res;
}

DBResultPtr CDBAdapter::executeSQL( const char* szSt)
{
    DBResultPtr res = prepareStatement(szSt);
    if ( res->getStatement() == null )
        return res;

    return executeStatement(res);
}

DBResultPtr CDBAdapter::executeStatement(DBResultPtr& res)
{
    int rc = sqlite3_step(res->getStatement());
    if ( rc != SQLITE_ROW )
    {
        res->setStatement(null);
        if ( rc != SQLITE_OK && rc != SQLITE_ROW && rc != SQLITE_DONE )
        {
            //TODO: raise exception
            return res;
        }
    }

    return res;
}

void CDBAdapter::startTransaction()
{
    Lock();
	m_bInsideTransaction=true;
    char *zErr;
    int rc = sqlite3_exec(m_dbHandle, "BEGIN IMMEDIATE;",0,0,&zErr);
}

void CDBAdapter::endTransaction()
{
    char *zErr;
    int rc = sqlite3_exec(m_dbHandle, "END;",0,0,&zErr);

	m_bInsideTransaction=false;
    Unlock();
	
}

}
}