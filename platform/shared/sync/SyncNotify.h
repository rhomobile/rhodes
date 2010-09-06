#pragma once

#include "common/RhoStd.h"
#include "logging/RhoLog.h"
#include "common/AutoPointer.h"

typedef int (*RHOC_CALLBACK)(const char* szNotify, void* callback_data);

namespace rho {
namespace db {
    class CDBAdapter;
}

namespace net {
    struct INetRequest;
}

namespace sync {
class CSyncEngine;
class CSyncSource;

struct CSyncNotification
{
    String m_strUrl, m_strParams;
    RHOC_CALLBACK m_cCallback;
    void*         m_cCallbackData;

    boolean m_bRemoveAfterFire;
    CSyncNotification(): m_cCallback(0), m_cCallbackData(0){m_bRemoveAfterFire = false;}

    CSyncNotification(String strUrl, String strParams, boolean bRemoveAfterFire);
    CSyncNotification(RHOC_CALLBACK callback, void* callback_data, boolean bRemoveAfterFire) : 
        m_cCallback(callback), m_cCallbackData(callback_data), m_bRemoveAfterFire(false){}

    String toString()const;

	~CSyncNotification();
};

class CSyncNotify
{
    DEFINE_LOGCLASS;

public:
    enum ENotifyType{ enNone, enDelete, enUpdate, enCreate };

private:

    CSyncEngine& m_syncEngine;

    static String m_strObjectNotifyUrl;
    HashtablePtr<int, Hashtable<String,int>* > m_hashSrcIDAndObject;
    HashtablePtr<int, Hashtable<String,String>* > m_hashCreateObjectErrors;
    String m_strSingleObjectSrcName, m_strSingleObjectID;
    Hashtable<int,int> m_hashSrcObjectCount;
    boolean m_bEnableReporting;

    static common::CMutex m_mxObjectNotify;

    HashtablePtr<int,CSyncNotification*> m_mapSyncNotifications;
    common::CAutoPtr<CSyncNotification> m_pAllNotification;
    common::CAutoPtr<CSyncNotification> m_pSearchNotification;
    CSyncNotification m_emptyNotify;
    common::CMutex m_mxSyncNotifications;
    String m_strNotifyBody;

    net::INetRequest& getNet();
    CSyncEngine& getSync(){ return m_syncEngine; }
public:
    CSyncNotify( CSyncEngine& syncEngine ) : m_syncEngine(syncEngine), m_bEnableReporting(false){}

    //Object notifications
    void fireObjectsNotification();
    void onObjectChanged(int nSrcID, const String& strObject, int nType);
    void addCreateObjectError(int nSrcID, const String& strObject, const String& strError);

    void addObjectNotify(int nSrcID, const String& strObject );
    void addObjectNotify(const String& strSrcName, const String& strObject );
    static void setObjectNotifyUrl(String strUrl);
    static String getObjectNotifyUrl();
    void cleanObjectNotifications();
    void cleanCreateObjectErrors();

    //Sync notifications
    void setSyncNotification(int source_id, CSyncNotification* pNotify);
    void setSearchNotification(CSyncNotification* pNotify);

    void clearSyncNotification(int source_id);

    void onSyncSourceEnd( int nSrc, VectorPtr<CSyncSource*>& sources );
    void fireSyncNotification( CSyncSource* psrc, boolean bFinish, int nErrCode, String strMessage);

    void fireBulkSyncNotification( boolean bFinish, String status, String partition, int nErrCode );

    void cleanLastSyncObjectCount();
    int incLastSyncObjectCount(int nSrcID);
    int getLastSyncObjectCount(int nSrcID);

    void callLoginCallback(const CSyncNotification& oNotify, int nErrCode, String strMessage);

    void enableReporting(boolean bEnable){m_bEnableReporting = bEnable;}

    const String& getNotifyBody(){ return m_strNotifyBody; }
    void cleanNotifyBody(){ m_strNotifyBody = ""; }

    void fireAllSyncNotifications( boolean bFinish, int nErrCode, String strError );
    void reportSyncStatus(String status, int error, String strDetails);

private:
    CSyncNotification* getSyncNotifyBySrc(CSyncSource* src);

    String makeCreateObjectErrorBody(int nSrcID);
    void processSingleObject();

    void doFireSyncNotification( CSyncSource* src, boolean bFinish, int nErrCode, String strError, String strParams);

    boolean callNotify(const CSyncNotification& oNotify, const String& strBody );

    void clearNotification(CSyncSource* src);

};

}
}
