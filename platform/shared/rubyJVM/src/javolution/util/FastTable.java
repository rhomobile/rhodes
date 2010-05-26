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
import java.util.NoSuchElementException;

import j2me.io.ObjectInputStream;
import j2me.io.ObjectOutputStream;
import j2me.lang.IllegalStateException;
import j2me.lang.UnsupportedOperationException;
import j2me.util.Collection;
import j2me.util.Iterator;
import j2me.util.List;
import j2me.util.ListIterator;
import j2me.util.RandomAccess;
import j2mex.realtime.MemoryArea;
import javolution.context.ObjectFactory;
import javolution.context.PersistentContext;
import javolution.lang.MathLib;
import javolution.lang.Reusable;

/**
 * <p> This class represents a random access collection with real-time behavior 
 *     (smooth capacity increase).</p>
 *     <img src="doc-files/list-add.png"/>
 *     
 * <p> This class has the following advantages over the widely used 
 *     <code>java.util.ArrayList</code>:<ul>
 *     <li> No large array allocation (for large collections multi-dimensional
 *          arrays are employed). The garbage collector is not stressed with
 *          large chunk of memory to allocate (likely to trigger a
 *          full garbage collection due to memory fragmentation).</li>
 *     <li> Support concurrent access/iteration without synchronization if the 
 *          collection values are not removed/inserted (Ref. 
 *          {@link javolution.util} discussion).</li>
 *     </ul></p>
 *     
 *  <p> Iterations over the {@link FastTable} values are faster when
 *      performed using the {@link #get} method rather than using collection
 *      records or iterators:[code]
 *     for (int i = 0, n = table.size(); i < n; i++) {
 *          table.get(i);
 *     }[/code]</p>
 *     
 *  <p> {@link FastTable} supports {@link #sort sorting} in place (quick sort) 
 *      using the {@link FastCollection#getValueComparator() value comparator}
 *      for the table (no object or array allocation when sorting).</p>
 *       
 *  <p> The size of a {@link FastTable} can be {@link #setSize set} directly
 *      and populated concurrently through the {@link #set(int, Object)} 
 *      method (e.g. table shared by multiple threads each working on 
 *      different index ranges).</p> 
 * 
 * @author <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.2, August 20, 2007
 */
public class FastTable/*<E>*/extends FastCollection/*<E>*/implements
        List/*<E>*/, Reusable, RandomAccess {

    /**
     * Holds the factory for this fast table.
     */
    private static final ObjectFactory FACTORY = new ObjectFactory() {
        public Object create() {
            return new FastTable();
        }

        public void cleanup(Object obj) {
            ((FastTable) obj).reset();
        }
    };

    // We do a full resize (and copy) only when the capacity is less than C1.
    // For large collections, multi-dimensional arrays are employed.

    private static final int B0 = 4; // Initial capacity in bits.

    private static final int C0 = 1 << B0; // Initial capacity (16)

    private static final int B1 = 10; // Low array maximum capacity in bits.

    private static final int C1 = 1 << B1; // Low array maximum capacity (1024).

    private static final int M1 = C1 - 1; // Mask.

    // Resizes up to 1024 maximum (16, 32, 64, 128, 256, 512, 1024). 
    private transient Object/*{E}*/[] _low;

    // For larger capacity use multi-dimensional array.
    private transient Object/*{E}*/[][] _high;

    /**
     * Holds the current capacity. 
     */
    private transient int _capacity;

    /**
     * Holds the current size.
     */
    private transient int _size;

    /**
     * Holds the value comparator.
     */
    private transient FastComparator/*<? super E>*/_valueComparator = FastComparator.DEFAULT;

    /**
     * Creates a table of small initial capacity.
     */
    public FastTable() {
        _capacity = C0;
        _low = (Object/*{E}*/[]) new Object[C0];
        _high = (Object/*{E}*/[][]) new Object[1][];
        _high[0] = _low;
    }

    /**
     * Creates a persistent table associated to the specified unique identifier
     * (convenience method).
     * 
     * @param id the unique identifier for this map.
     * @throws IllegalArgumentException if the identifier is not unique.
     * @see javolution.context.PersistentContext.Reference
     */
    public FastTable(String id) {
        this();
        new PersistentContext.Reference(id, this) {
            protected void notifyChange() {
                FastTable.this.clear();
                FastTable.this.addAll((FastList) this.get());
            }
        };
    }

    /**
     * Creates a table of specified initial capacity; unless the table size 
     * reaches the specified capacity, operations on this table will not 
     * allocate memory (no lazy object creation).
     * 
     * @param capacity the initial capacity.
     */
    public FastTable(int capacity) {
        this();
        while (capacity > _capacity) {
            increaseCapacity();
        }
    }

    /**
     * Creates a table containing the specified values, in the order they
     * are returned by the collection's iterator.
     *
     * @param values the values to be placed into this table.
     */
    public FastTable(Collection/*<? extends E>*/values) {
        this(values.size());
        addAll(values);
    }

    /**
     * Returns a new, preallocated or {@link #recycle recycled} table instance
     * (on the stack when executing in a {@link javolution.context.StackContext
     * StackContext}).
     *
     * @return a new, preallocated or recycled table instance.
     */
    public static/*<E>*/FastTable/*<E>*/newInstance() {
        return (FastTable/*<E>*/) FACTORY.object();
    }

    /**
     * Recycles a table {@link #newInstance() instance} immediately
     * (on the stack when executing in a {@link javolution.context.StackContext
     * StackContext}). 
     */
    public static void recycle(FastTable instance) {
        FACTORY.recycle(instance);
    }

    /**
     * Sets the size of this table. If the specified size is greater than
     * the current size then <code>null</code> elements are added; otherwise
     * the last elements are removed until the desired size is reached.
     *
     * @param size the new size.
     */
    public void setSize(int size) {
        while (_size < size) { // Adds null elements.
            addLast(null);
        }
        while (_size > size) { // Removes last elements.
            removeLast();
        }
    }

    /**
     * Returns the element at the specified index.
     *
     * @param index index of value to return.
     * @return the value at the specified position in this list.
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *         (index >= size())</code>
     */
    public final Object/*{E}*/get(int index) { // Short to be inlined.
        if (index >= _size)
            throw new IndexOutOfBoundsException();
        return index < C1 ? _low[index] : _high[index >> B1][index & M1];
    }

    /**
     * Replaces the value at the specified position in this table with the
     * specified value.
     *
     * @param index index of value to replace.
     * @param value value to be stored at the specified position.
     * @return previous value.
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *         (index >= size())</code>
     */
    public final Object/*{E}*/set(int index, Object/*{E}*/value) {
        if (index >= _size)
            throw new IndexOutOfBoundsException();
        final Object/*{E}*/[] low = _high[index >> B1];
        final Object/*{E}*/previous = low[index & M1];
        low[index & M1] = value;
        return previous;
    }

    /**
     * Appends the specified value to the end of this table.
     *
     * @param value the value to be appended to this table.
     * @return <code>true</code> (as per the general contract of the
     *         <code>Collection.add</code> method).
     */
    public final boolean add(Object/*{E}*/value) {
        if (_size >= _capacity)
            increaseCapacity();
        _high[_size >> B1][_size & M1] = value;
        _size += ONE_VOLATILE; // Prevents compiler reordering.
        return true;
    }

    /**
     * Returns the first value of this table.
     *
     * @return this table first value.
     * @throws NoSuchElementException if this table is empty.
     */
    public final Object/*{E}*/getFirst() {
        if (_size == 0)
            throw new NoSuchElementException();
        return _low[0];
    }

    /**
     * Returns the last value of this table.
     *
     * @return this table last value.
     * @throws NoSuchElementException if this table is empty.
     */
    public final Object/*{E}*/getLast() {
        if (_size == 0)
            throw new NoSuchElementException();
        return get(_size - 1);
    }

    /**
     * Appends the specified value to the end of this table <i>(fast)</i>.
     * 
     * @param value the value to be added.
     */
    public final void addLast(Object/*{E}*/value) {
        add(value);
    }

    /**
     * Removes and returns the last value of this table <i>(fast)</i>.
     *
     * @return this table's last value before this call.
     * @throws NoSuchElementException if this table is empty.
     */
    public final Object/*{E}*/removeLast() {
        if (_size == 0)
            throw new NoSuchElementException();
        _size--; // No need for volatile, removal are not thread-safe.
        final Object/*{E}*/[] low = _high[_size >> B1];
        final Object/*{E}*/previous = low[_size & M1];
        low[_size & M1] = null;
        return previous;
    }

    // Overrides.
    public final void clear() {
        for (int i = 0; i < _size; i += C1) {
            final int count = MathLib.min(_size - i, C1);
            final Object/*{E}*/[] low = _high[i >> B1];
            System.arraycopy(NULL_BLOCK, 0, low, 0, count);
        }
        _size = 0; // No need for volatile, removal are not thread-safe.
    }

    private static final Object[] NULL_BLOCK = (Object[]) new Object[C1];

    // Implements Reusable interface.
    public void reset() {
        clear();
        this.setValueComparator(FastComparator.DEFAULT);
    }

    /**
     * Inserts all of the values in the specified collection into this
     * table at the specified position. Shifts the value currently at that
     * position (if any) and any subsequent values to the right 
     * (increases their indices). 
     * 
     * <p>Note: If this method is used concurrent access must be synchronized
     *          (the table is no more thread-safe).</p>
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
        final int shift = values.size();
        shiftRight(index, shift);
        Iterator/*<? extends E>*/valuesIterator = values.iterator();
        for (int i = index, n = index + shift; i < n; i++) {
            _high[i >> B1][i & M1] = valuesIterator.next();
        }
        _size += shift * ONE_VOLATILE; // Increases size last (thread-safe)
        return shift != 0;
    }

    /**
     * Inserts the specified value at the specified position in this table.
     * Shifts the value currently at that position
     * (if any) and any subsequent values to the right (adds one to their
     * indices).
     *
     * <p>Note: If this method is used concurrent access must be synchronized
     *          (the table is no more thread-safe).</p>
     *
     * @param index the index at which the specified value is to be inserted.
     * @param value the value to be inserted.
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *         (index > size())</code>
     */
    public final void add(int index, Object/*{E}*/value) {
        if ((index < 0) || (index > _size))
            throw new IndexOutOfBoundsException("index: " + index);
        shiftRight(index, 1);
        _high[index >> B1][index & M1] = value;
        _size += ONE_VOLATILE; // Increases size last (thread-safe).
    }

    /**
     * Removes the value at the specified position from this table.
     * Shifts any subsequent values to the left (subtracts one
     * from their indices). Returns the value that was removed from the
     * table.
     *
     * <p>Note: If this method is used concurrent access must be synchronized
     *          (the table is no more thread-safe).</p>
     *
     * @param index the index of the value to removed.
     * @return the value previously at the specified position.
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *         (index >= size())</code>
     */
    public final Object/*{E}*/remove(int index) {
        final Object/*{E}*/previous = get(index);
        shiftLeft(index + 1, 1);
        _size--; // No need for volatile, removal are not thread-safe.
        _high[_size >> B1][_size & M1] = null; // Deallocates for GC.
        return previous;
    }

    /**
     * Removes the values between <code>[fromIndex..toIndex[<code> from
     * this table.
     * 
     * <p>Note: If this method is used concurrent access must be synchronized
     *          (the table is no more thread-safe).</p>
     *
     * @param fromIndex the beginning index, inclusive.
     * @param toIndex the ending index, exclusive.
     * @throws IndexOutOfBoundsException if <code>(fromIndex < 0) || (toIndex < 0) 
     *         || (fromIndex > toIndex) || (toIndex > this.size())</code>
     */
    public final void removeRange(int fromIndex, int toIndex) {
        if ((fromIndex < 0) || (toIndex < 0) || (fromIndex > toIndex)
                || (toIndex > _size))
            throw new IndexOutOfBoundsException();
        final int shift = toIndex - fromIndex;
        shiftLeft(fromIndex, shift);
        _size -= shift; // No need for volatile, removal are not thread-safe.        
        for (int i = _size, n = _size + shift; i < n; i++) {
            _high[i >> B1][i & M1] = null; // Deallocates for GC.
        }
    }

    /**
     * Returns the index in this table of the first occurrence of the specified
     * value, or -1 if this table does not contain this value.
     *
     * @param value the value to search for.
     * @return the index in this table of the first occurrence of the specified
     *         value, or -1 if this table does not contain this value.
     */
    public final int indexOf(Object value) {
        final FastComparator comp = this.getValueComparator();
        for (int i = 0; i < _size;) {
            final Object/*{E}*/[] low = _high[i >> B1];
            final int count = MathLib.min(low.length, _size - i);
            for (int j = 0; j < count; j++) {
                if (comp == FastComparator.DEFAULT ? defaultEquals(value,
                        low[j]) : comp.areEqual(value, low[j]))
                    return i + j;
            }
            i += count;
        }
        return -1;
    }

    /**
     * Returns the index in this table of the last occurrence of the specified
     * value, or -1 if this table does not contain this value.
     *
     * @param value the value to search for.
     * @return the index in this table of the last occurrence of the specified
     *         value, or -1 if this table does not contain this value.
     */
    public final int lastIndexOf(Object value) {
        final FastComparator comp = this.getValueComparator();
        for (int i = _size - 1; i >= 0;) {
            final Object/*{E}*/[] low = _high[i >> B1];
            final int count =  (i & M1) + 1;
            for (int j = count; --j >= 0;) {
                if (comp == FastComparator.DEFAULT ? defaultEquals(value,
                        low[j]) : comp.areEqual(value, low[j]))
                    return i + j - count + 1;
            }
            i -= count;
        }
        return -1;
    }

    /**
     * Returns an iterator over the elements in this list 
     * (allocated on the stack when executed in a 
     * {@link javolution.context.StackContext StackContext}).
     *
     * @return an iterator over this list values.
     */
    public Iterator/*<E>*/iterator() {
        return FastTableIterator.valueOf(this, 0, 0, _size);
    }

    /**
     * Returns a list iterator over the elements in this list 
     * (allocated on the stack when executed in a 
     * {@link javolution.context.StackContext StackContext}).
     *
     * @return an iterator over this list values.
     */
    public ListIterator/*<E>*/listIterator() {
        return FastTableIterator.valueOf(this, 0, 0, _size);
    }

    /**
     * Returns a list iterator from the specified position
     * (allocated on the stack when executed in a 
     * {@link javolution.context.StackContext StackContext}).
     * The list iterator being returned does not support insertion/deletion.
     * 
     * @param index the index of first value to be returned from the
     *        list iterator (by a call to the <code>next</code> method).
     * @return a list iterator of the values in this table
     *         starting at the specified position in this list.
     * @throws IndexOutOfBoundsException if the index is out of range 
     *         [code](index < 0 || index > size())[/code]
     */
    public ListIterator/*<E>*/listIterator(int index) {
        if ((index < 0) || (index > _size)) throw new IndexOutOfBoundsException();
        return FastTableIterator.valueOf(this, index, 0, _size);
    }

    /**
     * Returns a view of the portion of this list between the specified
     * indexes (instance of {@link FastList} allocated from the "stack" when
     * executing in a {@link javolution.context.StackContext StackContext}).
     * If the specified indexes are equal, the returned list is empty. 
     * The returned list is backed by this list, so non-structural changes in
     * the returned list are reflected in this list, and vice-versa. 
     *
     * This method eliminates the need for explicit range operations (of
     * the sort that commonly exist for arrays). Any operation that expects
     * a list can be used as a range operation by passing a subList view
     * instead of a whole list.  For example, the following idiom
     * removes a range of values from a list: [code]
     * list.subList(from, to).clear();[/code]
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
     *          toIndex > size || fromIndex > toIndex)[/code]
     */
    public final List/*<E>*/subList(int fromIndex, int toIndex) {
        if ((fromIndex < 0) || (toIndex > _size) || (fromIndex > toIndex))
            throw new IndexOutOfBoundsException("fromIndex: " + fromIndex
                    + ", toIndex: " + toIndex + " for list of size: " + _size);
        int size = toIndex - fromIndex;
        FastTable table = new FastTable(size);
        
        for( int i = fromIndex; i < size; i++ )
        	table.add(this.get(i));
        	
        return table;
//RHO    
        //return SubTable.valueOf(this, fromIndex, toIndex - fromIndex);
//RHO        
    }

    /**
     * Reduces the capacity of this table to the current size (minimize 
     * storage space).
     */
    public final void trimToSize() {
        while (_capacity - _size > C1) {
            _capacity -= C1;
            _high[_capacity >> B1] = null;
        }
    }

    /**
     * Sorts this table in place (quick sort) using this table 
     * {@link FastCollection#getValueComparator() value comparator}
     * (smallest first).
     * 
     * @return <code>this</code>
     */
    public final FastTable/*<E>*/sort() {
        if (_size > 1) {
            quicksort(0, _size - 1, this.getValueComparator());
        }
        return this;
    }

    // From Wikipedia Quick Sort - http://en.wikipedia.org/wiki/Quicksort
    //
    private void quicksort(int first, int last, FastComparator cmp) {
        int pivIndex = 0;
        if (first < last) {
            pivIndex = partition(first, last, cmp);
            quicksort(first, (pivIndex - 1), cmp);
            quicksort((pivIndex + 1), last, cmp);
        }
    }

    private int partition(int f, int l, FastComparator cmp) {
        int up, down;
        Object/*{E}*/piv = get(f);
        up = f;
        down = l;
        do {
            while (cmp.compare(get(up), piv) <= 0 && up < l) {
                up++;
            }
            while (cmp.compare(get(down), piv) > 0 && down > f) {
                down--;
            }
            if (up < down) { // Swaps.
                Object/*{E}*/temp = get(up);
                set(up, get(down));
                set(down, temp);
            }
        } while (down > up);
        set(f, get(down));
        set(down, piv);
        return down;
    }

    /**
     * Sets the comparator to use for value equality or comparison if the 
     * collection is ordered (see {@link #sort()}).
     *
     * @param comparator the value comparator.
     * @return <code>this</code>
     */
    public FastTable/*<E>*/setValueComparator(
            FastComparator/*<? super E>*/comparator) {
        _valueComparator = comparator;
        return this;
    }

    // Overrides.
    public FastComparator/*<? super E>*/getValueComparator() {
        return _valueComparator;
    }

    // Implements FastCollection abstract method.
    public final int size() {
        return _size;
    }

    // Implements FastCollection abstract method.
    public final Record head() {
        return Index.valueOf(-1);
    }

    // Implements FastCollection abstract method.
    public final Record tail() {
        return Index.valueOf(_size);
    }

    // Implements FastCollection abstract method.
    public final Object/*{E}*/valueOf(Record record) {
        return get(((Index) record).intValue());
    }

    // Implements FastCollection abstract method.
    public final void delete(Record record) {
        remove(((Index) record).intValue());
    }

    // Overrides  to return a list (JDK1.5+).
    public Collection/*List<E>*/unmodifiable() {
        return (Collection/*List<E>*/) super.unmodifiable();
    }

    // Overrides (optimization).
    public final boolean contains(Object value) {
        return indexOf(value) >= 0;
    }

    // Requires special handling during de-serialization process.
    private void readObject(ObjectInputStream stream) throws IOException,
            ClassNotFoundException {
        setValueComparator((FastComparator) stream.readObject());
        final int size = stream.readInt();
        _capacity = C0;
        while ((_capacity < _size) && (_capacity < C1)) {
            _capacity <<= 1; // Increases capacity up to C1 to avoid resizes.
        }
        _low = (Object/*{E}*/[]) new Object[_capacity];
        _high = (Object/*{E}*/[][]) new Object[1][];
        _high[0] = _low;
        for (int i = 0; i < size; i++) {
            addLast((Object/*{E}*/) stream.readObject());
        }
    }

    // Requires special handling during serialization process.
    private void writeObject(ObjectOutputStream stream) throws IOException {
        stream.writeObject(getValueComparator());
        final int size = _size;
        stream.writeInt(size);
        for (int i = 0; i < size; i++) {
            stream.writeObject(get(i));
        }
    }

    /**
     * Returns the current capacity of this table.
     *
     * @return this table's capacity.
     */
    protected final int getCapacity() {
        return _capacity;
    }

    /**
     * Increases this table capacity.
     */
    private void increaseCapacity() {
        MemoryArea.getMemoryArea(this).executeInArea(new Runnable() {
            public void run() {
                if (_capacity < C1) { // For small capacity, resize.
                    _capacity <<= 1;
                    Object/*{E}*/[] tmp = (Object/*{E}*/[]) new Object[_capacity];
                    System.arraycopy(_low, 0, tmp, 0, _low.length);
                    _low = tmp;
                    _high[0] = tmp;
                } else { // Add a new low block of 1024 elements.
                    int j = _capacity >> B1;
                    if (j >= _high.length) { // Resizes _high.
                        Object/*{E}*/[][] tmp = (Object/*{E}*/[][]) new Object[_high.length * 2][];
                        System.arraycopy(_high, 0, tmp, 0, _high.length);
                        _high = tmp;
                    }
                    _high[j] = (Object/*{E}*/[]) new Object[C1];
                    _capacity += C1;
                }
            }
        });
    }

    /**
     * This inner class implements a sub-table.
     */
/*    
    private static final class SubTable extends FastCollection implements List,
            RandomAccess {

        private static final ObjectFactory FACTORY = new ObjectFactory() {
            protected Object create() {
                return new SubTable();
            }

            protected void cleanup(Object obj) {
                SubTable st = (SubTable) obj;
                st._table = null;
            }
        };

        private FastTable _table;

        private int _offset;

        private int _size;

        public static SubTable valueOf(FastTable table, int offset, int size) {
            SubTable subTable = (SubTable) FACTORY.object();
            subTable._table = table;
            subTable._offset = offset;
            subTable._size = size;
            return subTable;
        }

        public int size() {
            return _size;
        }

        public Record head() {
            return Index.valueOf(-1);
        }

        public Record tail() {
            return Index.valueOf(_size);
        }

        public Object valueOf(Record record) {
            return _table.get(((Index) record).intValue() + _offset);
        }

        public void delete(Record record) {
            throw new UnsupportedOperationException(
                    "Deletion not supported, thread-safe collections.");
        }

        public boolean addAll(int index, Collection values) {
            throw new UnsupportedOperationException(
                    "Insertion not supported, thread-safe collections.");
        }

        public Object get(int index) {
            if ((index < 0) || (index >= _size))
                throw new IndexOutOfBoundsException("index: " + index);
            return _table.get(index + _offset);
        }

        public Object set(int index, Object value) {
            if ((index < 0) || (index >= _size))
                throw new IndexOutOfBoundsException("index: " + index);
            return _table.set(index + _offset, value);
        }

        public void add(int index, Object element) {
            throw new UnsupportedOperationException(
                    "Insertion not supported, thread-safe collections.");
        }

        public Object remove(int index) {
            throw new UnsupportedOperationException(
                    "Deletion not supported, thread-safe collections.");
        }

        public int indexOf(Object value) {
            final FastComparator comp = _table.getValueComparator();
            for (int i = -1; ++i < _size;) {
                if (comp.areEqual(value, _table.get(i + _offset)))
                    return i;
            }
            return -1;
        }

        public int lastIndexOf(Object value) {
            final FastComparator comp = _table.getValueComparator();
            for (int i = _size; --i >= 0;) {
                if (comp.areEqual(value, _table.get(i + _offset)))
                    return i;
            }
            return -1;
        }

        public ListIterator listIterator() {
            return listIterator(0);
        }

        public ListIterator listIterator(int index) {
            if ((index >= 0) && (index <= _size)) {
                return FastTableIterator.valueOf(_table, index + _offset,
                        _offset, _offset + _size);
            } else {
                throw new IndexOutOfBoundsException("index: " + index
                        + " for table of size: " + _size);
            }
        }

        public List subList(int fromIndex, int toIndex) {
            if ((fromIndex < 0) || (toIndex > _size) || (fromIndex > toIndex))
                throw new IndexOutOfBoundsException("fromIndex: " + fromIndex
                        + ", toIndex: " + toIndex + " for list of size: "
                        + _size);
            return SubTable.valueOf(_table, _offset + fromIndex, toIndex
                    - fromIndex);
        }
    }*/

    /**
     * This inner class implements a fast table iterator.
     */
    private static final class FastTableIterator implements ListIterator {

        private static final ObjectFactory FACTORY = new ObjectFactory() {
            protected Object create() {
                return new FastTableIterator();
            }

            protected void cleanup(Object obj) {
                FastTableIterator i = (FastTableIterator) obj;
                i._table = null;
                i._low = null;
                i._high = null;
            }
        };

        private FastTable _table;

        private int _currentIndex;

        private int _start; // Inclusive.

        private int _end; // Exclusive.

        private int _nextIndex;

        private Object[] _low;
        
        private Object[][] _high;

        public static FastTableIterator valueOf(FastTable table, int nextIndex,
                int start, int end) {
            FastTableIterator iterator = (FastTableIterator) FACTORY.object();
            iterator._table = table;
            iterator._start = start;
            iterator._end = end;
            iterator._nextIndex = nextIndex;
            iterator._low = table._low;
            iterator._high = table._high;            
            iterator._currentIndex = -1;
            return iterator;
        }

        public boolean hasNext() {
            return (_nextIndex != _end);
        }

        public Object next() {
            if (_nextIndex == _end)
                throw new NoSuchElementException();
            final int i = _currentIndex = _nextIndex++;
            return i < C1 ? _low[i] : _high[i >> B1][i & M1];
        }

        public int nextIndex() {
            return _nextIndex;
        }

        public boolean hasPrevious() {
            return _nextIndex != _start;
        }

        public Object previous() {
            if (_nextIndex == _start)
                throw new NoSuchElementException();
            final int i = _currentIndex = --_nextIndex;
            return i < C1 ? _low[i] : _high[i >> B1][i & M1];
        }

        public int previousIndex() {
            return _nextIndex - 1;
        }

        public void add(Object o) {
            _table.add(_nextIndex++, o);
            _end++;
            _currentIndex = -1;
        }

        public void set(Object o) {
            if (_currentIndex >= 0) {
                _table.set(_currentIndex, o);
            } else {
                throw new IllegalStateException();
            }
        }

        public void remove() {
            if (_currentIndex >= 0) {
                _table.remove(_currentIndex);
                _end--;
                if (_currentIndex < _nextIndex) {
                    _nextIndex--;
                }
                _currentIndex = -1;
            } else {
                throw new IllegalStateException();
            }
        }
    }

    // Shifts element from the specified index to the right (higher indexes). 
    private void shiftRight(int index, int shift) {
        while (_size + shift >= _capacity) {
            increaseCapacity();
        }
        for (int i = _size; --i >= index;) {
            final int dest = i + shift;
            _high[dest >> B1][dest & M1] = _high[i >> B1][i & M1];
        }
    }

    // Shifts element from the specified index to the left (lower indexes). 
    private void shiftLeft(int index, int shift) {
        for (int i = index; i < _size; i++) {
            final int dest = i - shift;
            _high[dest >> B1][dest & M1] = _high[i >> B1][i & M1];
        }
    }

    // For inlining of default comparator. 
    private static boolean defaultEquals(Object o1, Object o2) {
        return (o1 == null) ? (o2 == null) : (o1 == o2) || o1.equals(o2);
    }

    private static final long serialVersionUID = 1L;

    static volatile int ONE_VOLATILE = 1; // To prevent reordering.
}
