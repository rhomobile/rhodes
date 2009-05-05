/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util.concurrent;
import j2me.util.Map;

public interface ConcurrentMap extends Map {

    Object putIfAbsent(Object key, Object value);

    boolean remove(Object key, Object value);

    boolean replace(Object key, Object oldValue, Object newValue);

    Object replace(Object key, Object value);
    
}
