package com.rho.sync;

import com.rho.Mutex;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhoRuby;

import java.util.Enumeration;
import java.util.Hashtable;
import com.rho.*;
import com.rho.db.*;
import com.rho.net.*;
import java.util.Vector;

public class SyncNotify {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Sync");

    static class SyncNotification
    {
        String m_strUrl, m_strParams;
        boolean m_bRemoveAfterFire;
        
        SyncNotification(String strUrl, String strParams, boolean bRemoveAfterFire)
        { m_strUrl = strUrl; m_strParams = strParams; m_bRemoveAfterFire = bRemoveAfterFire; }
    };
	
	public static final Integer enNone = new Integer(0), enDelete=new Integer(1), enUpdate=new Integer(2), enCreate=new Integer(3);
	
    SyncEngine m_syncEngine;

    static String m_strObjectNotifyUrl = "";
    Hashtable/*<int, Hashtable<String,int>* >*/ m_hashSrcIDAndObject = new Hashtable();
    Hashtable/*<int, Hashtable<String,String>* >*/ m_hashCreateObjectErrors = new Hashtable();
    String m_strSingleObjectSrcName = "", m_strSingleObjectID ="";
    Hashtable/*<int,int>*/ m_hashSrcObjectCount = new Hashtable();
    
    static Mutex m_mxObjectNotify = new Mutex();

    Hashtable/*<int,SyncNotification>*/ m_mapSyncNotifications = new Hashtable();
    SyncNotification m_pSearchNotification;
    SyncNotification m_bulkSyncNotify;
    
    Mutex m_mxSyncNotifications = new Mutex();
    ISyncStatusListener m_syncStatusListener = null;
    boolean m_bEnableReporting = false;
    String m_strNotifyBody = "";
    
    SyncEngine getSync(){ return m_syncEngine; }
	NetRequest getNet(){ return getSync().getNet(); }

	String getNotifyBody(){ return m_strNotifyBody; }
	void cleanNotifyBody(){ m_strNotifyBody = ""; }
	
    SyncNotify( SyncEngine syncEngine ) 
    {
    	m_syncEngine = syncEngine;
    }
    
    void addObjectNotify(Integer nSrcID, String strObject)
    {
    	synchronized(m_mxObjectNotify)
    	{
	        Hashtable/*<String,int>*/ hashObject = (Hashtable)m_hashSrcIDAndObject.get(nSrcID);
	        if ( hashObject == null )
	        {
	            hashObject = new Hashtable/*<String,int>*/();
	            m_hashSrcIDAndObject.put( nSrcID, hashObject );
	        }
	
	        hashObject.put(strObject,enNone);
    	}
    }

    void addObjectNotify(String strSrcName, String strObject )
    {
    	synchronized(m_mxObjectNotify)
    	{
	        m_strSingleObjectSrcName = strSrcName;
	        m_strSingleObjectID = strObject.charAt(0) == '{' ? strObject.substring(1,strObject.length()-2) : strObject ;
    	}
    }

    void cleanObjectNotifications()
    {
        synchronized(m_mxObjectNotify)
        {
            m_strSingleObjectSrcName = "";
            m_strSingleObjectID = "";
            m_hashSrcIDAndObject.clear();
        }
    }

    void cleanCreateObjectErrors()
    {
        synchronized(m_mxObjectNotify)
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
            Integer nSrcID = Src.getID();
            addObjectNotify(nSrcID,m_strSingleObjectID);
        }

        m_strSingleObjectSrcName = "";
        m_strSingleObjectID = "";
    }

    void fireObjectsNotification()throws Exception
    {
    	String strBody = "";
        String strUrl = "";

        synchronized(m_mxObjectNotify)
        {
            if ( m_strObjectNotifyUrl.length() == 0 )
                return;

            strUrl = getNet().resolveUrl(m_strObjectNotifyUrl);

        	Enumeration valsNotify = m_hashSrcIDAndObject.elements();
        	Enumeration keysNotify = m_hashSrcIDAndObject.keys();
    		while (valsNotify.hasMoreElements()) 
    		{
    			Integer nSrcID = (Integer)keysNotify.nextElement();
    			Hashtable hashObject = (Hashtable)valsNotify.nextElement();

            	Enumeration valsObject = hashObject.elements();
            	Enumeration keysObject = hashObject.keys();
        		while (valsObject.hasMoreElements()) 
        		{
        			int nNotifyType = ((Integer)valsObject.nextElement()).intValue();
        			String strObject = (String)keysObject.nextElement();
        			
                    if (nNotifyType == enNone.intValue())
                        continue;

                  //This is slow operation
/*
                    if ( nNotifyType == enDelete.intValue() )
                    {
                        IDBResult res = getDB().executeSQL("SELECT object FROM object_values where object=? LIMIT 1 OFFSET 0", strObject );
                        if ( !res.isEnd() )
                            nNotifyType = enUpdate.intValue();    
                    }
*/
                    if ( strBody.length() > 0 )
                    	strBody += "&rho_callback=1&";

                    if (nNotifyType == enDelete.intValue() ) 
                    {
                        strBody += "deleted[][object]=" + strObject;
                        strBody += "&deleted[][source_id]=" + nSrcID;
                    }else if ( nNotifyType == enUpdate.intValue() )
                    {
                        strBody += "updated[][object]=" + strObject;
                        strBody += "&updated[][source_id]=" + nSrcID;
                    }else if ( nNotifyType == enCreate.intValue() )
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

        callNotify(strUrl, strBody);
    }

    void onObjectChanged(Integer nSrcID, String strObject, Integer nType)
    {
        synchronized(m_mxObjectNotify)
        {
            processSingleObject();

            Hashtable/*<String,int>*/ hashObject = (Hashtable)m_hashSrcIDAndObject.get(nSrcID);
            if ( hashObject == null )
                return;

            if( hashObject.containsKey(strObject) )
                hashObject.put(strObject, nType);
        }
    }

    void addCreateObjectError(Integer nSrcID, String strObject, String strError)
    {
        synchronized(m_mxObjectNotify)
        {
            Hashtable/*<String,String>*/ hashErrors = (Hashtable)m_hashCreateObjectErrors.get(nSrcID);
            if ( hashErrors == null )
            {
                hashErrors = new Hashtable/*<String,String>*/();
                m_hashCreateObjectErrors.put( nSrcID, hashErrors );
            }

            hashErrors.put(strObject,strError);
        }
    }

    String makeCreateObjectErrorBody(Integer nSrcID)
    {
        String strBody = "";

        synchronized(m_mxObjectNotify)
        {
            Hashtable/*<String,String>*/ hashErrors = (Hashtable)m_hashCreateObjectErrors.get(nSrcID);
            if ( hashErrors == null )
                return "";

        	Enumeration valsErrors = m_hashSrcIDAndObject.elements();
        	Enumeration keysErrors = m_hashSrcIDAndObject.keys();
    		while (valsErrors.hasMoreElements()) 
    		{
    			String strObject = (String)keysErrors.nextElement();
    			String strError = (String)valsErrors.nextElement();
                strBody += "&create_error[][object]=" + strObject;
                strBody += "&create_error[][error_message]=" + strError;
            }

            hashErrors.clear();
        }

        return strBody;
    }

    static void setObjectNotifyUrl(String strUrl)
    { 
        synchronized(m_mxObjectNotify)
        {
            m_strObjectNotifyUrl = strUrl; 
        }
    }

    String getObjectNotifyUrl()
    {
        synchronized(m_mxObjectNotify)
        {
            return m_strObjectNotifyUrl;
        }
    }
   
    void onSyncSourceEnd( int nSrc, Vector/*Ptr<CSyncSource*>&*/ sources )
    {
        SyncSource src = (SyncSource)sources.elementAt(nSrc);

        if ( getSync().getState() == SyncEngine.esStop )
    		fireAllSyncNotifications(true, src.m_nErrCode, src.m_strError, sources );
        else
            fireSyncNotification( src, true, src.m_nErrCode, "");

        cleanCreateObjectErrors();
    }
    
	void setSyncNotification(int source_id, String strUrl, String strParams )throws Exception
	{
		LOG.INFO( "Set notification. Source ID: " + source_id + "; Url :" + strUrl + "; Params: " + strParams );
	    String strFullUrl = getNet().resolveUrl(strUrl);
		
		if ( source_id == -1 )
		{
			synchronized(m_mxSyncNotifications){
				m_mapSyncNotifications.clear();
				
				if ( strFullUrl.length() > 0 )
				{
	                IDBResult res = DBAdapter.getUserDB().executeSQL("SELECT source_id from sources order by source_id");
	                for ( ; !res.isEnd(); res.next() )
			    	    m_mapSyncNotifications.put( new Integer(res.getIntByIdx(0)), new SyncNotification( strFullUrl, strParams, false ) );
				}
			}
			LOG.INFO( " Done Set notification for all sources; Url :" + strFullUrl + "; Params: " + strParams );			
		}else
		{
		    //clearSyncNotification(source_id);
		    if ( strFullUrl.length() > 0 )
		    {
		        synchronized(m_mxSyncNotifications){
		        	m_mapSyncNotifications.put(new Integer(source_id),new SyncNotification( strFullUrl, strParams, true ) );
		        }
				LOG.INFO( " Done Set notification. Source ID: " + source_id + "; Url :" + strFullUrl + "; Params: " + strParams );
		    }
		}
	}

	void setSearchNotification(String strUrl, String strParams )throws Exception
	{
		LOG.INFO( "Set search notification. Url :" + strUrl + "; Params: " + strParams );
	    String strFullUrl = getNet().resolveUrl(strUrl);
		
	    if ( strFullUrl.length() > 0 )
	    {
	        synchronized(m_mxSyncNotifications){
	        	m_pSearchNotification = new SyncNotification( strFullUrl, strParams, true );
	        }
			LOG.INFO( " Done Set search notification. Url :" + strFullUrl + "; Params: " + strParams );
	    }
	}

	void setBulkSyncNotification(String strUrl, String strParams )throws Exception
	{
	    String strFullUrl = getNet().resolveUrl(strUrl);
		
		m_bulkSyncNotify = new SyncNotification( strFullUrl, strParams, false );
	}
	
    public void setSyncStatusListener(ISyncStatusListener listener) 
    { 
    	synchronized(m_mxSyncNotifications){
    		m_syncStatusListener = listener;
    	}
    }
    
    void enableReporting(boolean bEnable)
    {
    	m_bEnableReporting = bEnable;
    }
    
    private void reportSyncStatus(String status, int error, String strDetails) 
    {
    	synchronized(m_mxSyncNotifications)
    	{    	
	    	if (m_syncStatusListener != null && (m_bEnableReporting || error == RhoRuby.ERR_SYNCVERSION) ) {
	    		
	    		if ( error == RhoRuby.ERR_SYNCVERSION )
	    			status = RhoRuby.getErrorText(error);
	    		else
	    		{
		    		if ( strDetails.length() == 0 )
		    			strDetails = RhoRuby.getErrorText(error);
		    		status += (strDetails.length() > 0 ? RhoRuby.getMessageText("details") + strDetails: "");
	    		}
	    		
	        	LOG.INFO("Status: "+status);
	    		
	        	m_syncStatusListener.reportStatus( status, error);
	    	}
    	}
    }

	void fireAllSyncNotifications( boolean bFinish, int nErrCode, String strError, Vector/*Ptr<CSyncSource*>&*/ sources )
	{
	    for( int i = 0; i < sources.size(); i++ )
	    {
	    	doFireSyncNotification( (SyncSource)sources.elementAt(i), bFinish, nErrCode, strError );
	    }
	}

	void fireBulkSyncNotification( boolean bFinish, String status, String partition, int nErrCode )
	{
		if ( getSync().getState() == SyncEngine.esExit )
			return;
		
		//TODO: show report
		if( nErrCode != RhoRuby.ERR_NONE)
		{
			String strMessage = RhoRuby.getMessageText("sync_failed_for") + "bulk.";
			reportSyncStatus(strMessage,nErrCode,"");
		}
		
		try{
		    boolean bRemoveAfterFire = bFinish;
		    String strBody = "", strUrl;
			synchronized(m_mxSyncNotifications)
			{
		        if ( m_bulkSyncNotify == null )
		            return;
		        
		        strUrl = m_bulkSyncNotify.m_strUrl;
		        strBody = "rho_callback=1";
		        strBody += "&partition=" + partition;
		        strBody += "&bulk_status="+status;
		        strBody += "&status=";
		        if ( bFinish )
		        {
			        if ( nErrCode == RhoRuby.ERR_NONE )
			        	strBody += "ok";
			        else
			        {
			        	if ( getSync().isStoppedByUser() )
		                    nErrCode = RhoRuby.ERR_CANCELBYUSER;
			        	
			        	strBody += "error";				        	
					    strBody += "&error_code=" + nErrCode;
			        }
		        }
		        else
		        	strBody += "in_progress";
		        
		        if ( m_bulkSyncNotify.m_strParams.length() > 0 )
		            strBody += "&" + m_bulkSyncNotify.m_strParams;
		        
		        bRemoveAfterFire = bRemoveAfterFire && m_bulkSyncNotify.m_bRemoveAfterFire;
			}
			
		    if ( bRemoveAfterFire )
		    	clearBulkSyncNotification();
		    
			LOG.INFO( "Fire bulk notification.Url :" + strUrl + "; Body: " + strBody );

            if ( callNotify(strUrl, strBody) )
    	        clearBulkSyncNotification();
		}catch(Exception exc)
		{
			LOG.ERROR("Fire bulk notification failed.", exc);
		}
    	
	}
	
	void fireSyncNotification( SyncSource src, boolean bFinish, int nErrCode, String strMessage )
	{
		if ( getSync().getState() == SyncEngine.esExit )
			return;
		
		if( strMessage.length() > 0 || nErrCode != RhoRuby.ERR_NONE)
		{
			if ( !( src != null && src.isSearch()) )
			{
				if ( src != null && (strMessage==null || strMessage.length() == 0) )
					strMessage = RhoRuby.getMessageText("sync_failed_for") + src.getName() + ".";
				
				reportSyncStatus(strMessage,nErrCode,src!= null?src.m_strError:"");
			}
		}
		
		doFireSyncNotification(src, bFinish, nErrCode, "" );
	}
	
	void doFireSyncNotification( SyncSource src, boolean bFinish, int nErrCode, String strError )
	{
		if ( src == null || getSync().isStoppedByUser() )
			return; //TODO: implement all sources callback
		
		try{
		    String strBody = "", strUrl;
		    boolean bRemoveAfterFire = bFinish;
		    {
		    	synchronized(m_mxSyncNotifications){
			        SyncNotification sn = null;
					if ( src.isSearch() )
						sn = m_pSearchNotification;
					else
						sn = (SyncNotification)m_mapSyncNotifications.get(src.getID());
			        
			        if ( sn == null )
			            return;
			
			        strUrl = sn.m_strUrl;
			        strBody += "total_count=" + src.getTotalCount();
			        strBody += "&processed_count=" + src.getCurPageCount();
			        strBody += "&processed_objects_count=" + getLastSyncObjectCount(src.getID());
			        strBody += "&cumulative_count=" + src.getServerObjectsCount();			        
			        strBody += "&source_id=" + src.getID();
			        strBody += "&source_name=" + src.getName();
			        strBody += "&rho_callback=1";
			        
			        strBody += "&status=";
			        if ( bFinish )
			        {
				        if ( nErrCode == RhoRuby.ERR_NONE )
				        	strBody += "ok";
				        else
				        {
				        	if ( getSync().isStoppedByUser() )
		                        nErrCode = RhoRuby.ERR_CANCELBYUSER;
				        	
				        	strBody += "error";				        	
						    strBody += "&error_code=" + nErrCode;
						    
						    if ( strError != null && strError.length() > 0 )
						    	strBody += "&error_message=" + URI.urlEncode(strError);
						    else
						    	strBody += "&error_message=" + URI.urlEncode(src.m_strError);
				        }
			        }
			        else
			        	strBody += "in_progress";
			        
			        if ( sn.m_strParams.length() > 0 )
			            strBody += "&" + sn.m_strParams;
			        
			        bRemoveAfterFire = bRemoveAfterFire && sn.m_bRemoveAfterFire;
		        }
		    }
		    if ( bRemoveAfterFire )
		    	clearNotification(src);
		    
			LOG.INFO( "Fire notification. Source ID: " + src.getID() + "; Url :" + strUrl + "; Body: " + strBody );

            if ( callNotify(strUrl, strBody) )
                clearNotification(src);
		}catch(Exception exc)
		{
			LOG.ERROR("Fire notification failed.", exc);
		}
	}

    boolean callNotify(String strUrl, String strBody )throws Exception
    {
        if ( getSync().isNoThreadedMode() )
        {
            m_strNotifyBody = strBody;
            return false;
        }

        NetResponse resp = getNet().pushData( strUrl, strBody, null );
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
		LOG.INFO( "Clear notification. Source : " + src.getName());

	    synchronized(m_mxSyncNotifications)
	    {
	        if ( src.isSearch() )
	            m_pSearchNotification = null;
	        else
	            m_mapSyncNotifications.remove(src.getID());
	    }
	}
	
	void clearSyncNotification(int source_id) 
	{
		LOG.INFO( "Clear notification. Source ID: " + source_id );
		
		synchronized(m_mxSyncNotifications){
			if ( source_id == -1 )//Clear all
				m_mapSyncNotifications.clear();
			else			
				m_mapSyncNotifications.remove(new Integer(source_id));
		}
	}

	void clearBulkSyncNotification() 
	{
		LOG.INFO( "Clear bulk notification." );
		
		synchronized(m_mxSyncNotifications){
			m_bulkSyncNotify = null;
		}
	}
	
	void cleanLastSyncObjectCount()
	{
	    synchronized(m_mxSyncNotifications)
	    {
	        m_hashSrcObjectCount.clear();
	    }
	}

	int incLastSyncObjectCount(Integer nSrcID)
	{
		Integer nCount = null;
	    synchronized(m_mxSyncNotifications)
	    {
	        nCount = ((Integer)m_hashSrcObjectCount.get(nSrcID));
	        if ( nCount == null )
	        	nCount = new Integer(0);
	        
	        nCount = new Integer(nCount.intValue()+1);
	        
	        m_hashSrcObjectCount.put(nSrcID,nCount);
	    }
	    
	    return nCount != null ? nCount.intValue() : 0;
	}

	int getLastSyncObjectCount(Integer nSrcID)
	{
	    int nCount = 0;
	    synchronized(m_mxSyncNotifications)
	    {
	        Integer nCountVal = (Integer)m_hashSrcObjectCount.get(nSrcID);
	        if ( nCountVal != null )
	        	nCount = nCountVal.intValue();
	    }

	    return nCount;
	}

	void callLoginCallback(String callback, int nErrCode, String strMessage)
	{
		try{
		    String strBody = "error_code=" + nErrCode;
	        strBody += "&error_message=" + URI.urlEncode(strMessage != null? strMessage : "");
	        strBody += "&rho_callback=1";
	        
	        String strUrl = getNet().resolveUrl(callback);
	        
			LOG.INFO( "Login callback: " + callback + ". Body: "+ strBody );

            callNotify(strUrl, strBody);	
		}catch(Exception exc)
		{
			LOG.ERROR("Call Login callback failed.", exc);
		}
	}
	
	
}
