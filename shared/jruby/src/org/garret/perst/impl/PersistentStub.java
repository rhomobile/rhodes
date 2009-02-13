package org.garret.perst.impl;
import org.garret.perst.*;

public class PersistentStub implements IPersistent { 
    public void load() {
        throw new StorageError(StorageError.ACCESS_TO_STUB);
    }

    public void loadAndModify() {
        load();
        modify();
    }

    public final boolean isRaw() { 
        return true;
    } 
    
    public final boolean isModified() { 
        return false;
    } 
    
    public final boolean isDeleted() { 
        return false;
    } 

    public final boolean isPersistent() { 
        return true;
    }
    
    public void makePersistent(Storage storage) { 
        throw new StorageError(StorageError.ACCESS_TO_STUB);

    }

    public void store() {
        throw new StorageError(StorageError.ACCESS_TO_STUB);
    }
  
    public void modify() { 
        throw new StorageError(StorageError.ACCESS_TO_STUB);
    }

    public PersistentStub(Storage storage, int oid) { 
        this.storage = storage;
        this.oid = oid;
    }

    public final int getOid() {
        return oid;
    }

    public void deallocate() { 
        throw new StorageError(StorageError.ACCESS_TO_STUB);
    }

    public boolean recursiveLoading() {
        return true;
    }
    
    public final Storage getStorage() {
        return storage;
    }
    
    public boolean equals(Object o) { 
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
        throw new StorageError(StorageError.ACCESS_TO_STUB);
    }

    transient Storage storage;
    transient int     oid;

    public void assignOid(Storage storage, int oid, boolean raw) { 
        throw new StorageError(StorageError.ACCESS_TO_STUB);
    }


    public void writeObject(IOutputStream out) {}

    public void readObject(IInputStream in) {}

    public boolean isPinnable() { 
        return false;
    }
}





