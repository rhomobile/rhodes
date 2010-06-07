#include "DBAdapter.h"
#include "sync/SyncThread.h"

#include "common/RhoFile.h"
#include "common/RhoFilePath.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "ruby/ext/rho/rhoruby.h"

namespace rho{
namespace db{
IMPLEMENT_LOGCLASS(CDBAdapter,"DB");
HashtablePtr<String,CDBAdapter*> CDBAdapter::m_mapDBPartitions;

using namespace rho::common;

static int onDBBusy(void* data,int nTry)
{
    LOGC(ERROR,CDBAdapter::getLogCategory())+"Database BUSY";
    return 0;
}

void SyncBlob_DeleteCallback(sqlite3_context* dbContext, int nArgs, sqlite3_value** ppArgs)
{
    if ( nArgs < 3 )
        return;

    CDBAttrManager& attrMgr = CDBAdapter::getDBByHandle(sqlite3_context_db_handle(dbContext)).getAttrMgr();

    char* szAttrName = (char*)sqlite3_value_text(*(ppArgs+2));
    int nSrcID = sqlite3_value_int(*(ppArgs+1));
    if ( attrMgr.isBlobAttr(nSrcID, szAttrName) )
    {
        String strFilePath = RHODESAPP().resolveDBFilesPath((char*)sqlite3_value_text(*(ppArgs)));
        CRhoFile::deleteFile(strFilePath.c_str());
    }

    attrMgr.remove( nSrcID, szAttrName );
}

void SyncBlob_DeleteSchemaCallback(sqlite3_context* dbContext, int nArgs, sqlite3_value** ppArgs)
{
    CDBAttrManager& attrMgr = CDBAdapter::getDBByHandle(sqlite3_context_db_handle(dbContext)).getAttrMgr();
    //TODO: SyncBlob_DeleteSchemaCallback
    //TODO: add to reset - set callback
}

void SyncBlob_InsertCallback(sqlite3_context* dbContext, int nArgs, sqlite3_value** ppArgs)
{
    if ( nArgs < 2 )
        return;

    CDBAdapter::getDBByHandle(sqlite3_context_db_handle(dbContext)).getAttrMgr().add( sqlite3_value_int(*(ppArgs)), (char*)sqlite3_value_text(*(ppArgs+1)) );
}

boolean CDBAdapter::checkDbError(int rc)
{
    if ( rc == SQLITE_OK || rc == SQLITE_ROW || rc == SQLITE_DONE )
        return true;

    const char * szErrMsg = sqlite3_errmsg(m_dbHandle);
    int nErrCode = sqlite3_errcode(m_dbHandle);

    LOG(ERROR)+"DB query failed. Error code: " + nErrCode + "; Message: " + szErrMsg;

    return false;
}

boolean CDBAdapter::checkDbErrorEx(int rc, rho::db::CDBResult& res)
{
    if ( rc == SQLITE_OK || rc == SQLITE_ROW || rc == SQLITE_DONE )
        return true;

    const char * szErrMsg = sqlite3_errmsg(m_dbHandle);
    int nErrCode = sqlite3_errcode(m_dbHandle);

    res.setErrorCode(nErrCode);
    if ( nErrCode == SQLITE_CONSTRAINT && res.getReportNonUnique() )
        return true;

    LOG(ERROR)+"DB query failed. rc: " + rc + "; Error code: " + nErrCode + "; Message: " + szErrMsg;
    return false;
}

void CDBAdapter::open (String strDbPath, String strVer, boolean bTemp)
{
    if ( strcasecmp(strDbPath.c_str(),m_strDbPath.c_str() ) == 0 )
        return;
    close();

    m_strDbPath = strDbPath;
    if ( !bTemp )
    {
        m_strDbVerPath = strDbPath+".version";
        m_strDbVer = strVer;

        checkDBVersion(strVer);
    }

    boolean bExist = CRhoFile::isFileExist(strDbPath.c_str());
    int nRes = sqlite3_open(strDbPath.c_str(),&m_dbHandle);
    if ( !checkDbError(nRes) )
        return;
    //TODO: raise exception if error
    if ( !bExist )
        createSchema();

    sqlite3_create_function( m_dbHandle, "rhoOnDeleteObjectRecord", 3, SQLITE_ANY, 0,
	    SyncBlob_DeleteCallback, 0, 0 );
    sqlite3_create_function( m_dbHandle, "rhoOnDeleteRecord", 1, SQLITE_ANY, 0,
	    SyncBlob_DeleteSchemaCallback, 0, 0 );

    sqlite3_create_function( m_dbHandle, "rhoOnInsertObjectRecord", 2, SQLITE_ANY, 0,
	    SyncBlob_InsertCallback, 0, 0 );

    sqlite3_busy_handler(m_dbHandle, onDBBusy, 0 );

    getAttrMgr().load(*this);
}

boolean CDBAdapter::migrateDB(const CDBVersion& dbVer, const String& strRhoDBVer, const String& strAppDBVer )
{
    LOG(INFO) + "Try migrate database from " + dbVer.m_strRhoVer + " to " + strRhoDBVer;
    if ( (dbVer.m_strRhoVer.find("1.4") == 0)&& (strRhoDBVer.find("1.5")==0||strRhoDBVer.find("1.4")==0) )
    {
        LOG(INFO) + "No migration required from " + dbVer.m_strRhoVer + " to " + strRhoDBVer;
        writeDBVersion( CDBVersion(strRhoDBVer, strAppDBVer) );
        return true;
    }

    //1.2.x -> 1.5.x,1.4.x
    if ( (dbVer.m_strRhoVer.find("1.2") == 0)&& (strRhoDBVer.find("1.5")==0||strRhoDBVer.find("1.4")==0) )
    {
    //sources
    //priority INTEGER, ADD
    //backend_refresh_time int default 0, ADD
    //id INTEGER PRIMARY KEY, REMOVE

    //changed_values
    //id INTEGER PRIMARY KEY, REMOVE
        LOG(INFO) + "Migrate database from " + dbVer.m_strRhoVer + " to " + strRhoDBVer;

        CDBAdapter db(m_strDbPartition.c_str(), true);
        db.open( m_strDbPath, m_strDbVer, true );
        DBResult( res, db.executeSQL( "ALTER TABLE sources ADD priority INTEGER" ));
        DBResult( res1, db.executeSQL( "ALTER TABLE sources ADD backend_refresh_time int default 0" ));

        {
            Vector<int> vecSrcIds;
            DBResult( res2, db.executeSQL( "SELECT source_id FROM sources" ));
            for ( ; !res2.isEnd(); res2.next() )
                vecSrcIds.addElement(res2.getIntByIdx(0));

            for( size_t i = 0; i < vecSrcIds.size(); i++)
            {
                DBResult( res3, db.executeSQL( "UPDATE sources SET priority=? where source_id=?", 
                    vecSrcIds.elementAt(i), vecSrcIds.elementAt(i) ));
            }
        }
        db.close();

        writeDBVersion( CDBVersion(strRhoDBVer, strAppDBVer) );

        return true;
    }


    return false;
}

void CDBAdapter::checkDBVersion(String& strRhoDBVer)
{
	String strAppDBVer = RHOCONF().getString("app_db_version");
	
	CDBVersion dbVer = readDBVersion();

	boolean bRhoReset = false;
    boolean bAppReset = false;

	if ( strRhoDBVer.length() > 0 )
	{
		if ( dbVer.m_strRhoVer.compare(strRhoDBVer) != 0 )
			bRhoReset = true;
	}
	if ( strAppDBVer.length() > 0 )
	{
		if ( dbVer.m_strAppVer.compare(strAppDBVer) != 0 )
			bAppReset = true;
	}

    if ( bRhoReset && !bAppReset )
        bRhoReset = !migrateDB(dbVer, strRhoDBVer, strAppDBVer);

	if ( bRhoReset || bAppReset )
	{
        LOG(INFO) + "Reset database because version is changed.";

        CRhoFile::deleteFile(m_strDbPath.c_str());
        CRhoFile::deleteFile((m_strDbPath+"-journal").c_str());

        CRhoFile::deleteFilesInFolder(RHODESAPP().getBlobsDirPath().c_str());

        writeDBVersion( CDBVersion(strRhoDBVer, strAppDBVer) );
	}
}

CDBAdapter::CDBVersion CDBAdapter::readDBVersion()//throws Exception
{
    String strFullVer;
    CRhoFile oFile;
    if ( !oFile.open(m_strDbVerPath.c_str(),CRhoFile::OpenReadOnly) )
        return CDBVersion();

    oFile.readString(strFullVer);
	
	if ( strFullVer.length() == 0 )
		return CDBVersion();
	
	int nSep = strFullVer.find(';');
	if ( nSep == -1 )
		return CDBVersion(strFullVer, "");
	
	return CDBVersion(strFullVer.substr(0,nSep), strFullVer.substr(nSep+1) );
}

void CDBAdapter::writeDBVersion(const CDBVersion& ver)//throws Exception
{
    String strFullVer = ver.m_strRhoVer + ";" + ver.m_strAppVer;
    CRhoFile oFile;
    if ( !oFile.open(m_strDbVerPath.c_str(),CRhoFile::OpenForWrite) )
        return;

    oFile.write(strFullVer.c_str(), strFullVer.length() );
}

sqlite3_stmt* CDBAdapter::createInsertStatement(rho::db::CDBResult& res, const String& tableName, CDBAdapter& db, String& strInsert)
{
    sqlite3_stmt* stInsert = 0;
    int nColCount = sqlite3_data_count(res.getStatement());

  	if ( strInsert.length() == 0 )
    {
	    strInsert = "INSERT INTO ";
	
	    strInsert += tableName;
	    strInsert += "(";
	    String strQuest = ") VALUES(";
        String strValues = "";
	    for (int nCol = 0; nCol < nColCount; nCol++ )
	    {
            String strColName = sqlite3_column_name(res.getStatement(),nCol);
            if ( strColName == "id")
                continue;

		    if ( strValues.length() > 0 )
		    {
			    strValues += ",";
			    strQuest += ",";
		    }
    		
		    strValues += strColName; 
		    strQuest += "?";
	    }
    	
	    strInsert += strValues + strQuest + ")";
    }

    int rc = sqlite3_prepare_v2(db.getDbHandle(), strInsert.c_str(), -1, &stInsert, NULL);
    if ( !checkDbError(rc) )
    	return 0;
    
    int nBindCol = 1;
	for (int nCol = 0; nCol < nColCount; nCol++ )
	{
		int nColType = sqlite3_column_type(res.getStatement(),nCol);
        String strColName = sqlite3_column_name(res.getStatement(),nCol);
        if ( strColName == "id")
            continue;

		switch(nColType){
			case SQLITE_NULL:
                rc = sqlite3_bind_text(stInsert, nBindCol, null, -1, SQLITE_TRANSIENT);
                break;
            case SQLITE_INTEGER:
            {
                sqlite_int64 nValue = sqlite3_column_int64(res.getStatement(), nCol);
                rc = sqlite3_bind_int64(stInsert, nBindCol, nValue);
                break;
            }
			default:{
                char* szValue = (char *)sqlite3_column_text(res.getStatement(), nCol);
                rc = sqlite3_bind_text(stInsert, nBindCol, szValue, -1, SQLITE_TRANSIENT);
				break;
			}
		}
        nBindCol++;
    }

	return stInsert;
}

static boolean destroyTableName(String tableName, const rho::Vector<rho::String>& arIncludeTables, const rho::Vector<rho::String>& arExcludeTables )
{
    int i;
    for (i = 0; i < (int)arExcludeTables.size(); i++ )
    {
        if ( arExcludeTables.elementAt(i).compare(tableName) == 0 )
            return false;
    }

    for (i = 0; i < (int)arIncludeTables.size(); i++ )
    {
        if ( arIncludeTables.elementAt(i).compare(tableName) == 0 )
            return true;
    }

    return arIncludeTables.size()==0;
}

boolean CDBAdapter::isTableExist(String strTableName)
{
    Vector<String> vecTables;

    DBResult( res , executeSQL( "SELECT name FROM sqlite_master WHERE type='table' AND name=?", strTableName.c_str() ) );
    return !res.isEnd();
}

void CDBAdapter::destroy_tables(const rho::Vector<rho::String>& arIncludeTables, const rho::Vector<rho::String>& arExcludeTables)
{
    getAttrMgr().reset(*this);
    CFilePath oFilePath(m_strDbPath);
	String dbNewName  = oFilePath.changeBaseName("resetdbtemp.sqlite");

    CRhoFile::deleteFile(dbNewName.c_str());
    CRhoFile::deleteFile((dbNewName+"-journal").c_str());
    CRhoFile::deleteFile((dbNewName+".version").c_str());

    CDBAdapter db(m_strDbPartition.c_str(), true);
    db.open( dbNewName, m_strDbVer, true );

    //Copy all tables

    Vector<String> vecTables;
    DBResult( res , executeSQL( "SELECT name FROM sqlite_master WHERE type='table' " ) );
    for ( ; !res.isEnd(); res.next() )
        vecTables.addElement(res.getStringByIdx(0));

    db.startTransaction();
    for ( int i = 0; i < (int)vecTables.size(); i++ )
    {
        String tableName = vecTables.elementAt(i);
        if (destroyTableName(tableName, arIncludeTables, arExcludeTables)  )
            continue;

        copyTable(tableName, *this, db );
    }

    db.endTransaction();
    db.close();

    String dbOldName = m_strDbPath;
    close();

    CRhoFile::deleteFilesInFolder(RHODESAPP().getBlobsDirPath().c_str());

    CRhoFile::deleteFile(dbOldName.c_str());
    CRhoFile::renameFile(dbNewName.c_str(),dbOldName.c_str());
    open( dbOldName, m_strDbVer, false );
}

void CDBAdapter::copyTable(String tableName, CDBAdapter& dbFrom, CDBAdapter& dbTo)
{
    String strSelect = "SELECT * from " + tableName;
    DBResult( res , dbFrom.executeSQL( strSelect.c_str() ) );
	String strInsert = "";
    int rc = 0;
    for ( ; !res.isEnd(); res.next() )
    {
    	sqlite3_stmt* stInsert = createInsertStatement(res, tableName, dbTo, strInsert);

        if (stInsert)
        {
            rc = sqlite3_step(stInsert);
            checkDbError(rc);
            sqlite3_finalize(stInsert);
        }
    }
}

void CDBAdapter::setBulkSyncDB(String fDataName)
{
    CDBAdapter db(m_strDbPartition.c_str(), true);
    db.open( fDataName, m_strDbVer, true );
    db.createTriggers();

    db.startTransaction();

    copyTable("client_info", *this, db );

    //update User partition
    if ( m_strDbPartition.compare(USER_PARTITION_NAME())==0 )
    {
        //copy all NOT user sources from current db to bulk db
        executeSQL("DELETE FROM sources WHERE partition=?", m_strDbPartition);
        copyTable("sources", *this, db );
    }else
    {
        //remove all m_strDbPartition sources from user db
        //copy all sources from bulk db to user db
        CDBAdapter& dbUser  = getDB(USER_PARTITION_NAME());
        dbUser.executeSQL("DELETE FROM sources WHERE partition=?", m_strDbPartition);

        copyTable("sources", db, dbUser );
    }

    db.endTransaction();
    db.close();

    String dbOldName = m_strDbPath;
    close();

    CRhoFile::deleteFilesInFolder(RHODESAPP().getBlobsDirPath().c_str());

    CRhoFile::deleteFile(dbOldName.c_str());
    CRhoFile::renameFile(fDataName.c_str(),dbOldName.c_str());
    open( dbOldName, m_strDbVer, false );
}

void CDBAdapter::createSchema()
{
    char* errmsg = 0;
    CFilePath oPath(m_strDbPath);

    String strSqlScript;
    CRhoFile::loadTextFile(oPath.changeBaseName("syncdb.schema").c_str(), strSqlScript);

    if ( strSqlScript.length() == 0 )
    {
        LOG(ERROR)+"createSchema failed. Cannot read schema file: " + strSqlScript;
        return;
    }

    int rc = sqlite3_exec(m_dbHandle, strSqlScript.c_str(),  NULL, NULL, &errmsg);

    if ( rc != SQLITE_OK )
        LOG(ERROR)+"createSchema failed. Error code: " + rc + ";Message: " + (errmsg ? errmsg : "");

    if ( errmsg )
        sqlite3_free(errmsg);

    if ( rc == SQLITE_OK )
        createTriggers();
}

void CDBAdapter::createTriggers()
{
    char* errmsg = 0;
    CFilePath oPath(m_strDbPath);

    String strSqlTriggers;
    CRhoFile::loadTextFile(oPath.changeBaseName("syncdb.triggers").c_str(), strSqlTriggers);

    if ( strSqlTriggers.length() == 0 )
    {
        LOG(ERROR)+"createSchema failed. Cannot read triggers file: " + strSqlTriggers;
        return;
    }

    int rc = sqlite3_exec(m_dbHandle, strSqlTriggers.c_str(),  NULL, NULL, &errmsg);

    if ( rc != SQLITE_OK )
        LOG(ERROR)+"createTriggers failed. Error code: " + rc + ";Message: " + (errmsg ? errmsg : "");

    if ( errmsg )
        sqlite3_free(errmsg);
}

void CDBAdapter::createDeleteTrigger(const String& strTable)
{
String strTrigger = String("CREATE TRIGGER rhodeleteSchemaTrigger BEFORE DELETE ON ") + strTable + " FOR EACH ROW "
"   BEGIN "
"       SELECT rhoOnDeleteRecord( OLD );"
"   END;"
";";
    char* errmsg = 0;
    //TODO: createDeleteTrigger
/*    int rc = sqlite3_exec(m_dbHandle, strTrigger.c_str(),  NULL, NULL, &errmsg);

    if ( rc != SQLITE_OK )
        LOG(ERROR)+"createTriggers failed. Error code: " + rc + ";Message: " + (errmsg ? errmsg : "");
*/
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

int CDBAdapter::prepareSqlStatement(const char* szSql, int nByte, sqlite3_stmt **ppStmt)
{
    int rc = SQLITE_OK;
    sqlite3_stmt* st = m_mapStatements.get(szSql);
    if ( st != null )
        *ppStmt	 = st;
    else
    {
        rc = sqlite3_prepare_v2(m_dbHandle, szSql, nByte, ppStmt, NULL);
        if ( rc == SQLITE_OK )
            m_mapStatements.put(szSql, st);
    }

    return rc;
}

DBResultPtr CDBAdapter::prepareStatement( const char* szSt )
{
    if ( m_dbHandle == null )
        return new CDBResult();

	DBResultPtr res = new CDBResult(0,this);
    sqlite3_stmt* st = m_mapStatements.get(szSt);
    if ( st != null )
	{
		res->setStatement(st);
        return res;
	}
	
    int rc = sqlite3_prepare_v2(m_dbHandle, szSt, -1, &st, NULL);
    if ( !checkDbErrorEx(rc,*res) )
    {
        //TODO: raise exception
        return res;
    }

    res->setStatement(st);
    m_mapStatements.put(szSt, st);

    return res;
}

DBResultPtr CDBAdapter::executeSQLReportNonUniqueEx( const char* szSt, Vector<String>& arValues )
{
    DBResultPtr res = prepareStatement(szSt);
    if ( res->getStatement() == null )
        return res;

    for (int i = 0; i < (int)arValues.size(); i++ )
        bind(res->getStatement(), i+1, arValues.elementAt(i));

    res->setReportNonUnique(true);
    return executeStatement(res);
}

DBResultPtr CDBAdapter::executeSQLEx( const char* szSt, Vector<String>& arValues)
{
    DBResultPtr res = prepareStatement(szSt);
    if ( res->getStatement() == null )
        return res;

    for (int i = 0; i < (int)arValues.size(); i++ )
        bind(res->getStatement(), i+1, arValues.elementAt(i));

    return executeStatement(res);
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
            checkDbErrorEx(rc, *res);
            //TODO: raise exception
            return res;
        }
    }

    return res;
}

void CDBAdapter::Lock()
{ 
    if ( m_mxRuby.isMainRubyThread() )
        m_bUIWaitDB = true;

    m_mxRuby.Lock();
    m_mxDB.Lock(); 

    if ( m_mxRuby.isMainRubyThread() )
        m_bUIWaitDB = false;
}

void CDBAdapter::Unlock()
{ 
    m_mxDB.Unlock();
    m_mxRuby.Unlock();
}

void CDBAdapter::startTransaction()
{
    Lock();
	m_nTransactionCounter++;

    char *zErr = 0;
    int rc = 0;
	if ( m_dbHandle && m_nTransactionCounter == 1)
    {
		rc = sqlite3_exec(m_dbHandle, "BEGIN IMMEDIATE;",0,0,&zErr);
        checkDbError(rc);
    }
}

void CDBAdapter::endTransaction()
{
    char *zErr = 0;
    int rc = 0;

	m_nTransactionCounter--;
	if (m_dbHandle && m_nTransactionCounter == 0)
    {
        getAttrMgr().save(*this);
		rc = sqlite3_exec(m_dbHandle, "END;",0,0,&zErr);
        checkDbError(rc);
    }

    Unlock();
}

void CDBAdapter::rollback()
{
    char *zErr = 0;
    int rc = 0;

	m_nTransactionCounter--;
	if (m_dbHandle && m_nTransactionCounter == 0)
    {
		rc = sqlite3_exec(m_dbHandle, "ROLLBACK;",0,0,&zErr);
        checkDbError(rc);
    }

    Unlock();
}

/*static*/ void CDBAdapter::closeAll()
{
    for (Hashtable<String,CDBAdapter*>::iterator it = m_mapDBPartitions.begin();  it != m_mapDBPartitions.end(); ++it )
        it->second->close();
}

/*static*/ void CDBAdapter::initAttrManager()
{
    for (Hashtable<String,CDBAdapter*>::iterator it = m_mapDBPartitions.begin();  it != m_mapDBPartitions.end(); ++it )
        it->second->getAttrMgr().loadBlobAttrs(*(it->second));
}

/*static*/ CDBAdapter& CDBAdapter::getUserDB()
{
    return *getDBPartitions().get(USER_PARTITION_NAME());
}

/*static*/ CDBAdapter& CDBAdapter::getDB(const char* szPartition)
{
    return *getDBPartitions().get(szPartition);
}

/*static*/ boolean CDBAdapter::isAnyInsideTransaction()
{
    for (Hashtable<String,CDBAdapter*>::iterator it = m_mapDBPartitions.begin();  it != m_mapDBPartitions.end(); ++it )
    {
        if ( it->second->isInsideTransaction() )
            return true;
    }

    return false;
}

/*static*/ db::CDBAdapter& CDBAdapter::getDBByHandle(sqlite3* db)
{
    for (Hashtable<String,CDBAdapter*>::iterator it = m_mapDBPartitions.begin();  it != m_mapDBPartitions.end(); ++it )
    {
        if ( it->second->getDbHandle() == db )
            return *(it->second);
    }

    return *getDBPartitions().get(USER_PARTITION_NAME());
}

}
}

extern "C" {
using namespace rho::db;

int rho_db_open(const char* szDBPath, const char* szDBPartition, void** ppDB)
{
    CFilePath oPath(szDBPath);

    CDBAdapter* pDB = CDBAdapter::getDBPartitions().get(szDBPartition);
    if ( !pDB )
    {
        pDB = new CDBAdapter(szDBPartition, false);
        CDBAdapter::getDBPartitions().put(szDBPartition, pDB);
    }

    rho::String strVer = RhoRuby_getRhoDBVersion(); 
    pDB->open(szDBPath,strVer, false);

    *ppDB = pDB;
    return 0;
}

int rho_db_startTransaction(void* pDB)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    db.startTransaction();

    //TODO: get error code from DBException
    return 0;
}

int rho_db_commitTransaction(void* pDB)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    db.endTransaction();
    //TODO: get error code from DBException
    return 0;
}

int rho_db_rollbackTransaction(void* pDB)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    db.rollback();
    //TODO: get error code from DBException
    return 0;
}

int rho_db_is_ui_waitfordb(void* pDB)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    return db.isUIWaitDB() ? 1 :0;
}

extern "C" void
string_iter(const char* szVal, void* par)
{
    rho::Vector<rho::String>& ar = *((rho::Vector<rho::String>*)(par));
    ar.addElement(szVal);
}

int rho_db_destroy_tables(void* pDB, unsigned long arInclude, unsigned long arExclude)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);

    rho::Vector<rho::String> arIncludeTables;
    rho_ruby_enum_strary(arInclude, string_iter, &arIncludeTables);

    rho::Vector<rho::String> arExcludeTables;
    rho_ruby_enum_strary(arExclude, string_iter, &arExcludeTables);

    db.destroy_tables(arIncludeTables,arExcludeTables);
    return 0;
}

void* rho_db_get_handle(void* pDB)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    return db.getDbHandle();
}

void* rho_db_user_get_handle()
{
    rho::db::CDBAdapter& db = rho::db::CDBAdapter::getUserDB();
    return db.getDbHandle();
}

void* rho_db_user_get_adapter()
{
    rho::db::CDBAdapter& db = rho::db::CDBAdapter::getUserDB();
    return &db;
}

int rho_db_prepare_statement(void* pDB, const char* szSql, int nByte, sqlite3_stmt **ppStmt)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    return db.prepareSqlStatement(szSql, nByte, ppStmt);
}

void rho_db_lock(void* pDB)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    db.Lock();
}

void rho_db_unlock(void* pDB)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    db.Unlock();
}

int rho_db_is_table_exist(void* pDB, const char* szTableName)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    return db.isTableExist(szTableName) ? 1 : 0;
}

void rho_db_init_attr_manager()
{
    rho::db::CDBAdapter::initAttrManager();
}

}

namespace rho{
namespace common{

CRubyMutex::CRubyMutex(boolean bIgnore) : m_nLockCount(0), m_valThread(0), m_valMutex(null)
{
    if ( !bIgnore )
        m_valMutex = rho_ruby_create_mutex();
}

CRubyMutex::~CRubyMutex()
{
    if ( m_valMutex )
        rho_ruby_destroy_mutex(m_valMutex);
}

boolean CRubyMutex::isMainRubyThread()
{
    return rho_ruby_main_thread() == rho_ruby_current_thread();
}

void CRubyMutex::Lock()
{
    if ( m_valMutex == null )
        return;

    unsigned long curThread = rho_ruby_current_thread();
    if ( curThread == null )
        return;

    if ( m_valThread != curThread )
    {
        rho_ruby_lock_mutex(m_valMutex);
        m_valThread = curThread;
        m_nLockCount = 1;
    }else
        m_nLockCount += 1;
}

void CRubyMutex::Unlock()
{
    if ( m_valMutex == null )
        return;

    m_nLockCount--;
    if ( m_nLockCount == 0 )
    {
        m_valThread = null;
        rho_ruby_unlock_mutex(m_valMutex);
    }
}

}
}