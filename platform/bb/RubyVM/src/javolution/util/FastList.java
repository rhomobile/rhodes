/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.util;

import java.io.IOException;

import j2me.io.ObjectInputStream;
import j2me.io.ObjectOutputStream;
import j2me.io.Serializable;
import j2me.lang.IllegalStateException;
import j2me.util.Collection;
import j2me.util.Iterator;
import j2me.util.List;
import j2me.util.ListIterator;
import j2mex.realtime.MemoryArea;
import java.util.NoSuchElementException;

import javolution.context.ObjectFactory;
import javolution.context.PersistentContext;
import javolution.lang.Reusable;

/**
 * <p> This class represents a linked list with real-time behavior; 
 *     smooth capacity increase and no memory allocation as long as the
 *     list size does not exceed its initial capacity.</p>
 * 
 * <p> All of the operations perform as could be expected for a doubly-linked
 *     list ({@link #addLast insertion}/{@link #removeLast() deletion}
 *     at the end of the list are nonetheless the fastest). 
 *     Operations that index into the list will traverse the list from
 *     the begining or the end whichever is closer to the specified index. 
 *     Random access operations can be significantly accelerated by 
 *     {@link #subList splitting} the list into smaller ones.</p> 
 * 
 * <p> {@link FastList} (as for any {@link FastCollection} sub-class) supports
 *     thread-safe, fast iterations without using iterators.[code]
 *     FastList<String> list = new FastList<String>();
 *     for (FastList.Node<String> n = list.head(), end = list.tail(); (n = n.getNext()) != end;) {
 *         String value = n.getValue(); // No typecast necessary.    
 *     }[/code]</p>
 *     
 * <p> {@link FastList} are fully {@link Reusable reusable}, they maintain 
 *     internal pools of {@link Node nodes} objects. When a node is removed
 *     from its list, it is automatically restored to its pool.</p>
 * 
 * <p> Custom list implementations may override the {@link #newNode} method 
 *     in order to return their own {@link Node} implementation (with 
 *     additional fields for example).</p>
 *     
 * @author <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 4.2, December 18, 2006
 */
public class FastList/*<E>*/extends FastCollection/*<E>*/
implements List/*<E>*/, Reusable {

    /**
     * Holds the main list factory.
     */
    private static final ObjectFactory FACTORY = new ObjectFactory() {

        public Object create() {
            return new FastList();
        }

        public void cleanup(Object obj) {
            ((FastList) obj).reset();
        }
    };

    /**
     * Holds the node marking the beginning of the list (not included).
     */
    private transient Node/*<E>*/_head = newNode();

    /**
     * Holds the node marking the end of the list (not included).
     */
    private transient Node/*<E>*/_tail = newNode();

    /**
     * Holds the value comparator.
     */
    private transient FastComparator/*<? super E>*/_valueComparator = FastComparator.DEFAULT;

    /**
     * Holds the current size.
     */
    private transient int _size;

    /**
     * Creates a list of small initial capacity.
     */
    public FastList() {
        this(4);
    }

    /**
     * Creates a persistent list associated to the specified unique identifier
     * (convenience method).
     * 
     * @param id the unique identifier for this map.
     * @throws IllegalArgumentException if the identifier is not unique.
     * @see javolution.context.PersistentContext.Reference
     */
    public FastList(String id) {
        this();
        new PersistentContext.Reference(id, this) {
            protected void notifyChange() {
                FastList.this.clear();
                FastList.this.addAll((FastList) this.get());
            }
        };
    }

    /**
     * Creates a list of specified initial capacity; unless the list size 
     * reaches the specified capacity, operations on this list will not allocate
     * memory (no lazy object creation).
     * 
     * @param capacity the initial capacity.
     */
    public FastList(int capacity) {
        _head._next = _tail;
        _tail._previous = _head;
        Node/*<E>*/previous = _tail;
        for (int i = 0; i++ < capacity;) {
            Node/*<E>*/newNode = newNode();
            newNode._previous = previous;
            previous._next = newNode;
            previous = newNode;
        }
    }

    /**
     * Creates a list containing the specified values, in the order they
     * are returned by the collection's iterator.
     *
     * @param values the values to be placed into this list.
     */
    public FastList(Collection/*<? extends E>*/values) {
        this(values.size());
        addAll(values);
    }

    /**
     * Returns a new, preallocated or {@link #recycle recycled} list instance
     * (on the stack when executing in a {@link javolution.context.StackContext
     * StackContext}).
     *
     * @return a new, preallocated or recycled list instance.
     */
    public static/*<E>*/FastList/*<E>*/newInstance() {
        return (FastList/*<E>*/) FACTORY.object();
    }

    /**
     * Recycles a list {@link #newInstance() instance} immediately
     * (on the stack when executing in a {@link javolution.context.StackContext
     * StackContext}). 
     */
    public static void recycle(FastList instance) {
        FACTORY.recycle(instance);
    }

    /**
     * Appends the specified value to the end of this list
     * (equivalent to {@link #addLast}).
     *
     * @param value the value to be appended to this list.
     * @return <code>true</code> (as per the general contract of the
     *         <code>Collection.add</code> method).
     */
    public final boolean add(Object/*{E}*/value) {
        addLast(value);
        return true;
    }

    /**
     * Returns the hash code value for this list.  The hash code of a list
     * is defined to be the result of the following calculation:[code]
     *  h = 1;
     *  Iterator i = list.iterator();
     *  while (i.hasNext()) {
     *      Object obj = i.next();
     *      h = 31 * h +  this.getValueComparator().hashCodeOf(obj);
     *  }[/code]
     *
     * @return the hash code value for this list.
     */
    public int hashCode() {
        final FastComparator comp = this.getValueComparator();
        int h = 1;
        for (Node n = _head, end = _tail; (n = n._next) != end;) {
            h = 31 * h + comp.hashCodeOf(n._value);
        }
        return h;
    }

    /**
     * Returns the value at the specified position in this list.
     *
     * @param index the index of value to return.
     * @return the value at the specified position in this list.
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *         (index >= size())</code>
     */
    public final Object/*{E}*/get(int index) {
        if ((index < 0) || (index >= _size))
            throw new IndexOutOfBoundsException("index: " + index);
        return nodeAt(index)._value;
    }

    /**
     * Replaces the value at the specified position in this list with the
     * specified value.
     *
     * @param index the index of value to replace.
     * @param value the value to be stored at the specified position.
     * @return the value previously at the specified position.
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *         (index >= size())</code>
     */
    public final Object/*{E}*/set(int index, Object/*{E}*/value) {
        if ((index < 0) || (index >= _size))
            throw new IndexOutOfBoundsException("index: " + index);
        final Node/*<E>*/node = nodeAt(index);
        Object/*{E}*/previousValue = node._value;
        node._value = value;
        return previousValue;
    }

    /**
     * Inserts the specified value at the specified position in this list.
     * Shifts the value currently at that position
     * (if any) and any subsequent values to the right (adds one to their
     * indices).
     *
     * @param index the index at which the specified value is to be inserted.
     * @param value the value to be inserted.
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *         (index > size())</code>
     */
    public final void add(int index, Object/*{E}*/value) {
        if ((index < 0) || (index > _size))
            throw new IndexOutOfBoundsException("index: " + index);
        addBefore(nodeAt(index), value);
    }

    /**
     * Inserts all of the values in the specified collection into this
     * list at the specified position. Shifts the value currently at that
     * position (if any) and any subsequent values to the right 
     * (increases their indices). 
     *
     * @param index the index at which to insert first value from the specified
     *        collection.
     * @param values the values to be inserted into this list.
     * @return <code>true</code> if this list changed as a result of the call;
     *         <code>false</code> otherwise.
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *         (index > size())</code>
     */
    public final boolean addAll(int index, Collection/*<? extends E>*/values) {
        if ((index < 0) || (index > _size))
            throw new IndexOutOfBoundsException("index: " + index);
        final Node indexNode = nodeAt(index);
        Iterator/*<? extends E>*/i = values.iterator();
        while (i.hasNext()) {
            addBefore(indexNode, i.next());
        }
        return values.size() != 0;
    }

    /**
     * Removes the value at the specified position in this list.
     * Shifts any subsequent values to the left (subtracts one
     * from their indices). Returns the value that was removed from the
     * list.
     *
     * @param index the index of the value to removed.
     * @return the value previously at the specified position.
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *         (index >= size())</code>
     */
    public final Object/*{E}*/remove(int index) {
        if ((index < 0) || (index >= _size))
            throw new IndexOutOfBoundsException("index: " + index);
        final Node/*<E>*/node = nodeAt(index);
        Object/*{E}*/previousValue = node._value;
        delete(node);
        return previousValue;
    }

    /**
     * Returns the index in this list of the first occurrence of the specified
     * value, or -1 if this list does not contain this value.
     *
     * @param value the value to search for.
     * @return the index in this list of the first occurrence of the specified
     *         value, or -1 if this list does not contain this value.
     */
    public final int indexOf(Object value) {
        final FastComparator comp = this.getValueComparator();
        int index = 0;
        for (Node n = _head, end = _tail; (n = n._next) != end; index++) {
            if (comp == FastComparator.DEFAULT ? defaultEquals(value, n._value)
                    : comp.areEqual(value, n._value))
                return index;
        }
        return -1;
    }

    /**
     * Returns the index in this list of the last occurrence of the specified
     * value, or -1 if this list does not contain this value.
     *
     * @param value the value to search for.
     * @return the index in this list of the last occurrence of the specified
     *         value, or -1 if this list does not contain this value.
     */
    public final int lastIndexOf(Object value) {
        final FastComparator comp = this.getValueComparator();
        int index = size() - 1;
        for (Node n = _tail, end = _head; (n = n._previous) != end; index--) {
            if (comp == FastComparator.DEFAULT ? defaultEquals(value, n._value)
                    : comp.areEqual(value, n._value))
                return index;
        }
        return -1;
    }

    /**
     * Returns a simple iterator over the elements in this list 
     * (allocated on the stack when executed in a 
     * {@link javolution.context.StackContext StackContext}).
     *
     * @return an iterator over this list values.
     */
    public Iterator/*<E>*/iterator() {
        return listIterator();
    }

    /**
     * Returns a list iterator over the elements in this list 
     * (allocated on the stack when executed in a 
     * {@link javolution.context.StackContext StackContext}).
     *
     * @return an iterator over this list values.
     */
    public ListIterator/*<E>*/listIterator() {
        return FastListIterator.valueOf(this, _head._next, 0, _size);
    }

    /**
     * Returns a list iterator from the specified position
     * (allocated on the stack when executed in a 
     * {@link javolution.context.StackContext StackContext}).
     * 
     * The specified index indicates the first value that would be returned by
     * an initial call to the <code>next</code> method.  An initial call to
     * the <code>previous</code> method would return the value with the
     * specified index minus one.
     *
     * @param index index of first value to be returned from the
     *        list iterator (by a call to the <code>next</code> method).
     * @return a list iterator over the values in this list
     *         starting at the specified position in this list.
     * @throws IndexOutOfBoundsException if the index is out of range
     *        [code](index < 0 || index > size())[/code].
     */
    public ListIterator/*<E>*/listIterator(int index) {
        if ((index < 0) || (index > _size))
            throw new IndexOutOfBoundsException("index: " + index);
        return FastListIterator.valueOf(this, nodeAt(index), index, _size);
    }

    /**
     * Returns a view of the portion of this list between the specified
     * indexes (allocated from the "stack" when executing in a 
     * {@link javolution.context.StackContext StackContext}).
     * If the specified indexes are equal, the returned list is empty. 
     * The returned list is backed by this list, so non-structural changes in
     * the returned list are reflected in this list, and vice-versa. 
     *
     * This method eliminates the need for explicit range operations (of
     * the sort that commonly exist for arrays). Any operation that expects
     * a list can be used as a range operation by passing a subList view
     * instead of a whole list.  For example, the following idiom
     * removes a range of values from a list:
     * <code>list.subList(from, to).clear();</code>
     * Similar idioms may be constructed for <code>indexOf</code> and
     * <code>lastIndexOf</code>, and all of the algorithms in the
     * <code>Collections</code> class can be applied to a subList.
     *
     * The semantics of the list returned by this method become undefined if
     * the backing list (i.e., this list) is <i>structurally modified</i> in
     * any way other than via the returned list (structural modifications are
     * those that change the size of this list, or otherwise perturb it in such
     * a fashion that iterations in progress may yield incorrect results).
     *
     * @param fromIndex low endpoint (inclusive) of the subList.
     * @param toIndex high endpoint (exclusive) of the subList.
     * @return a view of the specified range within this list.
     * 
     * @throws IndexOutOfBoundsException if [code](fromIndex < 0 ||
     *          toIndex > size || fromIndex < toIndex)[/code]
     */
    public final List/*<E>*/subList(int fromIndex, int toIndex) {
        if ((fromIndex < 0) || (toIndex > _size) || (fromIndex > toIndex))
            throw new IndexOutOfBoundsException("fromIndex: " + fromIndex
                    + ", toIndex: " + toIndex + " for list of size: " + _size);
        return SubList.valueOf(this, nodeAt(fromIndex)._previous,
                nodeAt(toIndex), toIndex - fromIndex);
    }

    /**
     * Returns the first value of this list.
     *
     * @return this list's first value.
     * @throws NoSuchElementException if this list is empty.
     */
    public final Object/*{E}*/getFirst() {
        final Node/*<E>*/node = _head._next;
        if (node == _tail)
            throw new NoSuchElementException();
        return node._value;
    }

    /**
     * Returns the last value of this list.
     *
     * @return this list's last value.
     * @throws NoSuchElementException if this list is empty.
     */
    public final Object/*{E}*/getLast() {
        final Node/*<E>*/node = _tail._previous;
        if (node == _head)
            throw new NoSuchElementException();
        return node._value;
    }

    /**
     * Inserts the specified value at the beginning of this list.
     * 
     * @param value the value to be inserted.
     */
    public final void addFirst(Object/*{E}*/value) {
        addBefore(_head._next, value);
    }

    /**
     * Appends the specified value to the end of this list <i>(fast)</i>.
     * 
     * @param value the value to be inserted.
     */
    public void addLast(Object/*{E}*/value) { // Optimized.
        if (_tail._next == null) {
            increaseCapacity();
        }
        _tail._value = value;
        _tail = _tail._next;
        _size += ONE_VOLATILE; // Done last.
    }

    /**
     * Removes and returns the first value of this list.
     *
     * @return this list's first value before this call.
     * @throws NoSuchElementException if this list is empty.
     */
    public final Object/*{E}*/removeFirst() {
        final Node/*<E>*/first = _head._next;
        if (first == _tail)
            throw new NoSuchElementException();
        Object/*{E}*/previousValue = first._value;
        delete(first);
        return previousValue;
    }

    /**
     * Removes and returns the last value of this list <i>(fast)</i>.
     *
     * @return this list's last value before this call.
     * @throws NoSuchElementException if this list is empty.
     */
    public final Object/*{E}*/removeLast() {
        if (_size == 0)
            throw new NoSuchElementException();
        _size -= ONE_VOLATILE; // Done first.
        final Node/*<E>*/last = _tail._previous;
        final Object/*{E}*/previousValue = last._value;
        _tail = last;
        last._value = null;
        return previousValue;
    }

    ///////////////////////
    // Nodes operations. //
    ///////////////////////

    /**
     * Inserts the specified value before the specified Node.
     * 
     * @param next the Node before which this value is inserted.
     * @param value the value to be inserted.   
     */
    public final void addBefore(Node/*<E>*/next, Object/*{E}*/value) {
        if (_tail._next == null) {
            increaseCapacity();// Increases capacity.
        }
        final Node newNode = _tail._next;
        // Detaches newNode.
        final Node tailNext = _tail._next = newNode._next;
        if (tailNext != null) {
            tailNext._previous = _tail;
        }
        // Inserts before next.
        final Node previous = next._previous;
        previous._next = newNode;
        next._previous = newNode;
        newNode._next = next;
        newNode._previous = previous;

        newNode._value = value;
        _size += ONE_VOLATILE; // Done last.
    }

    /**
     * Returns the node at the specified index. This method returns
     * the {@link #headNode} node when [code]index < 0 [/code] or 
     * the {@link #tailNode} node when [code]index >= size()[/code].
     * 
     * @param index the index of the Node to return.
     */
    private final Node/*<E>*/nodeAt(int index) {
        // We cannot do backward search because of thread-safety.
        Node/*<E>*/node = _head;
        for (int i = index; i-- >= 0;) {
            node = node._next;
        }
        return node;
    }

    // Implements FastCollection abstract method.
    public final Record/*Node<E>*/head() {
        return _head;
    }

    // Implements FastCollection abstract method.
    public final Record/*Node<E>*/tail() {
        return _tail;
    }

    // Implements FastCollection abstract method.
    public final Object/*{E}*/valueOf(Record record) {
        return ((Node/*<E>*/) record)._value;
    }

    // Implements FastCollection abstract method.
    public final void delete(Record record) {
        Node/*<E>*/node = (Node/*<E>*/) record;
        _size -= ONE_VOLATILE; // Done first.
        node._value = null;
        // Detaches.
        node._previous._next = node._next;
        node._next._previous = node._previous;
        // Inserts after _tail.
        final Node/*<E>*/next = _tail._next;
        node._previous = _tail;
        node._next = next;
        _tail._next = node;
        if (next != null) {
            next._previous = node;
        }
    }

    // Overrides (optimization).
    public final boolean contains(Object value) {
        return indexOf(value) >= 0;
    }

    ///////////////////////
    // Contract Methods. //
    ///////////////////////

    // Implements abstract method.
    public final int size() {
        return _size;
    }

    // Overrides (optimization).
    public final void clear() {
        _size = ONE_VOLATILE - 1; // Done first.
        for (Node/*<E>*/n = _head, end = _tail; (n = n._next) != end;) {
            n._value = null;
        }
        _tail = _head._next;
    }

    // Implements Reusable interface.
    public void reset() {
        this.clear();
        _valueComparator = FastComparator.DEFAULT;
    }

    /**
     * Sets the comparator to use for value equality.
     *
     * @param comparator the value comparator.
     * @return <code>this</code>
     */
    public FastList/*<E>*/setValueComparator(
            FastComparator/*<? super E>*/comparator) {
        _valueComparator = comparator;
        return this;
    }

    // Overrides.
    public FastComparator/*<? super E>*/getValueComparator() {
        return _valueComparator;
    }

    // Overrides  to return a list (JDK1.5+).
    public Collection/*List<E>*/unmodifiable() {
        return (Collection/*List<E>*/) super.unmodifiable();
    }

    /**
     * Returns a new node for this list; this method can be overriden by 
     * custom list implementation. 
     *
     * @return a new node.
     */
    protected Node/*<E>*/newNode() {
        return new Node();
    }

    // Requires special handling during de-serialization process.
    private void readObject(ObjectInputStream stream) throws IOException,
            ClassNotFoundException {

        // Initial setup.
        _head = new Node();
        _tail = new Node();
        _head._next = _tail;
        _tail._previous = _head;

        setValueComparator((FastComparator) stream.readObject());
        final int size = stream.readInt();
        for (int i = size; i-- != 0;) {
            addLast((Object/*{E}*/) stream.readObject());
        }
    }

    // Requires special handling during serialization process.
    private void writeObject(ObjectOutputStream stream) throws IOException {
        stream.writeObject(getValueComparator());
        stream.writeInt(_size);
        Node node = _head;
        for (int i = _size; i-- != 0;) {
            node = node._next;
            stream.writeObject(node._value);
        }
    }

    // Increases capacity (_tail._next == null)
    private void increaseCapacity() {
        MemoryArea.getMemoryArea(this).executeInArea(new Runnable() {
            public void run() {
                Node/*<E>*/newNode0 = newNode();
                _tail._next = newNode0;
                newNode0._previous = _tail;

                Node/*<E>*/newNode1 = newNode();
                newNode0._next = newNode1;
                newNode1._previous = newNode0;

                Node/*<E>*/newNode2 = newNode();
                newNode1._next = newNode2;
                newNode2._previous = newNode1;

                Node/*<E>*/newNode3 = newNode();
                newNode2._next = newNode3;
                newNode3._previous = newNode2;
            }
        });
    }

    /**
     * This class represents a {@link FastList} node; it allows for direct 
     * iteration over the list {@link #getValue values}.
     * Custom {@link FastList} may use a derived implementation.
     * For example:[code]
     *    static class MyList<E,X> extends FastList<E> {
     *        protected MyNode newNode() {
     *            return new MyNode();
     *        }
     *        class MyNode extends Node<E> {
     *            X xxx; // Additional node field (e.g. cross references).
     *        }        
     *    }[/code]
     */
    public static class Node/*<E>*/implements Record, Serializable {

        /**
         * Holds the next node.
         */
        private Node/*<E>*/_next;

        /**
         * Holds the previous node.
         */
        private Node/*<E>*/_previous;

        /**
         * Holds the node value.
         */
        private Object/*{E}*/_value;

        /**
         * Default constructor.
         */
        protected Node() {
        }

        /**
         * Returns the value for this node.
         * 
         * @return the node value.
         */
        public final Object/*{E}*/getValue() {
            return _value;
        }

        // Implements Record interface.
        public final Record/*Node<E>*/getNext() {
            return _next;
        }

        // Implements Record interface.
        public final Record/*Node<E>*/getPrevious() {
            return _previous;
        }

    }

    /**
     * This inner class implements a sub-list.
     */
    private static final class SubList extends FastCollection implements List,
            Serializable {

        private static final ObjectFactory FACTORY = new ObjectFactory() {
            protected Object create() {
                return new SubList();
            }

            protected void cleanup(Object obj) {
                SubList sl = (SubList) obj;
                sl._list = null;
                sl._head = null;
                sl._tail = null;
            }
        };

        private FastList _list;

        private Node _head;

        private Node _tail;

        private int _size;

        public static SubList valueOf(FastList list, Node head, Node tail,
                int size) {
            SubList subList = (SubList) FACTORY.object();
            subList._list = list;
            subList._head = head;
            subList._tail = tail;
            subList._size = size;
            return subList;
        }

        public int size() {
            return _size;
        }

        public Record head() {
            return _head;
        }

        public Record tail() {
            return _tail;
        }

        public Object valueOf(Record record) {
            return _list.valueOf(record);
        }

        public void delete(Record record) {
            _list.delete(record);
        }

        public boolean addAll(int index, Collection values) {
            if ((index < 0) || (index > _size))
                throw new IndexOutOfBoundsException("index: " + index);
            final Node indexNode = nodeAt(index);
            Iterator i = values.iterator();
            while (i.hasNext()) {
                _list.addBefore(indexNode, i.next());
            }
            return values.size() != 0;
        }

        public Object get(int index) {
            if ((index < 0) || (index >= _size))
                throw new IndexOutOfBoundsException("index: " + index);
            return nodeAt(index)._value;
        }

        public Object set(int index, Object value) {
            if ((index < 0) || (index >= _size))
                throw new IndexOutOfBoundsException("index: " + index);
            final Node node = nodeAt(index);
            Object previousValue = node._value;
            node._value = value;
            return previousValue;
        }

        public void add(int index, Object element) {
            if ((index < 0) || (index > _size))
                throw new IndexOutOfBoundsException("index: " + index);
            _list.addBefore(nodeAt(index), element);
        }

        public Object remove(int index) {
            if ((index < 0) || (index >= _size))
                throw new IndexOutOfBoundsException("index: " + index);
            final Node node = nodeAt(index);
            Object previousValue = node._value;
            _list.delete(node);
            return previousValue;
        }

        public int indexOf(Object value) {
            final FastComparator comp = _list.getValueComparator();
            int index = 0;
            for (Node n = _head, end = _tail; (n = n._next) != end; index++) {
                if (comp.areEqual(value, n._value))
                    return index;
            }
            return -1;
        }

        public int lastIndexOf(Object value) {
            final FastComparator comp = this.getValueComparator();
            int index = size() - 1;
            for (Node n = _tail, end = _head; (n = n._previous) != end; index--) {
                if (comp.areEqual(value, n._value)) {
                    return index;
                }
            }
            return -1;
        }

        public ListIterator listIterator() {
            return listIterator(0);
        }

        public ListIterator listIterator(int index) {
            if ((index >= 0) && (index <= _size)) {
                return FastListIterator.valueOf(_list, nodeAt(index), index,
                        _size);
            } else {
                throw new IndexOutOfBoundsException("index: " + index
                        + " for list of size: " + _size);
            }
        }

        public List subList(int fromIndex, int toIndex) {
            if ((fromIndex < 0) || (toIndex > _size) || (fromIndex > toIndex))
                throw new IndexOutOfBoundsException("fromIndex: " + fromIndex
                        + ", toIndex: " + toIndex + " for list of size: "
                        + _size);
            SubList subList = SubList.valueOf(_list,
                    nodeAt(fromIndex)._previous, nodeAt(toIndex), toIndex
                            - fromIndex);
            return subList;
        }

        private final Node nodeAt(int index) {
            if (index <= (_size >> 1)) { // Forward search.
                Node node = _head;
                for (int i = index; i-- >= 0;) {
                    node = node._next;
                }
                return node;
            } else { // Backward search.
                Node node = _tail;
                for (int i = _size - index; i-- > 0;) {
                    node = node._previous;
                }
                return node;
            }
        }
    }

    /**
     * This inner class implements a fast list iterator.
     */
    private static final class FastListIterator implements ListIterator {

        private static final ObjectFactory FACTORY = new ObjectFactory() {
            protected Object create() {
                return new FastListIterator();
            }

            protected void cleanup(Object obj) {
                FastListIterator i = (FastListIterator) obj;
                i._list = null;
                i._currentNode = null;
                i._nextNode = null;
            }
        };

        private FastList _list;

        private Node _nextNode;

        private Node _currentNode;

        private int _length;

        private int _nextIndex;

        public static FastListIterator valueOf(FastList list, Node nextNode,
                int nextIndex, int size) {
            FastListIterator itr = (FastListIterator) FACTORY.object();
            itr._list = list;
            itr._nextNode = nextNode;
            itr._nextIndex = nextIndex;
            itr._length = size;
            return itr;
        }

        public boolean hasNext() {
            return (_nextIndex != _length);
        }

        public Object next() {
            if (_nextIndex == _length)
                throw new NoSuchElementException();
            _nextIndex++;
            _currentNode = _nextNode;
            _nextNode = _nextNode._next;
            return _currentNode._value;
        }

        public int nextIndex() {
            return _nextIndex;
        }

        public boolean hasPrevious() {
            return _nextIndex != 0;
        }

        public Object previous() {
            if (_nextIndex == 0)
                throw new NoSuchElementException();
            _nextIndex--;
            _currentNode = _nextNode = _nextNode._previous;
            return _currentNode._value;
        }

        public int previousIndex() {
            return _nextIndex - 1;
        }

        public void add(Object o) {
            _list.addBefore(_nextNode, o);
            _currentNode = null;
            _length++;
            _nextIndex++;
        }

        public void set(Object o) {
            if (_currentNode == null)
                throw new IllegalStateException();
            _currentNode._value = o;
        }

        public void remove() {
            if (_currentNode == null)
                throw new IllegalStateException();
            if (_nextNode == _currentNode) { // previous() has been called.
                _nextNode = _nextNode._next;
            } else {
                _nextIndex--;
            }
            _list.delete(_currentNode);
            _currentNode = null;
            _length--;
        }
    }

    // For inlining of default comparator. 
    private static boolean defaultEquals(Object o1, Object o2) {
        return (o1 == null) ? (o2 == null) : (o1 == o2) || o1.equals(o2);
    }

    static volatile int ONE_VOLATILE = 1; // To prevent reordering.

    private static final long serialVersionUID = 1L;

}
