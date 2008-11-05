package com.rho.sync;

import java.util.Date;

import com.xruby.runtime.builtin.*;

import com.rho.db.PerstLiteAdapter;

public class SyncThread implements Runnable {

	private boolean quit = false;
	private String sync = "sync";
	private static long SYNC_WAIT_INTERVAL = 5000L;
	private static PerstLiteAdapter adapter = PerstLiteAdapter.alloc(null);

	SyncThread() {
		new Thread(this).start();
		System.out.println("SyncEngine is started...");
	}

	public void run() {
		while (!quit) {
			synchronized (sync) {
				adapter.initialize(null);
				System.out.println("SyncEngine is awake..."
						+ new Date(System.currentTimeMillis()).toString());
				RubyArray arr = createArray(3);
				RubyArray cols = createArray(2);
				cols.insert(0, createString("source_id"));
				cols.insert(1, createString("source_url"));
				arr.insert(0, createString("sources"));
				arr.insert(1, cols);
				arr.insert(2,null);
				RubyArray results = (RubyArray)adapter.selectFromTable(arr);
				
				for(int i = 0; i < results.size(); i++) {
					System.out.println("Result: " + results.at(createInteger(i)).asString());
				}
				try {
					sync.wait(SYNC_WAIT_INTERVAL);
				} catch (InterruptedException e) {
				}
			}
		}
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
	
	private RubyArray createArray(int size) {
		return ObjectFactory.createArray(size, null);
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
