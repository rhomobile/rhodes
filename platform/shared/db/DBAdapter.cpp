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
#include "DBAdapter.h"
#include "sync/RhoconnectClientManager.h"

#include "common/RhoFile.h"
#include "common/RhoFilePath.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "common/RhoAppAdapter.h"
#include "common/Tokenizer.h"
#ifndef RHO_NO_RUBY 
#include "ruby/ext/rho/rhoruby.h"
#ifndef WINDOWS_PLATFORM
#include "ruby/ruby.h"
#endif
#endif //RHO_NO_RUBY
#include "common/app_build_configs.h"
#include "DBImportTransaction.h"
#include "DBRequestHelper.h"

#include <sstream>


#undef DEBUG_SQL_TRACE

int rho_sys_zip_files_with_path_array_ptr(const char* szZipFilePath, const char *base_path, const rho::Vector<rho::String>& arFiles, const char* psw);

namespace rho{
namespace db{
IMPLEMENT_LOGCLASS(CDBAdapter,"DB");
HashtablePtr<String,CDBAdapter*> CDBAdapter::m_mapDBPartitions;

using namespace rho::common;
using namespace rho;	

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
        String strFilePath = RHODESAPPBASE().resolveDBFilesPath((char*)sqlite3_value_text(*(ppArgs)));
        CRhoFile::deleteFile(strFilePath.c_str());
    }

    //attrMgr.remove( nSrcID, szAttrName );
}

void SyncBlob_UpdateCallback(sqlite3_context* dbContext, int nArgs, sqlite3_value** ppArgs)
{
    if ( nArgs < 3 )
        return;

    CDBAttrManager& attrMgr = CDBAdapter::getDBByHandle(sqlite3_context_db_handle(dbContext)).getAttrMgr();

    char* szAttrName = (char*)sqlite3_value_text(*(ppArgs+2));
    int nSrcID = sqlite3_value_int(*(ppArgs+1));
    if ( attrMgr.isBlobAttr(nSrcID, szAttrName) )
    {
        String strFilePath = RHODESAPPBASE().resolveDBFilesPath((char*)sqlite3_value_text(*(ppArgs)));
        CRhoFile::deleteFile(strFilePath.c_str());
    }
}

void SyncBlob_DeleteSchemaCallback(sqlite3_context* dbContext, int nArgs, sqlite3_value** ppArgs)
{
    //LOG(INFO) + "SyncBlob_DeleteSchemaCallback";

    const char* szValue = (char*)sqlite3_value_text(*ppArgs);
    if ( szValue && *szValue )
    {
        String strFilePath = RHODESAPPBASE().resolveDBFilesPath(szValue);
        CRhoFile::deleteFile(strFilePath.c_str());
    }
}

void SyncBlob_UpdateSchemaCallback(sqlite3_context* dbContext, int nArgs, sqlite3_value** ppArgs)
{
    //LOG(INFO) + "SyncBlob_UpdateSchemaCallback";
    const char* szOldValue = (char*)sqlite3_value_text(*(ppArgs+0));
    const char* szNewValue = (char*)sqlite3_value_text(*(ppArgs+1));
    if ( szOldValue == szNewValue || szOldValue == 0 )
        return;

    if ( szOldValue && szNewValue &&  strcmp(szOldValue, szNewValue) == 0 )
        return;

    if ( szOldValue && *szOldValue )
    {
        String strFilePath = RHODESAPPBASE().resolveDBFilesPath(szOldValue);
        CRhoFile::deleteFile(strFilePath.c_str());
    }
}

#ifdef DEBUG_SQL_TRACE

void SqliteTraceCallback(void* aDb, const char* aQueryStr) {
    LOGC(INFO,"DBT") + "SQLite trace: sql=[" + aQueryStr + "]";

}

void SqliteProfileCallback(void* aDb, const char* aQueryStr,
                           sqlite3_uint64 aTimeInNs)
{
    sqlite3* db = static_cast<sqlite3*>(aDb);
    //const char* dbName = sqlite3_db_filename(db, "main");

    // Statistics per DB connection.
    // See: http://www.sqlite.org/c3ref/db_status.html
    int cacheUsed[2]  = { 0, 0 };
    int schemaUsed[2] = { 0, 0 };
    int stmtUsed[2]   = { 0, 0 };
    int cacheHit[2]   = { 0, 0 };
    int cacheMiss[2]  = { 0, 0 };
    ;
    sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED,  &cacheUsed[0],
                      &cacheUsed[1],  0);
    sqlite3_db_status(db, SQLITE_DBSTATUS_SCHEMA_USED, &schemaUsed[0],
                      &schemaUsed[1], 0);
    sqlite3_db_status(db, SQLITE_DBSTATUS_STMT_USED,   &stmtUsed[0],
                      &stmtUsed[1],   0);
    //    sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_HIT,   &cacheHit[0],
    //                      &cacheHit[1],   0);
    //    sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_MISS,  &cacheMiss[0],
    //                      &cacheMiss[1],  0);

#define BUFFER_SIZE 1000
    static char s_buff[BUFFER_SIZE];

    //

    int size = snprintf(s_buff,BUFFER_SIZE,"SQLite profile: msec=[%llu] mem/high/lim=[%lld/%lld/%lld] "
                        "schema=[%d] stmt=[%d] cache=[%d]"
                        "hit/miss=[%d/%d] sql=[%s]",
                        aTimeInNs/1000000,             // Time taken by the query in milliseconds.

                        sqlite3_memory_used(),         // Global memory used by SQLite now in bytes.
                        sqlite3_memory_highwater(0),   // Global high water mark of memory used by SQLite in bytes.
                        sqlite3_soft_heap_limit64(-1), // Global current heap limit in bytes (a hint only).

                        schemaUsed[0],                 // Memory used by this connection for the schema.
                        stmtUsed[0],                   // Memory used by this connection for statements.
                        cacheUsed[0],                  // Memory used by this connection for cache.
                        
                        cacheHit[0], cacheMiss[0],     // SQLite cache hit/miss stats.
                        aQueryStr);
    s_buff[size] = 0;
    
    LOGC(INFO,"DBI") + s_buff;
}
#endif

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

    res.getDBError().setError(nErrCode, szErrMsg);
    if ( nErrCode == SQLITE_CONSTRAINT && res.getReportNonUnique() )
        return true;

    LOG(ERROR)+"DB query failed. rc: " + rc + "; Error code: " + nErrCode + "; Message: " + szErrMsg;
    return false;
}

void CDBAdapter::open (String strDbPath, String strVer, boolean bTemp, boolean checkImportState)
{
    if ( strcasecmp(strDbPath.c_str(),m_strDbPath.c_str() ) == 0 && m_dbHandle )
        return;

    LOG(INFO) + "Open DB: " + strDbPath;
    //close();

    m_mxRuby.create();
	
    m_strDbPath = strDbPath;
    if ( !bTemp )
    {
        m_strDbVerPath = strDbPath+".version";
        m_strDbVer = strVer;

        checkDBVersion(strVer);
    }	
	
	if (checkImportState) {
		CDBImportTransaction importTxn(*this);
		if (importTxn.pending()) {
			//if (!importTxn.commit()) {
				importTxn.rollback();
			//}
		}
	}

    boolean bExist = CRhoFile::isFileExist(strDbPath.c_str());
    int nRes = sqlite3_open(strDbPath.c_str(),&m_dbHandle);
    if ( !checkDbError(nRes) )
        return;
    //TODO: raise exception if error

    //if (RHOCONF().getBool("encrypt_database"))
    const char* szEncrypt = get_app_build_config_item("encrypt_database");
    if ( szEncrypt && strcmp(szEncrypt, "1") == 0 )
    {
        m_ptrCrypt = rho_get_RhoClassFactory()->createRhoCrypt();
        if ( m_strCryptKey.length() > 0 )
            m_ptrCrypt->set_db_CryptKey( m_strDbPartition.c_str(), m_strCryptKey.c_str(), !bTemp );

        CDBError dbError;
        String strKey = "PRAGMA key = \"";
        strKey += m_strDbPartition + "\";";
	    executeBatch(strKey.c_str(), dbError);
    }

    if ( !bExist )
        createSchema();

    sqlite3_create_function( m_dbHandle, "rhoOnDeleteObjectRecord", 3, SQLITE_ANY, 0,
	    SyncBlob_DeleteCallback, 0, 0 );
    sqlite3_create_function( m_dbHandle, "rhoOnUpdateObjectRecord", 3, SQLITE_ANY, 0,
	    SyncBlob_UpdateCallback, 0, 0 );
    sqlite3_create_function( m_dbHandle, "rhoOnDeleteSchemaRecord", 1, SQLITE_ANY, 0,
	    SyncBlob_DeleteSchemaCallback, 0, 0 );
    sqlite3_create_function( m_dbHandle, "rhoOnUpdateSchemaRecord", 2, SQLITE_ANY, 0,
	    SyncBlob_UpdateSchemaCallback, 0, 0 );

    #ifdef DEBUG_SQL_TRACE
    sqlite3_profile( m_dbHandle, SqliteProfileCallback, m_dbHandle);

    sqlite3_trace( m_dbHandle, SqliteTraceCallback, m_dbHandle);
    #endif

    sqlite3_busy_handler(m_dbHandle, onDBBusy, 0 );

    //getAttrMgr().load(*this);

    //copy client_info table
    if ( !bTemp && !bExist && CRhoFile::isFileExist((strDbPath+"_oldver").c_str()) )
    {
        LOG(INFO) + "Copy client_info table from old database";
        CDBAdapter db(m_strDbPartition.c_str(), true);
        db.open( strDbPath+"_oldver", m_strDbVer, true, false );
        copyTable( "client_info", db, *this );
        {
            IDBResult res = executeSQL( "SELECT client_id FROM client_info" );
            if ( !res.isEnd() &&  res.getStringByIdx(0).length() > 0 )
            {
                LOG(INFO) + "Set reset=1 in client_info";
                executeSQL( "UPDATE client_info SET reset=1" );
            }
        }

//        db.updateAllAttribChanges();
//        db.copyTable("changed_values", db, *this );
        db.close();

        CRhoFile::deleteFile( (m_strDbPath+"_oldver").c_str());
        CRhoFile::deleteFile( (m_strDbPath+"_oldver-journal").c_str());
    }

}

void CDBAdapter::CDBVersion::fromFile(const String& strFilePath)//throws Exception
{
    String strData;
    CRhoFile::readStringFromFile(strFilePath.c_str(), strData);
    
    CTokenizer oTokenizer( strData, ";" );
    int nPos = 0;
	while (oTokenizer.hasMoreTokens()) 
    {
		String tok = oTokenizer.nextToken();
		tok = String_trim(tok);
		
		switch(nPos)
		{
		case 0:
			m_strRhoVer = tok;
			break;
		case 1:
			m_strAppVer = tok;
			break;
		case 2:
			m_bEncrypted = tok.compare("encrypted") == 0;
			break;
		}
		nPos++;
    }
}

void CDBAdapter::CDBVersion::toFile(const String& strFilePath)const//throws Exception
{
	String strFullVer = m_strRhoVer + ";" + m_strAppVer + 
		";" + (m_bEncrypted ? "encrypted":"");
	
	//try{
        CRhoFile::deleteFile( strFilePath.c_str() );
        CRhoFile::writeStringToFile(strFilePath.c_str(), strFullVer);
	//}catch (Exception e) {
   // 	LOG.ERROR("writeDBVersion failed.", e);
    //	throw e;
    //}
}

boolean CDBAdapter::migrateDB(const CDBVersion& dbVer, const CDBVersion& dbNewVer )
{
    LOG(INFO) + "Try migrate database from " + dbVer.m_strRhoVer + " to " + dbNewVer.m_strRhoVer;
    if ( (dbVer.m_strRhoVer.find("1.4") == 0)&& (dbNewVer.m_strRhoVer.find("1.5")==0||dbNewVer.m_strRhoVer.find("1.4")==0) )
    {
        LOG(INFO) + "No migration required from " + dbVer.m_strRhoVer + " to " + dbNewVer.m_strRhoVer;
        dbNewVer.toFile(m_strDbVerPath);
        return true;
    }

    if ( (dbVer.m_strRhoVer.find("2.0") == 0||dbVer.m_strRhoVer.find("2.1") == 0||dbVer.m_strRhoVer.find("2.2") == 0)&& 
         (dbNewVer.m_strRhoVer.find("2.0")==0||dbNewVer.m_strRhoVer.find("2.1")==0||dbNewVer.m_strRhoVer.find("2.2")==0) )
    {
        LOG(INFO) + "No migration required from " + dbVer.m_strRhoVer + " to " + dbNewVer.m_strRhoVer;
        dbNewVer.toFile(m_strDbVerPath);
        return true;
    }

    //1.2.x -> 1.5.x,1.4.x
    if ( (dbVer.m_strRhoVer.find("1.2") == 0)&& (dbNewVer.m_strRhoVer.find("1.5")==0||dbNewVer.m_strRhoVer.find("1.4")==0) )
    {
    //sources
    //priority INTEGER, ADD
    //backend_refresh_time int default 0, ADD
    //id INTEGER PRIMARY KEY, REMOVE

    //changed_values
    //id INTEGER PRIMARY KEY, REMOVE
        LOG(INFO) + "Migrate database from " + dbVer.m_strRhoVer + " to " + dbNewVer.m_strRhoVer;

        CDBAdapter db(m_strDbPartition.c_str(), true);
        db.open( m_strDbPath, m_strDbVer, true, false );
        IDBResult res = db.executeSQL( "ALTER TABLE sources ADD priority INTEGER" );
        IDBResult res1 = db.executeSQL( "ALTER TABLE sources ADD backend_refresh_time int default 0" );

        {
            Vector<int> vecSrcIds;
            IDBResult res2 = db.executeSQL( "SELECT source_id FROM sources" );
            for ( ; !res2.isEnd(); res2.next() )
                vecSrcIds.addElement(res2.getIntByIdx(0));

            for( size_t i = 0; i < vecSrcIds.size(); i++)
            {
                IDBResult res3 = db.executeSQL( "UPDATE sources SET priority=? where source_id=?", 
                    vecSrcIds.elementAt(i), vecSrcIds.elementAt(i) );
            }
        }
        db.close();

        dbNewVer.toFile(m_strDbVerPath);

        return true;
    }

    return false;
}

void CDBAdapter::checkDBVersion(String& strRhoDBVer)
{
    CDBVersion dbNewVer;
	dbNewVer.m_strRhoVer = strRhoDBVer; 
	dbNewVer.m_strAppVer = RHOCONF().getString("app_db_version");

    const char* szEncrypt = get_app_build_config_item("encrypt_database");
	dbNewVer.m_bEncrypted = szEncrypt && strcmp(szEncrypt, "1") == 0;
		
	CDBVersion dbVer;  
	dbVer.fromFile(m_strDbVerPath);

	if (dbVer.m_strRhoVer.length() == 0 )
	{
		dbNewVer.toFile(m_strDbVerPath);
		return;
	}
	
	boolean bRhoReset = dbVer.isRhoVerChanged(dbNewVer);
    boolean bAppReset = dbVer.isAppVerChanged(dbNewVer);
	
	boolean bDbFormatChanged = dbVer.isDbFormatChanged(dbNewVer);
	if ( !bDbFormatChanged && dbVer.m_bEncrypted )
	{
		//if (!com.rho.RhoCrypto.isKeyExist(strEncryptionInfo) )
		//	bDbFormatChanged = true;
	}
	
	if ( bDbFormatChanged )
		LOG(INFO) + "Reset Database( format changed ):" + m_strDbPath;
	
    if ( bRhoReset && !bAppReset && !bDbFormatChanged )
        bRhoReset = !migrateDB(dbVer, dbNewVer);

    if ( bRhoReset || bAppReset || bDbFormatChanged)
	{
        LOG(INFO) + "Reset database because version is changed.";

        CRhoFile::deleteFile( (m_strDbPath+"_oldver").c_str());
        CRhoFile::deleteFile( (m_strDbPath+"_oldver-journal").c_str());

        if ( bDbFormatChanged )
        {
            CRhoFile::deleteFile( m_strDbPath.c_str() );
            CRhoFile::deleteFile((m_strDbPath+"-journal").c_str());
        }else
        {
            CRhoFile::renameFile( m_strDbPath.c_str(), (m_strDbPath+"_oldver").c_str());
            CRhoFile::renameFile((m_strDbPath+"-journal").c_str(), (m_strDbPath+"_oldver-journal").c_str());
        }

        if ( m_strDbPartition.compare("user") == 0 ) //do it only once
            CRhoFile::deleteFilesInFolder(RHODESAPPBASE().getBlobsDirPath().c_str());

        dbNewVer.toFile(m_strDbVerPath);

        if ( RHOCONF().isExist("bulksync_state") && RHOCONF().getInt("bulksync_state") != 0)
            RHOCONF().setInt("bulksync_state", 0, true);
    }
}

sqlite3_stmt* CDBAdapter::createInsertStatement(IDBResult& res, const String& tableName, CDBAdapter& db, String& strInsert)
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
                rc = sqlite3_bind_text(stInsert, nBindCol, NULL, -1, SQLITE_TRANSIENT);
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
    Lock();
    IDBResult res = executeSQL( "SELECT name FROM sqlite_master WHERE type='table' AND name=?", strTableName.c_str() );
    boolean bRes = !res.isEnd();
    Unlock();
    return bRes;
}

void CDBAdapter::destroy_tables(const rho::Vector<rho::String>& arIncludeTables, const rho::Vector<rho::String>& arExcludeTables)
{
    //getAttrMgr().reset(*this);
    CFilePath oFilePath(m_strDbPath);
	String dbNewName  = oFilePath.changeBaseName("resetdbtemp.sqlite");

    CRhoFile::deleteFile(dbNewName.c_str());
    CRhoFile::deleteFile((dbNewName+"-journal").c_str());
    CRhoFile::deleteFile((dbNewName+".version").c_str());

    CDBAdapter db(m_strDbPartition.c_str(), true);
    db.open( dbNewName, m_strDbVer, true, false );

    //Copy all tables

    Vector<String> vecTables;
    {
        IDBResult res = executeSQL( "SELECT name FROM sqlite_master WHERE type='table' " );
        for ( ; !res.isEnd(); res.next() )
            vecTables.addElement(res.getStringByIdx(0));
    }

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

    CRhoFile::deleteFilesInFolder(RHODESAPPBASE().getBlobsDirPath().c_str());

    CRhoFile::deleteFile(dbOldName.c_str());
    CRhoFile::renameFile(dbNewName.c_str(),dbOldName.c_str());
    open( dbOldName, m_strDbVer, false, false );
}

void CDBAdapter::copyTable(String tableName, CDBAdapter& dbFrom, CDBAdapter& dbTo)
{
    /* Create table on destination DB if it doesn't exists */
    if (!dbTo.isTableExist(tableName)) {
        IDBResult res = dbFrom.executeSQL("SELECT sql FROM sqlite_master WHERE type='table' AND name=?", tableName);
        if ( !res.isEnd() ) {
            String createTableStatement = res.getStringByIdx(0);
            dbTo.executeSQL(createTableStatement.c_str());
        }
    }

    String strSelect = "SELECT * from " + tableName;
    IDBResult res = dbFrom.executeSQL( strSelect.c_str() );
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
    
// add file.blob attrib_type for blob attribs in changed_values
void CDBAdapter::updateAllBlobAttribChanges()
    {
        //Check for attrib = object
        IDBResult res = executeSQL("SELECT object, source_id, attrib "
                                   "FROM changed_values where update_type = 'update' and sent=0");
        
        if ( res.isEnd() )
            return;
        
        startTransaction();
        
        Vector<String> arObj, arAttrib;
        Vector<int> arSrcID;
        for( ; !res.isEnd(); res.next() )
        {
            arObj.addElement(res.getStringByIdx(0));
            arSrcID.addElement(res.getIntByIdx(1));
            arAttrib.addElement(res.getStringByIdx(2));
        }
        
        for( int i = 0; i < (int)arObj.size(); i++ )
        {
            String attribType = getAttrMgr().isBlobAttr(arSrcID.elementAt(i), arAttrib.elementAt(i).c_str()) ? "blob.file" : "";
            if (attribType.length() > 0) {
                executeSQL("UPDATE changed_values SET attrib_type=? WHERE source_id=? and object=? and attrib=? and sent=0", attribType, arSrcID.elementAt(i), arObj.elementAt(i), arAttrib.elementAt(i));
            }
        }
        
        endTransaction();
    }
    

void CDBAdapter::updateAllAttribChanges()
{
    //Check for attrib = object
    IDBResult res = executeSQL("SELECT object, source_id, update_type "
        "FROM changed_values where attrib = 'object' and sent=0");

    if ( res.isEnd() )
        return;

    startTransaction();

    Vector<String> arObj, arUpdateType;
    Vector<int> arSrcID;
    for( ; !res.isEnd(); res.next() )
    {
        arObj.addElement(res.getStringByIdx(0));
        arSrcID.addElement(res.getIntByIdx(1));
        arUpdateType.addElement(res.getStringByIdx(2));
    }

    for( int i = 0; i < (int)arObj.size(); i++ )
    {
        IDBResult resSrc = executeSQL("SELECT name, schema FROM sources where source_id=?", arSrcID.elementAt(i) );
        boolean bSchemaSource = false;
        String strTableName = "object_values";
        if ( !resSrc.isEnd() )
        {
            bSchemaSource = resSrc.getStringByIdx(1).length() > 0;
            if ( bSchemaSource )
                strTableName = resSrc.getStringByIdx(0);
        }

        if (bSchemaSource)
        {
            IDBResult res2 = executeSQL((String("SELECT * FROM ") + strTableName + " where object=?").c_str(), arObj.elementAt(i) );
            for( int j = 0; j < res2.getColCount(); j ++)
            {
                if ( res2.isNullByIdx(j) )
                    continue;

                String strAttrib = res2.getColName(j);
                String value = res2.getStringByIdx(j);
                String attribType = getAttrMgr().isBlobAttr(arSrcID.elementAt(i), strAttrib.c_str()) ? "blob.file" : "";

                executeSQLReportNonUnique("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type,sent) VALUES(?,?,?,?,?,?,?)", 
                    arSrcID.elementAt(i), arObj.elementAt(i), strAttrib, value, arUpdateType.elementAt(i), attribType, 0);
            }
        }else
        {
            IDBResult res2 = executeSQL((String("SELECT attrib, value FROM ") + strTableName + " where object=? and source_id=?").c_str(), 
                arObj.elementAt(i), arSrcID.elementAt(i) );

            for( ; !res2.isEnd(); res2.next() )
            {
                if ( res2.isNullByIdx(1) )
                    continue;

                String strAttrib = res2.getStringByIdx(0);
                String value = res2.getStringByIdx(1);
                String attribType = getAttrMgr().isBlobAttr(arSrcID.elementAt(i), strAttrib.c_str()) ? "blob.file" : "";

                executeSQLReportNonUnique("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type,sent) VALUES(?,?,?,?,?,?,?)", 
                    arSrcID.elementAt(i), arObj.elementAt(i), strAttrib, value, arUpdateType.elementAt(i), attribType, 0);
            }
        }
    }

    executeSQL("DELETE FROM changed_values WHERE attrib='object'"); 

    endTransaction();
}

void CDBAdapter::updateFullUpdateChanges(int nSrcID)
{
    IDBResult res = executeSQL("SELECT object, source_id, update_type, attrib FROM changed_values where source_id=? and update_type=? and sent=0", nSrcID, "update");
    if ( res.isEnd() )
        return;

    startTransaction();

    Vector<String> arObj, arUpdateType, arAttribs;
    Vector<int> arSrcID;
    for( ; !res.isEnd(); res.next() )
    {
        arObj.addElement(res.getStringByIdx(0));
        arSrcID.addElement(res.getIntByIdx(1));
        arUpdateType.addElement(res.getStringByIdx(2));
        arAttribs.addElement(res.getStringByIdx(3));
    }

    Hashtable<String, int> hashObjs;
    for( int i = 0; i < (int)arObj.size(); i++ )
    {
        if (hashObjs.containsKey(arObj.elementAt(i)))
            continue;
        hashObjs.put(arObj.elementAt(i), 1);

        IDBResult resSrc = executeSQL("SELECT name, schema FROM sources where source_id=?", arSrcID.elementAt(i) );
        boolean bSchemaSource = false;
        String strTableName = "object_values";
        if ( !resSrc.isEnd() )
        {
            bSchemaSource = resSrc.getStringByIdx(1).length() > 0;
            if ( bSchemaSource )
                strTableName = resSrc.getStringByIdx(0);
        }

        if (bSchemaSource)
        {
            IDBResult res2 = executeSQL((String("SELECT * FROM ") + strTableName + " where object=?").c_str(), arObj.elementAt(i) );
            for( int j = 0; j < res2.getColCount(); j ++)
            {
                if ( res2.isNullByIdx(j) )
                    continue;

                String strAttrib = res2.getColName(j);
                String value = res2.getStringByIdx(j);
                String attribType = getAttrMgr().isBlobAttr(arSrcID.elementAt(i), strAttrib.c_str()) ? "blob.file" : "";

                boolean bDuplicate = false;
                for( int k = i; k < (int)arObj.size(); k++ )
                {
                    if ( arObj.elementAt(k) == arObj.elementAt(i) && strAttrib == arAttribs.elementAt(k) )
                    {
                        bDuplicate = true;
                        break;
                    }
                }
                if ( bDuplicate )
                    continue;

                executeSQLReportNonUnique("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type,sent) VALUES(?,?,?,?,?,?,?)", 
                    arSrcID.elementAt(i), arObj.elementAt(i), strAttrib, value, arUpdateType.elementAt(i), attribType, 0);
            }
        }else
        {
            IDBResult res2 = executeSQL((String("SELECT attrib, value FROM ") + strTableName + " where object=? and source_id=?").c_str(), 
                arObj.elementAt(i), arSrcID.elementAt(i) );

            for( ; !res2.isEnd(); res2.next() )
            {
                if ( res2.isNullByIdx(1) )
                    continue;

                String strAttrib = res2.getStringByIdx(0);
                String value = res2.getStringByIdx(1);
                String attribType = getAttrMgr().isBlobAttr(arSrcID.elementAt(i), strAttrib.c_str()) ? "blob.file" : "";

                boolean bDuplicate = false;
                for( int k = i; k < (int)arObj.size(); k++ )
                {
                    if ( arObj.elementAt(k) == arObj.elementAt(i) && strAttrib == arAttribs.elementAt(k) )
                    {
                        bDuplicate = true;
                        break;
                    }
                }
                if ( bDuplicate )
                    continue;

                executeSQLReportNonUnique("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type,sent) VALUES(?,?,?,?,?,?,?)", 
                    arSrcID.elementAt(i), arObj.elementAt(i), strAttrib, value, arUpdateType.elementAt(i), attribType, 0);
            }
        }
    }

    endTransaction();
}

void CDBAdapter::copyChangedValues(CDBAdapter& db)
{
    updateAllAttribChanges();

    copyTable("changed_values", *this, db );
    {
        Vector<int> arOldSrcs;
        {
            IDBResult resSrc = db.executeSQL( "SELECT DISTINCT(source_id) FROM changed_values" );
            for ( ; !resSrc.isEnd(); resSrc.next() )
                arOldSrcs.addElement( resSrc.getIntByIdx(0) );
        }
        for( int i = 0; i < (int)arOldSrcs.size(); i++)
        {
            int nOldSrcID = arOldSrcs.elementAt(i);

            IDBResult res = executeSQL("SELECT name from sources WHERE source_id=?", nOldSrcID);
            if ( !res.isEnd() )
            {
                String strSrcName = res.getStringByIdx(0);

                IDBResult res2 = db.executeSQL("SELECT source_id from sources WHERE name=?", strSrcName);
                if ( !res2.isEnd() )
                {
                    if ( nOldSrcID != res2.getIntByIdx(0) )
                        db.executeSQL("UPDATE changed_values SET source_id=? WHERE source_id=?", res2.getIntByIdx(0), nOldSrcID);

                    continue;
                }
            }

            //source not exist in new partition, remove this changes
            db.executeSQL("DELETE FROM changed_values WHERE source_id=?", nOldSrcID);
        }
    }
}

void CDBAdapter::setBulkSyncDB(String fDataName, String strCryptKey)
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {
    CDBAdapter db(m_strDbPartition.c_str(), true);
    db.setCryptKey(strCryptKey);
    db.open( fDataName, m_strDbVer, true, false );
    db.createTriggers();

    db.startTransaction();

    copyTable("client_info", *this, db );
    copyChangedValues(db);

    getDBPartitions().put(m_strDbPartition.c_str(), &db);
//    sync::CSyncThread::getSyncEngine().applyChangedValues(db);
	rho::sync::RhoconnectClientManager::syncEngineApplyChangedValues(db);
    getDBPartitions().put(m_strDbPartition.c_str(), this);

    db.endTransaction();
    db.close();

    String dbOldName = m_strDbPath;
    close(false);

    CRhoFile::deleteFilesInFolder(RHODESAPPBASE().getBlobsDirPath().c_str());

    CRhoFile::deleteFile(dbOldName.c_str());
    CRhoFile::renameFile(fDataName.c_str(),dbOldName.c_str());
    setCryptKey(strCryptKey);
    open( dbOldName, m_strDbVer, false, false );
	}
}
	
	void CDBAdapter::setImportDB(String fDataName, String strCryptKey)
	{
		CDBAdapter db(m_strDbPartition.c_str(), true);
		db.setCryptKey(strCryptKey);
		db.open( fDataName, m_strDbVer, true, false );
		//db.createTriggers();
		
		db.startTransaction();
		
		copyTable("client_info", *this, db );
		//copyChangedValues(db);
		
		//getDBPartitions().put(m_strDbPartition.c_str(), &db);
		//sync::CSyncThread::getSyncEngine().applyChangedValues(db);
		//getDBPartitions().put(m_strDbPartition.c_str(), this);
		
		db.endTransaction();
		db.close();
		
		String dbOldName = m_strDbPath;
		close(false);
		
		CRhoFile::deleteFilesInFolder(RHODESAPPBASE().getBlobsDirPath().c_str());
		
		CRhoFile::deleteFile(dbOldName.c_str());
		CRhoFile::renameFile(fDataName.c_str(),dbOldName.c_str());
		setCryptKey(strCryptKey);
		open( dbOldName, m_strDbVer, false, false );
	}

void CDBAdapter::executeBatch(const char* szSql, CDBError& error)
{
    char* errmsg = 0;
    int rc = sqlite3_exec(m_dbHandle, szSql,  NULL, NULL, &errmsg);
	
    if ( rc != SQLITE_OK )
        LOG(ERROR)+"execute batch failed. Error code: " + rc + ";Message: " + (errmsg ? errmsg : "");
	
	error.setError(rc, errmsg);
	
    if ( errmsg )
        sqlite3_free(errmsg);
}
	
void CDBAdapter::createSchema()
{
#ifdef RHODES_EMULATOR
    String strPath = CFilePath::join( RHOSIMCONF().getRhodesPath(), "platform/shared/db/res/db/syncdb.schema" );
#else
    String strPath = CFilePath::join( RHODESAPP().getRhoRootPath(), "db/syncdb.schema" );
#endif

    String strSqlScript;
    CRhoFile::loadTextFile(strPath.c_str(), strSqlScript);

    if ( strSqlScript.length() == 0 )
    {
        LOG(ERROR)+"createSchema failed. Cannot read schema file: " + strSqlScript;
        return;
    }

	CDBError dbError;
	executeBatch(strSqlScript.c_str(), dbError);
	
    if ( dbError.isOK() )
        createTriggers();
}

void CDBAdapter::createTriggers()
{
    char* errmsg = 0;
#ifdef RHODES_EMULATOR
    String strPath = CFilePath::join( RHOSIMCONF().getRhodesPath(), "platform/shared/db/res/db/syncdb.triggers" );
#else
    String strPath = CFilePath::join( RHODESAPP().getRhoRootPath(), "db/syncdb.triggers" );
#endif

    String strSqlTriggers;
    CRhoFile::loadTextFile(strPath.c_str(), strSqlTriggers);

    if ( strSqlTriggers.length() == 0 )
    {
        LOG(ERROR)+"createTriggers failed. Cannot read triggers file: " + strSqlTriggers;
        return;
    }

    int rc = sqlite3_exec(m_dbHandle, strSqlTriggers.c_str(),  NULL, NULL, &errmsg);

    if ( rc != SQLITE_OK )
        LOG(ERROR)+"createTriggers failed. Error code: " + rc + ";Message: " + (errmsg ? errmsg : "");

    if ( errmsg )
        sqlite3_free(errmsg);
}

void CDBAdapter::createTrigger(const String& strSQL)
{
    char* errmsg = 0;
    int rc = sqlite3_exec(m_dbHandle, strSQL.c_str(),  NULL, NULL, &errmsg);

    if ( rc != SQLITE_OK )
        LOG(ERROR)+"createTrigger failed. Error code: " + rc + ";Message: " + (errmsg ? errmsg : "");
}

void CDBAdapter::dropTrigger(const String& strName)
{
    String strSQL = "DROP TRIGGER " + strName + ";";
    char* errmsg = 0;
    int rc = sqlite3_exec(m_dbHandle, strSQL.c_str(),  NULL, NULL, &errmsg);

    if ( rc != SQLITE_OK )
        LOG(ERROR)+"dropTrigger failed. Error code: " + rc + ";Message: " + (errmsg ? errmsg : "");
}

void CDBAdapter::close(boolean bCloseRubyMutex/* = true*/)
{
    for (Hashtable<String,sqlite3_stmt*>::iterator it = m_mapStatements.begin();  it != m_mapStatements.end(); ++it )
        sqlite3_finalize( it->second );

    m_mapStatements.clear();

    if ( m_dbHandle != 0 )
        sqlite3_close(m_dbHandle);

    m_dbHandle = 0;
    m_strDbPath = String();

    m_nTransactionCounter = 0;

    m_ptrCrypt = 0;
    m_strCryptKey = "";

    if ( bCloseRubyMutex )
        m_mxRuby.close();
}

int CDBAdapter::prepareSqlStatement(const char* szSql, int nByte, sqlite3_stmt **ppStmt)
{
    int rc = SQLITE_OK;
    sqlite3_stmt* st = m_mapStatements.get(szSql);
    if ( st != NULL )
        *ppStmt	 = st;
    else
    {
        rc = sqlite3_prepare_v2(m_dbHandle, szSql, nByte, ppStmt, NULL);
        if ( rc == SQLITE_OK )
            m_mapStatements.put(szSql, *ppStmt);
    }

    return rc;
}

DBResultPtr CDBAdapter::prepareStatement( const char* szSt )
{
    if ( m_dbHandle == NULL )
        return new CDBResult();

	DBResultPtr res = new CDBResult(0,this);
    sqlite3_stmt* st = m_mapStatements.get(szSt);
    if ( st != NULL )
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
    if ( res->getStatement() == NULL )
        return res;

    for (int i = 0; i < (int)arValues.size(); i++ )
        bind(res->getStatement(), i+1, arValues.elementAt(i));

    res->setReportNonUnique(true);
    return executeStatement(res, szSt);
}

DBResultPtr CDBAdapter::executeSQLEx( const char* szSt, Vector<String>& arValues)
{
    DBResultPtr res = prepareStatement(szSt);
    if ( res->getStatement() == NULL )
        return res;

    for (int i = 0; i < (int)arValues.size(); i++ )
        bind(res->getStatement(), i+1, arValues.elementAt(i));

    return executeStatement(res, szSt);
}

DBResultPtr CDBAdapter::executeSQL( const char* szSt)
{
    DBResultPtr res = prepareStatement(szSt);
    if ( res->getStatement() == NULL )
        return res;

    return executeStatement(res, szSt);
}

DBResultPtr CDBAdapter::executeStatement(DBResultPtr& res, const char* szSt)
{
    //LOG(INFO) + "executeStatement:" + szSt;

    int rc = sqlite3_step(res->getStatement());
    if ( rc != SQLITE_ROW )
    {
        res->setStatement(NULL);
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

    if (m_dbHandle && m_nTransactionCounter == 0)
    {
        char *zErr = 0;
        int rc = sqlite3_exec(m_dbHandle, "BEGIN IMMEDIATE;",0,0,&zErr);
        checkDbError(rc);
    }

    m_nTransactionCounter++;
}

void CDBAdapter::endTransaction()
{
    if (m_nTransactionCounter == 0)
    {
        return;
    }

    m_nTransactionCounter--;

    if (m_dbHandle && m_nTransactionCounter == 0)
    {
        //getAttrMgr().save(*this);
        char *zErr = 0;
        int rc = sqlite3_exec(m_dbHandle, "END;",0,0,&zErr);
        checkDbError(rc);
    }

    Unlock();
}

void CDBAdapter::rollback()
{
    if (m_nTransactionCounter == 0)
    {
        return;
    }

    m_nTransactionCounter--;

    if (m_dbHandle && m_nTransactionCounter == 0)
    {
        char *zErr = 0;
        int rc = sqlite3_exec(m_dbHandle, "ROLLBACK;",0,0,&zErr);
        checkDbError(rc);
    }

    Unlock();
}
	
String CDBAdapter::exportDatabase() {
	
	String basePath = CFilePath(m_strDbPath).getFolderName();
    unsigned long now_time_ms = CTimeInterval::getCurrentTime().toULong();
	String zipName = m_strDbPath + "_" + convertToStringA(now_time_ms) + ".zip";
	
	DBLock lock(*this);
		
	CDBRequestHelper reqHelper(*this);
	Vector<String> fileList = reqHelper.requestBlobs();
	
	for ( Vector<String>::iterator it = fileList.begin(); it != fileList.end(); ++it ) {
		String rel = RHODESAPP().getRelativeDBFilesPath(*it);
		
		if ( ((rel).find('/') == String::npos) && ((rel).find('\\') == String::npos) ) {
			(*it) = common::CFilePath::join( RHODESAPP().getBlobsDirPath(),rel);
		} else {
			*it = common::CFilePath::join( RHODESAPP().getDBFileRoot(),rel);
		}
	}

	fileList.push_back(m_strDbPath);

	String path = m_strDbPath;
	String ver = m_strDbVer;

	close(false);

	String ret = zipName;
	
	if (rho_sys_zip_files_with_path_array_ptr(zipName.c_str(),basePath.c_str(),fileList,0)!=0) {
		ret = "";
	}

	open(path,ver,false,false);
	
	return ret;
}
	
bool CDBAdapter::importDatabase( const String& zipName ) {
	CDBImportTransaction importTxn(*this,zipName);
	if (!importTxn.commit()) {
		importTxn.rollback();
	  return false;
	}
	return true;
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

/*static*/ Vector<String> CDBAdapter::getDBAllPartitionNames()
{
    Vector<String> vecNames;
    for (Hashtable<String,CDBAdapter*>::iterator it = m_mapDBPartitions.begin();  it != m_mapDBPartitions.end(); ++it )
        vecNames.addElement(it->first);

    return vecNames;
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

/*static void CDBAdapter::destroy_tables_allpartitions(const rho::Vector<rho::String>& arIncludeTables, const rho::Vector<rho::String>& arExcludeTables)
{
    for (Hashtable<String,CDBAdapter*>::iterator it = m_mapDBPartitions.begin();  it != m_mapDBPartitions.end(); ++it )
    {
        it->second->destroy_tables(arIncludeTables, arExcludeTables);
    }
}*/

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

    rho::String strVer = RhoAppAdapter.getRhoDBVersion();
    pDB->open(szDBPath,strVer, false,true);

    *ppDB = pDB;
    return 0;
}

int rho_db_close(void* pDB)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    db.close();

    //TODO: get error code from DBException
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
string_iter(const char* szVal, int valueLen, void* par)
{
    rho::Vector<rho::String>& ar = *((rho::Vector<rho::String>*)(par));
    ar.addElement(rho::String(szVal,valueLen));
}

#ifndef RHO_NO_RUBY
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
	
VALUE rho_db_export(void* pDB)
{
	rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
	return rho_ruby_create_string(db.exportDatabase().c_str());
}
	
int rho_db_import(void* pDB, const char* zipName)
{
	rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
	return db.importDatabase(zipName)? 1 : 0;
}
	
unsigned long rho_sys_is_blob_attr(const char* szPartition, int nSrcID, const char* szAttrName)
{
	return rho_ruby_create_boolean( CDBAdapter::getDB(szPartition).getAttrMgr().isBlobAttr(nSrcID, szAttrName) );
}

#endif //RHO_NO_RUBY

void* rho_db_get_handle(void* pDB)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)pDB);
    return db.getDbHandle();
}
/*
void* rho_db_user_get_handle()
{
    rho::db::CDBAdapter& db = rho::db::CDBAdapter::getUserDB();
    return db.getDbHandle();
}

void* rho_db_user_get_adapter()
{
    rho::db::CDBAdapter& db = rho::db::CDBAdapter::getUserDB();
    return &db;
} */

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

void rho_sys_update_blob_attribs(const char* szPartition, int source_id)
{
    rho::db::CDBAdapter& db = rho::db::CDBAdapter::getDB(szPartition);
    db.getAttrMgr().loadBlobAttrs(db);
}

void rho_db_encrypt( const char* szPartition, int nPartLen, int size, unsigned char* data, unsigned char* dataOut )
{
    String strPartition(szPartition, nPartLen);
    rho::db::CDBAdapter& db = rho::db::CDBAdapter::getDB(strPartition.c_str());
    if ( db.getCrypt() )
        db.getCrypt()->db_encrypt(strPartition.c_str(), size, data, dataOut);
    else
        memcpy(dataOut, data, size);
}

void rho_db_decrypt( const char* szPartition, int nPartLen, int size, unsigned char* data )
{
    String strPartition(szPartition, nPartLen);
    rho::db::CDBAdapter& db = rho::db::CDBAdapter::getDB( strPartition.c_str() );
    if ( db.getCrypt() )
        db.getCrypt()->db_decrypt(strPartition.c_str(), size, data);
}

}

namespace rho{
namespace common{

#ifndef RHO_NO_RUBY
CRubyMutex::CRubyMutex(boolean bIgnore) : m_nLockCount(0), m_valThread(0), m_valMutex(NULL)
{
    m_bIgnore = bIgnore || RHOCONF().getBool("no_ruby_threads");
}

void CRubyMutex::create()
{
    if ( !m_bIgnore && !m_valMutex)
    {
        unsigned long curThread = rho_ruby_current_thread();
#ifndef WINDOWS_PLATFORM
        if ( curThread != Qnil)
            m_valMutex = rho_ruby_create_mutex();
#else
        if ( curThread != (VALUE)4) //this is ruby Qnil. If we'll include ruby.h - God, help us fix all of this
            m_valMutex = rho_ruby_create_mutex();
#endif
    }
}

CRubyMutex::~CRubyMutex()
{
    close();    
}

void CRubyMutex::close()
{
    if ( m_valMutex )
    {
        rho_ruby_destroy_mutex(m_valMutex);
        m_valMutex = 0;
    }
}

boolean CRubyMutex::isMainRubyThread()
{
    return rho_ruby_main_thread() == rho_ruby_current_thread();
}

void CRubyMutex::Lock()
{
    if ( m_valMutex == NULL )
        return;

    unsigned long curThread = rho_ruby_current_thread();
    if ( curThread == NULL )
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
    if ( m_valMutex == NULL || m_nLockCount == 0)
        return;

    m_nLockCount--;
    if ( m_nLockCount == 0 )
    {
        m_valThread = NULL;
        rho_ruby_unlock_mutex(m_valMutex);
    }
}
#else //RHO_NO_RUBY
CRubyMutex::CRubyMutex(boolean bIgnore) : m_nLockCount(0), m_valThread(0), m_valMutex(NULL)
{
}

CRubyMutex::~CRubyMutex()
{
}

boolean CRubyMutex::isMainRubyThread()
{
	if ( (!sync::RhoconnectClientManager::haveRhoconnectClientImpl()) || (!sync::RhoconnectClientManager::haveSyncThread()))
//    if ( !sync::CSyncThread::getInstance() )
        return true;

    //return sync::CSyncThread::getInstance()->getThreadID() != CSystem::getThreadID();
	return sync::RhoconnectClientManager::syncThreadGetThreadID() != CSystem::getThreadID();
}

void CRubyMutex::Lock()
{
}

void CRubyMutex::Unlock()
{
}

void CRubyMutex::close()
{
}

void CRubyMutex::create()
    {
        
    }
#endif //RHO_NO_RUBY
}
}
