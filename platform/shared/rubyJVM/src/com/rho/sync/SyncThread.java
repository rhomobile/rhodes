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
import com.xruby.runtime.lang.*;

public class SyncThread extends RhoThread
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Sync");
	private static final int SYNC_POLL_INTERVAL_SECONDS = 300;
	private static final int SYNC_POLL_INTERVAL_INFINITE = Integer.MAX_VALUE/1000;
	private static final int SYNC_WAIT_BEFOREKILL_SECONDS  = 3;
	
	static SyncThread m_pInstance;

   	public final static int scNone = 0, scResetDB = 1, scSyncAll = 2, scSyncOne = 3, scChangePollInterval=4, scExit=5; 
    
    SyncEngine  m_oSyncEngine;
    RhoClassFactory m_ptrFactory;
    int           m_curCommand;
	int           m_nPollInterval;
    int           m_nCmdParam;
    
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
	    m_curCommand = scNone;
		m_nPollInterval = SYNC_POLL_INTERVAL_SECONDS;
		m_ptrFactory = factory;
	
	    m_oSyncEngine.setFactory(factory);
	
	    start(epLow);
	}

    public static SyncThread getInstance(){ return m_pInstance; }
    static SyncEngine getSyncEngine(){ return m_pInstance.m_oSyncEngine; }
    static DBAdapter getDBAdapter(){ return DBAdapter.getInstance(); }

    void addSyncCommand(int curCommand){ m_curCommand = curCommand; stopWait(); }
    void addSyncCommand(int curCommand, int nCmdParam){ m_curCommand = curCommand; m_nCmdParam = nCmdParam; stopWait(); }
	
	public void run()
	{
		LOG.INFO( "Starting sync engine main routine..." );
	
		while( m_oSyncEngine.getState() != SyncEngine.esExit )
		{
	        int nWait = m_nPollInterval > 0 ? m_nPollInterval : SYNC_POLL_INTERVAL_INFINITE;
			LOG.INFO( "Sync engine blocked for " + nWait + " seconds..." );
	        wait(nWait);
	
	        if ( m_oSyncEngine.getState() != SyncEngine.esExit )
	        {
	        	try{
	        		processCommand();
	        	}catch(Exception e)
	        	{
	        		LOG.ERROR("processCommand failed", e);
	        	}
	        }
		}
	}
	
	void processCommand()throws Exception
	{
	    //TODO: implement stack of commands
	    switch(m_curCommand)
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
	    	m_oSyncEngine.doSyncSource(m_nCmdParam);
	        break;
	    case scResetDB:
	        m_oSyncEngine.resetSyncDB();
	        break;
	
	    }
	    m_curCommand = scNone;
	}

	public void setPollInterval(int nInterval)
	{ 
	    m_nPollInterval = nInterval; 
	    if ( m_nPollInterval == 0 )
	        m_oSyncEngine.stopSync();
	
	    addSyncCommand(scChangePollInterval); 
	}
	
	public static void doSyncAllSources()
	{
		getInstance().addSyncCommand(SyncThread.scSyncAll);
	}

	public static void doSyncSource(int nSrcID)
	{
		getInstance().addSyncCommand(SyncThread.scSyncOne, nSrcID );
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("dosync", new RubyNoOrOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block) {
				try{
					doSyncSource(arg.toInt());
				}catch(Exception e)
				{
					LOG.ERROR("dosync failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				
				return RubyConstant.QNIL;
			}
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					doSyncAllSources();
				}catch(Exception e)
				{
					LOG.ERROR("dosync failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				
				return RubyConstant.QNIL;
			}
			
		});
		klass.getSingletonClass().defineMethod("stop_sync", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					getSyncEngine().stopSync();
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
					try{
						String name = arg1.toStr();
						String password = arg2.toStr();
					    //TODO: stop sync
					    return getSyncEngine().login(name,password) ? 
					    		ObjectFactory.createInteger(1) : ObjectFactory.createInteger(0);
					}catch(Exception e)
					{
						LOG.ERROR("login failed", e);
						//throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}
				    return ObjectFactory.createInteger(0);
				    
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
					    //TODO: stop sync
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
		
		klass.getSingletonClass().defineMethod("trigger_sync_db_reset",
			new RubyNoArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyBlock block) {
					try{
						getInstance().addSyncCommand(SyncThread.scResetDB);
					}catch(Exception e)
					{
						LOG.ERROR("trigger_sync_db_reset failed", e);
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
