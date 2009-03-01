package org.garret.perst.impl;
import  org.garret.perst.*;
import  java.util.Date;
import  java.util.NoSuchElementException;

public class Btree extends PersistentResource implements Index { 
    int       root;
    int       height;
    int       type;
    int       nElems;
    boolean   unique;

    public boolean isPinnable() { 
        return true;
    }

    public void writeObject(IOutputStream out) {
        out.writeInt(root);
        out.writeInt(height);
        out.writeInt(type);
        out.writeInt(nElems);
        out.writeBoolean(unique);
    }

    public void readObject(IInputStream in) {
        root = in.readInt();
        height = in.readInt();
        type = in.readInt();
        nElems = in.readInt();
        unique = in.readBoolean();
    }

    transient int updateCounter;

    static final int sizeof = ObjectHeader.sizeof + 4*4 + 1;

    public Btree() {}

    static int checkType(int elemType) { 
        if (elemType > Types.Object && elemType != Types.ArrayOfByte) { 
            throw new StorageError(StorageError.UNSUPPORTED_INDEX_TYPE, Types.getSignature(elemType));
        }
        return elemType;
    }
       
    int compareByteArrays(byte[] key, byte[] item, int offs, int length) { 
        int n = key.length >= length ? length : key.length;
        for (int i = 0; i < n; i++) { 
            int diff = key[i] - item[i+offs];
            if (diff != 0) { 
                return diff;
            }
        }
        return key.length - length;
    }

    Btree(int elemType, boolean unique) {
        this.unique = unique;
        type = checkType(elemType);
    }

    Btree(byte[] obj, int offs) {
        root = Bytes.unpack4(obj, offs);
        offs += 4;
        height = Bytes.unpack4(obj, offs);
        offs += 4;
        type = Bytes.unpack4(obj, offs);
        offs += 4;
        nElems = Bytes.unpack4(obj, offs);
        offs += 4;
        unique = obj[offs] != 0;
    }

    static final int op_done      = 0;
    static final int op_overflow  = 1;
    static final int op_underflow = 2;
    static final int op_not_found = 3;
    static final int op_duplicate = 4;
    static final int op_overwrite = 5;

    Key checkKey(Key key) { 
        if (key != null) { 
            if (key.type != type) { 
                throw new StorageError(StorageError.INCOMPATIBLE_KEY_TYPE);
            }
            if (type == Types.Object && key.ival == 0 && key.oval != null) { 
                IPersistent obj = (IPersistent)key.oval;
                getStorage().makePersistent(obj);
                key = new Key(obj, key.inclusion != 0);
            }
            if (key.oval instanceof String) { 
                key = new Key(((String)key.oval).toCharArray(), key.inclusion != 0);
            }
        }
        return key;
    }            

    public IPersistent get(Key key) { 
        key = checkKey(key);
        if (root != 0) { 
            ArrayList list = new ArrayList();
            BtreePage.find((StorageImpl)getStorage(), root, key, key, this, height, list);
            if (list.size() > 1) { 
                throw new StorageError(StorageError.KEY_NOT_UNIQUE);
            } else if (list.size() == 0) { 
                return null;
            } else { 
                return (IPersistent)list.get(0);
            }
        }
        return null;
    }

    static final IPersistent[] emptySelection = new IPersistent[0];

    public IPersistent[] prefixSearch(String key) { 
        if (Types.String != type) { 
            throw new StorageError(StorageError.INCOMPATIBLE_KEY_TYPE);
        }
        if (root != 0) { 
            ArrayList list = new ArrayList();
            BtreePage.prefixSearch((StorageImpl)getStorage(), root, key.toCharArray(), height, list);
            if (list.size() != 0) { 
                return (IPersistent[])list.toArray(new IPersistent[list.size()]);
            }
        }
        return emptySelection;
    }

    public IPersistent[] get(Key from, Key till) {
        if (root != 0) { 
            ArrayList list = new ArrayList();
            BtreePage.find((StorageImpl)getStorage(), root, checkKey(from), checkKey(till), this, height, list);
            if (list.size() != 0) { 
                return (IPersistent[])list.toArray(new IPersistent[list.size()]);
            }
        }
        return emptySelection;
    }

    public boolean put(Key key, IPersistent obj) {
        return insert(key, obj, false) >= 0;
    }

    public IPersistent set(Key key, IPersistent obj) {
        int oid = insert(key, obj, true);
        return (oid != 0) ? ((StorageImpl)getStorage()).lookupObject(oid) :  null;
    }

    final int insert(Key key, IPersistent obj, boolean overwrite) {
        StorageImpl db = (StorageImpl)getStorage();
        if (db == null) {             
            throw new StorageError(StorageError.DELETED_OBJECT);
        }
        key = checkKey(key);
        if (!obj.isPersistent()) { 
            db.makePersistent(obj);
        }
        BtreeKey ins = new BtreeKey(key, obj.getOid());
        if (root == 0) { 
            root = BtreePage.allocate(db, 0, type, ins);
            height = 1;
        } else { 
            int result = BtreePage.insert(db, root, this, ins, height, unique, overwrite);
            if (result == op_overflow) { 
                root = BtreePage.allocate(db, root, type, ins);
                height += 1;
            } else if (result == op_duplicate) { 
                return -1;
            } else if (result == op_overwrite) { 
                return ins.oldOid;
            }
        }
        updateCounter += 1;
        nElems += 1;
        modify();
        return 0;
    }

    public void remove(Key key, IPersistent obj) 
    {
        remove(new BtreeKey(checkKey(key), obj.getOid()));
    }

    
    void remove(BtreeKey rem) 
    {
        if (!removeIfExists(rem)) { 
            throw new StorageError(StorageError.KEY_NOT_FOUND);
        }
    }

    boolean removeIfExists(BtreeKey rem) 
    {            
        StorageImpl db = (StorageImpl)getStorage();
        if (db == null) {             
            throw new StorageError(StorageError.DELETED_OBJECT);
        }
        if (root == 0) {
            return false;
        }
        int result = BtreePage.remove(db, root, this, rem, height);
        if (result == op_not_found) { 
            return false;
        }
        nElems -= 1;
        if (result == op_underflow) { 
            Page pg = db.getPage(root);
            if (BtreePage.getnItems(pg) == 0) {                         
                int newRoot = 0;
                if (height != 1) { 
                    newRoot = (type == Types.String || type == Types.ArrayOfByte) 
                        ? BtreePage.getKeyStrOid(pg, 0)
                        : BtreePage.getReference(pg, BtreePage.maxItems-1);
                }
                db.freePage(root);
                root = newRoot;
                height -= 1;
            }
            db.pool.unfix(pg);
        } else if (result == op_overflow) { 
            root = BtreePage.allocate(db, root, type, rem);
            height += 1;
        }
        updateCounter += 1;
        modify();
        return true;
    }
        
    public IPersistent remove(Key key) {
        if (!unique) { 
            throw new StorageError(StorageError.KEY_NOT_UNIQUE);
        }
        BtreeKey rk = new BtreeKey(checkKey(key), 0);
        StorageImpl db = (StorageImpl)getStorage();
        remove(rk);
        return db.lookupObject(rk.oldOid);
    }
        
        
    public IPersistent get(String key) { 
        return get(new Key(key.toCharArray(), true));
    }

    public IPersistent[] getPrefix(String prefix) { 
        return get(new Key(prefix.toCharArray(), true), 
                   new Key((prefix + Character.MAX_VALUE).toCharArray(), false));
    }

    public boolean put(String key, IPersistent obj) {
        return put(new Key(key.toCharArray(), true), obj);
    }

    public IPersistent set(String key, IPersistent obj) {
        return set(new Key(key.toCharArray(), true), obj);
    }

    public void  remove(String key, IPersistent obj) {
        remove(new Key(key.toCharArray(), true), obj);
    }
    
    public IPersistent remove(String key) {
        return remove(new Key(key.toCharArray(), true));
    }

    public int size() {
        return nElems;
    }
    
    public void clear() {
        if (root != 0) { 
            BtreePage.purge((StorageImpl)getStorage(), root, type, height);
            root = 0;
            nElems = 0;
            height = 0;
            updateCounter += 1;
            modify();
        }
    }
        
    public IPersistent[] toPersistentArray() {
        IPersistent[] arr = new IPersistent[nElems];
        if (root != 0) { 
            BtreePage.traverseForward((StorageImpl)getStorage(), root, type, height, arr, 0);
        }
        return arr;
    }

    public IPersistent[] toPersistentArray(IPersistent[] arr) {
        if (arr.length < nElems) { 
            throw new IllegalArgumentException();
        }
        if (root != 0) { 
            BtreePage.traverseForward((StorageImpl)getStorage(), root, type, height, arr, 0);
        }
        if (arr.length > nElems) { 
            arr[nElems] = null;
        }
        return arr;
    }

    public void deallocate() { 
        if (root != 0) { 
            BtreePage.purge((StorageImpl)getStorage(), root, type, height);
        }
        super.deallocate();
    }

    static class BtreeEntry implements Map.Entry {
        public Object getKey() { 
            return key;
        }

        public Object getValue() { 
            return db.lookupObject(oid);
        }

        public Object setValue(Object value) { 
            throw new org.garret.perst.UnsupportedOperationException();
        }

        public boolean equals(Object o) {
            if (!(o instanceof Map.Entry)) {
                return false;
            }
            Map.Entry e = (Map.Entry)o;
            return (getKey()==null 
                    ? e.getKey()==null : getKey().equals(e.getKey())) 
                && (getValue()==null 
                    ? getValue()==null : getValue().equals(e.getValue()));
        }

        BtreeEntry(StorageImpl db, Object key, int oid) {
            this.db = db;
            this.key = key;
            this.oid = oid;
        }

        private Object      key;
        private StorageImpl db;
        private int         oid;
    }

    Object unpackKey(StorageImpl db, Page pg, int pos) { 
        byte[] data = pg.data;
        int offs =  BtreePage.firstKeyOffs + pos*ClassDescriptor.sizeof[type];
        switch (type) { 
          case Types.Boolean:
            return (data[offs] != 0) ? Boolean.TRUE : Boolean.FALSE;
          case Types.Byte:
            return new Byte(data[offs]);
          case Types.Short:
            return new Short(Bytes.unpack2(data, offs));
          case Types.Char:
            return new Character((char)Bytes.unpack2(data, offs));
          case Types.Int:
            return new Integer(Bytes.unpack4(data, offs));
          case Types.Object:
            return db.lookupObject(Bytes.unpack4(data, offs));
          case Types.Long:
            return new Long(Bytes.unpack8(data, offs));
          case Types.Date:
            return new Date(Bytes.unpack8(data, offs));
          case Types.Float:
            return new Float(Float.intBitsToFloat(Bytes.unpack4(data, offs)));
          case Types.Double:
            return new Double(Double.longBitsToDouble(Bytes.unpack8(data, offs)));
          case Types.String:
            return unpackStrKey(pg, pos);
          case Types.ArrayOfByte:
            return unpackByteArrayKey(pg, pos);
          default:
            Assert.failed("Invalid type");
        }
        return null;
    }
    
    static String unpackStrKey(Page pg, int pos) {
        int len = BtreePage.getKeyStrSize(pg, pos);
        int offs = BtreePage.firstKeyOffs + BtreePage.getKeyStrOffs(pg, pos);
        byte[] data = pg.data;
        char[] sval = new char[len];
        for (int j = 0; j < len; j++) { 
            sval[j] = (char)Bytes.unpack2(data, offs);
            offs += 2;
        }
        return new String(sval);
    }
            
    Object unpackByteArrayKey(Page pg, int pos) {
        int len = BtreePage.getKeyStrSize(pg, pos);
        int offs = BtreePage.firstKeyOffs + BtreePage.getKeyStrOffs(pg, pos);
        byte[] val = new byte[len];
        System.arraycopy(pg.data, offs, val, 0, len);
        return val;
    }
            
              
    public Iterator iterator() { 
        return iterator(null, null, ASCENT_ORDER);
    }

    public Iterator entryIterator() { 
        return entryIterator(null, null, ASCENT_ORDER);
    }


    final int compareByteArrays(Key key, Page pg, int i) { 
        return compareByteArrays((byte[])key.oval, 
                                 pg.data, 
                                 BtreePage.getKeyStrOffs(pg, i) + BtreePage.firstKeyOffs, 
                                 BtreePage.getKeyStrSize(pg, i));
    }


    class BtreeSelectionIterator extends Iterator { 
        BtreeSelectionIterator(Key from, Key till, int order) { 
            this.from = from;
            this.till = till;
            this.order = order;
            reset();
        }

        void reset() { 
            int i, l, r;
            
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
            
            pageStack = new int[h];
            posStack =  new int[h];
            
            if (type == Types.String) { 
                if (order == ASCENT_ORDER) { 
                    if (from == null) { 
                        while (--h >= 0) { 
                            posStack[sp] = 0;
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            pageId = BtreePage.getKeyStrOid(pg, 0);
                            end = BtreePage.getnItems(pg);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                    } else { 
                        while (--h > 0) { 
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            l = 0;
                            r = BtreePage.getnItems(pg);
                            while (l < r)  {
                                i = (l+r) >> 1;
                                if (BtreePage.compareStr(from, pg, i) >= from.inclusion) {
                                    l = i + 1; 
                                } else { 
                                    r = i;
                                }
                            }
                            Assert.that(r == l); 
                            posStack[sp] = r;
                            pageId = BtreePage.getKeyStrOid(pg, r);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                        pageStack[sp] = pageId;
                        Page pg = db.getPage(pageId);
                        l = 0;
                        end = r = BtreePage.getnItems(pg);
                        while (l < r)  {
                            i = (l+r) >> 1;
                            if (BtreePage.compareStr(from, pg, i) >= from.inclusion) {
                                l = i + 1; 
                            } else { 
                                r = i;
                            }
                        }
                        Assert.that(r == l); 
                        if (r == end) {
                            sp += 1;
                            gotoNextItem(pg, r-1);
                        } else { 
                            posStack[sp++] = r;
                            db.pool.unfix(pg);
                        }
                    }
                    if (sp != 0 && till != null) { 
                        Page pg = db.getPage(pageStack[sp-1]);
                        if (-BtreePage.compareStr(till, pg, posStack[sp-1]) >= till.inclusion) { 
                            sp = 0;
                        }
                        db.pool.unfix(pg);
                    }
                } else { // descent order
                    if (till == null) { 
                        while (--h > 0) { 
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            posStack[sp] = BtreePage.getnItems(pg);
                            pageId = BtreePage.getKeyStrOid(pg, posStack[sp]);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                        pageStack[sp] = pageId;
                        Page pg = db.getPage(pageId);
                        posStack[sp++] = BtreePage.getnItems(pg)-1;
                        db.pool.unfix(pg);
                    } else {
                        while (--h > 0) { 
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            l = 0;
                            r = BtreePage.getnItems(pg);
                            while (l < r)  {
                                i = (l+r) >> 1;
                                if (BtreePage.compareStr(till, pg, i) >= 1-till.inclusion) {
                                    l = i + 1; 
                                } else { 
                                    r = i;
                                }
                            }
                            Assert.that(r == l); 
                            posStack[sp] = r;
                            pageId = BtreePage.getKeyStrOid(pg, r);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                        pageStack[sp] = pageId;
                        Page pg = db.getPage(pageId);
                        l = 0;
                        r = BtreePage.getnItems(pg);
                        while (l < r)  {
                            i = (l+r) >> 1;
                            if (BtreePage.compareStr(till, pg, i) >= 1-till.inclusion) {
                                l = i + 1; 
                            } else { 
                                r = i;
                            }
                        }
                        Assert.that(r == l); 
                        if (r == 0) {
                            sp += 1;
                            gotoNextItem(pg, r);
                        } else { 
                            posStack[sp++] = r-1;
                            db.pool.unfix(pg);
                        }
                    }
                    if (sp != 0 && from != null) { 
                        Page pg = db.getPage(pageStack[sp-1]);
                        if (BtreePage.compareStr(from, pg, posStack[sp-1]) >= from.inclusion) { 
                            sp = 0;
                        }
                        db.pool.unfix(pg);
                    }
                }
            } else if (type == Types.ArrayOfByte) { 
                if (order == ASCENT_ORDER) { 
                    if (from == null) { 
                        while (--h >= 0) { 
                            posStack[sp] = 0;
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            pageId = BtreePage.getKeyStrOid(pg, 0);
                            end = BtreePage.getnItems(pg);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                    } else { 
                        while (--h > 0) { 
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            l = 0;
                            r = BtreePage.getnItems(pg);
                            while (l < r)  {
                                i = (l+r) >> 1;
                                if (compareByteArrays(from, pg, i) >= from.inclusion) {
                                    l = i + 1; 
                                } else { 
                                    r = i;
                                }
                            }
                            Assert.that(r == l); 
                            posStack[sp] = r;
                            pageId = BtreePage.getKeyStrOid(pg, r);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                        pageStack[sp] = pageId;
                        Page pg = db.getPage(pageId);
                        l = 0;
                        end = r = BtreePage.getnItems(pg);
                        while (l < r)  {
                            i = (l+r) >> 1;
                            if (compareByteArrays(from, pg, i) >= from.inclusion) {
                                l = i + 1; 
                            } else { 
                                r = i;
                            }
                        }
                        Assert.that(r == l); 
                        if (r == end) {
                            sp += 1;
                            gotoNextItem(pg, r-1);
                        } else { 
                            posStack[sp++] = r;
                            db.pool.unfix(pg);
                        }
                    }
                    if (sp != 0 && till != null) { 
                        Page pg = db.getPage(pageStack[sp-1]);
                        if (-compareByteArrays(till, pg, posStack[sp-1]) >= till.inclusion) { 
                            sp = 0;
                        }
                        db.pool.unfix(pg);
                    }
                } else { // descent order
                    if (till == null) { 
                        while (--h > 0) { 
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            posStack[sp] = BtreePage.getnItems(pg);
                            pageId = BtreePage.getKeyStrOid(pg, posStack[sp]);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                        pageStack[sp] = pageId;
                        Page pg = db.getPage(pageId);
                        posStack[sp++] = BtreePage.getnItems(pg)-1;
                        db.pool.unfix(pg);
                    } else {
                        while (--h > 0) { 
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            l = 0;
                            r = BtreePage.getnItems(pg);
                            while (l < r)  {
                                i = (l+r) >> 1;
                                if (compareByteArrays(till, pg, i) >= 1-till.inclusion) {
                                    l = i + 1; 
                                } else { 
                                    r = i;
                                }
                            }
                            Assert.that(r == l); 
                            posStack[sp] = r;
                            pageId = BtreePage.getKeyStrOid(pg, r);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                        pageStack[sp] = pageId;
                        Page pg = db.getPage(pageId);
                        l = 0;
                        r = BtreePage.getnItems(pg);
                        while (l < r)  {
                            i = (l+r) >> 1;
                            if (compareByteArrays(till, pg, i) >= 1-till.inclusion) {
                                l = i + 1; 
                            } else { 
                                r = i;
                            }
                        }
                        Assert.that(r == l); 
                        if (r == 0) {
                            sp += 1;
                            gotoNextItem(pg, r);
                        } else { 
                            posStack[sp++] = r-1;
                            db.pool.unfix(pg);
                        }
                    }
                    if (sp != 0 && from != null) { 
                        Page pg = db.getPage(pageStack[sp-1]);
                        if (compareByteArrays(from, pg, posStack[sp-1]) >= from.inclusion) { 
                            sp = 0;
                        }
                        db.pool.unfix(pg);
                    }
                }
            } else { // scalar type
                if (order == ASCENT_ORDER) { 
                    if (from == null) { 
                        while (--h >= 0) { 
                            posStack[sp] = 0;
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            pageId = BtreePage.getReference(pg, BtreePage.maxItems-1);
                            end = BtreePage.getnItems(pg);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                    } else { 
                        while (--h > 0) { 
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            l = 0;
                            r = BtreePage.getnItems(pg);
                            while (l < r)  {
                                i = (l+r) >> 1;
                                if (BtreePage.compare(from, pg, i) >= from.inclusion) {
                                    l = i + 1; 
                                } else { 
                                    r = i;
                                }
                            }
                            Assert.that(r == l); 
                            posStack[sp] = r;
                            pageId = BtreePage.getReference(pg, BtreePage.maxItems-1-r);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                        pageStack[sp] = pageId;
                        Page pg = db.getPage(pageId);
                        l = 0;
                        r = end = BtreePage.getnItems(pg);
                        while (l < r)  {
                            i = (l+r) >> 1;
                            if (BtreePage.compare(from, pg, i) >= from.inclusion) {
                                l = i + 1; 
                            } else { 
                                r = i;
                            }
                        }
                        Assert.that(r == l); 
                        if (r == end) {
                            sp += 1;
                            gotoNextItem(pg, r-1);
                        } else { 
                            posStack[sp++] = r;
                            db.pool.unfix(pg);
                        }
                    }
                    if (sp != 0 && till != null) { 
                        Page pg = db.getPage(pageStack[sp-1]);
                        if (-BtreePage.compare(till, pg, posStack[sp-1]) >= till.inclusion) { 
                            sp = 0;
                        }
                        db.pool.unfix(pg);
                    }
                } else { // descent order
                    if (till == null) { 
                        while (--h > 0) { 
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            posStack[sp] = BtreePage.getnItems(pg);
                            pageId = BtreePage.getReference(pg, BtreePage.maxItems-1-posStack[sp]);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                        pageStack[sp] = pageId;
                        Page pg = db.getPage(pageId);
                        posStack[sp++] = BtreePage.getnItems(pg)-1;
                        db.pool.unfix(pg);
                     } else {
                        while (--h > 0) { 
                            pageStack[sp] = pageId;
                            Page pg = db.getPage(pageId);
                            l = 0;
                            r = BtreePage.getnItems(pg);
                            while (l < r)  {
                                i = (l+r) >> 1;
                                if (BtreePage.compare(till, pg, i) >= 1-till.inclusion) {
                                    l = i + 1; 
                                } else { 
                                    r = i;
                                }
                            }
                            Assert.that(r == l); 
                            posStack[sp] = r;
                            pageId = BtreePage.getReference(pg, BtreePage.maxItems-1-r);
                            db.pool.unfix(pg);
                            sp += 1;
                        }
                        pageStack[sp] = pageId;
                        Page pg = db.getPage(pageId);
                        l = 0;
                        r = BtreePage.getnItems(pg);
                        while (l < r)  {
                            i = (l+r) >> 1;
                            if (BtreePage.compare(till, pg, i) >= 1-till.inclusion) {
                                l = i + 1; 
                            } else { 
                                r = i;
                            }
                        }
                        Assert.that(r == l);  
                        if (r == 0) { 
                            sp += 1;
                            gotoNextItem(pg, r);
                        } else { 
                            posStack[sp++] = r-1;
                            db.pool.unfix(pg);
                        }
                    }
                    if (sp != 0 && from != null) { 
                        Page pg = db.getPage(pageStack[sp-1]);
                        if (BtreePage.compare(from, pg, posStack[sp-1]) >= from.inclusion) { 
                            sp = 0;
                        }
                        db.pool.unfix(pg);
                    }
                }
            }
        }
                

        public boolean hasNext() {
            if (counter != updateCounter) { 
                if (((StorageImpl)getStorage()).concurrentIterator) { 
                    refresh();
                } else { 
                    throw new ConcurrentModificationException();
                }
            }
            return sp != 0;
        }

        public Object next() {
            if (!hasNext()) { 
                throw new NoSuchElementException();
            }
            StorageImpl db = (StorageImpl)getStorage();
            int pos = posStack[sp-1];   
            currPos = pos;
            currPage = pageStack[sp-1];
            Page pg = db.getPage(currPage);
            Object curr = getCurrent(pg, pos);
            if (db.concurrentIterator) { 
                currKey = getCurrentKey(pg, pos);
            }
            gotoNextItem(pg, pos);
            return curr;
        }

        public int nextOid() {
           if (!hasNext()) { 
                throw new NoSuchElementException();
            }
            StorageImpl db = (StorageImpl)getStorage();
            int pos = posStack[sp-1];   
            currPos = pos;
            currPage = pageStack[sp-1];
            Page pg = db.getPage(currPage);
            int oid = getReference(pg, pos);
            if (db.concurrentIterator) { 
                currKey = getCurrentKey(pg, pos);
            }
            gotoNextItem(pg, pos);
            return oid;
        }

        private int getReference(Page pg, int pos) { 
            return (type == Types.String || type == Types.ArrayOfByte)
                ? BtreePage.getKeyStrOid(pg, pos)
                : BtreePage.getReference(pg, BtreePage.maxItems-1-pos);
        }


        protected Object getCurrent(Page pg, int pos) { 
            StorageImpl db = (StorageImpl)getStorage();
            return db.lookupObject(getReference(pg, pos));
        }

        protected final void gotoNextItem(Page pg, int pos)
        {
            StorageImpl db = (StorageImpl)getStorage();
            if (type == Types.String) { 
                if (order == ASCENT_ORDER) {                     
                    if (++pos == end) { 
                        while (--sp != 0) { 
                            db.pool.unfix(pg);
                            pos = posStack[sp-1];
                            pg = db.getPage(pageStack[sp-1]);
                            if (++pos <= BtreePage.getnItems(pg)) {
                                posStack[sp-1] = pos;
                                do { 
                                    int pageId = BtreePage.getKeyStrOid(pg, pos);
                                    db.pool.unfix(pg);
                                    pg = db.getPage(pageId);
                                    end = BtreePage.getnItems(pg);
                                    pageStack[sp] = pageId;
                                    posStack[sp] = pos = 0;
                                } while (++sp < pageStack.length);
                                break;
                            }
                        }
                    } else { 
                        posStack[sp-1] = pos;
                    }
                    if (sp != 0 && till != null && -BtreePage.compareStr(till, pg, pos) >= till.inclusion) { 
                        sp = 0;
                    }
                } else { // descent order
                    if (--pos < 0) { 
                        while (--sp != 0) { 
                            db.pool.unfix(pg);
                            pos = posStack[sp-1];
                            pg = db.getPage(pageStack[sp-1]);
                            if (--pos >= 0) {
                                posStack[sp-1] = pos;
                                do { 
                                    int pageId = BtreePage.getKeyStrOid(pg, pos);
                                    db.pool.unfix(pg);
                                    pg = db.getPage(pageId);
                                    pageStack[sp] = pageId;
                                    posStack[sp] = pos = BtreePage.getnItems(pg);
                                } while (++sp < pageStack.length);
                                posStack[sp-1] = --pos;
                                break;
                            }
                        }
                    } else { 
                        posStack[sp-1] = pos;
                    }
                    if (sp != 0 && from != null && BtreePage.compareStr(from, pg, pos) >= from.inclusion) { 
                        sp = 0;
                    }                    
                }
            } else if (type == Types.ArrayOfByte) { 
                if (order == ASCENT_ORDER) {                     
                    if (++pos == end) { 
                        while (--sp != 0) { 
                            db.pool.unfix(pg);
                            pos = posStack[sp-1];
                            pg = db.getPage(pageStack[sp-1]);
                            if (++pos <= BtreePage.getnItems(pg)) {
                                posStack[sp-1] = pos;
                                do { 
                                    int pageId = BtreePage.getKeyStrOid(pg, pos);
                                    db.pool.unfix(pg);
                                    pg = db.getPage(pageId);
                                    end = BtreePage.getnItems(pg);
                                    pageStack[sp] = pageId;
                                    posStack[sp] = pos = 0;
                                } while (++sp < pageStack.length);
                                break;
                            }
                        }
                    } else { 
                        posStack[sp-1] = pos;
                    }
                    if (sp != 0 && till != null && -compareByteArrays(till, pg, pos) >= till.inclusion) { 
                        sp = 0;
                    }
                } else { // descent order
                    if (--pos < 0) { 
                        while (--sp != 0) { 
                            db.pool.unfix(pg);
                            pos = posStack[sp-1];
                            pg = db.getPage(pageStack[sp-1]);
                            if (--pos >= 0) {
                                posStack[sp-1] = pos;
                                do { 
                                    int pageId = BtreePage.getKeyStrOid(pg, pos);
                                    db.pool.unfix(pg);
                                    pg = db.getPage(pageId);
                                    pageStack[sp] = pageId;
                                    posStack[sp] = pos = BtreePage.getnItems(pg);
                                } while (++sp < pageStack.length);
                                posStack[sp-1] = --pos;
                                break;
                            }
                        }
                    } else { 
                        posStack[sp-1] = pos;
                    }
                    if (sp != 0 && from != null && compareByteArrays(from, pg, pos) >= from.inclusion) { 
                        sp = 0;
                    }                    
                }
            } else { // scalar type
                if (order == ASCENT_ORDER) {                     
                    if (++pos == end) { 
                        while (--sp != 0) { 
                            db.pool.unfix(pg);
                            pos = posStack[sp-1];
                            pg = db.getPage(pageStack[sp-1]);
                            if (++pos <= BtreePage.getnItems(pg)) {
                                posStack[sp-1] = pos;
                                do { 
                                    int pageId = BtreePage.getReference(pg, BtreePage.maxItems-1-pos);
                                    db.pool.unfix(pg);
                                    pg = db.getPage(pageId);
                                    end = BtreePage.getnItems(pg);
                                    pageStack[sp] = pageId;
                                    posStack[sp] = pos = 0;
                                } while (++sp < pageStack.length);
                                break;
                            }
                        }
                    } else { 
                        posStack[sp-1] = pos;
                    }
                    if (sp != 0 && till != null && -BtreePage.compare(till, pg, pos) >= till.inclusion) { 
                        sp = 0;
                    }
                } else { // descent order
                    if (--pos < 0) { 
                        while (--sp != 0) { 
                            db.pool.unfix(pg);
                            pos = posStack[sp-1];
                            pg = db.getPage(pageStack[sp-1]);
                            if (--pos >= 0) {
                                posStack[sp-1] = pos;
                                do { 
                                    int pageId = BtreePage.getReference(pg, BtreePage.maxItems-1-pos);
                                    db.pool.unfix(pg);
                                    pg = db.getPage(pageId);
                                    pageStack[sp] = pageId;
                                    posStack[sp] = pos = BtreePage.getnItems(pg);
                                } while (++sp < pageStack.length);
                                posStack[sp-1] = --pos;
                                break;
                            }
                        }
                    } else { 
                        posStack[sp-1] = pos;
                    }
                    if (sp != 0 && from != null && BtreePage.compare(from, pg, pos) >= from.inclusion) { 
                        sp = 0;
                    }                    
                }
            }
            if (db.concurrentIterator && sp != 0) { 
                nextKey = getCurrentKey(pg, pos);
            }
            db.pool.unfix(pg);
        }


        private void refresh() { 
            if (sp != 0) { 
                if (nextKey == null) { 
                    reset();
                } else { 
                    if (order == ASCENT_ORDER) { 
                        from = nextKey.key;
                    } else { 
                        till = nextKey.key;
                    }
                    int next = nextKey.oid;
                    reset();
                    StorageImpl db = (StorageImpl)getStorage();
                    while (true) { 
                        int pos = posStack[sp-1];   
                        Page pg = db.getPage(pageStack[sp-1]);
                        int oid = type == Types.String || type == Types.ArrayOfByte
                            ? BtreePage.getKeyStrOid(pg, pos)
                            : BtreePage.getReference(pg, BtreePage.maxItems-1-pos);
                        if (oid != next) { 
                            gotoNextItem(pg, pos);
                        } else { 
                            db.pool.unfix(pg);
                            break;
                        }
                    }
                }
            }
            counter = updateCounter;
        }
            
        BtreeKey getCurrentKey(Page pg, int pos) { 
            BtreeKey key;
            switch (type) { 
            case Types.String:
                key = new BtreeKey(null, BtreePage.getKeyStrOid(pg, pos));
                key.getStr(pg, pos);
                break;
            case Types.ArrayOfByte:
                key = new BtreeKey(null, BtreePage.getKeyStrOid(pg, pos));
                key.getByteArray(pg, pos);
                break;
            default:
                key = new BtreeKey(null, BtreePage.getReference(pg, BtreePage.maxItems-1-pos));
                key.extract(pg, BtreePage.firstKeyOffs + pos*ClassDescriptor.sizeof[type], type);
            }
            return key;
        }

        public void remove() { 
            if (currPage == 0) { 
                throw new NoSuchElementException();
            }
            StorageImpl db = (StorageImpl)getStorage();
            if (!db.concurrentIterator) { 
                if (counter != updateCounter) { 
                    throw new ConcurrentModificationException();
                }
                Page pg = db.getPage(currPage);
                currKey = getCurrentKey(pg, currPos);
                db.pool.unfix(pg);
                if (sp != 0) { 
                    int pos = posStack[sp-1];   
                    pg = db.getPage(pageStack[sp-1]);
                    nextKey = getCurrentKey(pg, pos);
                    db.pool.unfix(pg);
                }
                // System.out.println("Deleted key=" + deletedKey.key.ival + ", next key=" + nextKey.key.ival);
            }    
            Btree.this.removeIfExists(currKey);
            refresh();
            currPage = 0;
        }

        int[]       pageStack;
        int[]       posStack;
        int         currPage;
        int         currPos;
        int         sp;
        int         end;
        Key         from;
        Key         till;
        int         order;
        int         counter;
        BtreeKey    nextKey;
        BtreeKey    currKey;
    }

    class BtreeSelectionEntryIterator extends BtreeSelectionIterator { 
        BtreeSelectionEntryIterator(Key from, Key till, int order) {
            super(from, till, order);
        }
            
        protected Object getCurrent(Page pg, int pos) { 
            StorageImpl db = (StorageImpl)getStorage();
            switch (type) { 
              case Types.String:
                return new BtreeEntry(db, unpackStrKey(pg, pos), BtreePage.getKeyStrOid(pg, pos));
              case Types.ArrayOfByte:
                return new BtreeEntry(db, unpackByteArrayKey(pg, pos), BtreePage.getKeyStrOid(pg, pos));
              default:
                return new BtreeEntry(db, unpackKey(db, pg, pos), BtreePage.getReference(pg, BtreePage.maxItems-1-pos));
            }
        }
    }

    class BtreeEntryStartFromIterator extends BtreeSelectionEntryIterator
    { 
        BtreeEntryStartFromIterator(int start, int order) {
            super(null, null, order);
            this.start = start;
            reset();
        }
        
        void reset() { 
            super.reset();
            int skip = (order == ASCENT_ORDER) ? start : nElems - start - 1;
            while (--skip >= 0 && hasNext()) {
                next();
            }
        }
        
        int start;
    }
           
    public Iterator iterator(Key from, Key till, int order) { 
        return new BtreeSelectionIterator(checkKey(from), checkKey(till), order);
    }

    public Iterator prefixIterator(String prefix) {
        return iterator(new Key(prefix.toCharArray()), 
                        new Key((prefix + Character.MAX_VALUE).toCharArray(), false), ASCENT_ORDER);
    }


    public Iterator entryIterator(Key from, Key till, int order) { 
        return new BtreeSelectionEntryIterator(checkKey(from), checkKey(till), order);
    }

    public Object getAt(int i) {
        Iterator iterator;
        if (i < 0 || i >= nElems) {
            throw new IndexOutOfBoundsException("Position " + i + ", index size "  + nElems);
        }            
        if (i <= (nElems/2)) {
            iterator = entryIterator(null, null, ASCENT_ORDER);
            while (--i >= 0) { 
                iterator.next();
            }
        } else {
            iterator = entryIterator(null, null, DESCENT_ORDER);
            i -= nElems;
            while (++i < 0) { 
                iterator.next();
            }
        }
        return ((Map.Entry)iterator.next()).getValue();   
    }

    public Iterator entryIterator(int start, int order) {
        return new BtreeEntryStartFromIterator(start, order);
    }
}

