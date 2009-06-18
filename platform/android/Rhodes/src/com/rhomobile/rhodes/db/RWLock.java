package com.rhomobile.rhodes.db;

public class RWLock {
	private int givenLocks;
	private int waitingWriters;

	private Object mutex;

	public RWLock() {
		mutex = new Object();
		givenLocks = 0;
		waitingWriters = 0;
	}

	public void getReadLock() {
		synchronized (mutex) {

			try {
				while ((givenLocks == -1) || (waitingWriters != 0)) {
					mutex.wait();
				}
			} catch (java.lang.InterruptedException e) {
				System.out.println(e);
			}

			givenLocks++;
		}
	}

	public void getWriteLock() {
		synchronized (mutex) {
			waitingWriters++;
			try {
				while (givenLocks != 0) {
					mutex.wait();
				}
			} catch (java.lang.InterruptedException e) {
				System.out.println(e);
			}

			waitingWriters--;
			givenLocks = -1;
		}
	}

	public void releaseLock() {

		synchronized (mutex) {

			if (givenLocks == 0)
				return;

			if (givenLocks == -1)
				givenLocks = 0;
			else
				givenLocks--;

			mutex.notifyAll();
		}
	}

}
