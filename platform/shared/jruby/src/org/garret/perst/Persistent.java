package org.garret.perst;

/**
 * Base class for all persistent capable classes
 */
public class Persistent implements IPersistent 
{ 
    public synchronized void load() {
        if (oid != 0 && (state & RAW) != 0) { 
            storage.loadObject(this);
        }
    }

    public synchronized void loadAndModify() {
        load();
        modify();
    }
    
    public final boolean isRaw() { 
        return (state & RAW) != 0;
    } 
    
    public final boolean isModified() { 
        return (state & DIRTY) != 0;
    } 
    
    public final boolean isDeleted() { 
        return (state & DELETED) != 0;
    } 
    
    public final boolean isPersistent() { 
        return oid != 0;
    }
    
    public void makePersistent(Storage storage) { 
        if (oid == 0) { 
            storage.makePersistent(this);
        }
    }

    public void store() {
        if ((state & RAW) != 0) { 
            throw new StorageError(StorageError.ACCESS_TO_STUB);
        }
        if (storage != null) { 
            storage.storeObject(this);
            state &= ~DIRTY;
        }
    }
  
    public void modify() { 
        if ((state & DIRTY) == 0 && oid != 0) {
            if ((state & RAW) != 0) { 
                throw new StorageError(StorageError.ACCESS_TO_STUB);
            }
            Assert.that((state & DELETED) == 0);
            storage.modifyObject(this);
            state |= DIRTY;
        }
    }

    public Persistent() {}

    public Persistent(Storage storage) { 
        this.storage = storage;
    }

    public final int getOid() {
        return oid;
    }

    public void deallocate() { 
        if (oid != 0) { 
            storage.deallocateObject(this);
        }
    }

    public boolean recursiveLoading() {
        return true;
    }
    
    public final Storage getStorage() {
        return storage;
    }
    
    public boolean equals(Object o) { 
        if (oid == 0) { 
            return super.equals(o);
        }
        return o instanceof IPersistent && ((IPersistent)o).getOid() == oid;
    }

    public int hashCode() {
        return oid;
    }

    public void onLoad() {
    }

    public void onStore() {
    }

    public void invalidate() { 
        state &= ~DIRTY;
        state |= RAW;
    }

    transient Storage storage;
    transient int     oid;
    transient int     state;

    static private final int RAW     = 1;
    static private final int DIRTY   = 2;
    static private final int DELETED = 4;

    public void assignOid(Storage storage, int oid, boolean raw) { 
        this.oid = oid;
        this.storage = storage;
        if (raw) {
            state |= RAW;
        } else { 
            state &= ~RAW;
        }
    }

    public void writeObject(IOutputStream out) {}

    public void readObject(IInputStream in) {}

    public boolean isPinnable() { 
        return false;
    }
}





