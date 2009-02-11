/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.util;

import j2me.lang.IllegalStateException;
import j2me.util.Iterator;
import j2me.util.NoSuchElementException;
import javolution.context.ObjectFactory;
import javolution.util.FastCollection.Record;

/**
 * <p> This class represents an iterator over a {@link Fastcollection).
 *     Iterations are thread-safe if the collections records are not removed 
 *     or inserted at arbitrary position (appending/prepending is fine).</p>
 *     
 * <p> Iterators are allocated on the stack when executing in a 
 *     {@link StackContext javolution.context.StackContext}.</p>
 *          
 * @author <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 3.7, March 17, 2005
 */
final class FastIterator implements Iterator {

    private static final ObjectFactory FACTORY = new ObjectFactory() {
        protected Object create() {
            return new FastIterator();
        }

        protected void cleanup(Object obj) {
            FastIterator iterator = (FastIterator) obj;
            iterator._collection = null;
            iterator._current = null;
            iterator._next = null;
            iterator._tail = null;
        }
    };

    private FastCollection _collection;

    private Record _current;

    private Record _next;

    private Record _tail;

    public static FastIterator valueOf(FastCollection collection) {
        FastIterator iterator = (FastIterator) FastIterator.FACTORY.object();
        iterator._collection = collection;
        iterator._next = collection.head().getNext();
        iterator._tail = collection.tail();
        return iterator;
    }

    private FastIterator() {
    }

    public boolean hasNext() {
        return (_next != _tail);
    }

    public Object next() {
        if (_next == _tail)
            throw new NoSuchElementException();
        _current = _next;
        _next = _next.getNext();
        return _collection.valueOf(_current);
    }

    public void remove() {
        if (_current != null) {
            // Uses the previous record (not affected by the remove)
            // to set the next record.
            final Record previous = _current.getPrevious();
            _collection.delete(_current);
            _current = null;
            _next = previous.getNext();
        } else {
            throw new IllegalStateException();
        }
    }
}
