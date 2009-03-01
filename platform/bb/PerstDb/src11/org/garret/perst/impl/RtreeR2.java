package org.garret.perst.impl;

import org.garret.perst.*;
import java.util.*;

public class RtreeR2 extends PersistentResource implements SpatialIndexR2 {
    private int           height;
    private int           n;
    private RtreeR2Page   root;
    private transient int updateCounter;

    public void writeObject(IOutputStream out) {
        out.writeInt(height);
        out.writeInt(n);
        out.writeObject(root);
    }

    public void readObject(IInputStream in) {
        height = in.readInt();
        n = in.readInt();
        root = (RtreeR2Page)in.readObject();
    }

    public RtreeR2() {}

    RtreeR2(Storage storage) {
        super(storage);
    }

    public void put(RectangleR2 r, IPersistent obj) {
        if (root == null) { 
            root = new RtreeR2Page(getStorage(), obj, r);
            height = 1;
        } else { 
            RtreeR2Page p = root.insert(getStorage(), r, obj, height); 
            if (p != null) {
                root = new RtreeR2Page(getStorage(), root, p);
                height += 1;
            }
        }
        n += 1;
        updateCounter += 1;
        modify();
    }
    
    public int size() { 
        return n;
    }

    public void remove(RectangleR2 r, IPersistent obj) {
        if (root == null) { 
            throw new StorageError(StorageError.KEY_NOT_FOUND);
        }
        ArrayList reinsertList = new ArrayList();
        int reinsertLevel = root.remove(r, obj, height, reinsertList);
        if (reinsertLevel < 0) { 
             throw new StorageError(StorageError.KEY_NOT_FOUND);
        }        
        for (int i = reinsertList.size(); --i >= 0;) {
            RtreeR2Page p = (RtreeR2Page)reinsertList.get(i);
            for (int j = 0, n = p.n; j < n; j++) { 
                RtreeR2Page q = root.insert(getStorage(), p.b[j], p.branch.get(j), height - reinsertLevel); 
                if (q != null) { 
                    // root splitted
                    root = new RtreeR2Page(getStorage(), root, q);
                    height += 1;
                }
            }
            reinsertLevel -= 1;
            p.deallocate();
        }
        if (root.n == 1 && height > 1) { 
            RtreeR2Page newRoot = (RtreeR2Page)root.branch.get(0);
            root.deallocate();
            root = newRoot;
            height -= 1;
        }
        n -= 1;
        updateCounter += 1;
        modify();
    }
    
    public IPersistent[] get(RectangleR2 r) {
        ArrayList result = new ArrayList();
        if (root != null) { 
            root.find(r, result, height);
        }
        return (IPersistent[])result.toArray(new IPersistent[result.size()]);
    }

    public ArrayList getList(RectangleR2 r) { 
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

    public RectangleR2 getWrappingRectangle() {
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
        updateCounter += 1;
        modify();
    }

    public void deallocate() {
        clear();
        super.deallocate();
    }

    class RtreeIterator extends Iterator {
        RtreeIterator(RectangleR2 r) { 
            counter = updateCounter;
            if (height == 0) { 
                return;
            }
            this.r = r;            
            pageStack = new RtreeR2Page[height];
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
        
        private boolean gotoFirstItem(int sp, RtreeR2Page pg) { 
            for (int i = 0, n = pg.n; i < n; i++) { 
                if (r.intersects(pg.b[i])) { 
                    if (sp+1 == height || gotoFirstItem(sp+1, (RtreeR2Page)pg.branch.get(i))) { 
                        pageStack[sp] = pg;
                        posStack[sp] = i;
                        return true;
                    }
                }
            }
            return false;
        }
              
 
        private boolean gotoNextItem(int sp) {
            RtreeR2Page pg = pageStack[sp];
            for (int i = posStack[sp], n = pg.n; ++i < n;) { 
                if (r.intersects(pg.b[i])) { 
                    if (sp+1 == height || gotoFirstItem(sp+1, (RtreeR2Page)pg.branch.get(i))) { 
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

        RtreeR2Page[] pageStack;
        int[]         posStack;
        int           counter;
        RectangleR2   r;
    }
    
    static class RtreeEntry implements Map.Entry {
        RtreeR2Page pg;
        int         pos;

	public Object getKey() {
	    return pg.b[pos];
	}

	public Object getValue() {
	    return pg.branch.get(pos);
	}

  	public Object setValue(Object value) {
            throw new org.garret.perst.UnsupportedOperationException();
        }

        RtreeEntry(RtreeR2Page pg, int pos) { 
            this.pg = pg;
            this.pos = pos;
        }
    }
        
    class RtreeEntryIterator extends RtreeIterator {
        RtreeEntryIterator(RectangleR2 r) { 
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

    public Iterator iterator(RectangleR2 r) { 
        return new RtreeIterator(r);
    }

    public Iterator entryIterator(RectangleR2 r) { 
        return new RtreeEntryIterator(r);
    }
}
    

