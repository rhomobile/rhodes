package org.garret.perst;

import java.util.*;

/**
 * Interface of multidimensional index.
 * The main idea of this index is to provide efficient search of object using multiple search criteria, for example
 * "select from StockOptions where Symbol between 'AAB' and 'ABC' and Price > 100 and Volume between 1000 and 10000".
 * Each component of the object is represented as separate dimension in the index.
 */
public interface MultidimensionalIndex extends IPersistent, IResource
{
    /**
     * Get comparator used in this index
     * @return comparator used to compare objects in this index
     */
    public MultidimensionalComparator getComparator();

    /**
     * Add new object in the index. 
     * @param obj object to be inserted in index. 
     * @return true if object was successfully added
     */
    public boolean add(IPersistent obj);

    /**
     * Remove object from the index
     * @param obj object removed from the index. 
     * @return true id object was removed, false if object was not found in the index
     */        
    public boolean remove(IPersistent obj);
        
    /**
     * Get iterator for traversing all objects in the index. 
     * Objects are iterated in the ascent key order. 
     * This iterator supports remove() method. 
     * @return index iterator
     */
    public Iterator iterator();

    /**
     * Get iterator through objects matching specified pattern object.
     * All fields which are part of multidimensional index and which values in pattern object is not null
     * are used as filter for index members.
     * @param pattern object which is used as search pattern, non null values of components of this object 
     * forms search condition
     * @return iterator through index members which field values are equal to correspondent non-null fields of pattern object
     */
    public Iterator iterator(Object pattern);

    /**
     * Get iterator through objects which field values belongs to the range specified by correspondent
     * fields of low and high objects.
     * @param low pattern object specifying inclusive low boundary for field values. If there is no low boundary for some particular field
     * it should be set to null. For scalar types (like int) you can use instead minimal possible value, like Integer.MIN_VALUE.
     * If low is null, then low boundary is not specified for all fields.     
     * @param high pattern object specifying inclusive high boundary for field values. If there is no high boundary for some particular field
     * it should be set to null. For scalar types (like int) you can use instead maximal possible value, like Integer.MAX_VALUE.
     * If high is null, then high boundary is not specified for all fields.
     * @return iterator through index members which field values are belongs to the range specified by correspondent fields
     * of low and high objects
     */
    public Iterator iterator(Object low, Object high);

    /**
     * Get array of index members matching specified pattern object.
     * All fields which are part of multidimensional index and which values in pattern object is not null
     * are used as filter for index members.
     * @param pattern object which is used as search pattern, non null values of components of this object 
     * forms search condition
     * @return array of index members which field values are equal to correspondent non-null fields of pattern object
     */
    public IPersistent[] queryByExample(Object pattern);

    /**
     * Get array of index members which field values belongs to the range specified by correspondent
     * fields of low and high objects.
     * @param low pattern object specifying inclusive low boundary for field values. If there is no low boundary for some particular field
     * it should be set to null. For scalar types (like int) you can use instead minimal possible value, like Integer.MIN_VALUE.
     * If low is null, then low boundary is not specified for all fields.     
     * @param high pattern object specifying inclusive high boundary for field values. If there is no high boundary for some particular field
     * it should be set to null. For scalar types (like int) you can use instead maximal possible value, like Integer.MAX_VALUE.
     * If high is null, then high boundary is not specified for all fields.
     * @return array of index members which field values are belongs to the range specified by correspondent fields
     * of low and high objects
     */
    public IPersistent[] queryByExample(Object low, Object high);

    /**
     * Remove all objects from the index
     */
    public void clear();

    /**
     * Check if specified object belongs to the index
     * @param member which is searched in the index
     * @return true if specified object is present in the index
     */
    public boolean contains(IPersistent member);

    /**
     * Get number of objects in the index
     * @return number of objects in the index
     */
    public int size();

    /**
     * Optimize index to make search more efficient.
     * This operation cause complete reconstruction of the index and so may take a long time.
     * Also please notice that this method doesn't build the ideally balanced tree - it just reinserts
     * elements in the tree in random order
     */
    public void optimize();

    /**
     * Get height of the tree. Height of the tree can be used by application
     * to determine when tree structure is no optimal and tree should be reconstructed 
     * using optimize method.
     * @return height of the tree
     */
    public int  getHeight();
}
