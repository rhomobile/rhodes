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

import com.rho.*;
import com.rho.db.*;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.*;
import java.io.IOException;
import j2me.util.LinkedList;

public class SyncThread extends RhoThread
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Sync");
	private static final int SYNC_POLL_INTERVAL_SECONDS = 300;
	private static final int SYNC_POLL_INTERVAL_INFINITE = Integer.MAX_VALUE/1000;
	private static final int SYNC_WAIT_BEFOREKILL_SECONDS  = 3;
	
	static SyncThread m_pInstance;

   	public final static int scNone = 0, scSyncAll = 2, scSyncOne = 3, scChangePollInterval=4, scExit=5; 
    
   	static private class SyncCommand
   	{
   		int m_nCmdCode;
   		int m_nCmdParam;
   		String m_strCmdParam;
   		
   		SyncCommand(int nCode, int nParam)
   		{
   			m_nCmdCode = nCode;
   			m_nCmdParam = nParam;
   		}
   		SyncCommand(int nCode, String strParam)
   		{
   			m_nCmdCode = nCode;
   			m_strCmdParam = strParam;
   		}
   		
   		SyncCommand(int nCode)
   		{
   			m_nCmdCode = nCode;
   			m_nCmdParam = 0;
   		}
   		
   	};
   	
    SyncEngine  m_oSyncEngine;
    RhoClassFactory m_ptrFactory;
	int           m_nPollInterval;
	Mutex         m_mxStackCommands = new Mutex();
	LinkedList	  m_stackCommands = new LinkedList();	         
	
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
	    LOG.INFO( "Sync engine thread shutdown" );
		
	    m_pInstance = null;
	}

	SyncThread(RhoClassFactory factory)throws Exception
	{
		super(factory);
		
		m_oSyncEngine = new SyncEngine(DBAdapter.getInstance());
		m_nPollInterval = SYNC_POLL_INTERVAL_SECONDS;
		m_ptrFactory = factory;
	
	    m_oSyncEngine.setFactory(factory);
	
	    ClientRegister.Create(factory);
	    start(epLow);
	}

    public static SyncThread getInstance(){ return m_pInstance; }
    static SyncEngine getSyncEngine(){ return m_pInstance.m_oSyncEngine; }
    static DBAdapter getDBAdapter(){ return DBAdapter.getInstance(); }

    void addSyncCommand(SyncCommand oSyncCmd)
    { 
    	//TODO: check for duplicates ???
    	synchronized(m_mxStackCommands)
    	{
    		m_stackCommands.add(oSyncCmd);
    	}
    	stopWait(); 
    }
    
	
    int getLastSyncInterval()
    {
    	try{
	    	TimeInterval nowTime = TimeInterval.getCurrentTime();
	    	
		    IDBResult res = m_oSyncEngine.getDB().executeSQL("SELECT last_updated from sources");
		    long latestTimeUpdated = 0;
		    for ( ; !res.isEnd(); res.next() )
		    { 
		        long timeUpdated = res.getLongByIdx(0);
		        if ( latestTimeUpdated < timeUpdated )
		        	latestTimeUpdated = timeUpdated;
		    }
	    	
	    	return latestTimeUpdated > 0 ? (int)(nowTime.toULong()-latestTimeUpdated) : 0;
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
	            nWait = (m_nPollInterval*1000 - nLastSyncInterval)/1000;

			if ( nWait >= 0 )
			{
				LOG.INFO( "Sync engine blocked for " + nWait + " seconds..." );
		        wait(nWait);
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
	}
	
	void processCommands()throws Exception
	{
		if ( m_stackCommands.isEmpty() )
			addSyncCommand(new SyncCommand(scNone));
		
		while(!m_stackCommands.isEmpty())
		{
			SyncCommand oSyncCmd = null;
	    	synchronized(m_mxStackCommands)
	    	{
	    		oSyncCmd = (SyncCommand)m_stackCommands.removeFirst();
	    	}
			
			processCommand(oSyncCmd);
		}
	}
	
	void processCommand(SyncCommand oSyncCmd)throws Exception
	{
	    switch(oSyncCmd.m_nCmdCode)
	    {
	    case scNone:
	        if ( m_nPollInterval > 0 )
	            m_oSyncEngine.doSyncAllSources();
	        break;
	    case scSyncAll:
	        m_oSyncEngine.doSyncAllSources();
	        break;
	    case scChangePollInterval:
	        break;
	    case scSyncOne:
	    	m_oSyncEngine.doSyncSource(oSyncCmd.m_nCmdParam,oSyncCmd.m_strCmdParam );
	        break;
	    }
	}

	static ISyncStatusListener m_statusListener = null;
	public boolean setStatusListener(ISyncStatusListener listener) {
		m_statusListener = listener;
		if (m_oSyncEngine != null) {
			m_oSyncEngine.setStatusListener(listener);
			return true;
		}
		return false;
	}
	
	public void setPollInterval(int nInterval)
	{ 
	    m_nPollInterval = nInterval; 
	    if ( m_nPollInterval == 0 )
	        m_oSyncEngine.stopSync();
	
	    addSyncCommand(new SyncCommand(scChangePollInterval)); 
	}
	
	public static void doSyncAllSources()
	{
		getInstance().addSyncCommand(new SyncCommand(SyncThread.scSyncAll));
	}

	public static void doSyncSource(int nSrcID)
	{
		getInstance().addSyncCommand(new SyncCommand(SyncThread.scSyncOne, nSrcID) );
	}

	public static void doSyncSource(String strSrcUrl)
	{
		getInstance().addSyncCommand(new SyncCommand(SyncThread.scSyncOne, strSrcUrl) );
	}
	
	public static void stopSync()
	{
		if ( getSyncEngine().isSyncing() )
		{
			getSyncEngine().stopSync();
			while( getSyncEngine().getState() != SyncEngine.esNone )
				try{ getInstance().sleep(100); }catch(Exception e){}
		}
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("dosync", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					if (m_statusListener != null) {
						m_statusListener.createStatusPopup();
					}
					doSyncAllSources();
				}catch(Exception e)
				{
					LOG.ERROR("dosync failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				
				return RubyConstant.QNIL;
			}
			
		});
		klass.getSingletonClass().defineMethod("dosync_source", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block) {
				try{
//					if ( arg instanceof RubyString )
//						doSyncSource(arg.toStr());
//					else
					if (m_statusListener != null) {
						m_statusListener.createStatusPopup();
					}
					doSyncSource(arg.toInt());
				}catch(Exception e)
				{
					LOG.ERROR("dosync_source failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				
				return RubyConstant.QNIL;
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
		
		klass.getSingletonClass().defineMethod("lock_sync_mutex",
			new RubyNoArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyBlock block) {
					try{
					    DBAdapter db = getDBAdapter();
					    db.setUnlockDB(true);
					    db.Lock();
					}catch(Exception e)
					{
						LOG.ERROR("lock_sync_mutex failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}
				    
				    return RubyConstant.QNIL;
				}
			});
		klass.getSingletonClass().defineMethod("unlock_sync_mutex",
			new RubyNoArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyBlock block) {
					try{
					    DBAdapter db = getDBAdapter();
					    db.Unlock();
					}catch(Exception e)
					{
						LOG.ERROR("unlock_sync_mutex failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}
					
				    return RubyConstant.QNIL;
				}
			});
		klass.getSingletonClass().defineMethod("login",
			new RubyTwoArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
					int nRes = 0;
					try{
						String name = arg1.toStr();
						String password = arg2.toStr();
						
						stopSync();
						nRes = getSyncEngine().login(name,password) ? 1 : 0;
					}catch(IOException e)
					{
						LOG.ERROR("login failed", e);
						RhoRuby.raise_RhoError(RhoRuby.ERR_NETWORK);
					}catch(Exception e)
					{
						LOG.ERROR("login failed", e);
						RhoRuby.raise_RhoError(RhoRuby.ERR_RUNTIME);
					}
					
				    return ObjectFactory.createInteger(nRes);
				    
				}
			});
		
		klass.getSingletonClass().defineMethod("logged_in",
			new RubyNoArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyBlock block) {
					DBAdapter db = getDBAdapter();

					try{
						db.setUnlockDB(true);
					    return getSyncEngine().isLoggedIn() ? 
					    		ObjectFactory.createInteger(1) : ObjectFactory.createInteger(0);
					}catch(Exception e)
					{
						LOG.ERROR("logged_in failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}finally
					{
						db.setUnlockDB(false);
					}
				    
				}
			});
		
		klass.getSingletonClass().defineMethod("logout",
			new RubyNoArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyBlock block) {
					DBAdapter db = getDBAdapter();

					try{
						stopSync();
						
						db.setUnlockDB(true);
					    getSyncEngine().logout();
					}catch(Exception e)
					{
						LOG.ERROR("logout failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}finally
					{
						db.setUnlockDB(false);
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
						getSyncEngine().setNotification(source_id, url, params);
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
						getSyncEngine().clearNotification(source_id);
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
	}

}
