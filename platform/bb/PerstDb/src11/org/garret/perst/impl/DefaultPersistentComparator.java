package org.garret.perst.impl;

import org.garret.perst.*;

public class DefaultPersistentComparator extends PersistentComparator { 
    public int compareMembers(IPersistent m1, IPersistent m2) {
        return ((Comparable)m1).compareTo(m2);
    }
        
    public int compareMemberWithKey(IPersistent mbr, Object key) { 
        return ((Comparable)mbr).compareTo(key);
    }
}