package org.garret.perst.impl;
import  org.garret.perst.*;
import  java.util.*;

class BitIndexImpl extends Btree implements BitIndex 
{ 
    BitIndexImpl() {
        super(Types.Int, true);
    }

    static class Key { 
        int key;
        int oid;

        Key(int key, int oid) { 
            this.key = key;
            this.oid = oid;
        }
    }
        
    public int get(IPersistent obj) 
    {
        StorageImpl db = (StorageImpl)getStorage();
        if (root == 0) { 
            throw new StorageError(StorageError.KEY_NOT_FOUND);
        } 
        return BitIndexPage.find(db, root, obj.getOid(), height);
    }
 

    public void put(IPersistent obj, int mask) 
    {
        StorageImpl db = (StorageImpl)getStorage();
        if (db == null) {             
            throw new StorageError(StorageError.DELETED_OBJECT);
        }
        if (!obj.isPersistent()) { 
            db.makePersistent(obj);
        }
        Key ins = new Key(mask, obj.getOid());
        if (root == 0) { 
            root = BitIndexPage.allocate(db, 0, ins);
            height = 1;
        } else { 
            int result = BitIndexPage.insert(db, root, ins, height);
            if (result == op_overflow) { 
                root = BitIndexPage.allocate(db, root, ins);
                height += 1;
            }
        }
        updateCounter += 1;
        nElems += 1;
        modify();
    }

    public void remove(IPersistent obj) 
    {
        StorageImpl db = (StorageImpl)getStorage();
        if (db == null) {             
            throw new StorageError(StorageError.DELETED_OBJECT);
        }
        if (root == 0) {
            throw new StorageError(StorageError.KEY_NOT_FOUND);
        }
        int result = BitIndexPage.remove(db, root, obj.getOid(), height);
        if (result == op_not_found) { 
            throw new StorageError(StorageError.KEY_NOT_FOUND);
        }
        nElems -= 1;
        if (result == op_underflow) { 
            Page pg = db.getPage(root);
            if (BitIndexPage.getnItems(pg) == 0) {                         
                int newRoot = 0;
                if (height != 1) { 
                    newRoot = BitIndexPage.getItem(pg, BitIndexPage.maxItems-1);
                }
                db.freePage(root);
                root = newRoot;
                height -= 1;
            }
            db.pool.unfix(pg);
        }
        updateCounter += 1;
        modify();
    }
        
    class BitIndexIterator extends Iterator 
    { 
        BitIndexIterator(int set, int clear) 
        { 
            sp = 0;
            counter = updateCounter;
            if (height == 0) { 
                return;
            }
            int pageId = root;
            StorageImpl db = (StorageImpl)getStorage();
            if (db == null) {             
                throw new StorageError(StorageError.DELETED_OBJECT);
            }
            int h = height;
            this.set = set;
            this.clear = clear;
            
            pageStack = new int[h];
            posStack = new int[h];
            
            while (true) { 
                pageStack[sp] = pageId;
                Page pg = db.getPage(pageId);
                sp += 1;
                if (--h == 0) { 
                    gotoNextItem(pg, 0);
                    break;
                }
                pageId = BitIndexPage.getItem(pg, BitIndexPage.maxItems-1);
                db.pool.unfix(pg);
            }
        }

        public boolean hasNext() 
        {
            if (counter != updateCounter) { 
                throw new ConcurrentModificationException();
            }
            return sp != 0;
        }

        public Object next() 
        {
            return ((StorageImpl)getStorage()).lookupObject(nextOid());
        }

        public int nextOid() 
        {
            if (!hasNext()) { 
                throw new NoSuchElementException();
            }
            StorageImpl db = (StorageImpl)getStorage();
            int pos = posStack[sp-1];   
            Page pg = db.getPage(pageStack[sp-1]);
            int oid = BitIndexPage.getItem(pg, BitIndexPage.maxItems-1-pos);
            gotoNextItem(pg, pos+1);
            return oid;
        }

        private final void gotoNextItem(Page pg, int pos)
        {
            StorageImpl db = (StorageImpl)getStorage();
                
            do { 
                int end = BitIndexPage.getnItems(pg); 
                while (pos < end) { 
                    int mask = BitIndexPage.getItem(pg, pos);
                    if ((set & mask) == set && (clear & mask) == 0) { 
                        posStack[sp-1] = pos;
                        db.pool.unfix(pg);
                        return;
                    }
                    pos += 1;
                }
                while (--sp != 0) { 
                    db.pool.unfix(pg);
                    pos = posStack[sp-1];
                    pg = db.getPage(pageStack[sp-1]);
                    if (++pos <= BitIndexPage.getnItems(pg)) {
                        posStack[sp-1] = pos;
                        do { 
                            int pageId = BitIndexPage.getItem(pg, BitIndexPage.maxItems-1-pos);
                            db.pool.unfix(pg);
                            pg = db.getPage(pageId);
                            pageStack[sp] = pageId;
                            posStack[sp] = pos = 0;
                        } while (++sp < pageStack.length);
                        break;
                    }
                }
            } while (sp != 0);

            db.pool.unfix(pg);
        }

        public void remove() 
        { 
            throw new org.garret.perst.UnsupportedOperationException();
        }

        int[]       pageStack;
        int[]       posStack;
        int         sp;
        int         set;
        int         clear;
        int         counter;
    }

    public Iterator iterator() 
    { 
        return iterator(0, 0);
    } 

    public Iterator iterator(int set, int clear) 
    { 
        return new BitIndexIterator(set, clear);
    }

    static class BitIndexPage extends BtreePage { 
        static final int max = keySpace / 8;    

        static int getItem(Page pg, int index) { 
            return Bytes.unpack4(pg.data, firstKeyOffs + index*4);
        }
    
        static void setItem(Page pg, int index, int mask) { 
            Bytes.pack4(pg.data, firstKeyOffs + index*4, mask);
        }

        static int allocate(StorageImpl db, int root, Key ins) 
        {
            int pageId = db.allocatePage();
            Page pg = db.putPage(pageId);
            setnItems(pg, 1);
            setItem(pg, 0, ins.key);
            setItem(pg, maxItems-1, ins.oid);
            setItem(pg, maxItems-2, root);
            db.pool.unfix(pg);
            return pageId;
        }
        
        static void memcpy(Page dst_pg, int dst_idx, Page src_pg, int src_idx, int len) 
        { 
            System.arraycopy(src_pg.data, firstKeyOffs + src_idx*4, 
                             dst_pg.data, firstKeyOffs + dst_idx*4, 
                             len*4);
        }
        
        static int find(StorageImpl db, int pageId, int oid, int height)
        {
            Page pg = db.getPage(pageId);
            try { 
                int i, n = getnItems(pg), l = 0, r = n;
                if (--height == 0) {
                    while (l < r)  {
                        i = (l+r) >> 1;
                        if (oid > getItem(pg, maxItems-1-i)) { 
                            l = i+1; 
                        } else { 
                            r = i;
                        }
                    }
                    if (r < n && getItem(pg, maxItems-r-1) == oid) {
                        return getItem(pg, r);
                    }
                    throw new StorageError(StorageError.KEY_NOT_FOUND);                    
                } else { 
                    while (l < r)  {
                        i = (l+r) >> 1;
                        if (oid > getItem(pg, i)) { 
                            l = i+1; 
                        } else { 
                            r = i;
                        }
                    }
                    return find(db, getItem(pg, maxItems-r-1), oid, height);
                }
            } finally { 
                if (pg != null) { 
                    db.pool.unfix(pg);
                }
            }
        }

        static int insert(StorageImpl db, int pageId, Key ins, int height)
        {
            Page pg = db.getPage(pageId);
            int l = 0, n = getnItems(pg), r = n;
            int oid = ins.oid;
            try { 
                if (--height != 0) {
                    while (l < r)  {
                        int i = (l+r) >> 1;
                        if (oid > getItem(pg, i)) { 
                            l = i+1; 
                        } else { 
                            r = i;
                        }
                    }
                    Assert.that(l == r);
                    /* insert before e[r] */
                    int result = insert(db, getItem(pg, maxItems-r-1), ins, height);
                    Assert.that(result != op_not_found);
                    if (result != op_overflow) {
                        return result;
                    }
                    n += 1;
                } else { 
                    while (l < r)  {
                        int i = (l+r) >> 1;
                        if (oid > getItem(pg,  maxItems-1-i)) { 
                            l = i+1; 
                        } else { 
                            r = i;
                        }
                    }
                    if (r < n && oid == getItem(pg,  maxItems-1-r)) { 
                        db.pool.unfix(pg);
                        pg = null;
                        pg = db.putPage(pageId);
                        setItem(pg, r, ins.key);
                        return op_overwrite;
                    }
                }
                db.pool.unfix(pg);
                pg = null;
                pg = db.putPage(pageId);
                if (n < max) {
                    memcpy(pg, r+1, pg, r, n - r);
                    memcpy(pg, maxItems-n-1, pg, maxItems-n, n-r);
                    setItem(pg, r, ins.key);
                    setItem(pg, maxItems-1-r, ins.oid);
                    setnItems(pg, getnItems(pg)+1);
                    return op_done;
                } else { /* page is full then divide page */
                    pageId = db.allocatePage();
                    Page b = db.putPage(pageId);
                    Assert.that(n == max);
                    int m = max/2;
                    if (r < m) {
                        memcpy(b, 0, pg, 0, r);
                        memcpy(b, r+1, pg, r, m-r-1);
                        memcpy(pg, 0, pg, m-1, max-m+1);
                        memcpy(b, maxItems-r, pg, maxItems-r, r);
                        setItem(b, r, ins.key);
                        setItem(b, maxItems-1-r, ins.oid);
                        memcpy(b, maxItems-m, pg, maxItems-m+1, m-r-1);
                        memcpy(pg, maxItems-max+m-1, pg, maxItems-max, max-m+1);
                    } else {
                        memcpy(b, 0, pg, 0, m);
                        memcpy(pg, 0, pg, m, r-m);
                        memcpy(pg, r-m+1, pg, r, max-r);
                        memcpy(b, maxItems-m, pg, maxItems-m, m);
                        memcpy(pg, maxItems-r+m, pg, maxItems-r, r-m);
                        setItem(pg, r-m, ins.key);
                        setItem(pg, maxItems-1-r+m, ins.oid);
                        memcpy(pg, maxItems-max+m-1, pg, maxItems-max, max-r);
                    }
                    ins.oid = pageId;
                    if (height == 0) {
                        ins.key = getItem(b, maxItems-m);
                        setnItems(pg, max - m + 1);
                        setnItems(b, m);
                    } else {
                        ins.key = getItem(b, m-1);
                        setnItems(pg, max - m);
                        setnItems(b, m - 1);
                    }                            
                    db.pool.unfix(b);
                    return op_overflow;
                }
            } finally { 
                if (pg != null) { 
                    db.pool.unfix(pg);
                }
            }
        }

    
        static int handlePageUnderflow(StorageImpl db, Page pg, int r, int height)
        {
            int nItems = getnItems(pg);
            Page a = db.putPage(getItem(pg, maxItems-r-1));
            int an = getnItems(a);
            if (r < nItems) { // exists greater page
                Page b = db.getPage(getItem(pg, maxItems-r-2));
                int bn = getnItems(b); 
                Assert.that(bn >= an);
                if (height != 1) { 
                    memcpy(a, an, pg, r, 1);
                    an += 1;
                    bn += 1;
                }
                if (an+bn > max) { 
                    // reallocation of nodes between pages a and b
                    int i = bn - ((an + bn) >> 1);
                    db.pool.unfix(b);
                    b = db.putPage(getItem(pg, maxItems-r-2));
                    memcpy(a, an, b, 0, i);
                    memcpy(b, 0, b, i, bn-i);
                    memcpy(a, maxItems-an-i, b, maxItems-i, i);
                    memcpy(b, maxItems-bn+i, b, maxItems-bn, bn-i);
                    if (height != 1) { 
                        memcpy(pg, r, a, an+i-1, 1);
                    } else { 
                        memcpy(pg, r, a, maxItems-an-i, 1);
                    }
                    setnItems(b, getnItems(b) - i);
                    setnItems(a, getnItems(a) + i);
                    db.pool.unfix(a);
                    db.pool.unfix(b);
                    return op_done;
                } else { // merge page b to a  
                    memcpy(a, an, b, 0, bn);
                    memcpy(a, maxItems-an-bn, b, maxItems-bn, bn);
                    db.freePage(getItem(pg, maxItems-r-2));
                    memcpy(pg, maxItems-nItems, pg, maxItems-nItems-1, 
                           nItems - r - 1);
                    memcpy(pg, r, pg, r+1, nItems - r - 1);
                    setnItems(a, getnItems(a) + bn);
                    setnItems(pg, nItems - 1);
                    db.pool.unfix(a);
                    db.pool.unfix(b);
                    return nItems < max/2 ? op_underflow : op_done;
                }
            } else { // page b is before a
                Page b = db.getPage(getItem(pg, maxItems-r));
                int bn = getnItems(b); 
                Assert.that(bn >= an);
                if (height != 1) { 
                    an += 1;
                    bn += 1;
                }
                if (an+bn > max) { 
                    // reallocation of nodes between pages a and b
                    int i = bn - ((an + bn) >> 1);
                    db.pool.unfix(b);
                    b = db.putPage(getItem(pg, maxItems-r));
                    memcpy(a, i, a, 0, an);
                    memcpy(a, 0, b, bn-i, i);
                    memcpy(a, maxItems-an-i, a, maxItems-an, an);
                    memcpy(a, maxItems-i, b, maxItems-bn, i);
                    if (height != 1) { 
                        memcpy(a, i-1, pg, r-1, 1);
                        memcpy(pg, r-1, b, bn-i-1, 1);
                    } else { 
                        memcpy(pg, r-1, b, maxItems-bn+i, 1);
                    }
                    setnItems(b, getnItems(b) - i);
                    setnItems(a, getnItems(a) + i);
                    db.pool.unfix(a);
                    db.pool.unfix(b);
                    return op_done;
                } else { // merge page b to a
                    memcpy(a, bn, a, 0, an);
                    memcpy(a, 0, b, 0, bn);
                    memcpy(a, maxItems-an-bn, a, maxItems-an, an);
                    memcpy(a, maxItems-bn, b, maxItems-bn, bn);
                    if (height != 1) { 
                        memcpy(a, bn-1, pg, r-1, 1);
                    }
                    db.freePage(getItem(pg, maxItems-r));
                    setItem(pg, maxItems-r, getItem(pg, maxItems-r-1));
                    setnItems(a, getnItems(a) + bn);
                    setnItems(pg, nItems - 1);
                    db.pool.unfix(a);
                    db.pool.unfix(b);
                    return nItems < max/2 ? op_underflow : op_done;
                }
            }
        }
   
        static int remove(StorageImpl db, int pageId, int oid, int height)
        {
            Page pg = db.getPage(pageId);
            try { 
                int i, n = getnItems(pg), l = 0, r = n;
                if (--height == 0) {
                    while (l < r)  {
                        i = (l+r) >> 1;
                        if (oid > getItem(pg, maxItems-1-i)) { 
                            l = i+1; 
                        } else { 
                            r = i;
                        }
                    }
                    if (r < n && getItem(pg, maxItems-r-1) == oid) {
                        db.pool.unfix(pg);
                        pg = null;
                        pg = db.putPage(pageId);
                        memcpy(pg, r, pg, r+1, n - r - 1);
                        memcpy(pg, maxItems-n+1, pg, maxItems-n, n - r - 1);
                        setnItems(pg, --n);
                        return n < max/2 ? op_underflow : op_done;
                    }
                    return op_not_found;
                } else { 
                    while (l < r)  {
                        i = (l+r) >> 1;
                        if (oid > getItem(pg, i)) { 
                            l = i+1; 
                        } else { 
                            r = i;
                        }
                    }
                    int result = remove(db, getItem(pg, maxItems-r-1), oid, height);
                    if (result == op_underflow) { 
                        db.pool.unfix(pg);
                        pg = null;
                        pg = db.putPage(pageId);
                        return handlePageUnderflow(db, pg, r, height);
                    }
                    return result;
                }
            } finally { 
                if (pg != null) { 
                    db.pool.unfix(pg);
                }
            }
        }
    }
}

