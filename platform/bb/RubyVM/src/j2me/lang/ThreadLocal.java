/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.lang;

import javolution.util.FastMap;

/**
 * Clean-room implementation of ThreadLocal for J2ME Platform support.
 */
public class ThreadLocal {

    private static final FastMap THREAD_TO_LOCAL_MAP = new FastMap(256)
            .setShared(true);

    public ThreadLocal() {
    }

    public Object get() {
        FastMap localMap = getLocalMap();
        Object value = localMap.get(this);
        if ((value == null) && !(localMap.containsKey(this))) {
            value = initialValue();
            localMap.put(this, value);
        }
        return value;
    }

    public void set(Object value) {
        getLocalMap().put(this, value);
    }

    public void remove() {
        getLocalMap().remove(this);
    }

    protected Object initialValue() {
        return null;
    }

    private FastMap getLocalMap() {
        FastMap localMap = (FastMap) THREAD_TO_LOCAL_MAP.get(Thread
                .currentThread());
        return (localMap != null) ? localMap : newLocalMap();
    }

    private FastMap newLocalMap() {
        // First, do some cleanup (remove dead threads).
        for (FastMap.Entry e = THREAD_TO_LOCAL_MAP.head(), end = THREAD_TO_LOCAL_MAP
                .tail(); (e = (FastMap.Entry) e.getNext()) != end;) {
            Thread thread = (Thread) e.getKey();
            if (!thread.isAlive()) {
                THREAD_TO_LOCAL_MAP.remove(thread);
            }
        }
        FastMap localMap = new FastMap();
        THREAD_TO_LOCAL_MAP.put(Thread.currentThread(), localMap);
        return localMap;
    }
}