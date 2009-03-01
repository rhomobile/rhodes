/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.util;

/**
 * <p> This class represents a reentrant lock with the same semantics as
 *     built-in Java synchronized locks: Once a thread has a lock, it can
 *     re-obtain it any number of times without blocking. </p>
 *
 * @author  <a href="http://gee.cs.oswego.edu/dl/">Doug Lea</a>
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 1.0, October 4, 2004
 */
public class ReentrantLock {

    /**
     * Holds the owner of this lock.
     */
    private Thread _owner;

    /**
     * Holds the number of time this lock has been acquired by its owner.
     */
    private long _count;

    /**
     * Default constructor.
     */
    public ReentrantLock() {
    }

    /**
     * Acquires the lock.
     */
    public void lock() {
        Thread caller = Thread.currentThread();
        synchronized (this) {
            if (caller == _owner) {
                _count++;
            } else {
                try {
                    while (_owner != null) {
                        this.wait();
                    }
                    _owner = caller;
                    _count = 1;
                } catch (InterruptedException exception) {
                    return;
                }
            }
        }
    }

    /**
     * Acquires the lock only if it not held by another thread.
     *
     * @return <code>true</code> if the lock was free and was acquired by the
     *         current thread, or the lock was already held by the current
     *         thread; <code>false</code> otherwise.
     */
    public boolean tryLock() {
        synchronized (this) {
            if (_owner == null) {
                lock();
                return true;
            } else {
                return false;
            }
        }
    }

    /**
     * Attempts to release this lock. The lock is actually released if at
     * least as many {@link #unlock} as {@link #lock} have been performed
     * on this {@link ReentrantLock} by the current thread.
     *
     * throws IllegalMonitorStateExeception if the current thread does not hold
     *        this lock.
     */
    public void unlock() {
        synchronized (this) {
            if (Thread.currentThread() == _owner) {
                if (--_count == 0) {
                    _owner = null;
                    this.notify();
                }
            } else {
                throw new IllegalMonitorStateException(
                        "Current thread does not hold this lock");
            }
        }
    }

    /**
     * Returns the thread owner of this {@link ReentrantLock}.
     *
     * @return the owner of this lock.
     */
    public Thread getOwner() {
        synchronized (this) {
            return _owner;
        }
    }
}