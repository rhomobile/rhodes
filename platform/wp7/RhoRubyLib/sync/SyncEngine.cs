using System;
using rho.common;
using rho.net;
using rho.db;
using rho.json;

namespace rho.sync
{
    public class SyncEngine : NetRequest.IRhoSession
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		    new RhoLogger("Sync");
        private static RhoProfiler PROF = RhoProfiler.RHO_STRIP_PROFILER ? new RhoEmptyProfiler() : 
    		new RhoProfiler();

        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }
	    RhoConf RHOCONF(){ return RhoConf.getInstance(); }
        public CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }
	
        public static int esNone = 0, esSyncAllSources = 1, esSyncSource = 2, esSearch=3, esStop = 4, esExit = 5;

        public class SourceID
        {
            String m_strName = "";
            int m_nID;

            public SourceID(int id, String strName ){ m_nID = id; m_strName = strName; }
            public SourceID(String strName ){ m_strName = strName; }
        
            public String toString()
            {
                if ( m_strName.length() > 0 )
                    return "name : " + m_strName;

                return "# : " + m_nID;
            }
            public boolean isEqual(SyncSource src)
            {
                if ( m_strName.length() > 0 )
                    return src.getName().equals(m_strName);

                return m_nID == src.getID();
            }
        };
    
        public class SourceOptions
        {
            private Mutex m_mxSrcOptions = new Mutex();
            private Hashtable<int, Hashtable<String,String> > m_hashSrcOptions = new Hashtable<int, Hashtable<String,String> >();
        
    	    public void setProperty(int nSrcID, String szPropName, String szPropValue)
    	    {
    	        lock(m_mxSrcOptions)
    	        {
    	            Hashtable<String,String> phashOptions = m_hashSrcOptions.get(nSrcID);
    	            if ( phashOptions == null )
    	            {
    	                phashOptions = new Hashtable<String,String>();
    	                m_hashSrcOptions.put( nSrcID, phashOptions );
    	            }

    	            //Hashtable<String,String>& hashOptions = *phashOptions;
    	            phashOptions.put(szPropName,szPropValue!=null?szPropValue:"");
    	        }
    	    }

            public String getProperty(int nSrcID, String szPropName)
            {
                String res = "";
                lock(m_mxSrcOptions)
                {
                    Hashtable<String,String> phashOptions = m_hashSrcOptions.get(nSrcID);
                    if ( phashOptions != null )
                    {
                        //Hashtable<String,String>& hashOptions = *phashOptions;
                        res = (String)phashOptions.get(szPropName);
                    }
                }

                return res != null ? res : "";
            }
            public boolean getBoolProperty(int nSrcID, String szPropName)
            {
                String strValue = getProperty(nSrcID, szPropName);
                return strValue.compareTo("1") == 0 || strValue.compareTo("true") == 0 ? true : false;
        	
            }
        };
    
        Vector<SyncSource> m_sources = new Vector<SyncSource>();
        NetRequest m_NetRequest, m_NetRequestClientID;
        ISyncProtocol m_SyncProtocol;
        int         m_syncState;
        String     m_clientID = "";
        Mutex m_mxLoadClientID = new Mutex();
        String m_strSession = "";
        SyncNotify m_oSyncNotify;
        boolean m_bStopByUser = false;
        int m_nSyncPageSize = 2000;
        boolean m_bNoThreaded = false;
        int m_nErrCode = RhoAppAdapter.ERR_NONE;
        String m_strError = "",m_strServerError = "";
        boolean m_bIsSearch, m_bIsSchemaChanged;
        static SourceOptions m_oSourceOptions = new SourceOptions();
    
        public void setState(int eState){ m_syncState = eState; }
        public int getState(){ return m_syncState; }
        public boolean isSearch(){ return m_bIsSearch; }
        public boolean isContinueSync(){ return m_syncState != esExit && m_syncState != esStop; }
        public boolean isSyncing() { return m_syncState == esSyncAllSources || m_syncState == esSyncSource; }
        public void stopSync()
        { 
    	    if (isContinueSync())
    	    { 
	    	    setState(esStop); 
	    	    if (m_NetRequest!=null) 
	    		    m_NetRequest.cancel(); 
	    	
	    	    if (m_NetRequestClientID!=null) 
	    		    m_NetRequestClientID.cancel(); 
    	    } 
        }
        public void stopSyncByUser() { m_bStopByUser = true; stopSync(); }
        public void exitSync()
        { 
    	    setState(esExit); 
    	    if (m_NetRequest!=null) 
    		    m_NetRequest.cancel(); 
    	
    	    if (m_NetRequestClientID!=null) 
    		    m_NetRequestClientID.cancel(); 
        }
        public boolean isStoppedByUser(){ return m_bStopByUser; }
    
        public String getClientID(){ return m_clientID; }
        void setSession(String strSession){m_strSession=strSession;}
        boolean isSessionExist(){ return m_strSession != null && m_strSession.length() > 0; }
    
        public void setSchemaChanged(boolean bChanged){ m_bIsSchemaChanged = bChanged; }
        public boolean isSchemaChanged(){ return m_bIsSchemaChanged; }

        public DBAdapter getUserDB() { return DBAdapter.getUserDB(); }
        public DBAdapter getDB(String strPartition) { return DBAdapter.getDB(strPartition); }
    
      //IRhoSession
        public String getSession(){ return m_strSession; }
        public String getContentType(){ return getProtocol().getContentType();}
    
        public SyncNotify getNotify(){ return m_oSyncNotify; }
        public NetRequest getNet() { return m_NetRequest;}
        NetRequest getNetClientID(){ return m_NetRequestClientID; }
        public ISyncProtocol getProtocol(){ return m_SyncProtocol; }
    
        public boolean isNoThreadedMode(){ return m_bNoThreaded; }
        void setNonThreadedMode(boolean b){m_bNoThreaded = b;}
        public static SourceOptions getSourceOptions(){ return m_oSourceOptions; }
    
        public SyncEngine(){
            m_oSyncNotify = new SyncNotify(this);
		    m_NetRequest = null;
		    m_NetRequestClientID = null;
    	    m_syncState = esNone;
    	
    	    initProtocol();
        }

        void initProtocol()
        {
            m_SyncProtocol = new SyncProtocol_3();
        }
    
        public int getSyncPageSize() { return m_nSyncPageSize; }
        void setSyncPageSize(int nPageSize){ m_nSyncPageSize = nPageSize; }
    
        public void setFactory(){ 
		    m_NetRequest = RhoClassFactory.createNetRequest();
		    m_NetRequestClientID = RhoClassFactory.createNetRequest();
		
		    m_oSyncNotify.setFactory();		
        }
    
        void prepareSync(int eState, SourceID oSrcID)
        {
            setState(eState);
            m_bIsSearch =  eState == esSearch;
            m_bStopByUser = false;
            m_nErrCode = RhoAppAdapter.ERR_NONE;
            m_strError = "";
            m_strServerError = "";
            m_bIsSchemaChanged = false;
        
            loadAllSources();

            m_strSession = loadSession();
            if ( isSessionExist()  )
            {
                m_clientID = loadClientID();
                if ( m_nErrCode == RhoAppAdapter.ERR_NONE )
                {
                    getNotify().cleanLastSyncObjectCount();
       	            doBulkSync();

                    return;
                }
            }else
                m_nErrCode = RhoAppAdapter.ERR_CLIENTISNOTLOGGEDIN;
        
            SyncSource src = null;
            if ( oSrcID != null )
        	    src = findSource(oSrcID);
        
    	    if ( src != null )
    	    {
                src.m_nErrCode = m_nErrCode;
                src.m_strError = m_strError;
                getNotify().fireSyncNotification(src, true, src.m_nErrCode, "");
            }else
            {
                getNotify().fireAllSyncNotifications(true, m_nErrCode, m_strError, "");
            }
        
            stopSync();
        }
    
        public void doSyncAllSources()
        {
	        try
	        {
	            prepareSync(esSyncAllSources, null);
	
	            if ( isContinueSync() )
	            {
			        PROF.CREATE_COUNTER("Net");	    
			        PROF.CREATE_COUNTER("Parse");
			        PROF.CREATE_COUNTER("DB");
			        PROF.CREATE_COUNTER("Data");
			        PROF.CREATE_COUNTER("Data1");
			        PROF.CREATE_COUNTER("Pull");
			        PROF.START("Sync");
	
	                syncAllSources();
	
			        PROF.DESTROY_COUNTER("Net");	    
			        PROF.DESTROY_COUNTER("Parse");
			        PROF.DESTROY_COUNTER("DB");
			        PROF.DESTROY_COUNTER("Data");
			        PROF.DESTROY_COUNTER("Data1");
			        PROF.DESTROY_COUNTER("Pull");
			        PROF.STOP("Sync");
	            }
	
	            getNotify().cleanCreateObjectErrors();
	        }catch(Exception exc)
	        {
	    	    LOG.ERROR("Sync failed.", exc);
	        }

            if ( getState() != esExit )
                setState(esNone);
        }

        public void doSearch(Vector<String> arSources, String strParams, String strAction, boolean bSearchSyncChanges, int nProgressStep)
        {
	        try
	        {
		        prepareSync(esSearch, null);
		        if ( !isContinueSync() )
		        {
		            if ( getState() != esExit )
		                setState(esNone);
		
		            return;
		        }
		
		        TimeInterval startTime = TimeInterval.getCurrentTime();
		
		        if ( bSearchSyncChanges )
		        {
		            for ( int i = 0; i < (int)arSources.size(); i++ )
		            {
		                SyncSource pSrc = findSourceByName((String)arSources.elementAt(i));
		                if ( pSrc != null )
		                    pSrc.syncClientChanges();
		            }
		        }
		
		        while( isContinueSync() )
		        {
		            int nSearchCount = 0;
		            String strUrl = getProtocol().getServerQueryUrl(strAction);
		            String strQuery = getProtocol().getServerQueryBody("", getClientID(), getSyncPageSize());
		
		            if ( strParams.length() > 0 )
		                strQuery += strParams;
		
		            String strTestResp = "";
		            for ( int i = 0; i < (int)arSources.size(); i++ )
		            {
		                SyncSource pSrc = findSourceByName((String)arSources.elementAt(i));
		                if ( pSrc != null )
		                {
		                    strQuery += "&sources[][name]=" + pSrc.getName();
		
		                    if ( !pSrc.isTokenFromDB() && pSrc.getToken() > 1 )
		                        strQuery += "&sources[][token]=" + pSrc.getToken();
		                
		                    strTestResp = getSourceOptions().getProperty(pSrc.getID(), "rho_server_response");
		                }
		            }
		
				    LOG.INFO("Call search on server. Url: " + (strUrl+strQuery) );
		            NetResponse resp = getNet().pullData(strUrl+strQuery, this);
		
		            if ( !resp.isOK() )
		            {
		                stopSync();
		                m_nErrCode = RhoAppAdapter.getErrorFromResponse(resp);
		                m_strError = resp.getCharData();
		                continue;
		            }
		
		            String szData = null;
		            if ( strTestResp != null && strTestResp.length() > 0 )
		        	    szData = strTestResp;
		            else
		        	    szData = resp.getCharData();		        
		
		            JSONArrayIterator oJsonArr = new JSONArrayIterator(szData);
		
		            for( ; !oJsonArr.isEnd() && isContinueSync(); oJsonArr.next() )
		            {
		                JSONArrayIterator oSrcArr = oJsonArr.getCurArrayIter();//new JSONArrayIterator(oJsonArr.getCurItem());
		                if (oSrcArr.isEnd())
		            	    break;
		            
		                int nVersion = 0;
		                if ( !oSrcArr.isEnd() && oSrcArr.getCurItem().hasName("version") )
		                {
		                    nVersion = oSrcArr.getCurItem().getInt("version");
		                    oSrcArr.next();
		                }
		
		                if ( nVersion != getProtocol().getVersion() )
		                {
		                    LOG.ERROR( "Sync server send search data with incompatible version. Client version: " + getProtocol().getVersion() +
		                        "; Server response version: " + nVersion );
		                    stopSync();
		                    m_nErrCode = RhoAppAdapter.ERR_SYNCVERSION;
		                    continue;
		                }
		
		                if ( !oSrcArr.isEnd() && oSrcArr.getCurItem().hasName("token"))
		                {
		                    oSrcArr.next();
		                }
		            
		                if ( !oSrcArr.getCurItem().hasName("source") )
		                {
		                    LOG.ERROR( "Sync server send search data without source name." );
		                    stopSync();
		                    m_nErrCode = RhoAppAdapter.ERR_UNEXPECTEDSERVERRESPONSE;
		                    m_strError = szData;
		                    continue;
		                }
		
		                String strSrcName = oSrcArr.getCurItem().getString("source");
		                SyncSource pSrc = findSourceByName(strSrcName);
		                if ( pSrc == null )
		                {
		                    LOG.ERROR("Sync server send search data for unknown source name:" + strSrcName);
		                    stopSync();
		                    m_nErrCode = RhoAppAdapter.ERR_UNEXPECTEDSERVERRESPONSE;
		                    m_strError = szData;
		                    continue;
		                }
		
		                oSrcArr.reset(0);
		                pSrc.setProgressStep(nProgressStep);
		                pSrc.processServerResponse_ver3(oSrcArr);
		
		                nSearchCount += pSrc.getCurPageCount();
		            
		                if ( pSrc.getServerError().length() > 0 )
		                {
		            	    if ( m_strServerError.length() > 0 )
		            		    m_strServerError +=  "&";
		            	
		            	    m_strServerError += pSrc.getServerError();
		            	    m_nErrCode = pSrc.getErrorCode();
		                }		            
		            }
		
		            if ( nSearchCount == 0 )
		            {
		        	    for ( int i = 0; i < (int)arSources.size(); i++ )
					    {
						    SyncSource pSrc = findSourceByName((String)arSources.elementAt(i));
						    if ( pSrc != null )
							    pSrc.processToken(0);
					    }		        	
		                break;
		            }
		        }  
		
		        getNotify().fireAllSyncNotifications(true, m_nErrCode, m_strError, m_strServerError);
		
		        //update db info
		        TimeInterval endTime = TimeInterval.getCurrentTime();
		        //unsigned long timeUpdated = CLocalTime().toULong();
		        for ( int i = 0; i < (int)arSources.size(); i++ )
		        {
		            SyncSource oSrc = findSourceByName((String)arSources.elementAt(i));
		            if ( oSrc == null )
		                continue;
		            oSrc.getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, "+
					     "last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?", 
					     endTime.toULong()/1000, oSrc.getInsertedCount(), oSrc.getDeletedCount(), 
					     endTime.minus(startTime).toULong(), oSrc.getGetAtLeastOnePage()?1:0, 
					     oSrc.getRefreshTime(),  oSrc.getID() );
		        }
		        //
		
		        getNotify().cleanCreateObjectErrors();
		        if ( getState() != esExit )
		            setState(esNone);

	        } catch(Exception exc) {
    		    LOG.ERROR("Search failed.", exc);

    		    getNotify().fireAllSyncNotifications(true, RhoAppAdapter.ERR_RUNTIME, "", "");
	        }
		    
        }

        public void doSyncSource(SourceID oSrcID)
        {
            SyncSource src = null;

	        try
	        {
	            prepareSync(esSyncSource, oSrcID);
	
	            if ( isContinueSync() )
	            {
	        	    src = findSource(oSrcID);
	                if ( src != null )
	                {
		                LOG.INFO("Started synchronization of the data source: " + src.getName() );
	
	                    src.sync();
	
				        getNotify().fireSyncNotification(src, true, src.m_nErrCode, src.m_nErrCode == RhoAppAdapter.ERR_NONE ? RhoAppAdapter.getMessageText("sync_completed") : "");
	                }else
	                {
    //                    LOG.ERROR( "Sync one source : Unknown Source " + oSrcID.toString() );
	            
		        	    src = new SyncSource(this, getUserDB());
			    	    //src.m_strError = "Unknown sync source.";
			    	    src.m_nErrCode = RhoAppAdapter.ERR_RUNTIME;
		        	
	    	    	    throw new Exception("Sync one source : Unknown Source " + oSrcID.toString() );
	                }
	            }
	
	        } catch(Exception exc) {
    		    LOG.ERROR("Sync source " + oSrcID.toString() + " failed.", exc);
	    	
	    	    if ( src != null && src.m_nErrCode == RhoAppAdapter.ERR_NONE )
	    		    src.m_nErrCode = RhoAppAdapter.ERR_RUNTIME;
	    	
	    	    getNotify().fireSyncNotification(src, true, src.m_nErrCode, "" ); 
	        }

            getNotify().cleanCreateObjectErrors();
            if ( getState() != esExit )
                setState(esNone);
        }

	    SyncSource findSource(SourceID oSrcID)
	    {
	        for( int i = 0; i < (int)m_sources.size(); i++ )
	        {
	            SyncSource src = (SyncSource)m_sources.elementAt(i);
	            if ( oSrcID.isEqual(src) )
	                return src;
	        }
	    
	        return null;
	    }
	
	    public SyncSource findSourceByName(String strSrcName)
	    {
		    return findSource(new SourceID(strSrcName));		
	    }
	
	    public void applyChangedValues(DBAdapter db)
	    {
	        IDBResult resSrc = db.executeSQL( "SELECT DISTINCT(source_id) FROM changed_values" );
	        for ( ; !resSrc.isEnd(); resSrc.next() )
	        {
	            int nSrcID = resSrc.getIntByIdx(0);
	            IDBResult res = db.executeSQL("SELECT source_id,sync_type,name, partition from sources WHERE source_id=?", nSrcID);
	            if ( res.isOneEnd() )
	                continue;

	            SyncSource src = new SyncSource( res.getIntByIdx(0), res.getStringByIdx(2), "none", db, this );

	            src.applyChangedValues();
	        }
	    }
	
	    void loadAllSources()
	    {
		    if (isNoThreadedMode())
	            RhoRuby.loadAllSyncSources();
	        else
	        {
	            getNet().pushData( getNet().resolveUrl("/system/loadallsyncsources"), "", null );
	        }
		
	        m_sources.removeAllElements();
	        Vector<String> arPartNames = DBAdapter.getDBAllPartitionNames();

	        for( int i = 0; i < (int)arPartNames.size(); i++ )
	        {
	            DBAdapter dbPart = DBAdapter.getDB((String)arPartNames.elementAt(i));	    
		        IDBResult res = dbPart.executeSQL("SELECT source_id,sync_type,name from sources ORDER BY sync_priority");
		        for ( ; !res.isEnd(); res.next() )
		        { 
		            String strShouldSync = res.getStringByIdx(1);
		            if ( strShouldSync.compareTo("none") == 0)
		                continue;
	
		            String strName = res.getStringByIdx(2);
		        
		            m_sources.addElement( new SyncSource( res.getIntByIdx(0), strName, strShouldSync, dbPart, this) );
		        }
	        }
	    
	        checkSourceAssociations();
	    }

	    static int findSrcIndex( Vector<SyncSource> sources, String strSrcName)
	    {
	        for ( int i = 0; i < (int)sources.size(); i++)
	        {
	            if (strSrcName.compareTo( ((SyncSource)sources.elementAt(i)).getName()) == 0 )
	                return i;
	        }

	        return -1;
	    }

	    void checkSourceAssociations()
	    {
	        Hashtable<String, int> hashPassed = new Hashtable<String, int>();
	    
	        for( int nCurSrc = m_sources.size()-1; nCurSrc > 0 ; )
	        {
	            SyncSource oCurSrc = (SyncSource)m_sources.elementAt(nCurSrc);
	            if ( oCurSrc.getAssociations().size() == 0 || hashPassed.containsKey(oCurSrc.getName()) )
	                nCurSrc--;
	            else
	            {
	                int nSrc = nCurSrc;
	                for( int i = 0; i < (int)oCurSrc.getAssociations().size(); i++ )
	                {
	                    SyncSource.CAssociation oAssoc = (SyncSource.CAssociation)oCurSrc.getAssociations().elementAt(i);
	                    int nAssocSrcIndex = findSrcIndex( m_sources, oAssoc.m_strSrcName);
	                    if ( nAssocSrcIndex >=0 && nAssocSrcIndex < nSrc )
	                    {
	                        m_sources.removeElementAt( nSrc );
	                        m_sources.insertElementAt( oCurSrc, nAssocSrcIndex );

	                        nSrc = nAssocSrcIndex;
	                    }
	                }
	            }

	            hashPassed.put(oCurSrc.getName(), 1 );
	        }
	    }

	    public String readClientID()
	    {
	        String clientID = "";
		
		    lock( m_mxLoadClientID )
		    {
	            IDBResult res = getUserDB().executeSQL("SELECT client_id,reset from client_info");
	            if ( !res.isOneEnd() )
	                clientID = res.getStringByIdx(0);
		    }
		
		    return clientID;
	    }
	
	    public String loadClientID()
	    {
	        String clientID = "";
		
		    lock( m_mxLoadClientID )
		    {
		        boolean bResetClient = false;
		        {
		            IDBResult res = getUserDB().executeSQL("SELECT client_id,reset from client_info");
		            if ( !res.isOneEnd() )
		            {
		                clientID = res.getStringByIdx(0);
		                bResetClient = res.getIntByIdx(1) > 0;
		            }
		        }
		    
		        if ( clientID.length() == 0 )
		        {
		            clientID = requestClientIDByNet();
		
	                IDBResult res = getUserDB().executeSQL("SELECT * FROM client_info");
	                if ( !res.isOneEnd() )
	            	    getUserDB().executeSQL("UPDATE client_info SET client_id=?", clientID);
	                else
	            	    getUserDB().executeSQL("INSERT INTO client_info (client_id) values (?)", clientID);
	            
		    	    if ( ClientRegister.getInstance() != null )
		    		    ClientRegister.getInstance().startUp();	    	
	            
		        }else if ( bResetClient )
		        {
		    	    if ( !resetClientIDByNet(clientID) )
		    		    stopSync();
		    	    else
		    		    getUserDB().executeSQL("UPDATE client_info SET reset=? where client_id=?", 0, clientID );	    	
		        }
		    }
		
		    return clientID;
	    }

	    void processServerSources(String strSources)
	    {
	        if ( strSources.length() > 0 )
	        {
	            if (isNoThreadedMode())
	                RhoRuby.loadServerSources(strSources);            
	            else
	            {
	        	    getNet().pushData( getNet().resolveUrl("/system/loadserversources"), strSources, null);
	            }
	        
	            loadAllSources();
	        
	            DBAdapter.initAttrManager();
	        }
	    }
	
	    boolean resetClientIDByNet(String strClientID)
	    {
            //String strBody = "";
            //TODO: send client register info in client reset 
    //        if ( ClientRegister.getInstance() != null )
    //            strBody += ClientRegister.getInstance().getRegisterBody();

	        NetResponse resp = getNetClientID().pullData(getProtocol().getClientResetUrl(strClientID), this);
    /*	    
	        processServerSources("{\"server_sources\":[{\"name\":\"Product\",\"partition\":\"application\",\"source_id\":\"2\",\"sync_priority\":\"0\","+
	    	        "\"schema_version\":\"7.0\",\"schema\":{"+
	    	        "\"columns\":[\'brand\',\'created_at\',\'name\',\'price\',\'quantity\',\'sku\',\'updated_at\']"+
	    	        "}}]}"); 
    */	    				
	    
	        if ( !resp.isOK() )
	        {
	    	    m_nErrCode = RhoAppAdapter.getErrorFromResponse(resp);
	    	    m_strError = resp.getCharData();
	        }
	        /*else
    	    {
    		    processServerSources(resp.getCharData());
    	    }*/
	    
	        return resp.isOK();
	    }

	    String requestClientIDByNet()
	    {
            //String strBody = "";
            //TODO: send client register info in client create 
    //        if ( ClientRegister.getInstance() != null )
    //            strBody += ClientRegister.getInstance().getRegisterBody();
	
	        NetResponse resp = getNetClientID().pullData(getProtocol().getClientCreateUrl(), this);
	        if ( resp.isOK() && resp.getCharData() != null )
	        {
	    	    String szData = resp.getCharData();
	    	
	            JSONEntry oJsonEntry = new JSONEntry(szData);
	
	            //if (oJsonEntry.hasName("sources") )
	            //    processServerSources(szData);
	        
	            JSONEntry oJsonObject = oJsonEntry.getEntry("client");
	            if ( !oJsonObject.isEmpty() )
	                return oJsonObject.getString("client_id");
	        }else
	        {
	    	    m_nErrCode = RhoAppAdapter.getErrorFromResponse(resp);
	    	    if ( m_nErrCode == RhoAppAdapter.ERR_NONE )
	    	    {
	    		    m_nErrCode = RhoAppAdapter.ERR_UNEXPECTEDSERVERRESPONSE;
	    		    m_strError = resp.getCharData();
	    	    }
	        }
	
	        return "";
	    }

	    void doBulkSync()
	    {
    //	    processServerSources("{\"partition\":\"" + "application" + "\"}");

	        if ( !RhoConf.getInstance().isExist("bulksync_state") )
	            return;
		
	        int nBulkSyncState = RhoConf.getInstance().getInt("bulksync_state");;
	        if ( nBulkSyncState >= 1 || !isContinueSync() )
	            return;

		    LOG.INFO("Bulk sync: start");
		    getNotify().fireBulkSyncNotification(false, "start", "", RhoAppAdapter.ERR_NONE);
		
		    Vector<String> arPartNames = DBAdapter.getDBAllPartitionNames();
	        for( int i = 0; i < (int)arPartNames.size()&& isContinueSync(); i++ )
	            loadBulkPartition( (String)arPartNames.elementAt(i));

	        if (isContinueSync())
	        {
	    	    RhoConf.getInstance().setInt("bulksync_state", 1, true);
	            getNotify().fireBulkSyncNotification(true, "", "", RhoAppAdapter.ERR_NONE);
	        }
	    }

	    void loadBulkPartition(String strPartition )
	    {
		    DBAdapter dbPartition = getDB(strPartition); 		
	        String serverUrl = RhoConf.getInstance().getPath("syncserver");
	        String strUrl = serverUrl + "bulk_data";
	        String strQuery = "?client_id=" + m_clientID + "&partition=" + strPartition;
	        String strDataUrl = "", strCmd = "", strCryptKey = "";

	        getNotify().fireBulkSyncNotification(false, "start", strPartition, RhoAppAdapter.ERR_NONE);
	    
	        while(strCmd.length() == 0&&isContinueSync())
	        {	    
	            NetResponse resp = getNet().pullData(strUrl+strQuery, this);
	            if ( !resp.isOK() || resp.getCharData() == null )
	            {
	    	        LOG.ERROR( "Bulk sync failed: server return an error." );
	    	        stopSync();
	    	        getNotify().fireBulkSyncNotification(true, "", strPartition,RhoAppAdapter.getErrorFromResponse(resp));
	    	        return;
	            }

		        LOG.INFO("Bulk sync: got response from server: " + resp.getCharData() );
	    	
	            String szData = resp.getCharData();
	            JSONEntry oJsonEntry = new JSONEntry(szData);
	            strCmd = oJsonEntry.getString("result");
	            if ( oJsonEntry.hasName("url") )
	   	            strDataUrl = oJsonEntry.getString("url");
	        
	            if ( strCmd.compareTo("wait") == 0)
	            {
	                int nTimeout = RhoConf.getInstance().getInt("bulksync_timeout_sec");
	                if ( nTimeout == 0 )
	                    nTimeout = 5;

	                SyncThread.getInstance().wait(nTimeout);
	                strCmd = "";
	            }
	        }

	        if ( strCmd.compareTo("nop") == 0)
	        {
		        LOG.INFO("Bulk sync return no data.");
		        getNotify().fireBulkSyncNotification(true, "", strPartition, RhoAppAdapter.ERR_NONE);		    
		        return;
	        }

            if ( !isContinueSync() )
                return;

	        getNotify().fireBulkSyncNotification(false, "download", strPartition, RhoAppAdapter.ERR_NONE);

            String fDataName = makeBulkDataFileName(strDataUrl, dbPartition.getDBPath(), "");
            String strZip = ".rzip";
            String strSqlDataUrl = CFilePath.join(getHostFromUrl(serverUrl), strDataUrl) +strZip;
            LOG.INFO("Bulk sync: download data from server: " + strSqlDataUrl);
            {
                NetResponse resp1 = getNet().pullFile(strSqlDataUrl, fDataName+strZip, this, null);
                if ( !resp1.isOK() )
                {
	                LOG.ERROR("Bulk sync failed: cannot download database file.");
	                stopSync();
	                getNotify().fireBulkSyncNotification(true, "", strPartition, RhoAppAdapter.getErrorFromResponse(resp1));
	                return;
                }
            }

            if ( !isContinueSync() )
                return;

            LOG.INFO("Bulk sync: unzip db");

            if ( !RHODESAPP().unzip_file(fDataName+strZip) )
            {
                CRhoFile.deleteFile(fDataName+strZip);
                LOG.ERROR("Bulk sync failed: cannot unzip database file.");
                stopSync();
                getNotify().fireBulkSyncNotification(true, "", strPartition, RhoAppAdapter.ERR_UNEXPECTEDSERVERRESPONSE);
                return;
            }
            CRhoFile.deleteFile(fDataName+strZip);

	        LOG.INFO("Bulk sync: start change db");
   	        getNotify().fireBulkSyncNotification(false, "change_db", strPartition, RhoAppAdapter.ERR_NONE);
    
            dbPartition.setBulkSyncDB(fDataName, strCryptKey);
            processServerSources("{\"partition\":\"" + strPartition + "\"}");

	        LOG.INFO("Bulk sync: end change db");
   	        getNotify().fireBulkSyncNotification(false, "", strPartition, RhoAppAdapter.ERR_NONE);
	    }
	
	    String makeBulkDataFileName(String strDataUrl, String strDbPath, String strExt)
	    {
	        String strNewName = CFilePath.getBaseName(strDataUrl);
	        String strOldName = RhoConf.getInstance().getString("bulksync_filename");
	        if ( strOldName.length() > 0 && strNewName.compareTo(strOldName) != 0 )
	        {
	            String strFToDelete = CFilePath.changeBaseName(strDbPath,strOldName+strExt);
	            LOG.INFO( "Bulk sync: remove old bulk file '" + strFToDelete + "'" );

	            //RhoFile.deleteFile( strFToDelete.c_str() );
	            CRhoFile.deleteFile(strFToDelete);	        
	        }

	        RhoConf.getInstance().setString("bulksync_filename", strNewName, true);

	        return CFilePath.changeBaseName(strDbPath, strNewName+strExt);
	    }
	
	    int getStartSource()
	    {
	        for( int i = 0; i < m_sources.size(); i++ )
	        {
	            SyncSource src = (SyncSource)m_sources.elementAt(i);
	            if ( !src.isEmptyToken() )
	                return i;
	        }
	
	        return -1;
	    }

	    boolean syncOneSource(int i)
	    {
    	    SyncSource src = null;
    	    boolean bError = false;
    	    try{
		        src = (SyncSource)m_sources.elementAt(i);
		        if ( src.getSyncType().compareTo("bulk_sync_only")==0 )
		            return true;
	
		        if ( isSessionExist() && getState() != esStop )
		            src.sync();
	
		        getNotify().onSyncSourceEnd(i, m_sources);
    	    }catch(Exception exc)
    	    {
	    	    if ( src.m_nErrCode == RhoAppAdapter.ERR_NONE )
	    		    src.m_nErrCode = RhoAppAdapter.ERR_RUNTIME;
	    	
	    	    setState(esStop);
    		    throw exc;
    	    }finally{
    		    getNotify().onSyncSourceEnd( i, m_sources );
    		    bError = src.m_nErrCode != RhoAppAdapter.ERR_NONE;
    	    }
    	
	        return !bError;
	    }
	
	    void syncAllSources()
	    {
	        boolean bError = false;

	        int nStartSrc = getStartSource();
	        if ( nStartSrc >= 0 )
	            bError = !syncOneSource(nStartSrc);

	        //TODO: do not stop on error source
	        for( int i = 0; i < (int)m_sources.size() && isContinueSync(); i++ )
	        {
	            bError = !syncOneSource(i);
	        }

	        if ( !bError && !isSchemaChanged() )
	    	    getNotify().fireSyncNotification(null, true, RhoAppAdapter.ERR_NONE, RhoAppAdapter.getMessageText("sync_completed"));
	    }

        public void login(String name, String password, SyncNotify.SyncNotification oNotify)
	    {
		    try {
    /*			
			    processServerSources("{\"sources\":{ \"ProductEx\":{ "+
	            "\"sync_type\":\"incremental\", \"partition\":\"application\", \"source_id\":\"7\","+
	            " \"sync_priority\":\"0\", \"model_type\":\"fixed_schema\", "+
	            " \"schema\":{\"version\":\"1.1\", \"property\":{\"brand\":\"string\", \"price\":\"string\", \"quantity\":\"string\", \"name\":\"string\", "+
	            " \"image_url\":\"blob\", \"image_url_ex\":\"blob,overwrite\"}, "+
	            " \"index\":[{\"by_brand_price1\":\"brand,price\"}, {\"by_quantity1\":\"quantity\"}], \"unique_index\":[{\"by_name1\":\"name\"}]}, "+
	            " \"belongs_to\":{\"brand\":\"Customer\"}}}}");//, \"schema_version\":\"1.0\"
	            */
		        NetResponse resp = null;
		        m_bStopByUser = false;
		    
		        try{
				
			        resp = getNet().pullCookies( getProtocol().getLoginUrl(), getProtocol().getLoginBody(name, password), this );
			        int nErrCode = RhoAppAdapter.getErrorFromResponse(resp);
			        if ( nErrCode != RhoAppAdapter.ERR_NONE )
			        {
			            getNotify().callLoginCallback(oNotify, nErrCode, resp.getCharData());
			            return;
			        }
		        }catch(Exception exc)
		        {
				    LOG.ERROR("Login failed.", exc);
		    	    getNotify().callLoginCallback(oNotify, RhoAppAdapter.getNetErrorCode(exc), "" );
		    	    return;
		        }
		    
		        String strSession = resp.getCharData();
		        if ( strSession == null || strSession.length() == 0 )
		        {
		    	    LOG.ERROR("Return empty session.");
		    	    getNotify().callLoginCallback(oNotify, RhoAppAdapter.ERR_UNEXPECTEDSERVERRESPONSE, "" );
		            return;
		        }
		    
		        if ( isStoppedByUser() )
		    	    return;
		    
		        IDBResult res = getUserDB().executeSQL("SELECT * FROM client_info");
		        if ( !res.isOneEnd() )
		    	    getUserDB().executeSQL( "UPDATE client_info SET session=?", strSession );
		        else
		    	    getUserDB().executeSQL("INSERT INTO client_info (session) values (?)", strSession);
		
		        if ( RHOCONF().isExist("rho_sync_user") )
		        {
		            String strOldUser = RHOCONF().getString("rho_sync_user");
		            if ( name.compareTo(strOldUser) != 0 )
		            {
		                if (isNoThreadedMode())
                            RhoRuby.resetDBOnSyncUserChanged();
		                else
		                {
		                    NetResponse resp1 = getNet().pushData( getNet().resolveUrl("/system/resetDBOnSyncUserChanged"), "", null );
		                }
		            }
		        }
		        RHOCONF().setString("rho_sync_user", name, true);
		    
	    	    getNotify().callLoginCallback(oNotify, RhoAppAdapter.ERR_NONE, "" );
		    
	    	    if ( ClientRegister.getInstance() != null )
	    		    ClientRegister.getInstance().startUp();	    	
	    	
		    }catch(Exception exc)
		    {
			    LOG.ERROR("Login failed.", exc);
	    	    getNotify().callLoginCallback(oNotify, RhoAppAdapter.ERR_RUNTIME, "" );
		    }
	    }

	    public boolean isLoggedIn()
	    {
	        String strRes = "";
	        IDBResult res = getUserDB().executeSQL("SELECT session FROM client_info");
	    
	        if ( !res.isOneEnd() )
	    	    strRes = res.getStringByIdx(0);
	    
	        return strRes.length() > 0;
	    }

	    public String loadSession()
	    {
		    m_strSession = "";
	        IDBResult res = getUserDB().executeSQL("SELECT session FROM client_info");
	    
	        if ( !res.isOneEnd() )
	    	    m_strSession = res.getStringByIdx(0);
	    
	        return m_strSession;
	    }
	
	    public void logout()
	    {
	        if(m_NetRequest!=null) 
	            m_NetRequest.cancel();
		
		    getUserDB().executeSQL( "UPDATE client_info SET session = NULL");
	        m_strSession = "";
	
	        //loadAllSources();
	    }
	
	    public void setSyncServer(String syncserver)
	    {
		    String strOldSrv = RhoConf.getInstance().getString("syncserver");
		    String strNewSrv = syncserver != null ? syncserver : "";
		
		    if ( strOldSrv.compareTo(strNewSrv) != 0)
		    {
			    RhoConf.getInstance().setString("syncserver", syncserver, true);
			
			    getUserDB().executeSQL("DELETE FROM client_info");

			    logout();
		    }
	    }
	
        static String getHostFromUrl( String strUrl )
        {
            int nHttp = strUrl.indexOf("://");
            if ( nHttp < 0 )
                nHttp = strUrl.indexOf(":\\\\");

            int nStartSrv = 0;
            if ( nHttp >= 0 )
                nStartSrv = nHttp + 3;

            int nEndSrv = strUrl.indexOf('/', nStartSrv);
            if ( nEndSrv < 0 )
                nEndSrv = strUrl.indexOf('\\', nStartSrv);

            int nSrvLen = nEndSrv >= 0 ? nEndSrv + 1 : strUrl.length();
            return strUrl.substring(nSrvLen);
        }    
    }
}
