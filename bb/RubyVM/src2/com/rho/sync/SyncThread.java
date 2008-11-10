package com.rho.sync;

import java.util.Date;

import com.rho.db.PerstLiteAdapter;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;

public class SyncThread implements Runnable {

	private static boolean quit = false;
	private static String sync = "sync";

	private static final long SYNC_WAIT_INTERVAL = 90000L;

	SyncThread() {
		SyncUtil.adapter = PerstLiteAdapter.alloc(null);
		new Thread(this).start();
		System.out.println("SyncEngine is started...");
	}

	public void run() {
		while (!quit) {
			synchronized (sync) {
				SyncUtil.adapter.initialize(null);
				System.out.println("SyncEngine is awake..."
						+ new Date(System.currentTimeMillis()).toString());
				RubyArray sources = SyncUtil.getSourceList();

				for (int i = 0; i < sources.size(); i++) {
					RubyHash element = (RubyHash) sources.at(SyncUtil
							.createInteger(i));
					String url = element.get(PerstLiteAdapter.URL).toString();
					int id = element.get(PerstLiteAdapter.SOURCE_ID).toInt();
					System.out.println("URL: " + url);
					int available = SyncUtil.fetchRemoteChanges(url, id);
					System.out.println("Successfully processed " + available
							+ " records...");
					if (SyncConstants.DEBUG) {
						RubyArray objects = SyncUtil.getObjectValueList(id);
						SyncUtil.printResults(objects);
					}
				}
				try {
					sync.wait(SYNC_WAIT_INTERVAL);
				} catch (InterruptedException e) {
				}
			}
		}
	}

	public boolean wakeUpSyncEngine(Object o) {
		synchronized (sync) {
			if (!quit) {
				sync.notify();
				return true;
			}
			return false;
		}
	}

	public void quit() {
		synchronized (sync) {
			quit = true;
			sync.notify();
		}
	}
}
