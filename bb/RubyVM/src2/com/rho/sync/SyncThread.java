package com.rho.sync;

import java.util.Date;

public class SyncThread implements Runnable {

	private boolean quit = false;
	private String sync = "sync";
	private static long SYNC_WAIT_INTERVAL = 5000L;

	SyncThread() {
		new Thread(this).start();
		System.out.println("SyncEngine is started...");
	}

	public void run() {
		while (!quit) {
			synchronized (sync) {
				System.out.println("SyncEngine is awake..."
						+ new Date(System.currentTimeMillis()).toString());
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
