/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util;

import j2me.util.Collection;

public interface Queue extends Collection {
    boolean offer(Object o);

    Object poll();

    Object remove();

    Object peek();

    Object element();
}
