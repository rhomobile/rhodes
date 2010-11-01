#include "SyncNotify.h"
#include "SyncEngine.h"
#include "net/URI.h"
#include "common/RhoFilePath.h"
#include "common/RhoAppAdapter.h"
#include "common/RhodesApp.h"

namespace rho {
namespace sync {
IMPLEMENT_LOGCLASS(CSyncNotify,"Sync");
using namespace rho::net;
using namespace rho::common;
using namespace rho::db;

String CSyncNotify::m_strObjectNotifyUrl;
common::CMutex CSyncNotify::m_mxObjectNotify;

INetRequest& CSyncNotify::getNet(){ return getSync().getNet(); }

void CSyncNotify::addObjectNotify(int nSrcID, const String& strObject)
{
    synchronized(m_mxObjectNotify)
    {
        Hashtable<String,int>* phashObject = m_hashSrcIDAndObject.get(nSrcID);
        if ( phashObject == null )
        {
            phashObject = new Hashtable<String,int>();
            m_hashSrcIDAndObject.put( nSrcID, phashObject );
        }

        Hashtable<String,int>& hashObject = *phashObject;
        hashObject.put(strObject,enNone);
    }
}

void CSyncNotify::addObjectNotify(const String& strSrcName, const String& strObject )
{
    synchronized(m_mxObjectNotify)
    {
        m_strSingleObjectSrcName = strSrcName;
        m_strSingleObjectID = strObject[0] == '{' ? strObject.substr(1,strObject.length()-2) : strObject ;
    }
}

void CSyncNotify::cleanObjectNotifications()
{
    synchronized(m_mxObjectNotify)
    {
        m_strSingleObjectSrcName = "";
        m_strSingleObjectID = "";
        m_hashSrcIDAndObject.clear();
    }
}

void CSyncNotify::cleanCreateObjectErrors()
{
    synchronized(m_mxObjectNotify)
    {
        m_hashCreateObjectErrors.clear();
    }
}

void CSyncNotify::processSingleObject()
{
    if ( m_strSingleObjectSrcName.length() == 0 )
        return;

    CSyncSource* pSrc = getSync().findSourceByName(m_strSingleObjectSrcName);
    if ( pSrc )
    {
        int nSrcID = pSrc->getID();
        addObjectNotify(nSrcID,m_strSingleObjectID);
    }

    m_strSingleObjectSrcName = "";
    m_strSingleObjectID = "";
}

void CSyncNotify::fireObjectsNotification()
{
	String strBody = "";
    String strUrl = "";

    {
        synchronized(m_mxObjectNotify)

        if ( m_strObjectNotifyUrl.length() == 0 )
            return;

        strUrl = getNet().resolveUrl(m_strObjectNotifyUrl);

        for (HashtablePtr<int, Hashtable<String,int>* >::iterator it = m_hashSrcIDAndObject.begin();  it != m_hashSrcIDAndObject.end(); ++it )
        {
            int nSrcID = it->first;
            Hashtable<String,int>& hashObject = *(it->second);
            for ( Hashtable<String,int>::iterator itObject = hashObject.begin();  itObject != hashObject.end(); ++itObject )
            {
                int nNotifyType = itObject->second;
                if (nNotifyType == enNone)
                    continue;
                //This is slow operation
/*
                if ( nNotifyType == enDelete )
                {
                    //TODO: get db for source
                    DBResult( res , getDB().executeSQL("SELECT object FROM object_values where object=? LIMIT 1 OFFSET 0", itObject->first ));
                    if ( !res.isEnd() )
                        nNotifyType = enUpdate;    
                }
*/
                if ( strBody.length() > 0 )
                    strBody += "&rho_callback=1&";

                switch(nNotifyType)
                {
                case enDelete:
                    strBody += "deleted[][object]=" + itObject->first;
                    strBody += "&deleted[][source_id]=" + convertToStringA(nSrcID);
                    break;
                case enUpdate:
                    strBody += "updated[][object]=" + itObject->first;
                    strBody += "&updated[][source_id]=" + convertToStringA(nSrcID);
                    break;
                case enCreate:
                    strBody += "created[][object]=" + itObject->first;
                    strBody += "&created[][source_id]=" + convertToStringA(nSrcID);
                    break;
                }

                hashObject.put(itObject->first, enNone);
            }
        }

        if ( strBody.length() == 0 )
            return;
    }

    callNotify( CSyncNotification(strUrl,"",false), strBody);
}

void CSyncNotify::onObjectChanged(int nSrcID, const String& strObject, int nType)
{
    synchronized(m_mxObjectNotify)
    {
        processSingleObject();

        Hashtable<String,int>* phashObject = m_hashSrcIDAndObject.get(nSrcID);
        if ( phashObject == null )
            return;

        Hashtable<String,int>& hashObject = *phashObject;
        if( hashObject.containsKey(strObject) )
            hashObject.put(strObject,nType);
    }
}

void CSyncNotify::addCreateObjectError(int nSrcID, const String& strObject, const String& strError)
{
    synchronized(m_mxObjectNotify)
    {
        Hashtable<String,String>* phashErrors = m_hashCreateObjectErrors.get(nSrcID);
        if ( phashErrors == null )
        {
            phashErrors = new Hashtable<String,String>();
            m_hashCreateObjectErrors.put( nSrcID, phashErrors );
        }

        Hashtable<String,String>& hashErrors = *phashErrors;
        hashErrors.put(strObject,strError);
    }
}

String CSyncNotify::makeCreateObjectErrorBody(int nSrcID)
{
    String strBody = "";

    synchronized(m_mxObjectNotify)
    {
        Hashtable<String,String>* phashErrors = m_hashCreateObjectErrors.get(nSrcID);
        if ( phashErrors == null )
            return "";

        Hashtable<String,String>& hashErrors = *phashErrors;
        for ( Hashtable<String,String>::iterator itError = hashErrors.begin();  itError != hashErrors.end(); ++itError )
        {
            strBody += "&create_error[][object]=" + itError->first;
            strBody += "&create_error[][error_message]=" + itError->second;
        }

        hashErrors.clear();
    }

    return strBody;
}

void CSyncNotify::setObjectNotifyUrl(String strUrl)
{ 
    synchronized(m_mxObjectNotify)
    {
        m_strObjectNotifyUrl = strUrl; 
    }
}

String CSyncNotify::getObjectNotifyUrl()
{
    synchronized(m_mxObjectNotify)
    {
        return m_strObjectNotifyUrl;
    }
}

void CSyncNotify::onSyncSourceEnd( int nSrc, VectorPtr<CSyncSource*>& sources )
{
    CSyncSource& src = *sources.elementAt(nSrc);

    if ( getSync().getState() == CSyncEngine::esStop && src.m_nErrCode != RhoAppAdapter.ERR_NONE )
    {
        fireSyncNotification(&src, true, src.m_nErrCode, "");
		fireAllSyncNotifications(true, src.m_nErrCode, src.m_strError );
    }
    else
        fireSyncNotification(&src, true, src.m_nErrCode, "");

    cleanCreateObjectErrors();
}

void CSyncNotify::setSyncNotification(int source_id, CSyncNotification* pNotify )
{
    LOG(INFO) + "Set notification. Source ID: " + source_id + ";" + (pNotify ? pNotify->toString() : "");

	if ( source_id == -1 )
	{
		synchronized(m_mxSyncNotifications)
        {
            m_pAllNotification = pNotify;
        }
	}else
	{
        synchronized(m_mxSyncNotifications)
        {
            m_mapSyncNotifications.put( source_id, pNotify );
        }
    }
}

CSyncNotification::CSyncNotification(String strUrl, String strParams, boolean bRemoveAfterFire) : 
    m_strParams(strParams), m_bRemoveAfterFire(bRemoveAfterFire),
    m_cCallback(null), m_cCallbackData(null)
{
    if ( strUrl .length() > 0 )
        m_strUrl = RHODESAPPBASE().canonicalizeRhoUrl(strUrl);
}

CSyncNotification::~CSyncNotification()
{
    if ( m_cCallbackData )
		rho_free_callbackdata(m_cCallbackData);
}
	
String CSyncNotification::toString()const
{
	if ( m_cCallback )
		return "C_Callback";
	
	String strRes = "Url :";
	strRes += m_strUrl;
	strRes += "; Params: ";
	strRes += m_strParams;
	return strRes;
}
	
void CSyncNotify::setSearchNotification(CSyncNotification* pNotify )
{
    LOG(INFO) + "Set search notification." + (pNotify ? pNotify->toString() : "");
    synchronized(m_mxSyncNotifications)
    {
        m_pSearchNotification = pNotify;
    }
}

extern "C" void alert_show_popup(const char* message);
void CSyncNotify::reportSyncStatus(String status, int error, String strDetails) {
    if ( error == RhoAppAdapter.ERR_SYNCVERSION )
    {
        status = RhoAppAdapter.getErrorText(error);
        alert_show_popup(status.c_str());
        return;
    }
    //TODO: reportStatus
	//if (m_statusListener != null) {
	//	if ( strDetails.length() == 0 )
	//		strDetails = RhoRuby.getErrorText(error);
    //    status += (strDetails.length() > 0 ? RhoRuby.getMessageText("details") + strDetails: "");
	//	m_statusListener.reportStatus( status, error);
	//}
	//LOG(INFO) + "Status: "+status;
}

void CSyncNotify::fireBulkSyncNotification( boolean bFinish, String status, String partition, int nErrCode )
{
    if ( getSync().getState() == CSyncEngine::esExit )
		return;

	if( nErrCode != RhoAppAdapter.ERR_NONE)
	{
		String strMessage = RhoAppAdapter.getMessageText("sync_failed_for") + "bulk.";
		reportSyncStatus(strMessage,nErrCode,"");
	}

    String strParams = "";
    strParams += "partition=" + partition;
    strParams += "&bulk_status="+status;
    strParams += "&sync_type=bulk";

    doFireSyncNotification( null, bFinish, nErrCode, "", strParams );
}

void CSyncNotify::fireAllSyncNotifications( boolean bFinish, int nErrCode, String strError )
{
    if ( getSync().getState() == CSyncEngine::esExit )
		return;

    synchronized(m_mxSyncNotifications)
    {
        CSyncNotification* pSN = getSyncNotifyBySrc(null);    
        if ( pSN != null )
            doFireSyncNotification( null, bFinish, nErrCode, strError, "" );
    }
}

void CSyncNotify::fireSyncNotification( CSyncSource* src, boolean bFinish, int nErrCode, String strMessage)
{
    if ( getSync().getState() == CSyncEngine::esExit )
		return;
	
	if( strMessage.length() > 0 || nErrCode != RhoAppAdapter.ERR_NONE)
	{
		if ( !getSync().isSearch() )
        {
	//		if ( src != null && strMessage.length() == 0 )
	//			strMessage = RhoRuby.getMessageText("sync_failed_for") + (*src).getName() + ".";
			
            reportSyncStatus(strMessage,nErrCode, (src != null ? (*src).m_strError : "") );
        }
	}

    doFireSyncNotification(src, bFinish, nErrCode, "", "" );
}

CSyncNotification* CSyncNotify::getSyncNotifyBySrc(CSyncSource* src)
{
    CSyncNotification* pSN = null;
	if ( getSync().isSearch() )
		pSN = m_pSearchNotification;
	else
    {
        if ( src != null )
		    pSN = m_mapSyncNotifications.get( (*src).getID());

        if ( pSN == null )
            pSN = m_pAllNotification;
    }

	if ( pSN == null && !getSync().isNoThreadedMode() )
        return null;

    return pSN != null ? pSN : &m_emptyNotify;
}

void CSyncNotify::doFireSyncNotification( CSyncSource* src, boolean bFinish, int nErrCode, String strError, String strParams)
{
	if ( getSync().isStoppedByUser() )
		return;

    CSyncNotification* pSN;
    String strBody;
    boolean bRemoveAfterFire = bFinish;
    {
        synchronized(m_mxSyncNotifications)
        {
            pSN = getSyncNotifyBySrc(src);
	        if ( pSN == null )
                return;

		    strBody = "";

            if ( src != null )
            {
                strBody += "total_count=" + convertToStringA( (*src).getTotalCount());
                strBody += "&processed_count=" + convertToStringA( (*src).getCurPageCount());
                strBody += "&cumulative_count=" + convertToStringA(getLastSyncObjectCount( (*src).getID()));
                strBody += "&source_id=" + convertToStringA( (*src).getID());
                strBody += "&source_name=" + (*src).getName();
            }

            if ( strParams.length() > 0 )
                strBody += (strBody.length() > 0 ? "&" : "") + strParams;
            else
                strBody += String(strBody.length() > 0 ? "&" : "") + "sync_type=incremental";

            strBody += "&status=";
            if ( bFinish )
            {
                if ( nErrCode == RhoAppAdapter.ERR_NONE )
                {
                    if ( getSync().isSchemaChanged() )
                        strBody += "schema_changed";
                    else
                        strBody += (src == null && strParams.length() == 0) ? "complete" : "ok";
                }
	            else
	            {
                    if ( getSync().isStoppedByUser() )
                        nErrCode = RhoAppAdapter.ERR_CANCELBYUSER;

	        	    strBody += "error";				        	
			        strBody += "&error_code=" + convertToStringA(nErrCode);
		            strBody += "&error_message=";

                    if ( strError.length() > 0 )
                        URI::urlEncode(strError,strBody);
                    else if ( src != null )
                        URI::urlEncode( (*src).m_strError,strBody);
	            }

                if ( src != null )
                    strBody += makeCreateObjectErrorBody( (*src).getID());
            }
            else
        	    strBody += "in_progress";

            strBody += "&rho_callback=1";
            if ( pSN->m_strParams.length() > 0 )
                strBody += "&" + pSN->m_strParams;

            bRemoveAfterFire = bRemoveAfterFire && pSN->m_bRemoveAfterFire;
        }
    }
    LOG(INFO) + "Fire notification. Source : " + (src != null ? (*src).getName():"") + "; " + pSN->toString();
	
    if ( callNotify(*pSN, strBody) || bRemoveAfterFire)
        clearNotification(src);
}

boolean CSyncNotify::callNotify(const CSyncNotification& oNotify, const String& strBody )
{
    if ( getSync().isNoThreadedMode() )
    {
        m_strNotifyBody = strBody;
        return false;
    }
    if ( oNotify.m_cCallback )
    {
        int nRet = (*oNotify.m_cCallback)(strBody.c_str(), oNotify.m_cCallbackData);
        return nRet == 1;
    }
    if ( oNotify.m_strUrl.length() == 0 )
        return true;

    NetResponse(resp,getNet().pushData( oNotify.m_strUrl, strBody, null ));
    if ( !resp.isOK() )
        LOG(ERROR) + "Fire notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
    else
    {
        const char* szData = resp.getCharData();
        return szData && strcmp(szData,"stop") == 0;
    }

    return false;
}

void CSyncNotify::clearNotification(CSyncSource* src)
{
    LOG(INFO) + "Clear notification. Source : " + (src != null ? (*src).getName() : "");

    synchronized(m_mxSyncNotifications)
    {
        if ( getSync().isSearch() )
            m_pSearchNotification = null;
        else if ( src != null )
            m_mapSyncNotifications.remove( (*src).getID());
    }
}

void CSyncNotify::clearSyncNotification(int source_id) 
{
	LOG(INFO) + "Clear notification. Source ID: " + source_id;
	
    synchronized(m_mxSyncNotifications)
    {
        if ( source_id == -1 )//Clear all
            m_pAllNotification = null;
        else
            m_mapSyncNotifications.remove(source_id);
    }
}

void CSyncNotify::cleanLastSyncObjectCount()
{
    synchronized(m_mxSyncNotifications)
    {
        m_hashSrcObjectCount.clear();
    }
}

int CSyncNotify::incLastSyncObjectCount(int nSrcID)
{
    int nCount = 0;
    synchronized(m_mxSyncNotifications)
    {
        nCount = m_hashSrcObjectCount.get(nSrcID)+1;
        m_hashSrcObjectCount.put(nSrcID,nCount);
    }

    return nCount;
}

int CSyncNotify::getLastSyncObjectCount(int nSrcID)
{
    int nCount = 0;
    synchronized(m_mxSyncNotifications)
    {
        nCount = m_hashSrcObjectCount.get(nSrcID);
    }

    return nCount;
}

void CSyncNotify::callLoginCallback(const CSyncNotification& oNotify, int nErrCode, String strMessage)
{
	if ( getSync().isStoppedByUser() )
		return;

	//try{
    String strBody = "error_code=" + convertToStringA(nErrCode);
    strBody += "&error_message=";
    URI::urlEncode(strMessage, strBody);
    strBody += "&rho_callback=1";

    LOG(INFO) + "Login callback: " + oNotify.toString() + ". Body: "+ strBody;

    callNotify(oNotify, strBody);
	//}catch(Exception exc)
	//{
	//	LOG.ERROR("Call Login callback failed.", exc);
	//}
}
}
}
