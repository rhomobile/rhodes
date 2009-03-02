/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.util;

import java.io.IOException;

import j2me.io.ObjectInputStream;
import j2me.io.ObjectOutputStream;
import j2me.util.Collection;
import j2me.util.Iterator;
import j2me.util.Set;
import javolution.context.ObjectFactory;
import javolution.lang.Reusable;

/**
 * <p> This class represents a set collection backed by a {@link FastMap};
 *     smooth capacity increase and no rehashing ever performed.</p>
 * 
 * <p> {@link FastSet}, as for any {@link FastCollection} sub-class, supports
 *     thread-safe fast iterations without using iterators. For example:[code]
 *     for (FastSet.Record r = set.head(), end = set.tail(); (r = r.getNext()) != end;) {
 *         Object value = set.valueOf(r);    
 *     }[/code]</p>
 *     
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 4.2, December 18, 2006
 */
public class FastSet/*<E>*/ extends FastCollection/*<E>*/ implements Set/*<E>*/, Reusable {

    /**
     * Holds the set factory.
     */
    private static final ObjectFactory FACTORY = new ObjectFactory() {

        public Object create() {
            return new FastSet();
        }

        public void cleanup(Object obj) {
            ((FastSet) obj).reset();
        }
    };

    /**
     * Holds the backing map.
     */
    private transient FastMap _map;

    /**
     * Creates a set of small initial capacity.
     */
    public FastSet() {
        this(new FastMap());
    }

    /**
     * Creates a persistent set associated to the specified unique identifier
     * (convenience method).
     * 
     * @param id the unique identifier for this map.
     * @throws IllegalArgumentException if the identifier is not unique.
     * @see javolution.context.PersistentContext.Reference
     */
    public FastSet(String id) {
        this(new FastMap(id));
    }

    /**
     * Creates a set of specified initial capacity; unless the set size 
     * reaches the specified capacity, operations on this set will not allocate
     * memory (no lazy object creation).
     * 
     * @param capacity the initial capacity.
     */
    public FastSet(int capacity) {
        this(new FastMap(capacity));
    }

    /**
     * Creates a set containing the specified elements, in the order they
     * are returned by the set iterator.
     *
     * @param elements the elements to be placed into this fast set.
     */
    public FastSet(Set/*<? extends E>*/ elements) {
        this(new FastMap(elements.size()));
        addAll(elements);
    }

    /**
     * Creates a set implemented using the specified map.
     * 
     * @param map the backing map.
     */
    private FastSet(FastMap map) {
        _map = map;
    }

    /**
     * Returns a new, preallocated or {@link #recycle recycled} set instance
     * (on the stack when executing in a {@link javolution.context.StackContext
     * StackContext}).
     *
     * @return a new, preallocated or recycled set instance.
     */
    public static /*<E>*/ FastSet/*<E>*/ newInstance() {
        return (FastSet/*<E>*/) FACTORY.object();
    }

    /**
     * Recycles a set {@link #newInstance() instance} immediately
     * (on the stack when executing in a {@link javolution.context.StackContext
     * StackContext}). 
     */
    public static void recycle(FastSet instance) {
        FACTORY.recycle(instance);
    }

    /**
     * Returns the number of elements in this set (its cardinality). 
     *
     * @return the number of elements in this set (its cardinality).
     */
    public final int size() {
        return _map.size();
    }

    /**
     * Adds the specified value to this set if it is not already present.
     *
     * @param value the value to be added to this set.
     * @return <code>true</code> if this set did not already contain the 
     *         specified element.
     * @throws NullPointerException if the value is <code>null</code>.
     */
    public final boolean add(Object/*{E}*/ value) {
        return _map.put(value, value) == null;
    }

    /**
     * Returns an iterator over the elements in this set 
     * (allocated on the stack when executed in a 
     * {@link javolution.context.StackContext StackContext}).
     *
     * @return an iterator over this set values.
     */
    public Iterator/*<E>*/iterator() {
        return _map.keySet().iterator();
    }

    // Overrides to return a set (JDK1.5+).
    public Collection/*Set<E>*/unmodifiable() {
        return (Collection/*Set<E>*/) super.unmodifiable();
    }

    // Overrides (optimization).
    public final void clear() {
        _map.clear();
    }

    // Overrides (optimization).
    public final boolean contains(Object o) {
        return _map.containsKey(o);
    }

    // Overrides (optimization).
    public final boolean remove(Object o) {
        return _map.remove(o) != null;
    }

    /**
     * Sets the comparator to use for value equality.
     *
     * @param comparator the value comparator.
     * @return <code>this</code>
     */
    public FastSet/*<E>*/ setValueComparator(FastComparator/*<? super E>*/ comparator) {
        _map.setKeyComparator(comparator);
        return this;
    }
    
    // Overrides.
    public FastComparator/*<? super E>*/ getValueComparator() {
        return _map.getKeyComparator();
    }

    // Implements Reusable.
    public void reset() {
        _map.reset();
    }

    // Requires special handling during de-serialization process.
    private void readObject(ObjectInputStream stream) throws IOException,
            ClassNotFoundException {
        FastComparator cmp = (FastComparator) stream.readObject();
        final int size = stream.readInt();        
        _map = new FastMap(size);
        this.setValueComparator(cmp);
        for (int i = size; i-- != 0;) {
            Object key = stream.readObject(); 
            _map.put(key, key);
        }
    }

    // Requires special handling during serialization process.
    private void writeObject(ObjectOutputStream stream) throws IOException {
        stream.writeObject(getValueComparator());
        stream.writeInt(size());
        for (FastMap.Entry e = _map.head(), end = _map.tail(); 
              (e = (FastMap.Entry) e.getNext()) != end;) {
            stream.writeObject(e.getKey());
        }
    }

    // Implements FastCollection abstract method.
    public final Record head() {
        return _map.head();
    }

    // Implements FastCollection abstract method.
    public final Record tail() {
        return _map.tail();
    }

    // Implements FastCollection abstract method.
    public final Object/*{E}*/ valueOf(Record record) {
        return (Object/*{E}*/) ((FastMap.Entry) record).getKey();
    }

    // Implements FastCollection abstract method.
    public final void delete(Record record) {
        _map.remove(((FastMap.Entry) record).getKey());
    }

    private static final long serialVersionUID = 1L;
}
