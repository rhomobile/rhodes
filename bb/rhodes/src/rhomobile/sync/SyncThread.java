/*
 *  rhodes
 *
 *  Copyright (C) 2008 Lars Burgess. All rights reserved.
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
package rhomobile.sync;

import java.util.Date;

import rhomobile.db.PerstLiteAdapter;

/**
 * The Class SyncThread.
 */
public class SyncThread implements Runnable {

	/** The quit. */
	//private boolean quit = false;

	/** The sync. */
	private String sync = "sync";

	private static boolean delaySync = false;
	private static boolean dbResetDelay = false;

	/** The Constant SYNC_WAIT_INTERVAL. */
	private static final long SYNC_WAIT_INTERVAL = 600000L;

	private static final int STATE_NONE = 0;
	private static final int STATE_SYNC = 1;
	private static final int STATE_CANCEL = 2;
	private static final int STATE_PAUSE = 3;
	private static final int STATE_DOSTOP = 4;
	
	private int m_nState = STATE_NONE;
	
	/**
	 * Instantiates a new sync thread.
	 */
	SyncThread() {
		SyncUtil.adapter = PerstLiteAdapter.alloc(null);
		//SyncUtil.adapter.initialize(null);
		delaySync = SyncUtil.getObjectCountFromDatabase("object_values") > 0 ? true : false;
		Thread thread = new Thread(this);
		thread.setPriority(Thread.MIN_PRIORITY);
		thread.start();
		System.out.println("SyncEngine is started...");
	}

	/**
	 * Quit.
	 */
	public void quit() {
		synchronized (sync) {
			setState(STATE_DOSTOP);
			sync.notify();
			
			SyncManager.closeConnection();
			/*int nTry = 0;
			while( nTry < 10 && getState() != STATE_NONE ){
				try{
					sync.wait(100);
					nTry++;
				} catch (Exception e) {
					System.out.println("Wait exception:" + e.getMessage());
				}				
			}*/
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Runnable#run()
	 */
	public void run() {
		//SyncUtil.fetch_client_login("lars","password");
		
		while (!isStop()) {
			synchronized (sync) {
				try {
					if (!isStop()){
						setState(STATE_PAUSE);
						sync.wait(SYNC_WAIT_INTERVAL);
					}
				} catch (Exception e) {
					System.out.println("Wait exception:" + e.getMessage());
				}
			}
			
			//synchronized (sync) {
				SyncUtil.adapter.initialize(null);
				System.out.println("SyncEngine is awake..."
						+ new Date(System.currentTimeMillis()).toString());

				if (!delaySync && !dbResetDelay) {
					// Thread is simple, process local changes and make sure
					// there are no errors before waiting for SYNC_WAIT_INTERVAL
					setState(STATE_SYNC);
					if (SyncUtil.processLocalChanges(this) != SyncConstants.SYNC_PROCESS_CHANGES_OK) {
						System.out
								.println("There was an error processing local changes");
						break;
					}
				} else if (dbResetDelay) {
					SyncUtil.resetSyncDb();
					dbResetDelay = false;
					delaySync = false;
				} else {
					delaySync = false;
				}

			//}
		}
		setState(STATE_NONE);
		System.out.println("Shutting down SyncEngine...");
		// SyncUtil.adapter.close();
		System.out.println("SyncEngine is shutdown...");
	}

	/**
	 * Wake up sync engine.
	 * 
	 * @return true, if successful
	 */
	public void wakeUpSyncEngine() {
		if ( getState() == STATE_PAUSE ){
			synchronized (sync) {
				delaySync = false;
				sync.notify(); 
				sync.notify();
			}
		}
		
		//synchronized (sync) {
			//if (!quit) {
			//	sync.notify();
			//	return true;
			//}
			//return false;
		//}
	}

	public synchronized int getState() {
		return m_nState;
	}

	public boolean isStop() {
		return getState()==STATE_DOSTOP;
	}
	
	private synchronized void setState(int state) {
		m_nState = state;
	}

	public static void setDbResetDelay(boolean dbResetDelay) {
		SyncThread.dbResetDelay = dbResetDelay;
	}
}