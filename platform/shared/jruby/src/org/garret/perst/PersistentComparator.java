package org.garret.perst;

/**
 * Base class for persistent comparator used in SortedCollection class
 */

public abstract class PersistentComparator extends Persistent { 
    /**
     * Compare two members of collection
     * @param m1 first members
     * @param m2 second members
     * @return negative number if m1 &lt; m2, zero if m1 == m2 and positive number if m1 &gt; m2
     */
    public abstract int compareMembers(IPersistent m1, IPersistent m2);

    /**
     * Compare member with specified search key
     * @param mbr collection member
     * @param key search key
     * @return negative number if mbr &lt; key, zero if mbr == key and positive number if mbr &gt; key
     */
    public abstract int compareMemberWithKey(IPersistent mbr, Object key);
}

