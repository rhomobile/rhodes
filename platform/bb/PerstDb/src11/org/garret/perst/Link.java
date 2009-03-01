package org.garret.perst;

/**
 * Interface for one-to-many relation. There are two types of relations:
 * embedded (when references to the relarted obejcts are stored in relation
 * owner obejct itself) and stanalone (when relation is separate object, which contains
 * the reference to the relation owner and relation members). Both kinds of relations
 * implements Link interface. Embedded relation is created by Storage.createLink method
 * and standalone relation is represented by Relation persistent class created by
 * Storage.createRelation method.
 */
public interface Link {
    /**
     * Get number of the linked objects 
     * @return the number of related objects
     */
    public int size();
    
    /**
     * Set number of the linked objects 
     * @param newSize new number of linked objects (if it is greater than original number, 
     * than extra elements will be set to null)
     */
    public void setSize(int newSize);
    
    /**
     * Get related object by index
     * @param i index of the object in the relation
     * @return referenced object
     */
    public IPersistent get(int i);

    /**
     * Get related object by index without loading it.
     * Returned object can be used only to get it OID or to compare with other objects using
     * <code>equals</code> method
     * @param i index of the object in the relation
     * @return stub representing referenced object
     */
    public IPersistent getRaw(int i);

    /**
     * Replace i-th element of the relation
     * @param i index in the relartion
     * @param obj object to be included in the relation     
     */
    public void set(int i, IPersistent obj);

    /**
     * Remove object with specified index from the relation
     * @param i index in the relartion
     */
    public void remove(int i);

    /**
     * Insert new object in the relation
     * @param i insert poistion, should be in [0,size()]
     * @param obj object inserted in the relation
     */
    public void insert(int i, IPersistent obj);

    /**
     * Add new object to the relation
     * @param obj object inserted in the relation
     */
    public void add(IPersistent obj);

    /**
     * Add all elements of the array to the relation
     * @param arr array of obects which should be added to the relation
     */
    public void addAll(IPersistent[] arr);
    
    /**
     * Add specified elements of the array to the relation
     * @param arr array of obects which should be added to the relation
     * @param from index of the first element in the array to be added to the relation
     * @param length number of elements in the array to be added in the relation
     */
    public void addAll(IPersistent[] arr, int from, int length);

    /**
     * Add all object members of the other relation to this relation
     * @param link another relation
     */
    public void addAll(Link link);

   /**
     * Return array with relation members. Members are not loaded and 
     * size of the array can be greater than actual number of members. 
     * @return array of object with relation members used in implementation of Link class
     */
    IPersistent[] toRawArray(); 

    /**
     * Get relation members as array of object
     * @return array of object with relation members
     */
    public IPersistent[] toArray();
    
    /**
     * Get all relation members as array.
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
     * @return array of object with relation members
     */
    public IPersistent[] toArray(IPersistent[] arr);

     /**
     * Check if there is linked object which is equal to the specified object.
     * More formally, returns <tt>true</tt> if and only if this
     * collection contains at least one element <tt>e</tt> such that
     * <tt>(obj==null ? e==null : obj.equals(e))</tt>.<p>
     * @param obj object to be searched in the index. Object should contain indexed field. 
     * @return <code>true</code> if collection contains object equals to the specified
     */
    public boolean contains(IPersistent obj);

    /**
     * Checks if relation contains specified object instance
     * @param obj specified object
     * @return <code>true</code> if object is present in the collection, <code>false</code> otherwise
     */
    public boolean containsObject(IPersistent obj);

    /**
     * Check if i-th element of Link is the same as specified obj
     * @param i element index
     * @param obj object to compare with
     * @return <code>true</code> if i-th element of Link reference the same object as "obj"
     */
    public boolean containsElement(int i, IPersistent obj);

    /**
     * Get index of the specified object in the relation
     * @param obj specified object
     * @return zero based index of the object or -1 if object is not in the relation
     */
    public int indexOf(IPersistent obj);

    /**
     * Remove all members from the relation
     */
    public void clear();

    /**
     * Get iterator through link members.
     * This iterator supports remove() method.
     * @return iterator through linked objects
     */
    public Iterator iterator();

    /**
     * Replace all direct references to linked objects with stubs. 
     * This method is needed tyo avoid memory exhaustion in case when 
     * there is a large numebr of objectys in databasse, mutually
     * refefencing each other (each object can directly or indirectly 
     * be accessed from other objects).
     */
    public void unpin();
     
    /**
     * Replace references to elements with direct references.
     * It will impove spped of manipulations with links, but it can cause
     * recursive loading in memory large number of objects and as a result - memory
     * overflow, because garabge collector will not be able to collect them
     */
    public void pin();     
}
