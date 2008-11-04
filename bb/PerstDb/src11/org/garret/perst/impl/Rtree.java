package org.garret.perst.impl;

import org.garret.perst.*;
import java.util.*;

public class Rtree extends PersistentResource implements SpatialIndex {
    private int       height;
    private int       n;
    private RtreePage root;
    private transient int updateCounter;

    public void writeObject(IOutputStream out) {
        out.writeInt(height);
        out.writeInt(n);
        out.writeObject(root);
    }

    public void readObject(IInputStream in) {
        height = in.readInt();
        n = in.readInt();
        root = (RtreePage)in.readObject();
    }

    public Rtree() {}

    public void put(Rectangle r, IPersistent obj) {
        if (root == null) { 
            root = new RtreePage(getStorage(), obj, r);
            height = 1;
        } else { 
            RtreePage p = root.insert(getStorage(), r, obj, height); 
            if (p != null) {
                root = new RtreePage(getStorage(), root, p);
                height += 1;
            }
        }
        updateCounter += 1;
        n += 1;
        modify();
    }
    
    public int size() { 
        return n;
    }

    public void remove(Rectangle r, IPersistent obj) {
        if (root == null) { 
            throw new StorageError(StorageError.KEY_NOT_FOUND);
        }
        ArrayList reinsertList = new ArrayList();
        int reinsertLevel = root.remove(r, obj, height, reinsertList);
        if (reinsertLevel < 0) { 
             throw new StorageError(StorageError.KEY_NOT_FOUND);
        }        
        for (int i = reinsertList.size(); --i >= 0;) {
            RtreePage p = (RtreePage)reinsertList.get(i);
            for (int j = 0, n = p.n; j < n; j++) { 
                RtreePage q = root.insert(getStorage(), p.b[j], p.branch.get(j), height - reinsertLevel); 
                if (q != null) { 
                    // root splitted
                    root = new RtreePage(getStorage(), root, q);
                    height += 1;
                }
            }
            reinsertLevel -= 1;
            p.deallocate();
        }
        if (root.n == 1 && height > 1) { 
            RtreePage newRoot = (RtreePage)root.branch.get(0);
            root.deallocate();
            root = newRoot;
            height -= 1;
        }
        n -= 1;
        updateCounter += 1;
        modify();
    }
    
    public IPersistent[] get(Rectangle r) {
        ArrayList result = getList(r);
        return (IPersistent[])result.toArray(new IPersistent[result.size()]);
    }

    public ArrayList getList(Rectangle r) { 
        ArrayList result = new ArrayList();
        if (root != null) { 
            root.find(r, result, height);
        }
        return result;
    }

    public IPersistent[] toArray() {
        return get(getWrappingRectangle());
    }

    public IPersistent[] toArray(IPersistent[] arr) {
        return (IPersistent[])getList(getWrappingRectangle()).toArray(arr);
    }
    
    public Rectangle getWrappingRectangle() {
        if (root != null) { 
            return root.cover();
        }
        return null;
    }

    public void clear() {
        if (root != null) { 
            root.purge(height);
            root = null;
        }
        height = 0;
        n = 0;
        modify();
    }

    public void deallocate() {
        clear();
        super.deallocate();
    }

    class RtreeIterator extends Iterator {
        RtreeIterator(Rectangle r) { 
            counter = updateCounter;
            if (height == 0) { 
                return;
            }
            this.r = r;            
            pageStack = new RtreePage[height];
            posStack = new int[height];

            if (!gotoFirstItem(0, root)) { 
                pageStack = null;
                posStack = null;
            }
        }

        public boolean hasNext() {
            if (counter != updateCounter) { 
                throw new ConcurrentModificationException();
            }
            return pageStack != null;
        }

        protected Object current(int sp) { 
            return pageStack[sp].branch.get(posStack[sp]);
        }

        public Object next() {
            if (!hasNext()) { 
                throw new NoSuchElementException();
            }
            Object curr = current(height-1);
            if (!gotoNextItem(height-1)) { 
                pageStack = null;
                posStack = null;
            }
            return curr;
        }
 
        public int nextOid() {
            if (!hasNext()) { 
                throw new NoSuchElementException();
            }
            int oid = pageStack[height-1].branch.getRaw(posStack[height-1]).getOid();
            if (!gotoNextItem(height-1)) { 
                pageStack = null;
                posStack = null;
            }
            return oid;
        }
 
        private boolean gotoFirstItem(int sp, RtreePage pg) { 
            for (int i = 0, n = pg.n; i < n; i++) { 
                if (r.intersects(pg.b[i])) { 
                    if (sp+1 == height || gotoFirstItem(sp+1, (RtreePage)pg.branch.get(i))) { 
                        pageStack[sp] = pg;
                        posStack[sp] = i;
                        return true;
                    }
                }
            }
            return false;
        }
              
 
        private boolean gotoNextItem(int sp) {
            RtreePage pg = pageStack[sp];
            for (int i = posStack[sp], n = pg.n; ++i < n;) { 
                if (r.intersects(pg.b[i])) { 
                    if (sp+1 == height || gotoFirstItem(sp+1, (RtreePage)pg.branch.get(i))) { 
                        pageStack[sp] = pg;
                        posStack[sp] = i;
                        return true;
                    }
                }
            }
            getStorage().throwObject(pageStack[sp]);
            pageStack[sp] = null;
            return (sp > 0) ? gotoNextItem(sp-1) : false;
        }
              
        public void remove() { 
            throw new org.garret.perst.UnsupportedOperationException();
        }

        RtreePage[] pageStack;
        int[]       posStack;
        int         counter;
        Rectangle   r;
    }
    
    static class RtreeEntry implements Map.Entry {
        RtreePage pg;
        int       pos;

	public Object getKey() {
	    return pg.b[pos];
	}

	public Object getValue() {
	    return pg.branch.get(pos);
	}

  	public Object setValue(Object value) {
            throw new org.garret.perst.UnsupportedOperationException();
        }

        RtreeEntry(RtreePage pg, int pos) { 
            this.pg = pg;
            this.pos = pos;
        }
    }
        
    class RtreeEntryIterator extends RtreeIterator {
        RtreeEntryIterator(Rectangle r) { 
            super(r);
        }
        
        protected Object current(int sp) { 
            return new RtreeEntry(pageStack[sp], posStack[sp]);
        }
    }

    public Iterator iterator() {
        return iterator(getWrappingRectangle());
    }

    public Iterator entryIterator() {
        return entryIterator(getWrappingRectangle());
    }

    public Iterator iterator(Rectangle r) { 
        return new RtreeIterator(r);
    }

    public Iterator  entryIterator(Rectangle r) { 
        return new RtreeEntryIterator(r);
    }
}
    

