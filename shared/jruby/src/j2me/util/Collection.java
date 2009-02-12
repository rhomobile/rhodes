/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util;
//import j2me.lang.Iterable;
import j2me.util.Iterator;

public interface Collection {//extends Iterable {

    int size();

    boolean isEmpty();

    boolean contains(Object o);

    Iterator iterator();

    Object[] toArray();

    Object[] toArray(Object[] a);

    boolean add(Object o);

    boolean remove(Object o);

    boolean containsAll(Collection c);

    boolean addAll(Collection c);

    boolean removeAll(Collection c);

    boolean retainAll(Collection c);

    void clear();

    boolean equals(Object o);

    int hashCode();
}
