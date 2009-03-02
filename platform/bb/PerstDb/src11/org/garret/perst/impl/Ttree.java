package org.garret.perst.impl;

import org.garret.perst.*;
import java.util.*;

public class Ttree extends PersistentResource implements SortedCollection {
    private PersistentComparator comparator;
    private boolean              unique;
    private TtreePage            root;
    private int                  nMembers;
    
    public Ttree() {} 

    public void writeObject(IOutputStream out) {
        out.writeObject(comparator);
        out.writeBoolean(unique);
        out.writeObject(root);
        out.writeInt(nMembers);
    }

    public void readObject(IInputStream in) {
        comparator = (PersistentComparator)in.readObject();
        unique = in.readBoolean();
        root = (TtreePage)in.readObject();
        nMembers = in.readInt();
    }

    Ttree(PersistentComparator comparator, boolean unique) { 
        this.comparator = comparator;
        this.unique = unique;
    }

    /**
     * Get comparator used in this collection
     * @return collection comparator
     */
    public PersistentComparator getComparator() { 
        return comparator;
    }

    public boolean recursiveLoading() {
        return false;
    }

    /*
     * Get member with specified key.
     * @param key specified key. It should match with type of the index and should be inclusive.
     * @return object with this value of the key or <code>null</code> if key not found
     * @exception StorageError(StorageError.KEY_NOT_UNIQUE) exception if there are more than 
     * one objects in the collection with specified value of the key.  
     */
    public IPersistent get(Object key) { 
        if (root != null) { 
            ArrayList list = new ArrayList();
            root.find(comparator, key, 1, key, 1, list);
            if (list.size() > 1) { 
                throw new StorageError(StorageError.KEY_NOT_UNIQUE);
            } else if (list.size() == 0) { 
                return null;
            } else { 
                return (IPersistent)list.get(0);
            }
        }
        return null;
    }

            

    /**
     * Get members which key value belongs to the specified range.
     * Either from boundary, either till boundary either both of them can be <code>null</code>.
     * In last case the method returns all objects from the collection.
     * @param from inclusive low boundary. If <code>null</code> then low boundary is not specified.
     * @param till inclusive high boundary. If <code>null</code> then high boundary is not specified.
     * @return array of objects which keys belongs to the specified interval, ordered by key value
     */
    static final IPersistent[] emptySelection = new IPersistent[0];

    public IPersistent[] get(Object from, Object till) { 
        return get(from, true, till, true);
    }

    public IPersistent[] get(Object from, boolean fromInclusive, Object till, boolean tillInclusive) { 
        if (root != null) { 
            ArrayList list = new ArrayList();
            root.find(comparator, from, fromInclusive ? 1 : 0, till, tillInclusive ? 1 : 0, list);
            if (list.size() != 0) { 
                return (IPersistent[])list.toArray(new IPersistent[list.size()]);
            }
        }
        return emptySelection;
    }


    /**
     * Add new member to collection
     * @param obj new member
     * @return <code>true</code> if object is successfully added in the index, 
     * <code>false</code> if collection was declared as unique and there is already member with such value
     * of the key in the collection. 
     */
    public boolean add(IPersistent obj) { 
        TtreePage newRoot;
        if (root == null) { 
            newRoot = new TtreePage(obj);
        } else { 
            TtreePage.PageReference ref = new TtreePage.PageReference(root);
            if (root.insert(comparator, obj, unique, ref) == TtreePage.NOT_UNIQUE) { 
                return false;
            }
            newRoot = ref.pg;
        }
        root = newRoot;
        nMembers += 1;
        modify();
        return true;
    }
                
                
    /**
     * Check if collections contains specified member
     * @return <code>true</code> if specified member belongs to the collection
     */
    public boolean contains(IPersistent member) {
        return (root != null && member != null) ? root.contains(comparator, member) : false;
    } 
       
    public boolean containsKey(Object key) {
        return (root != null && key != null) ? root.containsKey(comparator, key) : false;
    } 
       
    public boolean containsObject(IPersistent member) {
        return (root != null && member != null)  ? root.containsObject(comparator, member) : false;
    }        

    /**
     * Remove member from collection
     * @param obj member to be removed
     * @exception StorageError(StorageError.KEY_NOT_FOUND) exception if there is no such key in the collection
     */
    public void remove(IPersistent obj) {
        if (root == null) {
            throw new StorageError(StorageError.KEY_NOT_FOUND);
        }
        TtreePage.PageReference ref = new TtreePage.PageReference(root);
        if (root.remove(comparator, obj, ref) == TtreePage.NOT_FOUND) {             
            throw new StorageError(StorageError.KEY_NOT_FOUND);
        }
        root = ref.pg;
        nMembers -= 1;        
        modify();
    }

    /**
     * Get number of objects in the collection
     * @return number of objects in the collection
     */
    public int size() {
        return nMembers;
    }
    
    /**
     * Remove all objects from the collection
     */
    public void clear() {
        if (root != null) { 
            root.prune();
            root = null;
            nMembers = 0;
            modify();
        }
    }

    /**
     * T-Tree destructor
     */
    public void deallocate() {
        if (root != null) { 
            root.prune();
        }
        super.deallocate();
    }

    /**
     * Get all objects in the index as array ordered by index key.
     * @return array of objects in the index ordered by key value
     */
    public IPersistent[] toPersistentArray() {
        if (root == null) { 
            return emptySelection;
        }
        IPersistent[] arr = new IPersistent[nMembers];
        root.toArray(arr, 0);
        return arr;
    }

    /**
     * Get all objects in the index as array ordered by index key.
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
     * @return array of objects in the index ordered by key value
     */
    public IPersistent[] toPersistentArray(IPersistent[] arr) {
        if (arr.length < nMembers) { 
            new IllegalArgumentException();
        }
        if (root != null) { 
            root.toArray(arr, 0);
        }
        if (arr.length > nMembers) { 
            arr[nMembers] = null;
        }
        return arr;
    }

    static class TtreeIterator extends Iterator { 
        int           i;
        ArrayList     list;
        boolean       removed;
        Ttree         tree;

        TtreeIterator(Ttree tree, ArrayList list) { 
            this.tree = tree;
            this.list = list;
            i = -1;
        }
        
        public Object next() { 
            if (i+1 >= list.size()) { 
                throw new NoSuchElementException();
            }
            removed = false;
            return list.get(++i);
        }
        
        public int nextOid() { 
            return ((IPersistent)next()).getOid();
        }
        
        public void remove() { 
            if (removed || i < 0 || i >= list.size()) { 
                throw new IllegalStateException();
            }
            tree.remove((IPersistent)list.get(i));
            list.remove(i--);
            removed = true;
        }
            
        public boolean hasNext() {
            return i+1 < list.size();
        }
    }
        

    /**
     * Get iterator for traversing all collection members.
     * You should not update/remove or add members to the index during iteration
     * @return collection iterator
     */
    public Iterator iterator() {
        return iterator(null, null);
    }

    public Iterator iterator(Object from, Object till) {
        return iterator(from, true, till, true);
    }

    public Iterator iterator(Object from, boolean fromInclusive, Object till, boolean tillInclusive) {
        ArrayList list = new ArrayList();
        if (root != null) { 
            root.find(comparator, from, fromInclusive ? 1 : 0, till, tillInclusive ? 1 : 0, list);
        }            
        return new TtreeIterator(this, list);
    }
}

