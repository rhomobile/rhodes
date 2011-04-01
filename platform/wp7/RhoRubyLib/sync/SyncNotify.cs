using System;
using rho.common;
using rho.db;
using rho.net;

namespace rho.sync
{
    public class SyncNotify
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		    new RhoLogger("Sync");
        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }

        public class SyncNotification
        {
            public String m_strUrl ="", m_strParams="";
            public boolean m_bRemoveAfterFire;
        
            public SyncNotification(){m_bRemoveAfterFire = false;}
        
            public SyncNotification(String strUrl, String strParams, boolean bRemoveAfterFire)
            {
                if ( strUrl.length() > 0 )
                    m_strUrl = RHODESAPP().canonicalizeRhoUrl(strUrl);
        	
        	    m_strParams = strParams; 
        	    m_bRemoveAfterFire = bRemoveAfterFire; 
            }
        };
	
	    public static int enNone = 0, enDelete=1, enUpdate=2, enCreate=3;
	
        SyncEngine m_syncEngine;

        static String m_strObjectNotifyUrl = "";
        Hashtable<int, Hashtable<String,int> > m_hashSrcIDAndObject = new Hashtable<int, Hashtable<String,int> >();
        Hashtable<int, Hashtable<String,String> > m_hashCreateObjectErrors = new Hashtable<int, Hashtable<String,String> >();
        String m_strSingleObjectSrcName = "", m_strSingleObjectID ="";
        Hashtable<int,int> m_hashSrcObjectCount = new Hashtable<int,int>();
    
        static Mutex m_mxObjectNotify = new Mutex();

        Hashtable<int,SyncNotification> m_mapSyncNotifications = new Hashtable<int,SyncNotification>();
        SyncNotification m_pSearchNotification;
        SyncNotification m_pAllNotification;
        SyncNotification m_emptyNotify = new SyncNotification();
    
        Mutex m_mxSyncNotifications = new Mutex();
        ISyncStatusListener m_syncStatusListener = null;
        boolean m_bEnableReporting = false;
        boolean m_bEnableReportingGlobal = false;
        String m_strNotifyBody = "";
        NetRequest m_NetRequest;
    
        SyncEngine getSync(){ return m_syncEngine; }
        NetRequest getNet() { return m_NetRequest;}

	    public String getNotifyBody(){ return m_strNotifyBody; }
	    public void cleanNotifyBody(){ m_strNotifyBody = ""; }

        public boolean isReportingEnabled(){return m_bEnableReporting&&m_bEnableReportingGlobal;}
        public void enableReporting(boolean bEnable) {	m_bEnableReporting = bEnable; }
        public void enableStatusPopup(boolean bEnable){m_bEnableReportingGlobal = bEnable;}
    
        public SyncNotify( SyncEngine syncEngine ) 
        {
    	    m_syncEngine = syncEngine;
        }
    
        public void setFactory(){ 
		    m_NetRequest = RhoClassFactory.createNetRequest();
        }

        public void addObjectNotify(int nSrcID, String strObject)
        {
    	    lock(m_mxObjectNotify)
    	    {
	            Hashtable<String,int> hashObject = m_hashSrcIDAndObject.get(nSrcID);
	            if ( hashObject == null )
	            {
	                hashObject = new Hashtable<String,int>();
	                m_hashSrcIDAndObject.put( nSrcID, hashObject );
	            }
	
	            hashObject.put(strObject,enNone);
    	    }
        }

        public void addObjectNotify(String strSrcName, String strObject )
        {
    	    lock(m_mxObjectNotify)
    	    {
	            m_strSingleObjectSrcName = strSrcName;
	            m_strSingleObjectID = strObject.charAt(0) == '{' ? strObject.substring(1,strObject.length()-2) : strObject ;
    	    }
        }

        void cleanObjectNotifications()
        {
            lock(m_mxObjectNotify)
            {
                m_strSingleObjectSrcName = "";
                m_strSingleObjectID = "";
                m_hashSrcIDAndObject.clear();
            }
        }

        public void cleanCreateObjectErrors()
        {
            lock(m_mxObjectNotify)
            {
                m_hashCreateObjectErrors.clear();
            }
        }

        void processSingleObject()
        {
            if ( m_strSingleObjectSrcName.length() == 0 )
                return;

            SyncSource Src = getSync().findSourceByName(m_strSingleObjectSrcName);
            if ( Src != null)
            {
                int nSrcID = Src.getID();
                addObjectNotify(nSrcID,m_strSingleObjectID);
            }

            m_strSingleObjectSrcName = "";
            m_strSingleObjectID = "";
        }

        public void fireObjectsNotification()
        {
    	    String strBody = "";
            String strUrl = "";

            lock(m_mxObjectNotify)
            {
                //TODO: fireObjectsNotification
                //if ( m_strObjectNotifyUrl.length() == 0 )
                    return;

                strUrl = getNet().resolveUrl(m_strObjectNotifyUrl);
                Hashtable<int, Hashtable<String,int> >.Enumerator hashEnum = m_hashSrcIDAndObject.GetEnumerator();
		        while( hashEnum.MoveNext() )
		        {
				    int nSrcID = hashEnum.Current.Key;
				    Hashtable<String,int> hashObject = hashEnum.Current.Value;

                    Hashtable<String,int>.Enumerator hashObjectEnum = hashObject.GetEnumerator();
		            while( hashObjectEnum.MoveNext() )
		            {
        			    int nNotifyType = hashObjectEnum.Current.Value;
        			    String strObject = hashObjectEnum.Current.Key;
        			
                        if (nNotifyType == enNone)
                            continue;

                      //This is slow operation
    /*
                        if ( nNotifyType == enDelete.intValue() )
                        {
                            IDBResult res = getDB().executeSQL("SELECT object FROM object_values where object=? LIMIT 1 OFFSET 0", strObject );
                            if ( !res.isOneEnd() )
                                nNotifyType = enUpdate.intValue();    
                        }
    */
                        if ( strBody.length() > 0 )
                    	    strBody += "&rho_callback=1&";

                        if (nNotifyType == enDelete ) 
                        {
                            strBody += "deleted[][object]=" + strObject;
                            strBody += "&deleted[][source_id]=" + nSrcID;
                        }else if ( nNotifyType == enUpdate )
                        {
                            strBody += "updated[][object]=" + strObject;
                            strBody += "&updated[][source_id]=" + nSrcID;
                        }else if ( nNotifyType == enCreate )
                        {
                            strBody += "created[][object]=" + strObject;
                            strBody += "&created[][source_id]=" + nSrcID;
                        }

                        hashObject.put(strObject, enNone);
                    }
                }

                if ( strBody.length() == 0 )
                    return;
            }

            callNotify( new SyncNotification(strUrl,"",false), strBody);
        }

        public void onObjectChanged(int nSrcID, String strObject, int nType)
        {
            lock(m_mxObjectNotify)
            {
                processSingleObject();

                Hashtable<String,int> hashObject = m_hashSrcIDAndObject.get(nSrcID);
                if ( hashObject == null )
                    return;

                if( hashObject.containsKey(strObject) )
                    hashObject.put(strObject, nType);
            }
        }

        void addCreateObjectError(int nSrcID, String strObject, String strError)
        {
            lock(m_mxObjectNotify)
            {
                Hashtable<String,String> hashErrors = m_hashCreateObjectErrors.get(nSrcID);
                if ( hashErrors == null )
                {
                    hashErrors = new Hashtable<String,String>();
                    m_hashCreateObjectErrors.put( nSrcID, hashErrors );
                }

                hashErrors.put(strObject,strError);
            }
        }

        String makeCreateObjectErrorBody(int nSrcID)
        {
            String strBody = "";

            lock(m_mxObjectNotify)
            {
                Hashtable<String,String> hashErrors = m_hashCreateObjectErrors.get(nSrcID);
                if ( hashErrors == null )
                    return "";

                Hashtable<String,String>.Enumerator hashEnum = hashErrors.GetEnumerator();
		        while( hashEnum.MoveNext() )
		        {
    			    String strObject = hashEnum.Current.Key;
    			    String strError = hashEnum.Current.Value;
                    strBody += "&create_error[][object]=" + strObject;
                    strBody += "&create_error[][error_message]=" + strError;
                }

                hashErrors.clear();
            }

            return strBody;
        }

        public static void setObjectNotifyUrl(String strUrl)
        { 
            lock(m_mxObjectNotify)
            {
                m_strObjectNotifyUrl = strUrl; 
            }
        }

        String getObjectNotifyUrl()
        {
            lock(m_mxObjectNotify)
            {
                return m_strObjectNotifyUrl;
            }
        }
   
        public void onSyncSourceEnd( int nSrc, Vector<SyncSource> sources )
        {
            SyncSource src = sources.elementAt(nSrc);

            if ( getSync().getState() == SyncEngine.esStop && src.m_nErrCode != RhoAppAdapter.ERR_NONE )
            {
        	    SyncNotification pSN = getSyncNotifyBySrc(src);
        	    if ( pSN != null )        	
        		    fireSyncNotification(src, true, src.m_nErrCode, "");
        	    else
        		    fireAllSyncNotifications(true, src.m_nErrCode, src.m_strError, "" );
            }
            else
                fireSyncNotification( src, true, src.m_nErrCode, "");

            cleanCreateObjectErrors();
        }
    /*    
	    void setSyncNotification(int source_id, String strUrl, String strParams )throws Exception
	    {
		    LOG.INFO( "Set notification. Source ID: " + source_id + "; Url :" + strUrl + "; Params: " + strParams );
	        String strFullUrl = getNet().resolveUrl(strUrl);
		
		    if ( source_id == -1 )
		    {
			    synchronized(m_mxSyncNotifications)
			    {
				    m_pAllNotification = new SyncNotification( strFullUrl, strParams, false );				
			    }
			    LOG.INFO( " Done Set notification for all sources; Url :" + strFullUrl + "; Params: " + strParams );			
		    }else
		    {
		        if ( strFullUrl.length() > 0 )
		        {
		            synchronized(m_mxSyncNotifications){
		        	    m_mapSyncNotifications.put(new Integer(source_id),new SyncNotification( strFullUrl, strParams, true ) );
		            }
				    LOG.INFO( " Done Set notification. Source ID: " + source_id + "; Url :" + strFullUrl + "; Params: " + strParams );
		        }
		    }
	    }*/
        public void setSyncNotification(int source_id, SyncNotification pNotify)
        {
            LOG.INFO("Set notification. Source ID: " + source_id + ";" + (pNotify != null? pNotify.ToString() : "") );

    	    if ( source_id == -1 )
    	    {
    		    lock(m_mxSyncNotifications)
                {
                    m_pAllNotification = pNotify;
                }
    	    }else
    	    {
                lock(m_mxSyncNotifications)
                {
                    m_mapSyncNotifications.put( source_id, pNotify );
                }
            }
    	
        }

	    public void setSearchNotification(String strUrl, String strParams )
	    {
		    LOG.INFO( "Set search notification. Url :" + strUrl + "; Params: " + strParams );
	        String strFullUrl = getNet().resolveUrl(strUrl);
		
	        if ( strFullUrl.length() > 0 )
	        {
	            lock(m_mxSyncNotifications){
	        	    m_pSearchNotification = new SyncNotification( strFullUrl, strParams, true );
	            }
			    LOG.INFO( " Done Set search notification. Url :" + strFullUrl + "; Params: " + strParams );
	        }
	    }

        public void setSyncStatusListener(ISyncStatusListener listener) 
        { 
    	    lock(m_mxSyncNotifications){
    		    m_syncStatusListener = listener;
    	    }
        }
    
        public void reportSyncStatus(String status, int error, String strDetails) 
        {
    	    lock(m_mxSyncNotifications)
    	    {    	
	    	    if (m_syncStatusListener != null && (isReportingEnabled() || error == RhoAppAdapter.ERR_SYNCVERSION) ) {
	    		
	    		    if ( error == RhoAppAdapter.ERR_SYNCVERSION )
	    			    status = RhoAppAdapter.getErrorText(error);
	    		    else
	    		    {
		    		    if ( strDetails.length() == 0 )
		    			    strDetails = RhoAppAdapter.getErrorText(error);
		    		    status += (strDetails.length() > 0 ? RhoAppAdapter.getMessageText("details") + strDetails: "");
	    		    }
	    		
	        	    LOG.INFO("Status: "+status);
	    		
	        	    m_syncStatusListener.reportStatus( status, error);
	    	    }
    	    }
        }

	    public void fireBulkSyncNotification( boolean bFinish, String status, String partition, int nErrCode )
	    {
		    if ( getSync().getState() == SyncEngine.esExit )
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

	        doFireSyncNotification( null, bFinish, nErrCode, "", strParams, "" );
	    }

	    public void fireAllSyncNotifications( boolean bFinish, int nErrCode, String strError, String strServerError )
	    {
	        if ( getSync().getState() == SyncEngine.esExit )
			    return;

		    if( nErrCode != RhoAppAdapter.ERR_NONE)
		    {
			    if ( !getSync().isSearch() )
			    {
				    String strMessage = RhoAppAdapter.getMessageText("sync_failed_for") + "all.";
				
				    reportSyncStatus(strMessage,nErrCode,strError);
			    }
		    }
	    
	        lock(m_mxSyncNotifications)
	        {
	            SyncNotification pSN = getSyncNotifyBySrc(null);    
	            if ( pSN != null )
	                doFireSyncNotification( null, bFinish, nErrCode, strError, "", strServerError );
	        }
	    }

	    public void fireSyncNotification( SyncSource src, boolean bFinish, int nErrCode, String strMessage )
	    {
		    if ( getSync().getState() == SyncEngine.esExit )
			    return;
		
		    if( strMessage.length() > 0 || nErrCode != RhoAppAdapter.ERR_NONE)
		    {
			    if ( !getSync().isSearch() )
			    {
				    if ( src != null && (strMessage==null || strMessage.length() == 0) )
					    strMessage = RhoAppAdapter.getMessageText("sync_failed_for") + src.getName() + ".";
				
				    reportSyncStatus(strMessage,nErrCode,src!= null?src.m_strError:"");
			    }
		    }
		
		    doFireSyncNotification(src, bFinish, nErrCode, "", "", "" );
	    }
	
	    SyncNotification getSyncNotifyBySrc(SyncSource src)
	    {
	        SyncNotification pSN = null;
		    if ( getSync().isSearch() )
			    pSN = m_pSearchNotification;
		    else
	        {
	            if ( src != null )
			        pSN = (SyncNotification)m_mapSyncNotifications.get( src.getID());

	            if ( pSN == null )
	                pSN = m_pAllNotification;
	        }

		    if ( pSN == null && !getSync().isNoThreadedMode() )
	            return null;

	        return pSN != null ? pSN : m_emptyNotify;
	    }
	
	    void doFireSyncNotification( SyncSource src, boolean bFinish, int nErrCode, String strError, String strParams, String strServerError )
	    {
		    if ( getSync().isStoppedByUser() )
			    return;
		
		    try{
			    SyncNotification pSN = null;		
			
		        String strBody = "";
		        boolean bRemoveAfterFire = bFinish;
		        {
		    	    lock(m_mxSyncNotifications){
		    		    pSN = getSyncNotifyBySrc(src);
			            if ( pSN == null )
			                return;
			
				        strBody = "";

		                if ( src != null )
		                {
				            strBody += "total_count=" + src.getTotalCount();
				            strBody += "&processed_count=" + src.getCurPageCount();
				            strBody += "&processed_objects_count=" + getLastSyncObjectCount(src.getID());
				            strBody += "&cumulative_count=" + src.getServerObjectsCount();			        
				            strBody += "&source_id=" + src.getID();
				            strBody += "&source_name=" + src.getName();
		                }
		            
		                if ( strParams.length() > 0 )
		            	    strBody += (strBody.length() > 0 ? "&" : "") + strParams;
		                else
		            	    strBody += (strBody.length() > 0 ? "&" : "") + "sync_type=incremental";
			        
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
						        strBody += "&error_code=" + nErrCode;
						    
						        if ( strError != null && strError.length() > 0 )
						    	    strBody += "&error_message=" + URI.urlEncode(strError);
						        else  if ( src != null )
						    	    strBody += "&error_message=" + URI.urlEncode(src.m_strError);
						    
						        if ( strServerError != null && strServerError.length() > 0 )
						    	    strBody += "&" + strServerError;
						        else if ( src != null && src.m_strServerError != null && src.m_strServerError.length() > 0  )
						    	    strBody += "&" + src.m_strServerError;						    
				            }
				        
		                    if ( src != null )
		                        strBody += makeCreateObjectErrorBody( src.getID());
			            }
			            else
			        	    strBody += "in_progress";
			        
			            strBody += "&rho_callback=1";
			            if ( pSN.m_strParams != null && pSN.m_strParams.length() > 0 )
			            {
			        	    if ( !pSN.m_strParams.startsWith("&") )
			        		    strBody += "&";
			        	
			        	    strBody += pSN.m_strParams;
			            }			        	
			        
			            bRemoveAfterFire = bRemoveAfterFire && pSN.m_bRemoveAfterFire;
		            }
		        }
		        if ( bRemoveAfterFire )
		    	    clearNotification(src);
		    
		        LOG.INFO("Fire notification. Source : " + (src != null ? (src).getName():"") + "; " + pSN.ToString());
		    
                if ( callNotify(pSN, strBody) )
                    clearNotification(src);
		    }catch(Exception exc)
		    {
			    LOG.ERROR("Fire notification failed.", exc);
		    }
	    }

        boolean callNotify(SyncNotification oNotify, String strBody )
        {
            if ( getSync().isNoThreadedMode() )
            {
                m_strNotifyBody = strBody;
                return false;
            }

            if ( oNotify.m_strUrl.length() == 0 )
                return true;
        
            NetResponse resp = getNet().pushData( oNotify.m_strUrl, strBody, null );
            if ( !resp.isOK() )
                LOG.ERROR( "Fire object notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData() );
            else
            {
                String szData = resp.getCharData();
                return szData != null && szData.equals("stop");
            }

            return false;
        }

	    void clearNotification(SyncSource src)
	    {
		    LOG.INFO( "Clear notification. Source : " + (src != null ? src.getName() : "" ) );

	        lock(m_mxSyncNotifications)
	        {
	    	    if ( getSync().isSearch() )
	                m_pSearchNotification = null;
	            else
	                m_mapSyncNotifications.remove(src.getID());
	        }
	    }
	
	    void clearSyncNotification(int source_id) 
	    {
		    LOG.INFO( "Clear notification. Source ID: " + source_id );
		
		    lock(m_mxSyncNotifications){
			    if ( source_id == -1 )//Clear all
				    m_pAllNotification = null;
			    else			
				    m_mapSyncNotifications.remove(source_id);
		    }
	    }

	    public void cleanLastSyncObjectCount()
	    {
	        lock(m_mxSyncNotifications)
	        {
	            m_hashSrcObjectCount.clear();
	        }
	    }

	    public int incLastSyncObjectCount(int nSrcID)
	    {
		    int nCount = 0;
	        lock(m_mxSyncNotifications)
	        {
	            nCount = m_hashSrcObjectCount.get(nSrcID);
	            nCount = nCount + 1;
	        
	            m_hashSrcObjectCount.put(nSrcID,nCount);
	        }
	    
	        return nCount;
	    }

	    int getLastSyncObjectCount(int nSrcID)
	    {
	        int nCount = 0;
	        lock(m_mxSyncNotifications)
	        {
                nCount = m_hashSrcObjectCount.get(nSrcID);
	        }

	        return nCount;
	    }

	    public void callLoginCallback(SyncNotification oNotify, int nErrCode, String strMessage)
	    {
		    try{
			    if ( getSync().isStoppedByUser() )
				    return;
			
		        String strBody = "error_code=" + nErrCode;
	            strBody += "&error_message=" + URI.urlEncode(strMessage != null? strMessage : "");
	            strBody += "&rho_callback=1";
	        
			    LOG.INFO( "Login callback: " + oNotify.ToString() + ". Body: "+ strBody );

                callNotify(oNotify, strBody);	
		    }catch(Exception exc)
		    {
			    LOG.ERROR("Call Login callback failed.", exc);
		    }
	    }
    }
}
