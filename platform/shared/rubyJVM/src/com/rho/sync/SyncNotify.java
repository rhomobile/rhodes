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
        SyncNotification(String strUrl, String strParams){ m_strUrl = strUrl; m_strParams = strParams; }
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
    Mutex m_mxSyncNotifications = new Mutex();
    ISyncStatusListener m_syncStatusListener = null;
    
    SyncEngine getSync(){ return m_syncEngine; }
	DBAdapter getDB(){ return getSync().getDB(); }
	NetRequest getNet(){ return getSync().getNet(); }

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

                    if ( nNotifyType == enDelete.intValue() )
                    {
                        IDBResult res = getDB().executeSQL("SELECT object FROM object_values where object=? LIMIT 1 OFFSET 0", strObject );
                        if ( !res.isEnd() )
                            nNotifyType = enUpdate.intValue();    
                    }

                    if ( strBody.length() > 0 )
                        strBody += '&';

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

        NetResponse resp = getNet().pushData( strUrl, strBody, getSync() );
        if ( !resp.isOK() )
            LOG.ERROR( "Fire object notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData() );

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

        fireSyncNotification( src, true, src.m_nErrCode, "");
        if ( getSync().getState() == SyncEngine.esStop )
    		fireAllSyncNotifications(true, src.m_nErrCode, "", sources );

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
	                IDBResult res = getDB().executeSQL("SELECT source_id from sources order by source_id");
	                for ( ; !res.isEnd(); res.next() )
			    	    m_mapSyncNotifications.put( new Integer(res.getIntByIdx(0)), new SyncNotification( strFullUrl, strParams ) );
				}
			}
			LOG.INFO( " Done Set notification for all sources; Url :" + strFullUrl + "; Params: " + strParams );			
		}else
		{
		    //clearSyncNotification(source_id);
		    if ( strFullUrl.length() > 0 )
		    {
		        synchronized(m_mxSyncNotifications){
		        	m_mapSyncNotifications.put(new Integer(source_id),new SyncNotification( strFullUrl, strParams ) );
		        }
				LOG.INFO( " Done Set notification. Source ID: " + source_id + "; Url :" + strFullUrl + "; Params: " + strParams );
		    }
		}
	}

    public void setSyncStatusListener(ISyncStatusListener listener) { m_syncStatusListener = listener; }
    private void reportSyncStatus(String status, int error, String strDetails) {
    	if (m_syncStatusListener != null) {
    		if ( strDetails.length() == 0 )
    			strDetails = RhoRuby.getErrorText(error);
    		status += (strDetails.length() > 0 ? " Details: " + strDetails: "");
    		
        	LOG.INFO("Status: "+status);
    		
        	m_syncStatusListener.reportStatus( status, error);
    	}
    }
	
	void fireAllSyncNotifications( boolean bFinish, int nErrCode, String strMessage, Vector/*Ptr<CSyncSource*>&*/ sources )
	{
	    for( int i = 0; i < sources.size(); i++ )
	    {
	    	doFireSyncNotification( (SyncSource)sources.elementAt(i), bFinish, nErrCode, strMessage );
	    }
	}
	
	void fireSyncNotification( SyncSource src, boolean bFinish, int nErrCode, String strMessage )
	{
		if ( getSync().getState() == SyncEngine.esExit )
			return;
		
		if( strMessage.length() > 0 || nErrCode != RhoRuby.ERR_NONE)
		{
			if ( !( src != null && src.m_strParams.length()>0) )
			{
				if ( src != null && (strMessage==null || strMessage.length() == 0) )
					strMessage = "Sync failed for " + src.getName() + ".";
				
				reportSyncStatus(strMessage,nErrCode,src!= null?src.m_strError:"");
			}
		}
		
		doFireSyncNotification(src, bFinish, nErrCode, strMessage );
	}
	
	void doFireSyncNotification( SyncSource src, boolean bFinish, int nErrCode, String strMessage )
	{
		if ( src == null || getSync().isStoppedByUser() )
			return; //TODO: implement all sources callback
		
		try{
		    String strBody = "", strUrl;
		    {
		    	synchronized(m_mxSyncNotifications){
			        SyncNotification sn = (SyncNotification)m_mapSyncNotifications.get(src.getID());
			        if ( sn == null )
			            return;
			
			        strUrl = sn.m_strUrl;
			        strBody += "total_count=" + src.getTotalCount();
			        strBody += "&processed_count=" + src.getCurPageCount();
			        strBody += "&processed_objects_count=" + getLastSyncObjectCount(src.getID());
			        strBody += "&cumulative_count=" + src.getServerObjectsCount();			        
			        strBody += "&source_id=" + src.getID();
			        strBody += "&source_name=" + src.getName();
			        
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
					        strBody += "&error_message=" + URI.urlEncode(src.m_strError);
				        }
			        }
			        else
			        	strBody += "in_progress";
			        
			        if ( sn.m_strParams.length() > 0 )
			            strBody += "&" + sn.m_strParams;
		        }
		    }
		    if ( bFinish )
		    	clearSyncNotification(src.getID().intValue());
		    
			LOG.INFO( "Fire notification. Source ID: " + src.getID() + "; Url :" + strUrl + "; Body: " + strBody );
			
		    NetResponse resp = getNet().pushData( strUrl, strBody, getSync() );
		    if ( !resp.isOK() )
		        LOG.ERROR( "Fire notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData() );
		    else
		    {
		        String szData = resp.getCharData();
		        if ( szData != null && szData.equals("stop") )
		        {
		            clearSyncNotification(src.getID().intValue());
		        }
		    }
		
		}catch(Exception exc)
		{
			LOG.ERROR("Fire notification failed.", exc);
		}
	}

	void clearSyncNotification(int source_id) 
	{
		LOG.INFO( "Clear notification. Source ID: " + source_id );
		
		synchronized(m_mxSyncNotifications){
			m_mapSyncNotifications.remove(new Integer(source_id));
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
	
}
