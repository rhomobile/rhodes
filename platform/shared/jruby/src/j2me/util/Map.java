/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util;

public interface Map {
    int size();

    boolean isEmpty();

    boolean containsKey(Object key);

    boolean containsValue(Object value);

    Object get(Object key);

    Object put(Object key, Object value);

    Object remove(Object key);

    void putAll(Map t);

    void clear();

    Set keySet();

    Collection values();

    Set entrySet();

    interface Entry {
        Object getKey();

        Object getValue();

        Object setValue(Object value);

        boolean equals(Object o);

        int hashCode();
    }

    boolean equals(Object o);

    int hashCode();
}
