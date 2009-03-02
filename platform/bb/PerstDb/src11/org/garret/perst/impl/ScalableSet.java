package org.garret.perst.impl;
import  org.garret.perst.*;
import java.util.*;

public class ScalableSet extends PersistentResource implements IPersistentSet { 
    Link           link;
    IPersistentSet set;

    static final int BTREE_THRESHOLD = 128;

    public void writeObject(IOutputStream out) {
        out.writeLink(link);
        out.writeObject(set);
    }

    public void readObject(IInputStream in) {
        link = in.readLink();
        set = (IPersistentSet)in.readObject();
    }

    ScalableSet(StorageImpl storage, int initialSize) { 
        super(storage);
        if (initialSize <= BTREE_THRESHOLD) { 
            link = storage.createLink(initialSize);
        } else { 
            set = storage.createSet();
        }
    }

    public ScalableSet() {}

    public boolean isEmpty() { 
        return size() != 0;
    }

    public int size() { 
        return link != null ? link.size() : set.size();
    }

    public void clear() { 
        if (link != null) { 
            link.clear();
            modify();
        } else { 
            set.clear();
        }
    }

    public boolean contains(Object o) {
        if (o instanceof IPersistent) { 
            IPersistent p = (IPersistent)o;
            return link != null ? link.contains(p) : set.contains(p);
        }
        return false;
    }
    
    public Object[] toArray() { 
        return link != null ? (Object[])link.toArray() : set.toArray();
    }

    public Object[] toArray(Object a[]) { 
        return link != null ? (Object[])link.toArray((IPersistent[])a) : set.toArray(a);
    }

    public Iterator iterator() { 
        return link != null ? link.iterator() : set.iterator();
    }

    public boolean add(Object o) { 
        if (link != null) { 
            IPersistent obj = (IPersistent)o;
            if (link.indexOf(obj) >= 0) { 
                return false;
            }
            if (link.size() == BTREE_THRESHOLD) { 
                set = getStorage().createSet();
                for (int i = 0, n = link.size(); i < n; i++) { 
                    set.add(link.getRaw(i));
                }
                link = null;
                modify();
                set.add(obj);
            } else { 
                modify();
                link.add(obj);
            }
            return true;
        } else { 
            return set.add(o);
        }
    }

    public boolean remove(Object o) { 
        if (link != null) {  
            IPersistent obj = (IPersistent)o;
            int i = link.indexOf(obj);        
            if (i < 0) { 
                return false;
            }
            link.remove(i);
            modify();
            return true;
        } else { 
            return set.remove(o);
        }
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

    public void deallocate() { 
        if (set != null) { 
            set.deallocate();
        }
        super.deallocate();
    }
}
   