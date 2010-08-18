/*
 *  rhodes
 *
 *  Copyright (C) 2008 Rhomobile, Inc. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.rho.sync;

import j2me.util.LinkedList;

import com.rho.Mutex;
import com.rho.RhoClassFactory;
import com.rho.RhoConf;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhoRuby;
import com.rho.RhoThread;
import com.rho.TimeInterval;
import com.rho.db.DBAdapter;
import com.rho.db.IDBResult;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;
import java.util.Vector;

public class SyncThread extends RhoThread
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Sync");
	private static final int SYNC_POLL_INTERVAL_SECONDS = 300;
	private static final int SYNC_POLL_INTERVAL_INFINITE = Integer.MAX_VALUE/1000;
	private static final int SYNC_WAIT_BEFOREKILL_SECONDS  = 3;
	
	static SyncThread m_pInstance;

   	public final static int scNone = 0, scSyncAll = 2, scSyncOne = 3, scSyncOneByUrl = 4, scChangePollInterval=5, scExit=6, scLogin = 7, scSearchOne=8; 
    
   	static private class SyncCommand
   	{
   		int m_nCmdCode;
   		int m_nCmdParam;
   		String m_strCmdParam;
   		boolean m_bShowStatus;
   		
   		SyncCommand(int nCode, int nParam, boolean bShowStatus)
   		{
   			m_nCmdCode = nCode;
   			m_nCmdParam = nParam;
   			m_bShowStatus = bShowStatus;
   		}
   		SyncCommand(int nCode, String strParam, boolean bShowStatus)
   		{
   			m_nCmdCode = nCode;
   			m_strCmdParam = strParam;
   			m_bShowStatus = bShowStatus;
   		}
	    SyncCommand(int nCode, String strParam, int nCmdParam, boolean bShowStatus)
	    {
		    m_nCmdCode = nCode;
		    m_strCmdParam = strParam;
            m_nCmdParam = nCmdParam;
            m_bShowStatus = bShowStatus;
	    }
   		
   		SyncCommand(int nCode, boolean bShowStatus)
   		{
   			m_nCmdCode = nCode;
   			m_nCmdParam = 0;
   			m_bShowStatus = bShowStatus;
   		}
   		
   		public boolean equals(Object obj)
   		{
   			SyncCommand oSyncCmd = (SyncCommand)obj;
   			return m_nCmdCode == oSyncCmd.m_nCmdCode && m_nCmdParam == oSyncCmd.m_nCmdParam &&
   				(m_strCmdParam == oSyncCmd.m_strCmdParam ||
   				(m_strCmdParam != null && oSyncCmd.m_strCmdParam != null && m_strCmdParam.equals(oSyncCmd.m_strCmdParam)));  		
   		}
   	};
   	static private class SyncLoginCommand extends SyncCommand
   	{
   		String m_strName, m_strPassword;
   		public SyncLoginCommand(String name, String password, String callback)
   		{
   			super(scLogin,callback,false);
   			
   			m_strName = name;
   			m_strPassword = password;
   		}
   	};
    static class SyncSearchCommand extends SyncCommand
    {
	    String m_strFrom;
	    boolean   m_bSyncChanges;
	    Vector/*<rho::String>*/ m_arSources;
	    
        public SyncSearchCommand(String from, String params, Vector arSources, boolean sync_changes, int nProgressStep)
	    {
        	super(scSearchOne,params,nProgressStep, false);
		    m_strFrom = from;
		    m_bSyncChanges = sync_changes;
		    m_arSources = arSources;
	    }
    };
   	
    SyncEngine  m_oSyncEngine;
    RhoClassFactory m_ptrFactory;
	int           m_nPollInterval;
	Object        m_mxStackCommands;// = new Mutex();
	LinkedList	  m_stackCommands = new LinkedList();
	boolean m_bNoThreaded = false;
	
    boolean isNoThreadedMode(){ return m_bNoThreaded; }
    void setNonThreadedMode(boolean b){m_bNoThreaded = b;}
	
	public static SyncThread Create(RhoClassFactory factory)throws Exception
	{
	    if ( m_pInstance != null) 
	        return m_pInstance;
	
	    m_pInstance = new SyncThread(factory);
	    return m_pInstance;
	}

	public void Destroy()
	{
	    m_oSyncEngine.exitSync();
	    stop(SYNC_WAIT_BEFOREKILL_SECONDS);
		
	    if ( ClientRegister.getInstance() != null )
	    	ClientRegister.getInstance().Destroy();
	    
	    DBAdapter.closeAll();
	    
	    m_pInstance = null;
	}

	SyncThread(RhoClassFactory factory)throws Exception
	{
		super(factory);
		
		m_oSyncEngine = new SyncEngine();
		m_nPollInterval = SYNC_POLL_INTERVAL_SECONDS;
		if( RhoConf.getInstance().isExist("sync_poll_interval") )
			m_nPollInterval = RhoConf.getInstance().getInt("sync_poll_interval");
		
		m_ptrFactory = factory;
	
	    m_oSyncEngine.setFactory(factory);
	    m_mxStackCommands = getSyncObject();
	    	
	    ClientRegister.Create(factory);
	    
		if ( RhoConf.getInstance().getString("syncserver").length() > 0 )
			start(epLow);
	}

    public static SyncThread getInstance(){ return m_pInstance; }
    public static SyncEngine getSyncEngine(){ return m_pInstance!= null ? m_pInstance.m_oSyncEngine : null; }

    void addSyncCommand(SyncCommand oSyncCmd)
    { 
		if ( RhoConf.getInstance().getString("syncserver").length() == 0 )
			return;
		
    	LOG.INFO( "addSyncCommand: " + oSyncCmd.m_nCmdCode );
    	synchronized(m_mxStackCommands)
    	{
    		boolean bExist = false;
    		for ( int i = 0; i < m_stackCommands.size(); i++ )
    		{
    			if ( m_stackCommands.get(i).equals(oSyncCmd) )
    			{
    				bExist = true;
    				break;
    			}
    		}
    		
    		if ( !bExist )
    			m_stackCommands.add(oSyncCmd);
    	}
        if ( isNoThreadedMode()  )
        {
        	try{
        		processCommands();
        	}catch(Exception e)
        	{
        		LOG.ERROR("processCommand failed", e);
        	}
        }
        else
        	stopWait(); 
    }
    
    RubyValue getRetValue()
    {
    	RubyValue ret = RubyConstant.QNIL;
        if ( isNoThreadedMode()  )
        {
            ret = ObjectFactory.createString( getSyncEngine().getNotify().getNotifyBody() );
            getSyncEngine().getNotify().cleanNotifyBody();
        }

        return ret;
    }
	
    int getLastSyncInterval()
    {
    	try{
	    	long nowTime = (TimeInterval.getCurrentTime().toULong())/1000;
	    	
		    IDBResult res = DBAdapter.getUserDB().executeSQL("SELECT last_updated from sources");
		    long latestTimeUpdated = 0;
		    for ( ; !res.isEnd(); res.next() )
		    { 
		        long timeUpdated = res.getLongByIdx(0);
		        if ( latestTimeUpdated < timeUpdated )
		        	latestTimeUpdated = timeUpdated;
		    }
	    	
	    	return latestTimeUpdated > 0 ? (int)(nowTime-latestTimeUpdated) : 0;
    	}catch(Exception exc)
    	{
    		LOG.ERROR("isStartSyncNow failed.", exc);
    	}
    	return 0;
    }
    
	public void run()
	{
		LOG.INFO( "Starting sync engine main routine..." );
	
		int nLastSyncInterval = getLastSyncInterval();
		while( m_oSyncEngine.getState() != SyncEngine.esExit )
		{
	        int nWait = m_nPollInterval > 0 ? m_nPollInterval : SYNC_POLL_INTERVAL_INFINITE;

	        if ( m_nPollInterval > 0 && nLastSyncInterval > 0 )
	            nWait = m_nPollInterval - nLastSyncInterval;

	        synchronized(m_mxStackCommands)
	        {
				if ( nWait >= 0 && m_oSyncEngine.getState() != SyncEngine.esExit && 
					 isNoCommands() )
				{
					LOG.INFO( "Sync engine blocked for " + nWait + " seconds..." );
			        wait(nWait);
				}
	        }
	        nLastSyncInterval = 0;
			
	        if ( m_oSyncEngine.getState() != SyncEngine.esExit )
	        {
	        	try{
	        		processCommands();
	        	}catch(Exception e)
	        	{
	        		LOG.ERROR("processCommand failed", e);
	        	}
	        }
		}
		
		LOG.INFO("Sync engine thread shutdown");		
	}
	
	boolean isNoCommands()
	{
		boolean bEmpty = false;
    	synchronized(m_mxStackCommands)
    	{		
    		bEmpty = m_stackCommands.isEmpty();
    	}

    	return bEmpty;
	}
	
	void processCommands()throws Exception
	{
		if ( isNoCommands() )
			addSyncCommand(new SyncCommand(scNone, false));
    	
		while(!isNoCommands())
		{
			SyncCommand oSyncCmd = null;
	    	synchronized(m_mxStackCommands)
	    	{
	    		oSyncCmd = (SyncCommand)m_stackCommands.removeFirst();
	    	}
			
			processCommand(oSyncCmd);
		}
	}

	void checkShowStatus(SyncCommand oSyncCmd)
	{
		boolean bShowStatus = oSyncCmd.m_bShowStatus && !this.isNoThreadedMode();
		m_oSyncEngine.getNotify().enableReporting(bShowStatus);
		if (bShowStatus)
			m_statusListener.createStatusPopup(RhoRuby.getMessageText("syncronizing_data"));
	}	
	
	void processCommand(SyncCommand oSyncCmd)throws Exception
	{
	    switch(oSyncCmd.m_nCmdCode)
	    {
	    case scNone:
	        if ( m_nPollInterval > 0 )
	        {
	        	checkShowStatus(oSyncCmd);
	            m_oSyncEngine.doSyncAllSources();
	        }
	        break;
	    case scSyncAll:
	    	checkShowStatus(oSyncCmd);
	        m_oSyncEngine.doSyncAllSources();
	        break;
	    case scChangePollInterval:
	        break;
        case scSyncOne:
	        {
				checkShowStatus(oSyncCmd);
	            m_oSyncEngine.doSyncSource(new SyncEngine.SourceID(oSyncCmd.m_nCmdParam,oSyncCmd.m_strCmdParam));
	        }
	        break;
	        
	    case scSearchOne:
		    {
				checkShowStatus(oSyncCmd);
	            m_oSyncEngine.doSearch( ((SyncSearchCommand)oSyncCmd).m_arSources, oSyncCmd.m_strCmdParam, 
	                    ((SyncSearchCommand)oSyncCmd).m_strFrom, ((SyncSearchCommand)oSyncCmd).m_bSyncChanges,
	                    oSyncCmd.m_nCmdParam);
		    }
	        break;
	        
	    case scLogin:
	    	{
	    		SyncLoginCommand oLoginCmd = (SyncLoginCommand)oSyncCmd;
	    		checkShowStatus(oSyncCmd);
	    		m_oSyncEngine.login(oLoginCmd.m_strName, oLoginCmd.m_strPassword, oLoginCmd.m_strCmdParam );
	    	}
	        break;
	        
	    }
	}

	static ISyncStatusListener m_statusListener = null;
	public boolean setStatusListener(ISyncStatusListener listener) {
		m_statusListener = listener;
		if (m_oSyncEngine != null) {
			m_oSyncEngine.getNotify().setSyncStatusListener(listener);
			return true;
		}
		return false;
	}
	
	public void setPollInterval(int nInterval)
	{ 
	    m_nPollInterval = nInterval; 
	    if ( m_nPollInterval == 0 )
	        m_oSyncEngine.stopSync();
	
	    addSyncCommand(new SyncCommand(scChangePollInterval, false)); 
	}
	
	public static void doSyncAllSources(boolean bShowStatus)
	{
		getInstance().addSyncCommand(new SyncCommand(SyncThread.scSyncAll,bShowStatus));
	}

	public static void doSyncSource(int nSrcID, String strName, boolean bShowStatus)
	{
		getInstance().addSyncCommand(new SyncCommand(SyncThread.scSyncOne, strName, nSrcID, bShowStatus) );
	}
	
	public static void doSyncSourceByUrl(String strSrcUrl, boolean bShowStatus)
	{
		if (bShowStatus&&(m_statusListener != null)) {
			m_statusListener.createStatusPopup(RhoRuby.getMessageText("syncronizing_data"));
		}

			
	    int nLastSlash = strSrcUrl.lastIndexOf('\\');
	    if ( nLastSlash < 0 )
	    	nLastSlash = strSrcUrl.lastIndexOf('/');

	    String strName = nLastSlash >=0 ? strSrcUrl.substring(nLastSlash + 1) : strSrcUrl;
		
	    getInstance().addSyncCommand(new SyncCommand(SyncThread.scSyncOne, strName, (int)0, false ) );		
	}
	
	public static void stopSync()throws Exception
	{
		if ( getSyncEngine().isSyncing() )
		{
			getSyncEngine().stopSyncByUser();
			getInstance().stopWait();
			
			int nWait = 0;
			//while( nWait < 30000 && getSyncEngine().getState() != SyncEngine.esNone )
			while( nWait < 30000 && DBAdapter.isAnyInsideTransaction() )
				try{ Thread.sleep(100); nWait += 100; }catch(Exception e){}
				
			//if (getSyncEngine().getState() != SyncEngine.esNone)
			if ( DBAdapter.isAnyInsideTransaction() )
			{
				getSyncEngine().exitSync();
				getInstance().stop(0);
				RhoClassFactory ptrFactory = getInstance().m_ptrFactory;
				m_pInstance = null;
				
				Create(ptrFactory);
			}
		}
	}
	
	public void addobjectnotify_bysrcname(String strSrcName, String strObject)
	{
		getSyncEngine().getNotify().addObjectNotify(strSrcName, strObject);
	}
	
	public static void initMethods(RubyModule klass) {
		klass.getSingletonClass().defineMethod("dosync", new RubyNoOrOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
				try {
					doSyncAllSources(true);
				} catch(Exception e) {
					LOG.ERROR("dosync failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return getInstance().getRetValue();
			}
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
				try {
					String str = arg.asString();
					boolean show = arg.equals(RubyConstant.QTRUE)||"true".equalsIgnoreCase(str);
					doSyncAllSources(show);
				} catch(Exception e) {
					LOG.ERROR("dosync failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return getInstance().getRetValue();
			}
		});		
		klass.getSingletonClass().defineMethod("dosync_source", new RubyOneOrTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
				try {
					int nSrcID = 0;
					String strName = "";
					if ( arg instanceof RubyFixnum )
						nSrcID = arg.toInt();
					else
						strName = arg.toStr();
					
					doSyncSource( nSrcID, strName, true);
				} catch(Exception e) {
					LOG.ERROR("dosync_source failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return getInstance().getRetValue();
			}
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block )
			{
				try {
					String str = arg1.asString();
					boolean show = arg1.equals(RubyConstant.QTRUE)||"true".equalsIgnoreCase(str);
					
					int nSrcID = 0;
					String strName = "";
					if ( arg0 instanceof RubyFixnum )
						nSrcID = arg0.toInt();
					else
						strName = arg0.toStr();
					
					doSyncSource(nSrcID, strName, show);
				} catch(Exception e) {
					LOG.ERROR("dosync_source failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return getInstance().getRetValue();
			}
		});
		
		klass.getSingletonClass().defineMethod("dosearch",
			new RubyVarArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
					if ( args.size() != 7 )
						throw new RubyException(RubyRuntime.ArgumentErrorClass, 
								"in SyncEngine.dosearch_source: wrong number of arguments ( " + args.size() + " for " + 7 + " )");			
					
					try{
						Vector arSources = RhoRuby.makeVectorStringFromArray(args.get(0));
						
						String from = args.get(1).toStr();
						String params = args.get(2).toStr();
						
						String str = args.get(3).asString();
						int nProgressStep = args.get(4).toInt();
						String callback = args.get(5) != RubyConstant.QNIL ? args.get(5).toStr() : "";
						String callback_params = args.get(6) != RubyConstant.QNIL ? args.get(6).toStr() : "";
						
						boolean bSearchSyncChanges = args.get(3).equals(RubyConstant.QTRUE)||"true".equalsIgnoreCase(str);
						stopSync();

						if ( callback != null && callback.length() > 0 )
							getSyncEngine().getNotify().setSearchNotification(callback, callback_params);
						
						getInstance().addSyncCommand(new SyncSearchCommand(from,params,arSources,bSearchSyncChanges, nProgressStep) );
					}catch(Exception e)
					{
						LOG.ERROR("SyncEngine.login", e);
						RhoRuby.raise_RhoError(RhoRuby.ERR_RUNTIME);
					}
					
					return getInstance().getRetValue();
				    
				}
			});
		
		klass.getSingletonClass().defineMethod("stop_sync", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					stopSync();
				}catch(Exception e)
				{
					LOG.ERROR("stop_sync failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				
				return RubyConstant.QNIL;
			}
		});
		
		klass.getSingletonClass().defineMethod("login",
				new RubyVarArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
						if ( args.size() != 3 )
							throw new RubyException(RubyRuntime.ArgumentErrorClass, 
									"in SyncEngine.login: wrong number of arguments ( " + args.size() + " for " + 3 + " )");			
						
						try{
							String name = args.get(0).toStr();
							String password = args.get(1).toStr();
							String callback = args.get(2).toStr();
							
							stopSync();
							
							getInstance().addSyncCommand(new SyncLoginCommand(name, password, callback) );
						}catch(Exception e)
						{
							LOG.ERROR("SyncEngine.login", e);
							RhoRuby.raise_RhoError(RhoRuby.ERR_RUNTIME);
						}
						
						return getInstance().getRetValue();
					    
					}
				});
		
		klass.getSingletonClass().defineMethod("logged_in",
			new RubyNoArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyBlock block) {
					DBAdapter db = DBAdapter.getUserDB();

					try{
					    return getSyncEngine().isLoggedIn() ? 
					    		ObjectFactory.createInteger(1) : ObjectFactory.createInteger(0);
					}catch(Exception e)
					{
						LOG.ERROR("logged_in failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}
				    
				}
			});
		
		klass.getSingletonClass().defineMethod("logout",
			new RubyNoArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyBlock block) {
					DBAdapter db = DBAdapter.getUserDB();

					try{
						stopSync();
					    getSyncEngine().logout();
					}catch(Exception e)
					{
						LOG.ERROR("logout failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}
					
				    return RubyConstant.QNIL;
				}
			});
		
		klass.getSingletonClass().defineMethod("set_notification",
			new RubyVarArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
					
					try{
						int source_id = args.get(0).toInt();
						String url = args.get(1).toStr();
						String params = args.get(2).toStr();
						getSyncEngine().getNotify().setSyncNotification(source_id, url, params);
					}catch(Exception e)
					{
						LOG.ERROR("set_notification failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}
					return RubyConstant.QNIL;
				}
			});
		klass.getSingletonClass().defineMethod("clear_notification",
			new RubyOneArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyBlock block) {
					try{
						int source_id = arg1.toInt();
						getSyncEngine().getNotify().clearSyncNotification(source_id);
					}catch(Exception e)
					{
						LOG.ERROR("clear_notification failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}
					
					
					return RubyConstant.QNIL;
				}
			});
		
		klass.getSingletonClass().defineMethod("set_pollinterval",
			new RubyOneArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyBlock block) {
					try{
						int nInterval = arg1.toInt();
						getInstance().setPollInterval(nInterval);
					}catch(Exception e)
					{
						LOG.ERROR("set_pollinterval failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}
					
					return RubyConstant.QNIL;
				}
			});
		klass.getSingletonClass().defineMethod("set_syncserver",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyBlock block) {
						try{
							String syncserver = arg1.toStr();
							
							stopSync();							
							getSyncEngine().setSyncServer(syncserver);
							
						    if ( syncserver != null && syncserver.length() > 0 )
						    {
						        SyncThread.getInstance().start(SyncThread.epLow);
						    	if ( ClientRegister.getInstance() != null )
						    		ClientRegister.getInstance().startUp();	    	
						    }
						    else
						    {
						    	//DO NOT STOP thread. because they cannot be restarted
						        //SyncThread.getInstance().stop(SYNC_WAIT_BEFOREKILL_SECONDS);
						    	//if ( ClientRegister.getInstance() != null )
						    	//	ClientRegister.getInstance().stop(SYNC_WAIT_BEFOREKILL_SECONDS);
						    }
							
						}catch(Exception e)
						{
							LOG.ERROR("set_syncserver failed", e);
							throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
						}
						
						return RubyConstant.QNIL;
					}
			});
		
		klass.getSingletonClass().defineMethod("get_src_attrs",
				new RubyTwoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
						try{
							String strPartition = arg0.toStr(); 
							int nSrcID = arg1.toInt();
							return DBAdapter.getDB(strPartition).getAttrMgr().getAttrsBySrc(nSrcID);
						}catch(Exception e)
						{
							LOG.ERROR("get_src_attrs failed", e);
							throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
						}
					}
			});

		klass.getSingletonClass().defineMethod("is_blob_attr",
				new RubyVarArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
						try{
							String strPartition = args.get(0).toStr(); 
							Integer nSrcID = new Integer(args.get(1).toInt());
							String strAttrName = args.get(2).toStr();
							boolean bExists = DBAdapter.getDB(strPartition).getAttrMgr().isBlobAttr(nSrcID, strAttrName);
							return ObjectFactory.createBoolean(bExists);
						}catch(Exception e)
						{
							LOG.ERROR("get_src_attrs failed", e);
							throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
						}
					}
			});
		
		klass.getSingletonClass().defineMethod("set_objectnotify_url",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyBlock block) {
						try{
							String url = arg1.toStr();
							SyncNotify.setObjectNotifyUrl(url);
						}catch(Exception e)
						{
							LOG.ERROR("set_objectnotify_url failed", e);
							throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
						}
						
						return RubyConstant.QNIL;
					}
			});

		klass.getSingletonClass().defineMethod("add_objectnotify",
				new RubyTwoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
						try{
							Integer nSrcID = new Integer(arg1.toInt());
							String strObject = arg2.toStr();
							
							getSyncEngine().getNotify().addObjectNotify(nSrcID, strObject);
						}catch(Exception e)
						{
							LOG.ERROR("add_objectnotify failed", e);
							throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
						}
						
						return RubyConstant.QNIL;
					}
			});
		klass.getSingletonClass().defineMethod("clean_objectnotify",
				new RubyNoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyBlock block) {
						try{
							getSyncEngine().getNotify().cleanObjectNotifications();
						}catch(Exception e)
						{
							LOG.ERROR("clean_objectnotify failed", e);
							throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
						}
						
						return RubyConstant.QNIL;
					}
			});
		
		klass.getSingletonClass().defineMethod("get_lastsync_objectcount",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyBlock block) {
						try{
							Integer nSrcID = new Integer(arg1.toInt());
							int nCount = getSyncEngine().getNotify().getLastSyncObjectCount(nSrcID);
							
							return ObjectFactory.createInteger(nCount);
						}catch(Exception e)
						{
							LOG.ERROR("get_lastsync_objectcount failed", e);
							throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
						}
					}
			});
		klass.getSingletonClass().defineMethod("get_pagesize",
				new RubyNoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyBlock block) {
						try{
							return ObjectFactory.createInteger(getSyncEngine().getSyncPageSize());
						}catch(Exception e)
						{
							LOG.ERROR("get_pagesize failed", e);
							throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
						}
					}
			});
		
		klass.getSingletonClass().defineMethod("set_pagesize",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyBlock block) {
						try{
							getSyncEngine().setSyncPageSize(arg1.toInt());
						}catch(Exception e)
						{
							LOG.ERROR("set_pagesize failed", e);
							throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
						}
						
						return RubyConstant.QNIL;
					}
			});

		klass.getSingletonClass().defineMethod("set_threaded_mode",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyBlock block) {
						try{
							boolean bThreadMode = arg1 == RubyConstant.QTRUE;
							getInstance().setNonThreadedMode(!bThreadMode);
							getSyncEngine().setNonThreadedMode(!bThreadMode);
						}catch(Exception e)
						{
							LOG.ERROR("set_pagesize failed", e);
							throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
						}
						
						return RubyConstant.QNIL;
					}
			});
		
	}

}
