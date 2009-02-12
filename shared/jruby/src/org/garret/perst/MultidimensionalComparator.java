package org.garret.perst;

/**
 * Base class for multidimensional persistent comparator used in multidimensional index
 */

public abstract class MultidimensionalComparator extends Persistent { 
    public static final int LEFT_UNDEFINED = -2;
    public static final int LT = -1;
    public static final int EQ = 0;
    public static final int GT = 1;
    public static final int RIGHT_UNDEFINED = 2;
    public static final int NE = 3;

    /**
     * Compare  i-th component of two objects
     * @param m1 first object
     * @param m2 second object
     * @param i component index
     * @return LEFT_UNDEFINED if value of i-th component of m1 is null and 
     * value  of i-th component of m2 is not null, 
     * RIGHT_UNDEFINED if value of i-th component of m1 is not null and 
     * value  of i-th component of m2 is null, 
     * EQ if both values are null,
     * otherwise LT, EQ or GT depending on result of their comparison
     */
    public abstract int compare(Object m1, Object m2, int i);

    /**
     * Get number of dimensions
     * @return number of dimensions
     */
    public abstract int getNumberOfDimensions();

    /**
     * Create clone of the specified object contining copy of the specified field
     * @param obj original object
     * @param i component index
     * @return clone of the object
     */
    public abstract IPersistent cloneField(IPersistent obj, int i);

    protected MultidimensionalComparator(Storage storage) {
        super(storage);
    }

    protected MultidimensionalComparator() {}
}

