/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2mex.realtime;

/**
 * Stub for non-rtsj vms.
 */
public class MemoryArea {

    static final MemoryArea DEFAULT = new MemoryArea();

    private MemoryArea() {
    }

    public static MemoryArea getMemoryArea(Object object) {
        return DEFAULT;
    }
    
    
    public void executeInArea(Runnable logic) {
        logic.run();
    }

    public Object newInstance(Class type) throws InstantiationException,
           IllegalAccessException {
        return type.newInstance();
    }

}