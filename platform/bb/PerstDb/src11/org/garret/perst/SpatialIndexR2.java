package org.garret.perst;

import java.util.*;

/**
 * Interface of object spatial index.
 * Spatial index is used to allow fast selection of spatial objects belonging to the specified rectangle.
 * Spatial index is implemented using Guttman R-Tree with quadratic split algorithm.
 */
public interface SpatialIndexR2 extends IPersistent, IResource { 
    /**
     * Find all objects located in the selected rectangle
     * @param r selected rectangle
     * @return array of objects which enveloping rectangle intersects with specified rectangle
     */
    public IPersistent[] get(RectangleR2 r);
    

    /**
     * Get array of all members of the index
     * @return array of index members
     */
    public IPersistent[] toArray();

    /**
     * Get all objects in the index.
     * The runtime type of the returned array is that of the specified array.  
     * If the index fits in the specified array, it is returned therein.  
     * Otherwise, a new array is allocated with the runtime type of the 
     * specified array and the size of this index.<p>
     *
     * If this index fits in the specified array with room to spare
     * (i.e., the array has more elements than this index), the element
     * in the array immediately following the end of the index is set to
     * <tt>null</tt>.  This is useful in determining the length of this
     * index <i>only</i> if the caller knows that this index does
     * not contain any <tt>null</tt> elements.)<p>
     * @param arr specified array
     * @return array of all objects in the index
     */
    public IPersistent[] toArray(IPersistent[] arr);

    /**
     * Find all objects located in the selected rectangle
     * @param r selected rectangle
     * @return array list of objects which enveloping rectangle intersects with specified rectangle
     */
    public ArrayList getList(RectangleR2 r);

    /**
     * Put new object in the index. 
     * @param r enveloping rectangle for the object
     * @param obj object associated with this rectangle. Object can be not yet persistent, in this case
     * its forced to become persistent by assigning OID to it.
     */
    public void put(RectangleR2 r, IPersistent obj);

    /**
     * Remove object with specified enveloping rectangle from the tree.
     * @param r enveloping rectangle for the object
     * @param obj object removed from the index
     * @exception StorageError(StorageError.KEY_NOT_FOUND) exception if there is no such key in the index
     */
    public void remove(RectangleR2 r, IPersistent obj);

    /**
     * Get number of objects in the index
     * @return number of objects in the index
     */
    public int  size();

    /**
     * Get wrapping rectangle 
     * @return minimal rectangle containing all rectangles in the index, <code>null</code> if index is empty     
     */
    public RectangleR2 getWrappingRectangle();

    /**
     * Remove all objects from the index
     */
    public void clear();
    
    /**
     * Get iterator through all members of the index
     * This iterator doesn't support remove() method.
     * It is not possible to modify spatial index during iteration.
     * @return iterator through all objects in the index
     */
    public Iterator iterator();

    /**
     * Get entry iterator through all members of the index
     * This iterator doesn't support remove() method.
     * It is not possible to modify spatial index during iteration.
     * @return entry iterator which key specifies recrtangle and value - correspondent object
     */
    public Iterator entryIterator();

    /**
     * Get objects which rectangle intersects with specified rectangle
     * This iterator doesn't support remove() method.
     * It is not possible to modify spatial index during iteration.
     * @param r selected rectangle
     * @return iterator for objects which enveloping rectangle overlaps with specified rectangle
     */
    public Iterator iterator(RectangleR2 r); 

    /**
     * Get entry iterator through objects which rectangle intersects with specified rectangle
     * This iterator doesn't support remove() method.
     * It is not possible to modify spatial index during iteration.
     * @param r selected rectangle
     * @return entry iterator for objects which enveloping rectangle overlaps with specified rectangle
     */
    public Iterator entryIterator(RectangleR2 r);
}
