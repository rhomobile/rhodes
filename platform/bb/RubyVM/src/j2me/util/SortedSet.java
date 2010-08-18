/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util;

import j2me.util.Set;
import j2me.util.Comparator;

public interface SortedSet extends Set {
    Comparator comparator();

    SortedSet subSet(Object fromElement, Object toElement);

    SortedSet headSet(Object toElement);

    SortedSet tailSet(Object fromElement);

    Object first();

    Object last();
}
