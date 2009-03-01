package org.garret.perst;

import java.util.*;

/** 
 * PATRICIA trie (Practical Algorithm To Retrieve Information Coded In Alphanumeric).
 * Tries are a kind of tree where each node holds a common part of one or more keys. 
 * PATRICIA trie is one of the many existing variants of the trie, which adds path compression 
 * by grouping common sequences of nodes together.<BR>
 * This structure provides a very efficient way of storing values while maintaining the lookup time 
 * for a key in O(N) in the worst case, where N is the length of the longest key. 
 * This structure has it's main use in IP routing software, but can provide an interesting alternative 
 * to other structures such as hashtables when memory space is of concern.
 */
public interface PatriciaTrie extends IPersistent, IResource { 
    /** 
     * Add new key to the trie
     * @param key bit vector
     * @param obj persistent object associated with this key
     * @return previous object associtated with this key or <code>null</code> if there
     * was no such object
     */
    IPersistent add(PatriciaTrieKey key, IPersistent obj);
    
    /**
     * Find best match with specified key
     * @param key bit vector
     * @return object associated with this deepest possible match with specified key
     */
    IPersistent findBestMatch(PatriciaTrieKey key);
    
    /**
     * Find exact match with specified key
     * @param key bit vector
     * @return object associated with this key or NULL if match is not found
     */
    IPersistent findExactMatch(PatriciaTrieKey key);
    
    /**
     * Removes key from the triesKFind exact match with specified key
     * @param key bit vector
     * @return object associated with removed key or <code>null</code> if such key is not found
     */
    IPersistent remove(PatriciaTrieKey key);

    /**
     * Clear the trie: remove all elements from trie
     */
    void clear();

    /**
     * Get array of all members of the trie
     * @return array of trie members
     */
    public IPersistent[] toArray();

    /**
     * Get all objects in the trie.
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
     * Get all objects in the trie.
     * @return array list of all objects in the trie
     */
    public ArrayList elements();

    /**
     * Get iterator through all trie members. Iteration is performed on
     * the copy of the trie, so it is not possible to remove trie elements using this iterator
     * and any modification of trie has no influence on iterator.
     * @return iterator through all trie member
     */
    public Iterator iterator();
}