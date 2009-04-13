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
package rhomobile.sync;

import java.util.Date;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

import rhomobile.db.PerstLiteAdapter;

/**
 * The Class SyncThread.
 */
public class SyncThread implements Runnable {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("SyncThread");

	/** The quit. */
	// private boolean quit = false;
	/** The sync. */
	private String sync = "sync";

	//private static boolean delaySync = false;
	private static boolean dbResetDelay = false;

	/** The Constant SYNC_WAIT_INTERVAL. */
	private static final long SYNC_WAIT_INTERVAL = 300000L;

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
		//SyncUtil.adapter = PerstLiteAdapter.alloc(null);
		//SyncBlob.DBCallback callback = new SyncBlob.DBCallback();
		//SyncUtil.adapter.setDbCallback(callback);
		
		//delaySync = SyncUtil.getObjectCountFromDatabase("object_values") > 0 ? true
		//		: false;
		
		Thread thread = new Thread(this);
		thread.setPriority(Thread.MIN_PRIORITY);
		thread.start();
		
		LOG.INFO("SyncEngine is started...");
		//printStats();
	}

	/**
	 * Quit.
	 */
	public void quit() {
		synchronized (sync) {
			setState(STATE_DOSTOP);
			sync.notify();

			SyncManager.closeConnection();
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Runnable#run()
	 */
	public void run() {
		while (!isStop()) {
			synchronized (sync) {
				try {
					if (!isStop()) {
						setState(STATE_PAUSE);
						sync.wait(SYNC_WAIT_INTERVAL);
					}
				} catch (Exception e) {
					LOG.INFO("Wait exception:" + e.getMessage());
				}
			}

			if ( isStop() )
				break;
			
			// synchronized (sync) {
			//SyncUtil.adapter.initialize(null);
			LOG.INFO("SyncEngine is awake...");
			//printStats();

			if (/*!delaySync &&*/ !dbResetDelay) {
				// Thread is simple, process local changes and make sure
				// there are no errors before waiting for SYNC_WAIT_INTERVAL
				setState(STATE_SYNC);
				if (SyncUtil.processLocalChanges(this) != SyncConstants.SYNC_PROCESS_CHANGES_OK) {
					LOG.ERROR("There was an error processing local changes");
					break;
				}
			} else if (dbResetDelay) {
				SyncUtil.resetSyncDb();
				dbResetDelay = false;
				//delaySync = false;
			} else {
				//delaySync = false;
			}

			// }
		}
		setState(STATE_NONE);
		LOG.INFO("Shutting down SyncEngine...");
		// SyncUtil.adapter.close();
		LOG.INFO("SyncEngine is shutdown...");
	}

	/**
	 * Wake up sync engine.
	 * 
	 * @return true, if successful
	 */
	public void wakeUpSyncEngine() {
		if (getState() == STATE_PAUSE) {
			synchronized (sync) {
				//delaySync = false;
				sync.notify();
				sync.notify();
			}
		}

		// synchronized (sync) {
		// if (!quit) {
		// sync.notify();
		// return true;
		// }
		// return false;
		// }
	}

	public static void printStats() {
		long free = java.lang.Runtime.getRuntime().freeMemory();
		long total = java.lang.Runtime.getRuntime().totalMemory();
		
		LOG.INFO("Memory stats (free / total) => usage: (" + free
				+ " bytes / " + total + " bytes) => " + (total - free)
				+ " bytes");
	}

	public synchronized int getState() {
		return m_nState;
	}

	public boolean isStop() {
		return getState() == STATE_DOSTOP;
	}

	private synchronized void setState(int state) {
		m_nState = state;
	}

	public static void setDbResetDelay(boolean dbResetDelay) {
		SyncThread.dbResetDelay = dbResetDelay;
	}
}