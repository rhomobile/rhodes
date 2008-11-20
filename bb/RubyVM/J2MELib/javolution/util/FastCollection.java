/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.util;

import j2me.util.Collection;
import j2me.util.Iterator;
import j2me.util.List;
import j2me.util.ListIterator;
import j2me.util.Set;
import j2me.lang.UnsupportedOperationException;
import j2mex.realtime.MemoryArea;
import javolution.lang.Realtime;
import javolution.text.Text;
//import javolution.xml.XMLSerializable;

/**
 * <p> This class represents collections which can quickly be iterated over 
 *     (forward or backward) in a thread-safe manner without creating new 
 *     objects and without using {@link #iterator iterators} . For example:[code]
 *     boolean search(Object item, FastCollection c) {
 *         for (Record r = c.head(), end = c.tail(); (r = r.getNext()) != end;) {
 *              if (item.equals(c.valueOf(r))) return true;
 *         }
 *         return false;
 *     }[/code]</p>
 *     
 * <p> Iterations are thread-safe as long as the {@link Record record} sequence
 *     iterated over is not structurally modified by another thread 
 *     (objects can safely be append/prepend during iterations but not 
 *     inserted/removed).</p>
 *     
 * <p> Users may provide a read-only view of any {@link FastCollection} 
 *     instance using the {@link #unmodifiable()} method (the view is 
 *     thread-safe if iterations are thread-safe). For example:[code]
 *     public class Polynomial {
 *         private final FastTable<Coefficient> _coefficients = new FastTable<Coefficient>();
 *         public List<Coefficient> getCoefficients() { // Read-only view. 
 *             return _coefficients.unmodifiable();
 *         }
 *     }[/code]</p>
 *     
 * <p> Finally, {@link FastCollection} may use custom {@link #getValueComparator
 *     comparators} for element equality or ordering if the collection is 
 *     ordered (e.g. <code>FastTree</code>).
 *     
 * @author <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 4.2, December 18, 2006
 */
public abstract class FastCollection/*<E>*/ implements
        Collection/*<E>*/, /*XMLSerializable,*/ Realtime {

    /**
     * Holds the unmodifiable view (allocated in the same memory area as 
     * this collection).  
     */
    private Unmodifiable _unmodifiable;

    /**
     * Default constructor.  
     */
    protected FastCollection() {
    }

    /**
     * Returns the number of values in this collection. 
     *
     * @return the number of values.
     */
    public abstract int size();

    /**
     * Returns the head record of this collection; it is the record such as 
     * <code>head().getNext()</code> holds the first collection value.
     * 
     * @return the head record.
     */
    public abstract Record head();

    /**
     * Returns the tail record of this collection; it is the record such as
     * <code>tail().getPrevious()</code> holds the last collection value.
     * 
     * @return the tail record.
     */
    public abstract Record tail();

    /**
     * Returns the collection value for the specified record.
     *
     * @param record the record whose current value is returned.
     * @return the current value.
     */
    public abstract Object/*{E}*/valueOf(Record record);

    /**
     * Deletes the specified record from this collection.
     * 
     * <p> Implementation must ensure that removing a record from the 
     *     collection does not affect in any way the records preceding 
     *     the record being removed (it might affect the next records though,
     *     e.g. in a list collection, the indices of the subsequent records
     *     will change).</p>   
     *
     * @param record the record to be removed.
     * @throws UnsupportedOperationException if not supported.
     */
    public abstract void delete(Record record);

    /**
     * Returns the unmodifiable view associated to this collection. 
     * Attempts to modify the returned collection result in an 
     * {@link UnsupportedOperationException} being thrown. The view is 
     * typically part of the collection itself (created only once)
     * and also an instance of {@link FastCollection} supporting direct
     * iterations.  
     * 
     * @return the unmodifiable view over this collection.
     */
    public Collection/*<E>*/unmodifiable() {
        if (_unmodifiable == null) {
            MemoryArea.getMemoryArea(this).executeInArea(new Runnable() {
                public void run() {
                    _unmodifiable = new Unmodifiable();
                }
            });
        }
        return _unmodifiable;
    }

    /**
     * Returns an iterator over the elements in this collection 
     * (allocated on the stack when executed in a 
     * {@link javolution.context.StackContext StackContext}).
     *
     * @return an iterator over this collection's elements.
     */
    public Iterator/*<E>*/iterator() {
        return FastIterator.valueOf(this);
    }

    /**
     * Returns the value comparator for this collection (default 
     * {@link FastComparator#DEFAULT}).
     *
     * @return the comparator to use for value equality (or ordering if 
     *        the collection is ordered)
     */
    public FastComparator/*<? super E>*/ getValueComparator() {
        return FastComparator.DEFAULT;
    }

    /**
     * Appends the specified value to the end of this collection
     * (optional operation).
     * 
     * <p>Note: This default implementation always throws 
     *          <code>UnsupportedOperationException</code>.</p>
     * 
     * @param value the value to be appended to this collection.
     * @return <code>true</code> (as per the general contract of the
     *         <code>Collection.add</code> method).
     * @throws UnsupportedOperationException if not supported.
     */
    public boolean add(Object/*{E}*/value) {
        throw new UnsupportedOperationException();
    }

    /**
     * Removes the first occurrence in this collection of the specified value
     * (optional operation).
     *
     * @param value the value to be removed from this collection.
     * @return <code>true</code> if this collection contained the specified
     *         value; <code>false</code> otherwise.
     * @throws UnsupportedOperationException if not supported.
     */
    public boolean remove(Object value) {
        final FastComparator valueComp = this.getValueComparator();
        for (Record r = head(), end = tail(); (r = r.getNext()) != end;) {
            if (valueComp.areEqual(value, valueOf(r))) {
                delete(r);
                return true;
            }
        }
        return false;
    }

    /**
     * Removes all of the values from this collection (optional operation).
     *
     * @throws UnsupportedOperationException if not supported.
     */
    public void clear() {
        // Removes last record until empty.
        for (Record head = head(), r = tail().getPrevious(); r != head; r = r
                .getPrevious()) {
            delete(r);
        }
    }

    /**
     * Indicates if this collection is empty.
     *
     * @return <code>true</code> if this collection contains no value;
     *         <code>false</code> otherwise.
     */
    public final boolean isEmpty() {
        return size() == 0;
    }

    /**
     * Indicates if this collection contains the specified value.
     *
     * @param value the value whose presence in this collection 
     *        is to be tested.
     * @return <code>true</code> if this collection contains the specified
     *         value;<code>false</code> otherwise.
     */
    public boolean contains(Object value) {
        final FastComparator valueComp = this.getValueComparator();
        for (Record r = head(), end = tail(); (r = r.getNext()) != end;) {
            if (valueComp.areEqual(value, valueOf(r)))
                return true;
        }
        return false;
    }

    /**
     * Appends all of the values in the specified collection to the end of
     * this collection, in the order that they are returned by the specified
     * collection's iterator or the node order if the specified collection
     * is a {@link FastCollection}.
     *
     * @param c collection whose values are to be added to this collection.
     * @return <code>true</code> if this collection changed as a result of 
     *         the call; <code>false</code> otherwise.
     */
    public boolean addAll(Collection/*<? extends E>*/c) {
        if (c instanceof FastCollection)
            return addAll((FastCollection) c);
        boolean modified = false;
        Iterator/*<? extends E>*/itr = c.iterator();
        int pos = c.size();
        while (--pos >= 0) {
            if (add(itr.next())) {
                modified = true;
            }
        }
        return modified;
    }

    private boolean addAll(FastCollection/*<? extends E>*/c) {
        if (c instanceof FastTable)
            return addAll((FastTable) c);
        boolean modified = false;
        for (Record r = c.head(), end = c.tail(); (r = r.getNext()) != end;) {
            if (this.add(c.valueOf(r))) {
                modified = true;
            }
        }
        return modified;
    }

    private boolean addAll(FastTable/*<? extends E>*/c) {
        boolean modified = false;
        for (int i=0, n = c.size(); i < n;) { // Faster than direct iterators.
            if (this.add(c.get(i++))) {
                modified = true;
            }
        }
        return modified;
    }

    /**
     * Indicates if this collection contains all of the values of the
     * specified collection.
     *
     * @param  c collection to be checked for containment in this collection.
     * @return <code>true</code> if this collection contains all of the values
     *         of the specified collection; <code>false</code> otherwise.
     */
    public boolean containsAll(Collection/*<?>*/c) {
        if (c instanceof FastCollection)
            return containsAll((FastCollection) c);
        Iterator/*<?>*/itr = c.iterator();
        int pos = c.size();
        while (--pos >= 0) {
            if (!contains(itr.next())) {
                return false;
            }
        }
        return true;
    }

    private boolean containsAll(FastCollection/*<?>*/c) {
        for (Record r = c.head(), end = c.tail(); (r = r.getNext()) != end;) {
            if (!contains(c.valueOf(r))) {
                return false;
            }
        }
        return true;
    }

    /**
     * Removes from this collection all the values that are contained in the
     * specified collection.
     *
     * @param c collection that defines which values will be removed from
     *          this collection.
     * @return <code>true</code> if this collection changed as a result of 
     *         the call; <code>false</code> otherwise.
     */
    public boolean removeAll(Collection/*<?>*/c) {
        boolean modified = false;
        // Iterates from the tail and removes the record if present in c. 
        for (Record head = head(), r = tail().getPrevious(), previous; r != head; r = previous) {
            previous = r.getPrevious(); // Saves previous.
            if (c.contains(valueOf(r))) {
                delete(r);
                modified = true;
            }
        }
        return modified;
    }

    /**
     * Retains only the values in this collection that are contained in the
     * specified collection.
     *
     * @param c collection that defines which values this set will retain.
     * @return <code>true</code> if this collection changed as a result of 
     *         the call; <code>false</code> otherwise.
     */
    public boolean retainAll(Collection/*<?>*/c) {
        boolean modified = false;
        // Iterates from the tail and remove the record if not present in c. 
        for (Record head = head(), r = tail().getPrevious(), previous; r != head; r = previous) {
            previous = r.getPrevious(); // Saves previous.
            if (!c.contains(valueOf(r))) {
                delete(r);
                modified = true;
            }
        }
        return modified;
    }

    /**
     * Returns a new array allocated on the heap containing all of the values 
     * in this collection in proper sequence.
     * <p> Note: To avoid heap allocation {@link #toArray(Object[])} is 
     *           recommended.</p> 
     * @return <code>toArray(new Object[size()])</code>
     */
    public Object[] toArray() {
        return toArray(new Object[size()]);
    }

    /**
     * Fills the specified array with the values of this collection in 
     * the proper sequence.
     *  
     * <p> Note: Unlike standard Collection, this method does not try to resize
     *           the array using reflection (which might not be supported) if 
     *           the array is too small. UnsupportedOperationException is raised 
     *           if the specified array is too small for this collection.</p>
     *
     * @param  array the array into which the values of this collection
     *         are to be stored.
     * @return the specified array.
     * @throws UnsupportedOperationException if <code>array.length < size()</code> 
     */
    public Object/*{<T> T}*/[] toArray(Object/*{T}*/[] array) {
        int size = size();
        if (array.length < size)
            throw new UnsupportedOperationException(
                    "Destination array too small");
        if (array.length > size) {
            array[size] = null; // As per Collection contract.
        }
        int i = 0;
        Object[] arrayView = array;
        for (Record r = head(), end = tail(); (r = r.getNext()) != end;) {
            arrayView[i++] = valueOf(r);
        }
        return array;
    }

    /**
     * Returns the textual representation of this collection.
     * 
     * @return this collection textual representation.
     */
    public Text toText() {
        Text text = Text.valueOf("[");
        for (Record r = head(), end = tail(); (r = r.getNext()) != end;) {
            text = text.plus(valueOf(r));
            if (r.getNext() != end) {
                text = text.plus(", ");
            }
        }
        return text.plus("]");
    }

    /**
     * Returns the <code>String</code> representation of this 
     * {@link FastCollection}.
     *
     * @return <code>toText().toString()</code>
     */
    public final String toString() {
        return toText().toString();
    }


    /**
     * Compares the specified object with this collection for equality.  Returns
     * <code>true</code> if and only both collection contains the same values
     * regardless of the order; unless this collection is a list instance 
     * in which case both collections must be list with the same order. 
     *
     * @param obj the object to be compared for equality with this collection.
     * @return <code>true</code> if the specified object is equal to this
     *         collection; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (this instanceof List)
            return equalsList(obj);
        return obj == this
                || (obj instanceof Collection
                        && ((Collection) obj).size() == size() && containsAll((Collection) obj));
    }

    private boolean equalsList(Object obj) {
        if (obj == this)
            return true;
        if (!(obj instanceof List)) 
            return false;
        if (obj instanceof FastCollection)
            return equalsList((FastCollection)obj);
        List that = (List) obj;
        if (this.size() != that.size())
            return false;
        Iterator thatIterator = that.iterator();
        final FastComparator comp = this.getValueComparator();
        for (Record r = head(), end = tail(); (r = r.getNext()) != end;) {
            Object o1 = valueOf(r);
            Object o2 = thatIterator.next();
            if (!comp.areEqual(o1, o2))
                return false;
        }
        return true;
    }

    private boolean equalsList(FastCollection that) {
        if (this.size() != that.size())
            return false;
        Record t = that.head();
        final FastComparator comp = this.getValueComparator();
        for (Record r = head(), end = tail(); (r = r.getNext()) != end;) {
            Object o1 = valueOf(r);
            Object o2 = that.valueOf(t = t.getNext());
            if (!comp.areEqual(o1, o2))
                return false;
        }
        return true;
    }

    /**
     * Returns the hash code for this collection (independent from the 
     * collection order; unless this collection is a list instance).
     *
     * @return the hash code for this collection.
     */
    public int hashCode() {
        if (this instanceof List)
            return hashCodeList();
        final FastComparator valueComp = this.getValueComparator();
        int hash = 0;
        for (Record r = head(), end = tail(); (r = r.getNext()) != end;) {
            hash += valueComp.hashCodeOf(valueOf(r));
        }
        return hash;
    }

    private int hashCodeList() {
        final FastComparator comp = this.getValueComparator();
        int h = 1;
        for (Record r = head(), end = tail(); (r = r.getNext()) != end;) {
            h = 31 * h + comp.hashCodeOf(valueOf(r));
        }
        return h;

    }

    /**
     * This interface represents the collection records which can directly be
     * iterated over.
     */
    public interface Record {

        /**
         * Returns the record before this one.
         * 
         * @return the previous record.
         */
        public Record getPrevious();

        /**
         * Returns the record after this one.
         * 
         * @return the next record.
         */
        public Record getNext();

    }

    /**
     * This inner class represents an unmodifiable view over the collection.
     */
    private final class Unmodifiable extends FastCollection implements
            Set, List { // Allows to be used for unmodifiable set/list view.

        // Implements abstract method.
        public int size() {
            return FastCollection.this.size();
        }

        // Implements abstract method.
        public Record head() {
            return FastCollection.this.head();
        }

        // Implements abstract method.
        public Record tail() {
            return FastCollection.this.tail();
        }

        // Implements abstract method.
        public Object valueOf(Record record) {
            return FastCollection.this.valueOf(record);
        }

        // Forwards...
        public boolean contains(Object value) {
            return (FastCollection.this).contains(value);
        }

        // Forwards...
        public boolean containsAll(Collection c) {
            return (FastCollection.this).containsAll(c);
        }

        // Forwards...
        public FastComparator getValueComparator() {
            return FastCollection.this.getValueComparator();
        }

        // Disallows...
        public boolean add(Object obj) {
            throw new UnsupportedOperationException("Unmodifiable");
        }

        // Disallows...
        public void delete(Record node) {
            throw new UnsupportedOperationException("Unmodifiable");
        }

        //////////////////////////////////////////
        // List interface supplementary methods //
        //////////////////////////////////////////
        
        public boolean addAll(int index, Collection c) {
            throw new UnsupportedOperationException("Unmodifiable");
        }

        public Object get(int index) {
            return ((List)FastCollection.this).get(index);
        }

        public Object set(int index, Object element) {
            throw new UnsupportedOperationException("Unmodifiable");
        }

        public void add(int index, Object element) {
            throw new UnsupportedOperationException("Unmodifiable");
        }

        public Object remove(int index) {
            throw new UnsupportedOperationException("Unmodifiable");
        }

        public int indexOf(Object o) {
            return ((List)FastCollection.this).indexOf(o);
        }

        public int lastIndexOf(Object o) {
            return ((List)FastCollection.this).lastIndexOf(o);
        }

        public ListIterator listIterator() {
            throw new UnsupportedOperationException(
            "List iterator not supported for unmodifiable collection");
        }

        public ListIterator listIterator(int index) {
            throw new UnsupportedOperationException(
            "List iterator not supported for unmodifiable collection");
        }

        public List subList(int fromIndex, int toIndex) {
            throw new UnsupportedOperationException(
                    "Sub-List not supported for unmodifiable collection");
        }
    }
}
