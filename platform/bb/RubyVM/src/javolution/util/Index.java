/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.util;

import j2me.lang.Comparable;
import j2me.lang.Number;
import j2me.io.ObjectStreamException;
import j2mex.realtime.MemoryArea;
import javolution.lang.Configurable;
import javolution.lang.Immutable;
import javolution.util.FastCollection.Record;
//import javolution.xml.XMLSerializable;

/**
 * <p> This class represents a <b>unique</b> index which can be used instead of 
 *     <code>java.lang.Integer</code> for primitive data types collections. 
 *     For example:[code]
 *         class SparseVector<F> {
 *             FastMap<Index, F> _elements = new FastMap<Index, F>();
 *             ...
 *         }[/code]</p>
 *          
 * <p> Unicity is guaranteed and direct equality (<code>==</code>) can be used 
 *     in place of object equality (<code>Index.equals(Object)</code>).</p>
 * 
 * <p> Indices have no adverse effect on the garbage collector (persistent 
 *     instances), but should not be used for large integer values as that  
 *     would increase the permanent memory footprint significantly.</p> 
 * 
 * <p><b>RTSJ:</b> Instance of this classes are allocated in 
 *    <code>ImmortalMemory</code>. Indices can be pre-allocated at start-up
 *    to avoid run-time allocation delays by configuring 
 *    {@link #INITIAL_FIRST} and/or {@link #INITIAL_LAST} or through 
 *    {@link #setMinimumRange}.</p>
 *     
 * @author <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.1, July 26, 2007
 */
public final class Index extends Number implements 
        Comparable/*<Index>*/, Record, Immutable/*, XMLSerializable */ {

    /**
     * Holds the initial first index value (default <code>-1</code>).
     */
    public static final Configurable/*<Integer>*/ INITIAL_FIRST
        = new Configurable(new Integer(-1)) {
        protected void notifyChange() {
            // Ensures Index creation from minimum balue. 
            Index.valueOf(((Integer)INITIAL_FIRST.get()).intValue());
        }
    };
        
    /**
     * Holds the initial last index value (default <code>16</code>).
     */
    public static final Configurable/*<Integer>*/ INITIAL_LAST
        = new Configurable(new Integer(16)) {
        protected void notifyChange() {
            // Ensures Index creation to maximum value. 
            Index.valueOf(((Integer)INITIAL_LAST.get()).intValue());
        }
    };

    /**
     * Holds the index zero (value <code>0</code>).
     */
    public static final Index ZERO = new Index(0);

    /**
     * Holds positive indices (immortal memory).
     */
    private static Index[] PositiveIndices = new Index[16];
    static {
        PositiveIndices[0] = ZERO;
    }

    /**
     * Holds positive indices length.
     */
    private static int PositiveIndicesLength = 1;

    /**
     * Holds negative indices (immortal memory).
     */
    private static Index[] NegativeIndices = new Index[16];
    static {
        NegativeIndices[0] = ZERO;
    }
    
    /**
     * Holds the immortal memory area (static fields are initialized in 
     * immortal memory). 
     */
    private static final MemoryArea IMMORTAL_MEMORY = 
        MemoryArea.getMemoryArea(new Object());

    /**
     * Holds positive indices length.
     */
    private static int NegativeIndicesLength = 1;

    /**
     * Holds the index position.
     */
    private final int _value;

    /**
     * Creates an index at the specified position.
     * 
     * @param i the index position.
     */
    private Index(int i) {
        _value = i;
    }

    /**
     * Creates the indices for the specified range of values if they don't 
     * exist.
     * 
     * @param first the first index value.
     * @param last the last index value.
     * @throws IllegalArgumentException if <code>first > last</code>
     */
    public static void setMinimumRange(int first, int last) {
    	if (first > last) throw new IllegalArgumentException();
    	Index.valueOf(first);
    	Index.valueOf(last);
    }    
    
    /**
     * Returns the unique index for the specified <code>int</code> value 
     * (creating it as well as the indices toward {@link #ZERO zero} 
     *  if they do not exist). 
     * 
     * @param i the index value.
     * @return the corresponding unique index.
     */
    public static Index valueOf(int i) { // Short to be inlined.
        return (i >= 0) ? (i < Index.PositiveIndicesLength) ? PositiveIndices[i] 
            : createPositive(i) : valueOfNegative(-i);
    }    
    
    private static Index valueOfNegative(int i) {    
        return i < Index.NegativeIndicesLength ?
                    NegativeIndices[i] : createNegative(i);
    }

    private static synchronized Index createPositive(int i) {
        if (i < PositiveIndicesLength) // Synchronized check. 
            return PositiveIndices[i];
        while (i >= PositiveIndicesLength) {
            IMMORTAL_MEMORY.executeInArea(AUGMENT_POSITIVE);
        }
        return PositiveIndices[i];
    }
    
    private static synchronized Index createNegative(int i) {
            if (i < NegativeIndicesLength) // Synchronized check. 
                return NegativeIndices[i];
            while (i >= NegativeIndicesLength) {
                IMMORTAL_MEMORY.executeInArea(AUGMENT_NEGATIVE);
            }
            return NegativeIndices[i];
        
    }

    private static final Runnable AUGMENT_POSITIVE = new Runnable() {
        public void run() {
            for (int i = Index.PositiveIndicesLength, 
                     n = Index.PositiveIndicesLength + INCREASE_AMOUNT; i < n; i++) {
                
                Index index = new Index(i);
 
                if (Index.PositiveIndices.length <= i) { // Resize.
                    Index[] tmp = new Index[Index.PositiveIndices.length * 2];
                    System.arraycopy(Index.PositiveIndices, 0, tmp, 0, Index.PositiveIndices.length);
                    Index.PositiveIndices = tmp;
                }
                
                PositiveIndices[i] = index;
            }
            NoReordering = true; // Ensures instruction below is performed last. 
            PositiveIndicesLength += INCREASE_AMOUNT;
        }
    };

    private static final Runnable AUGMENT_NEGATIVE = new Runnable() {
        public void run() {
            for (int i = Index.NegativeIndicesLength, 
                     n = Index.NegativeIndicesLength + INCREASE_AMOUNT; i < n; i++) {
                
                Index index = new Index(-i);

                if (Index.NegativeIndices.length <= i) { // Resize.
                    Index[] tmp = new Index[Index.NegativeIndices.length * 2];
                    System.arraycopy(Index.NegativeIndices, 0, tmp, 0, Index.NegativeIndices.length);
                    Index.NegativeIndices = tmp;
                }
                
                NegativeIndices[i] = index;
            }
           NoReordering = true; // Ensures instruction below is performed last. 
           NegativeIndicesLength += INCREASE_AMOUNT;
        }
    };
    
    private static final int INCREASE_AMOUNT = 16;

    static volatile boolean NoReordering; 
    

    /**
     * Returns the index value as <code>int</code>.
     * 
     * @return the index value.
     */
    public final int intValue() {
        return _value;
    }

    /**
     * Returns the index value as <code>long</code>.
     * 
     * @return the index value.
     */
    public final long longValue() {
        return intValue();
    }
    
    /**
     * Returns the index value as <code>float</code>.
     * 
     * @return the index value.
     *@JVM-1.1+@
    public final float floatValue() {
        return (float) intValue();
    }
    /**/

    /**
     * Returns the index value as <code>int</code>.
     * 
     * @return the index value.
     *@JVM-1.1+@
    public final double doubleValue() {
        return (double) intValue();
    }
    /**/


    /**
     * Returns the <code>String</code> representation of this index.
     * 
     * @return this index value formatted as a string.
     */
    public final String toString() {
        return String.valueOf(_value);
    }

    /**
     * Indicates if this index is equals to the one specified (unicity 
     * ensures that this method is equivalent to <code>==</code>).
     * 
     * @return <code>this == obj</code>
     */
    public final boolean equals(Object obj) {
        return this == obj;
    }

    /**
     * Returns the hash code for this index.
     *
     * @return the index value.
     */
    public final int hashCode() {
        return _value;
    }

    /**
     * Ensures index unicity during deserialization.
     * 
     * @return the unique instance for this deserialized index.
     */
    protected final Object readResolve() throws ObjectStreamException {
        return Index.valueOf(_value);
    }    

    //  Implements Comparable interface.
    public int compareTo(Object/*{Index}*/ that) {
        return this._value - ((Index)that)._value;
    }

    // Implements Record interface.
    public final Record getNext() {
        return Index.valueOf(_value + 1);
    }

    // Implements Record interface.
    public final Record getPrevious() {
        return Index.valueOf(_value - 1);
    }    

    // Start-up initialization.
    static {
        // Ensures initial instances creation.
        Index.valueOf(((Integer)INITIAL_FIRST.get()).intValue());
        Index.valueOf(((Integer)INITIAL_LAST.get()).intValue());
    }    

    private static final long serialVersionUID = 1L;


}