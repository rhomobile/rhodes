/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util;

import j2me.util.Map;
import j2me.util.Comparator;

public interface SortedMap extends Map {
    Comparator comparator();

    SortedMap subMap(Object fromKey, Object toKey);

    SortedMap headMap(Object toKey);

    SortedMap tailMap(Object fromKey);

    Object firstKey();

    Object lastKey();
}
