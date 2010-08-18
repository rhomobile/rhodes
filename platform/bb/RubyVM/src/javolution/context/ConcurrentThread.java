/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;

import j2mex.realtime.MemoryArea;
import j2mex.realtime.RealtimeThread;
import javolution.lang.Reflection;

/**
 * <p> This class represents the concurrent executors used by the default 
 *     implementation of {@link ConcurrentContext}. Executions
 *     are performed in the same memory area and at the same priority
 *     as the calling thread.</p>
 *
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.1, July 1, 2007
 */
class ConcurrentThread extends RealtimeThread {

    private volatile Runnable _logic;

    private MemoryArea _memoryArea;

    private int _priority;

    private ConcurrentContext.Default _context;

    private boolean _terminate;

    private String _name;

    private Thread _source;

    /**
     * Default constructor.
     */
    public ConcurrentThread() {
        _name = "ConcurrentThread-" + getCount();
        if (SET_NAME != null) {
            SET_NAME.invoke(this, _name);
        }
        if (SET_DAEMON != null) {
            SET_DAEMON.invoke(this, new Boolean(true));
        }
    }

    private synchronized int getCount() {
        return _Count++;
    }

    private static int _Count;

    private static final Reflection.Method SET_NAME = Reflection
            .getMethod("java.lang.Thread.setName(String)");

    private static final Reflection.Method SET_DAEMON = Reflection
            .getMethod("java.lang.Thread.setDaemon(boolean)");

    /**
     * Executes the concurrent logics sequentially.
     */
    public void run() {
        while (true) { // Main loop.
            synchronized (this) { // Waits for a task.
                try {
                    while ((_logic == null) && !_terminate)
                        this.wait();
                } catch (InterruptedException e) {
                    throw new ConcurrentException(e);
                }
            }
            if (_logic == null)
                break; // Terminates.
            try {
                Thread current = Thread.currentThread();
                if (current.getPriority() != _priority) {
                    current.setPriority(_priority);
                }
                _context.started();
                _memoryArea.executeInArea(_logic);
            } catch (Exception error) {
                _context.error(error);
            } finally {
                _context.completed();
                _logic = null; // Last (ready).
            }
        }
    }

    /**
     * Executes the specified logic by this thread if ready.
     * 
     * @param logic the logic to execute.
     * @param context the concurrent context.
     */
    public boolean execute(Runnable logic, ConcurrentContext.Default context) {
        if (_logic != null)
            return false; // Shortcut to avoid synchronizing.
        synchronized (this) {
            if (_logic != null)
                return false; // Synchronized check.
            _memoryArea = RealtimeThread.getCurrentMemoryArea();
            _priority = Thread.currentThread().getPriority();
            _context = context;
            _logic = logic; // Must be last.
            this.notify();
            return true;
        }
    }

    // Implements ConcurrentExecutor
    public void terminate() {
        synchronized (this) {
            _terminate = true;
            this.notify();
        }
    }

    /**
     * Returns the name of this concurrent thread as well as its calling source 
     * (in parenthesis).
     * 
     * @return the string representation of this thread.
     */
    public String toString() {
        return _name + "(" + _source + ")";
    }

}
