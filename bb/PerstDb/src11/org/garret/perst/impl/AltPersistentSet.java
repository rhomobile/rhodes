package org.garret.perst.impl;
import  org.garret.perst.*;
import  java.util.*;

class AltPersistentSet extends AltBtree implements IPersistentSet { 
    AltPersistentSet() { 
        type = Types.Object;
        unique = true;
    }

    public boolean isEmpty() { 
        return nElems == 0;
    }

    public boolean contains(Object o) {
        if (o instanceof IPersistent) { 
            Key key = new Key((IPersistent)o);
            Iterator i = iterator(key, key, ASCENT_ORDER);
            return i.hasNext();
        }
        return false;
    }
    
    public Object[] toArray() { 
        return toPersistentArray();
    }

    public Object[] toArray(Object a[]) { 
        return toPersistentArray((IPersistent[])a);
    }

    public boolean add(Object o) { 
        IPersistent obj = (IPersistent)o;
        return put(new Key(obj), obj);
    }

    public boolean remove(Object o) { 
        IPersistent obj = (IPersistent)o;
        return removeIfExists(new BtreeKey(checkKey(new Key(obj)), obj));
    }
    
    public boolean containsAll(Collection c) { 
        Iterator i = c.iterator();
        while (i.hasNext()) { 
            if (!contains(i.next()))
                return false;
        }
        return true;
    }

    
    public boolean addAll(Collection c) {
        boolean modified = false;
        Iterator i = c.iterator();
        while (i.hasNext()) {
            modified |= add(i.next());
        }
        return modified;
    }

    public boolean retainAll(Collection c) {
        ArrayList toBeRemoved = new ArrayList();
        Iterator i = iterator();
        while (i.hasNext()) {
            Object o = i.next();
            if (!c.contains(o)) {
                toBeRemoved.add(o);
            }
        }
        int n = toBeRemoved.size();
        for (int j = 0; j < n; j++) { 
            remove(toBeRemoved.get(j));
        }
        return n != 0;         
    }

    public boolean removeAll(Collection c) {
        boolean modified = false;
        Iterator i = c.iterator();
        while (i.hasNext()) {
            modified |= remove(i.next());
        }
        return modified;
    }

    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }
        if (!(o instanceof Set)) {
            return false;
        }
        Collection c = (Collection) o;
        if (c.size() != size()) {
            return false;
        }
        return containsAll(c);
    }

    public int hashCode() {
        int h = 0;
        Iterator i = iterator();
        while (i.hasNext()) {
            h += ((IPersistent)i.next()).getOid();
        }
        return h;
    }
}
