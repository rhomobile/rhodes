package com.rho.sync;

import java.io.IOException;
import java.util.Date;

import com.xruby.runtime.builtin.*;

import com.rho.db.PerstLiteAdapter;

public class SyncThread implements Runnable {

	private static boolean quit = false;
	private static String sync = "sync";
	private static PerstLiteAdapter adapter;

	private static final long SYNC_WAIT_INTERVAL = 90000L;

	SyncThread() {
		adapter = PerstLiteAdapter.alloc(null);
		new Thread(this).start();
		System.out.println("SyncEngine is started...");
	}

	public void run() {
		while (!quit) {
			synchronized (sync) {
				adapter.initialize(null);
				System.out.println("SyncEngine is awake..."
						+ new Date(System.currentTimeMillis()).toString());
				RubyArray sources = getSourceList();

				for (int i = 0; i < sources.size(); i++) {
					RubyHash element = (RubyHash) sources.at(createInteger(i));
					String url = element.get(PerstLiteAdapter.URL).toString();
					System.out.println("URL: " + url);
					int available = SyncUtil.fetchRemoteChanges(url);
					System.out.println("Successfully processed " + available
							+ " records...");
				}
				try {
					sync.wait(SYNC_WAIT_INTERVAL);
				} catch (InterruptedException e) {
				}
			}
		}
	}

	private RubyArray getSourceList() {
		RubyArray arr = createArray();
		arr.add(createString("sources"));
		arr.add(createString("source_url"));
		return (RubyArray) adapter.selectFromTable(arr);
	}

	private RubyString createString(String val) {
		return ObjectFactory.createString(val);
	}

	private RubyInteger createInteger(long val) {
		return ObjectFactory.createInteger(val);
	}

	private RubyHash createHash() {
		return ObjectFactory.createHash();
	}

	private RubyArray createArray() {
		return new RubyArray();
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
