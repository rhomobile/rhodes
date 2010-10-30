#include "SyncSource.h"
#include "SyncEngine.h"
#include "SyncThread.h"

#include "common/RhoTime.h"
#include "common/StringConverter.h"
#include "common/RhodesApp.h"
#include "common/Tokenizer.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"
#include "common/RhoAppAdapter.h"
#include "json/JSONIterator.h"
#include "statistic/RhoProfiler.h"
#include "net/URI.h"

namespace rho {
namespace sync {
IMPLEMENT_LOGCLASS(CSyncSource,"Sync");

using namespace rho::net;
using namespace rho::db;
using namespace rho::common;
using namespace rho::json;
/*
CSyncSource::CSyncSource() : m_syncEngine( *new CSyncEngine(*new db::CDBAdapter()))
{
    m_bTokenFromDB = true;

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;

    m_nErrCode = RhoAppAdapter.ERR_NONE;

    m_bIsSearch = false;
}
*/
CSyncSource::CSyncSource(CSyncEngine& syncEngine, db::CDBAdapter& db  ) : m_syncEngine(syncEngine), m_dbAdapter(db)
{
    m_bTokenFromDB = true;

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;                                     
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;
    m_nRefreshTime = 0;

    m_nErrCode = RhoAppAdapter.ERR_NONE;
    m_bSchemaSource = db.isTableExist(m_strName);
}

CSyncSource::CSyncSource(int id, const String& strName, const String& strSyncType, db::CDBAdapter& db, CSyncEngine& syncEngine ) : m_syncEngine(syncEngine), m_dbAdapter(db)
{
    m_nID = id;
    m_strName = strName;
    m_strSyncType = strSyncType;

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;
    m_nRefreshTime = 0;

    m_nErrCode = RhoAppAdapter.ERR_NONE;

    DBResult( res, db.executeSQL("SELECT token,associations from sources WHERE source_id=?", m_nID) );
    if ( !res.isEnd() )
    {
        m_token = res.getUInt64ByIdx(0);
        m_bTokenFromDB = true;
    }else
    {
        m_token = 0;
        m_bTokenFromDB = true;
    }

    m_bSchemaSource = db.isTableExist(m_strName);
    parseAssociations(res.getStringByIdx(1));
}

void CSyncSource::parseAssociations(const String& strAssociations)
{
    if (strAssociations.length() == 0 )
        return;

    CTokenizer oTokenizer( strAssociations, "," );

    String strSrcName = "";
    while (oTokenizer.hasMoreTokens()) 
    {
	    String tok = oTokenizer.nextToken();
	    if (tok.length() == 0)
		    continue;
        
        if ( strSrcName.length() > 0 )
        {
            m_arAssociations.addElement( CAssociation(strSrcName, tok) );
            strSrcName = "";
        }else
            strSrcName = tok;
    }
}

INetRequest& CSyncSource::getNet(){ return getSync().getNet(); }
CSyncNotify& CSyncSource::getNotify(){ return getSync().getNotify(); }
ISyncProtocol& CSyncSource::getProtocol(){ return getSync().getProtocol(); }

void CSyncSource::sync()
{
    getNotify().reportSyncStatus(RhoAppAdapter.getMessageText("syncronizing") + getName() + "...", m_nErrCode, m_strError );

    CTimeInterval startTime = CTimeInterval::getCurrentTime();
    //m_bIsSearch = false;

    if ( isTokenFromDB() && !isEmptyToken() )
        syncServerChanges();  //sync only server changes, which was paused before
    else
    {
        if ( isEmptyToken() )
            processToken(1);

        boolean bSyncedServer = syncClientChanges();
        if ( !bSyncedServer )
            syncServerChanges();
    }

    CTimeInterval endTime = CTimeInterval::getCurrentTime();

    getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, \
						 last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?", 
                         CLocalTime().toULong(), getInsertedCount(), getDeletedCount(), (
                         endTime-startTime).toULong(), m_bGetAtLeastOnePage, m_nRefreshTime,
                         getID() );
}

boolean CSyncSource::syncClientChanges()
{
    boolean bSyncedServer = false;
    if ( isPendingClientChanges() )
    {
        syncServerChanges();
        bSyncedServer = true;
    }

    if ( bSyncedServer && isPendingClientChanges() )
        getSync().setState(CSyncEngine::esStop);
    else
    {   
      	PROF_START("Pull");

        boolean bSyncClient = false;
        {
            DBResult( res, getDB().executeSQL("SELECT object FROM changed_values WHERE source_id=? LIMIT 1 OFFSET 0", getID()) );
            bSyncClient = !res.isEnd();
        }
        if ( bSyncClient )
        {
            doSyncClientChanges();
            bSyncedServer = false;
        }

        PROF_STOP("Pull");
    }

    return bSyncedServer;
}

boolean CSyncSource::isPendingClientChanges()
{
    DBResult( res, getDB().executeSQL("SELECT object FROM changed_values WHERE source_id=? and update_type='create' and sent>1  LIMIT 1 OFFSET 0", getID()) );
    return !res.isEnd();
}

void CSyncSource::doSyncClientChanges()
{
    String arUpdateTypes[] = {"create", "update", "delete"};
    boolean arUpdateSent[] = {false, false, false};

    m_arMultipartItems.removeAllElements();
    m_arBlobAttrs.removeAllElements();
    String strBody = "{\"source_name\":" + CJSONEntry::quoteValue(getName()) + ",\"client_id\":" + CJSONEntry::quoteValue(getSync().getClientID());
    boolean bSend = false;
    int i = 0;
    for( i = 0; i < 3 && getSync().isContinueSync(); i++ )
    {
        String strBody1;
        makePushBody_Ver3(strBody1, arUpdateTypes[i], true);
        if (strBody1.length() > 0)
        {
            strBody += "," + strBody1;

            String strBlobAttrs = "";
            for ( int j = 0; j < (int)m_arBlobAttrs.size(); j++)
            {
                if ( strBlobAttrs.length() > 0 )   
                    strBlobAttrs += ",";

                strBlobAttrs += CJSONEntry::quoteValue(m_arBlobAttrs.elementAt(j));
            }

            if ( strBlobAttrs.length() > 0 )
                strBody += ",\"blob_fields\":[" + strBlobAttrs + "]";

            arUpdateSent[i] = true;
            bSend = true;
        }
    }
    strBody += "}";

    if ( bSend )
    {
        LOG(INFO) + "Push client changes to server. Source: " + getName() + "Size :" + strBody.length();
        LOG(TRACE) + "Push body: " + strBody;		

        if ( m_arMultipartItems.size() > 0 )
        {
            CMultipartItem* pItem = new CMultipartItem();
            CMultipartItem& oItem = *pItem;
            oItem.m_strBody = strBody;
            //oItem.m_strContentType = getProtocol().getContentType();
            oItem.m_strName = "cud";
            m_arMultipartItems.addElement(pItem);

            NetResponse( resp, getNet().pushMultipartData( getProtocol().getClientChangesUrl(), m_arMultipartItems, &getSync(), null) );
            if ( !resp.isOK() )
            {
                getSync().setState(CSyncEngine::esStop);
                m_nErrCode = RhoAppAdapter.ERR_REMOTESERVER;
            }
        }else
        {
            NetResponse( resp, getNet().pushData( getProtocol().getClientChangesUrl(), strBody, &getSync()) );
            if ( !resp.isOK() )
            {
                getSync().setState(CSyncEngine::esStop);
                m_nErrCode = RhoAppAdapter.ERR_REMOTESERVER;
            }
        }
    }

    for( i = 0; i < 3 && getSync().isContinueSync(); i++ )
    {
        if ( arUpdateSent[i] )
        {
            //oo conflicts
            if ( i < 1 ) //create
                getDB().executeSQL("UPDATE changed_values SET sent=2 WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
            else
            //
                getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
        }
    }

    m_arMultipartItems.removeAllElements();
    m_arBlobAttrs.removeAllElements();
}
/*
static void escapeDoubleQuotes(String& str)
{
    const char* szQuote = strchr(str.c_str(), '\"');
    while(szQuote)
    {
        int nPos = szQuote - str.c_str();
        str.insert(nPos, 1, '\\');
        if ( nPos+2 < str.length() )
            szQuote = strchr(str.c_str()+nPos+2, '\"');
        else
            szQuote = 0;
    }
}*/

//{"source_name":"SampleAdapter","client_id":1,"create":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
//{"source_name":"SampleAdapter","client_id":1,"update":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
//{"source_name":"SampleAdapter","client_id":1,"delete":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
//{"source_name":"SampleAdapter","client_id":1,"delete":{"3":{"brand":"HTC","name":"Fuze","price":"299.99"}},"create":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}},"update":{"2":{"brand":"Android","name":"G2","price":"99.99"}}}
void CSyncSource::makePushBody_Ver3(String& strBody, const String& strUpdateType, boolean isSync)
{
    getDB().Lock();
    DBResult( res , getDB().executeSQL("SELECT attrib, object, value, attrib_type "
        "FROM changed_values where source_id=? and update_type =? and sent<=1 ORDER BY object", getID(), strUpdateType.c_str() ) );

    if ( res.isEnd() )
    {
        getDB().Unlock();
        return;
    }

    String strCurObject = "";
    boolean bFirst = true;
    for( ; !res.isEnd(); res.next() )
    {
        String strAttrib = res.getStringByIdx(0);
        String strObject = res.getStringByIdx(1);
        String value = res.getStringByIdx(2);
        String attribType = res.getStringByIdx(3);

        if ( attribType.compare("blob.file") == 0 )
        {
            CMultipartItem* pItem = new CMultipartItem();
            CMultipartItem& oItem = *pItem;
            oItem.m_strFilePath = RHODESAPPBASE().resolveDBFilesPath(value);
            oItem.m_strContentType = "application/octet-stream";
            oItem.m_strName = strAttrib + "-" + strObject;

            m_arBlobAttrs.addElement(strAttrib);
            m_arMultipartItems.addElement(pItem);
        }

        if ( strBody.length() == 0 )
        {
            if ( !isSync )
                strBody += "{";
            else
                strBody += "\"" + strUpdateType + "\":{";
        }

        if ( strObject.compare(strCurObject) != 0 )
        {
            if ( strCurObject.length() > 0 )
            {
                if ( !bFirst )
                    strBody += "}";
                strBody += ",";
            }

            bFirst = true;
            strBody += CJSONEntry::quoteValue(strObject);
            strCurObject = strObject;
        }
            
        if (!bFirst)
            strBody += ",";

        if ( strAttrib.length() > 0  )
        {
            if ( bFirst )
                strBody += ":{";

            strBody += CJSONEntry::quoteValue(strAttrib) + ":" + CJSONEntry::quoteValue(value);
            bFirst = false;
        }
    }

    if ( strBody.length() > 0 )
    {
        if ( !bFirst )
            strBody += "}";

        strBody += "}";
    }

    if ( isSync )
        getDB().executeSQL("UPDATE changed_values SET sent=1 WHERE source_id=? and update_type=? and sent=0", getID(), strUpdateType.c_str() );

    getDB().Unlock();
}

void CSyncSource::applyChangedValues()
{
    String strBody = "";
    makePushBody_Ver3(strBody, "create", false);
    if ( strBody.length() > 0 )
    {
        CJSONEntry oEntry(strBody.c_str());
        processSyncCommand("insert", oEntry );
    }

    strBody = "";
    makePushBody_Ver3(strBody, "delete", false);
    if ( strBody.length() > 0 )
    {
        CJSONEntry oEntry(strBody.c_str());
        processSyncCommand("delete", oEntry );
    }

    strBody = "";
    makePushBody_Ver3(strBody, "update", false);
    if ( strBody.length() > 0 )
    {
        CJSONEntry oEntry(strBody.c_str());
        processSyncCommand("insert", oEntry );
    }
}

void CSyncSource::syncServerChanges()
{
    LOG(INFO) + "Sync server changes source ID :" + getID();

    while( getSync().isContinueSync() )
    {
        setCurPageCount(0);
        String strUrl = getProtocol().getServerQueryUrl("");
        String strQuery = getProtocol().getServerQueryBody(getName(), getSync().getClientID(), getSync().getSyncPageSize());

        if ( !m_bTokenFromDB && getToken() > 1 )
            strQuery += "&token=" + convertToStringA(getToken());

		LOG(INFO) + "Pull changes from server. Url: " + (strUrl+strQuery);
        PROF_START("Net");	    
        NetResponse(resp,getNet().pullData(strUrl+strQuery, &getSync()));
		PROF_STOP("Net");

        if ( !resp.isOK() )
        {
            getSync().stopSync();
			m_nErrCode = RhoAppAdapter.getErrorFromResponse(resp);
            m_strError = resp.getCharData();
            continue;
        }

        const char* szData = resp.getCharData();

        //const char* szData = "[{\"version\":3},{\"token\":\"35639160294387\"},{\"count\":3},{\"progress_count\":0},{\"total_count\":3},{\"metadata\":\"{\\\"foo\\\":\\\"bar\\\"}\",\"insert\":{\"1\":{\"price\":\"199.99\",\"brand\":\"Apple\",\"name\":\"iPhone\"}}}]";

        //LOG(INFO) + szData;
        PROF_START("Parse");
        CJSONArrayIterator oJsonArr(szData);
        PROF_STOP("Parse");

        processServerResponse_ver3(oJsonArr);

        if ( getToken() == 0 )
            break;
    }
}

void CSyncSource::processServerResponse_ver3(CJSONArrayIterator& oJsonArr)
{
    PROF_START("Data1");

    int nVersion = 0;
    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("version") )
    {
        nVersion = oJsonArr.getCurItem().getInt("version");
        oJsonArr.next();
    }

    if ( nVersion != getProtocol().getVersion() )
    {
        LOG(ERROR) + "Sync server send data with incompatible version. Client version: " + convertToStringA(getProtocol().getVersion()) +
            "; Server response version: " + convertToStringA(nVersion) + ". Source name: " + getName();
        getSync().stopSync();
        m_nErrCode = RhoAppAdapter.ERR_SYNCVERSION;
        return;
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("token"))
    {
        processToken(oJsonArr.getCurItem().getUInt64("token"));
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("source") )
    {
        //skip it. it uses in search only
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("count") )
    {
        setCurPageCount(oJsonArr.getCurItem().getInt("count"));
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("refresh_time") )
    {
        setRefreshTime(oJsonArr.getCurItem().getInt("refresh_time"));
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("progress_count") )
    {
        //TODO: progress_count
        //setTotalCount(oJsonArr.getCurItem().getInt("progress_count"));
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("total_count") )
    {
        setTotalCount(oJsonArr.getCurItem().getInt("total_count"));
        oJsonArr.next();
    }

   /* if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("source-error") )
    {
        CJSONEntry oJsonErr = oJsonArr.getCurItem().getEntry("source-error");
        m_strError = oJsonErr.getString("message");
        m_nErrCode = RhoAppAdapter.ERR_CUSTOMSYNCSERVER;
        getSync().stopSync();
        return;
    }*/

    //if ( getServerObjectsCount() == 0 )
    //    getNotify().fireSyncNotification(this, false, RhoAppAdapter.ERR_NONE, "");

    if ( getToken() == 0 )
    {
        //oo conflicts
        getDB().executeSQL("DELETE FROM changed_values where source_id=? and sent>=3", getID() );
        //

    }

	LOG(INFO) + "Got " + getCurPageCount() + "(Processed: " +  getServerObjectsCount() + ") records of " + getTotalCount() + " from server. Source: " + getName()
         + ". Version: " + nVersion;

    PROF_STOP("Data1");
    if ( !oJsonArr.isEnd() && getSync().isContinueSync() )
    {
        CJSONEntry oCmds = oJsonArr.getCurItem();
        PROF_START("Data");
        //TODO: use isUIWaitDB inside processSyncCommand
        //    if ( getDB().isUIWaitDB() )
        //    {
		//        LOG(INFO) + "Commit transaction because of UI request.";
        //        getDB().endTransaction();
        //        getDB().startTransaction();
        //    }

        getDB().startTransaction();
        if ( oCmds.hasName("metadata") && getSync().isContinueSync() )
        {
            String strMetadata = oCmds.getString("metadata");
            getDB().executeSQL("UPDATE sources SET metadata=? WHERE source_id=?", strMetadata, getID() );
        }
        if ( oCmds.hasName("links") && getSync().isContinueSync() )
            processSyncCommand("links", oCmds.getEntry("links") );
        if ( oCmds.hasName("delete") && getSync().isContinueSync() )
            processSyncCommand("delete", oCmds.getEntry("delete") );
        if ( oCmds.hasName("insert") && getSync().isContinueSync() )
            processSyncCommand("insert", oCmds.getEntry("insert") );

        PROF_STOP("Data");

	    PROF_START("DB");
        getDB().endTransaction();
        PROF_STOP("DB");

        getNotify().fireObjectsNotification();
    }

	PROF_START("Data1");
    if ( getCurPageCount() > 0 )
        getNotify().fireSyncNotification(this, false, RhoAppAdapter.ERR_NONE, "");
	PROF_STOP("Data1");
}

void CSyncSource::processSyncCommand(const String& strCmd, CJSONEntry oCmdEntry)
{
    CJSONStructIterator objIter(oCmdEntry);

    for( ; !objIter.isEnd() && getSync().isContinueSync(); objIter.next() )
    {
        String strObject = objIter.getCurKey();
        CJSONStructIterator attrIter( objIter.getCurValue() );
        if ( m_bSchemaSource )
            processServerCmd_Ver3_Schema(strCmd,strObject,attrIter);
        else
        {
            for( ; !attrIter.isEnd() && getSync().isContinueSync(); attrIter.next() )
            {
                String strAttrib = attrIter.getCurKey();
                String strValue = attrIter.getCurString();

                processServerCmd_Ver3(strCmd,strObject,strAttrib,strValue);
            }
        }

        if ( getSyncType().compare("none") == 0 )
            continue;

        int nSyncObjectCount  = getNotify().incLastSyncObjectCount(getID());
        if ( getProgressStep() > 0 && (nSyncObjectCount%getProgressStep() == 0) )
            getNotify().fireSyncNotification(this, false, RhoAppAdapter.ERR_NONE, "");

        if ( getDB().isUIWaitDB() )
        {
	        LOG(INFO) + "Commit transaction because of UI request.";
            getDB().endTransaction();
            CSyncThread::getInstance()->sleep(1000);
            getDB().startTransaction();
        }

    }
}

void CSyncSource::processAssociations(const String& strOldObject, const String& strNewObject)
{
    for ( int i = 0; i < (int)m_arAssociations.size(); i++ )
    {
        CSyncSource* pSrc = getSync().findSourceByName(m_arAssociations.elementAt(i).m_strSrcName);
        if ( pSrc != null )
            pSrc->updateAssociation(strOldObject, strNewObject, m_arAssociations.elementAt(i).m_strAttrib);
    }
}

void CSyncSource::updateAssociation(const String& strOldObject, const String& strNewObject, const String& strAttrib)
{
    if ( m_bSchemaSource )
    {
        String strSqlUpdate = "UPDATE ";
        strSqlUpdate += getName() + " SET " + strAttrib + "=? where " + strAttrib + "=?";

        getDB().executeSQL(strSqlUpdate.c_str(), strNewObject, strOldObject );
    }
    else
        getDB().executeSQL("UPDATE object_values SET value=? where attrib=? and source_id=? and value=?", 
            strNewObject, strAttrib, getID(), strOldObject );

    getDB().executeSQL("UPDATE changed_values SET value=? where attrib=? and source_id=? and value=?", 
        strNewObject, strAttrib, getID(), strOldObject );
}

void CSyncSource::processServerCmd_Ver3_Schema(const String& strCmd, const String& strObject, CJSONStructIterator& attrIter)//throws Exception
{
    if ( strCmd.compare("insert") == 0 )
    {
        Vector<String> vecValues, vecAttrs;
        String strCols = "", strQuest = "", strSet = "";
        for( ; !attrIter.isEnd() && getSync().isContinueSync(); attrIter.next() )
        {
            CAttrValue oAttrValue(attrIter.getCurKey(),attrIter.getCurString());
            if ( !processBlob(strCmd,strObject,oAttrValue) )
                continue;

            if ( strCols.length() > 0 )
                strCols += ",";
            if ( strQuest.length() > 0)
                strQuest += ",";
            if ( strSet.length() > 0)
                strSet += ",";

            strCols += oAttrValue.m_strAttrib;
            strQuest += "?";
            strSet += oAttrValue.m_strAttrib + "=?";
            vecAttrs.addElement(oAttrValue.m_strAttrib);
            vecValues.addElement(oAttrValue.m_strValue);
        }
        vecValues.addElement(strObject);
        if ( strCols.length() > 0 )
            strCols += ",";
        if ( strQuest.length() > 0)
            strQuest += ",";

        strCols += "object";
        strQuest += "?";

        String strSqlInsert = "INSERT INTO ";
        strSqlInsert += getName() + " (";
        strSqlInsert += strCols + ") VALUES(" + strQuest + ")";

        if ( !getSync().isContinueSync() )
            return;

        DBResult(resInsert, getDB().executeSQLReportNonUniqueEx(strSqlInsert.c_str(), vecValues ) );
        if ( resInsert.isNonUnique() )
        {
            String strSqlUpdate = "UPDATE ";
            strSqlUpdate += getName() + " SET " + strSet + " WHERE object=?";
            getDB().executeSQLEx(strSqlUpdate.c_str(), vecValues);

            if ( getSyncType().compare("none") != 0 )
            {
                // oo conflicts
                for( int i = 0; i < (int)vecAttrs.size(); i++ )
                {
                    getDB().executeSQL("UPDATE changed_values SET sent=4 where object=? and attrib=? and source_id=? and sent>1", 
                        strObject, vecAttrs.elementAt(i), getID() );
                }
                //
            }
        }

        if ( getSyncType().compare("none") != 0 )
            getNotify().onObjectChanged(getID(),strObject, CSyncNotify::enUpdate);

        m_nInserted++;
    }else if (strCmd.compare("delete") == 0)
    {
        Vector<String> vecAttrs;
        String strSet = "";
        for( ; !attrIter.isEnd() && getSync().isContinueSync(); attrIter.next() )
        {
            CAttrValue oAttrValue(attrIter.getCurKey(),attrIter.getCurString());

            if ( strSet.length() > 0 )
                strSet += ",";

            vecAttrs.addElement(oAttrValue.m_strAttrib);
            strSet += oAttrValue.m_strAttrib + "=NULL";
        }

        String strSqlUpdate = "UPDATE ";
        strSqlUpdate += getName() + " SET " + strSet + " WHERE object=?";

        if ( strSet.length() == 0 || !getSync().isContinueSync() )
            return;

        getDB().executeSQL(strSqlUpdate.c_str(), strObject);
        //Remove item if all nulls
        String strSelect = String("SELECT * FROM ") + getName() + " WHERE object=?";
        DBResult(res, getDB().executeSQL( strSelect.c_str(), strObject ) );
        if ( !res.isEnd() )
        {
            boolean bAllNulls = true;
            for( int i = 0; i < res.getColCount(); i ++)
            {
                if ( !res.isNullByIdx(i) && res.getColName(i).compare("object")!=0 )
                {
                    bAllNulls = false;
                    break;
                }
            }

            if (bAllNulls)
            {
                String strDelete = String("DELETE FROM ") + getName() + " WHERE object=?";
                getDB().executeSQL( strDelete.c_str(), strObject);
            }
        }

        if ( getSyncType().compare("none") != 0 )
        {
            getNotify().onObjectChanged(getID(), strObject, CSyncNotify::enDelete);
            // oo conflicts
            for( int i = 0; i < (int)vecAttrs.size(); i++ )
            {
                getDB().executeSQL("UPDATE changed_values SET sent=3 where object=? and attrib=? and source_id=?", 
                    strObject, vecAttrs.elementAt(i), getID() );
            }
            //
        }

        m_nDeleted++;
    }else if ( strCmd.compare("links") == 0 )
    {
        String strValue = attrIter.getCurString();
        processAssociations(strObject, strValue);

        String strSqlUpdate = "UPDATE ";
        strSqlUpdate += getName() + " SET object=? WHERE object=?";
        getDB().executeSQL(strSqlUpdate.c_str(), strValue, strObject);

        getDB().executeSQL("UPDATE changed_values SET object=?,sent=3 where object=? and source_id=?", strValue, strObject, getID() );
        getNotify().onObjectChanged(getID(), strObject, CSyncNotify::enCreate);
    }

}

boolean CSyncSource::processBlob( const String& strCmd, const String& strObject, CAttrValue& oAttrValue )
{
    //TODO: when server return delete with rhoblob postfix - delete isBlobAttr
    if ( !(oAttrValue.m_strBlobSuffix.length() > 0 || getDB().getAttrMgr().isBlobAttr(getID(), oAttrValue.m_strAttrib.c_str())) )
        return true;

    boolean bDownload = true;
    String strDbValue = "";
    if ( !getDB().getAttrMgr().isOverwriteBlobFromServer(getID(), oAttrValue.m_strAttrib) )
    {
        if ( m_bSchemaSource )
        {
            String strSelect = String("SELECT ") + oAttrValue.m_strAttrib + " FROM " + getName() + " WHERE object=?";
            DBResult(res, getDB().executeSQL( strSelect.c_str(), strObject));
            if (!res.isEnd())
            {
                strDbValue = res.getStringByIdx(0);
                bDownload = strDbValue.length() == 0;
            }
        }else
        {
            DBResult(res, getDB().executeSQL(
                "SELECT value FROM object_values WHERE object=? and attrib=? and source_id=?",
                strObject, oAttrValue.m_strAttrib, getID() ) );
            if (!res.isEnd())
            {
                strDbValue = res.getStringByIdx(0);
                bDownload = strDbValue.length() == 0;
            }
        }
    }

    if ( bDownload )
    {
        getDB().endTransaction();
        boolean bRes = downloadBlob(oAttrValue);
        getDB().startTransaction();

        return bRes;
    }

    oAttrValue.m_strValue = strDbValue;
    return true;
}

void CSyncSource::processServerCmd_Ver3(const String& strCmd, const String& strObject, const String& strAttriba, const String& strValuea)//throws Exception
{
    CAttrValue oAttrValue(strAttriba,strValuea);

    if ( strCmd.compare("insert") == 0 )
    {
        if ( !processBlob(strCmd,strObject,oAttrValue) )
            return;

        DBResult(resInsert, getDB().executeSQLReportNonUnique("INSERT INTO object_values \
            (attrib, source_id, object, value) VALUES(?,?,?,?)", 
            oAttrValue.m_strAttrib, getID(), strObject, oAttrValue.m_strValue ) );
        if ( resInsert.isNonUnique() )
        {
            getDB().executeSQL("UPDATE object_values \
                SET value=? WHERE object=? and attrib=? and source_id=?", 
                oAttrValue.m_strValue, strObject, oAttrValue.m_strAttrib, getID() );

            if ( getSyncType().compare("none") != 0 )
            {
                // oo conflicts
                getDB().executeSQL("UPDATE changed_values SET sent=4 where object=? and attrib=? and source_id=? and sent>1", 
                    strObject, oAttrValue.m_strAttrib, getID() );
                //
            }
        }

        if ( getSyncType().compare("none") != 0 )
           getNotify().onObjectChanged(getID(),strObject, CSyncNotify::enUpdate);

        m_nInserted++;
    }else if (strCmd.compare("delete") == 0)
    {
        getDB().executeSQL("DELETE FROM object_values where object=? and attrib=? and source_id=?", strObject, oAttrValue.m_strAttrib, getID() );

        if ( getSyncType().compare("none") != 0 )
        {
            getNotify().onObjectChanged(getID(), strObject, CSyncNotify::enDelete);
            // oo conflicts
            getDB().executeSQL("UPDATE changed_values SET sent=3 where object=? and attrib=? and source_id=?", strObject, oAttrValue.m_strAttrib, getID() );
            //
        }

        m_nDeleted++;
    }else if ( strCmd.compare("links") == 0 )
    {
        processAssociations(strObject, oAttrValue.m_strValue);

        getDB().executeSQL("UPDATE object_values SET object=? where object=? and source_id=?", oAttrValue.m_strValue, strObject, getID() );
        getDB().executeSQL("UPDATE changed_values SET object=?,sent=3 where object=? and source_id=?", oAttrValue.m_strValue, strObject, getID() );

        getNotify().onObjectChanged(getID(), strObject, CSyncNotify::enCreate);
    }

}

String CSyncSource::makeFileName(const CAttrValue& value)//throws Exception
{
	String strExt = "";

    URI uri(value.m_strValue);    
    String strQuest = uri.getQueryString();

    if (strQuest.length() > 0)
    {
		int nExt = strQuest.find("extension=");
		if ( nExt >= 0 )
        {
			int nExtEnd = strQuest.find("&", nExt);
			if (nExtEnd < 0 )
				nExtEnd = strQuest.length();
			
			strExt = strQuest.substr(nExt+10, nExtEnd);
		}
    }

    if ( strExt.length() == 0 )
    {
        String strFileName = uri.getLastNamePart();
        int nExt = strFileName.find_last_of('.');
		if ( nExt >= 0 )
            strExt = strFileName.substr(nExt);
    }

    if ( strExt.length() == 0 )
        strExt = ".bin";
    else if ( strExt.at(0) != '.' )    
        strExt = "." + strExt;

    String fName = RHODESAPPBASE().getBlobsDirPath() + "/id_" + CLocalTime().toString(true,true) + strExt;
	
	return  fName;
}

CAttrValue::CAttrValue(const String& strAttrib, const String& strValue)
{
    m_strAttrib = strAttrib;
    m_strValue = strValue;

	if ( String_endsWith(m_strAttrib,"-rhoblob")  )
    {
		m_strBlobSuffix = "-rhoblob";
        m_strAttrib = m_strAttrib.substr(0,m_strAttrib.length()-m_strBlobSuffix.length());
    }
}

boolean CSyncSource::downloadBlob(CAttrValue& value)//throws Exception
{
	String fName = makeFileName( value );
	String url = value.m_strValue;
	const char* nQuest = strchr(url.c_str(),'?');
	if ( nQuest > 0 )
		url += "&";
	else
		url += "?";
	url += "client_id=" + getSync().getClientID();

    NetResponse(resp, getNet().pullFile(url, fName, &getSync(), null));
    if ( !resp.isOK() )
    {
        getSync().stopSync();
		m_nErrCode = RhoAppAdapter.getErrorFromResponse(resp);
        //m_strError = resp.getCharData();
        return false;
    }

    value.m_strValue = CFilePath::getRelativePath( fName, RHODESAPPBASE().getRhoRootPath());
    
    return true;
}

void CSyncSource::processToken(uint64 token)
{
    if ( token > 1 && getToken() == token ){
		//Delete non-confirmed records

        setToken( token ); //For m_bTokenFromDB = false;
        //getDB().executeSQL("DELETE FROM object_values where source_id=? and token=?", getID(), token );
        //TODO: add special table for id,token
	}else
    {
        setToken( token );
        getDB().executeSQL("UPDATE sources SET token=? where source_id=?", token, getID() );
	}

}

}
}
