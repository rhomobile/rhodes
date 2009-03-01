package org.garret.perst.impl;
import  org.garret.perst.IPersistent;

public interface OidHashTable { 
    void        init(int initialCapacity, int nPinnedObjects);
    boolean     remove(int oid);
    void        put(int oid, IPersistent obj);
    IPersistent get(int oid);
    void        flush();
    void        invalidate();
    void        clear();
    int         size();
    void        clearDirty(IPersistent obj);
    void        setDirty(IPersistent obj);
}
