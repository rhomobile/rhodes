package org.garret.perst.impl;
import  org.garret.perst.*;
import  java.util.*;

public class LinkImpl implements EmbeddedLink { 
    private final void modify() { 
        if (owner != null) { 
            owner.modify();
        }
    }

    public int size() {
        return used;
    }

    public IPersistent get(int i) {
        if (i < 0 || i >= used) { 
            throw new IndexOutOfBoundsException();
        }
        return loadElem(i);
    }

    public IPersistent getRaw(int i) {
        if (i < 0 || i >= used) { 
            throw new IndexOutOfBoundsException();
        }
        return arr[i];
    }

    public void pin() { 
        for (int i = 0, n = used; i < n; i++) { 
            arr[i] = loadElem(i);
        }
    }

    public void unpin() { 
        for (int i = 0, n = used; i < n; i++) { 
            IPersistent elem = arr[i];
            if (elem != null && !elem.isRaw() && elem.isPersistent()) { 
                arr[i] = new PersistentStub(elem.getStorage(), elem.getOid());
            }
        }
    }
                

    public void set(int i, IPersistent obj) {
        if (i < 0 || i >= used) { 
            throw new IndexOutOfBoundsException();
        }
        arr[i] = obj;
        modify();
    }

    public void remove(int i) {
        if (i < 0 || i >= used) { 
            throw new IndexOutOfBoundsException();
        }
        used -= 1;
        System.arraycopy(arr, i+1, arr, i, used-i);
        arr[used] = null;
        modify();
    }

    public void setSize(int newSize) { 
        if (newSize < used) { 
            for (int i = used; --i >= newSize; arr[i] = null);
        } else { 
            reserveSpace(newSize - used);            
        }
        used = newSize;
        modify();
    }

    void reserveSpace(int len) { 
        if (used + len > arr.length) { 
            IPersistent[] newArr = new IPersistent[used + len > arr.length*2 ? used + len : arr.length*2];
            System.arraycopy(arr, 0, newArr, 0, used);
            arr = newArr;
        }
        modify();
    }

    public void insert(int i, IPersistent obj) { 
         if (i < 0 || i > used) { 
            throw new IndexOutOfBoundsException();
        }
        reserveSpace(1);
        System.arraycopy(arr, i, arr, i+1, used-i);
        arr[i] = obj;
        used += 1;
    }

    public void add(IPersistent obj) {
        reserveSpace(1);
        arr[used++] = obj;
    }

    public void addAll(IPersistent[] a) {
        addAll(a, 0, a.length);
    }
    
    public void addAll(IPersistent[] a, int from, int length) {
        reserveSpace(length);
        System.arraycopy(a, from, arr, used, length);
        used += length;
    }

    public void addAll(Link link) {        
        int n = link.size();
        reserveSpace(n);
        for (int i = 0, j = used; i < n; i++, j++) { 
            arr[j] = link.getRaw(i);
        }
        used += n;
    }

    public IPersistent[] toArray() {
        IPersistent[] a = new IPersistent[used];
        for (int i = used; --i >= 0;) { 
            a[i] = loadElem(i);
        }
        return a;
    }
    
    public IPersistent[] toRawArray() {
        return arr;
    }

    public IPersistent[] toArray(IPersistent[] arr) {
        if (arr.length < used) { 
            throw new IllegalArgumentException();
        }
        for (int i = used; --i >= 0;) { 
            arr[i] = loadElem(i);
        }
        if (arr.length > used) { 
            arr[used] = null;
        }
        return arr;
    }
    
    public boolean contains(IPersistent obj) {
        for (int i = used; --i >= 0;) {           
            if (loadElem(i).equals(obj)) {
                return true;
            }
        }
        return false;
    }

    public boolean containsObject(IPersistent obj) {
        return indexOf(obj) >= 0;
    }

    public int indexOf(IPersistent obj) {
        int oid;
        if (obj != null && (oid = ((IPersistent)obj).getOid()) != 0) { 
            for (int i = used; --i >= 0;) {
                IPersistent elem = arr[i];
                if (elem != null && elem.getOid() == oid) {
                    return i;
                }
            }
        } else { 
            for (int i = used; --i >= 0;) {
                if (arr[i] == obj) {
                    return i;
                }
            }
        }
        return -1;
    }
    
    public boolean containsElement(int i, IPersistent obj) {
        IPersistent elem = arr[i];
        return elem == obj || (elem != null && elem.getOid() != 0 && elem.getOid() == obj.getOid());
    }

    public void clear() { 
        for (int i = used; --i >= 0;) { 
            arr[i] = null;
        }
        used = 0;
        modify();
    }

    static class LinkIterator extends Iterator { 
        private Link link;
        private int  i;

        LinkIterator(Link link) { 
            this.link = link;
        }

        public boolean hasNext() {
            return i < link.size();
        }

        public Object next() throws NoSuchElementException { 
            if (!hasNext()) { 
                throw new NoSuchElementException();
            }
            return link.get(i++);
        }

        public int nextOid() throws NoSuchElementException { 
            if (!hasNext()) { 
                throw new NoSuchElementException();
            }
            return link.getRaw(i++).getOid();
        }

        public void remove() {
            link.remove(i);
        }
    }

    public Iterator iterator() { 
        return new LinkIterator(this);
    }

    private final IPersistent loadElem(int i) 
    {
        IPersistent elem = arr[i];
        if (elem != null && elem.isRaw()) { 
            // arr[i] = elem = ((StorageImpl)elem.getStorage()).lookupObject(elem.getOid(), null);
            elem = ((StorageImpl)elem.getStorage()).lookupObject(elem.getOid());
        }
        return elem;
    }

    public void setOwner(IPersistent obj) { 
        owner = obj;
    }

    public IPersistent getOwner() { 
        return owner;
    }
    

    LinkImpl() {}

    public LinkImpl(int initSize) {
        this.arr = new IPersistent[initSize];
    }

    public LinkImpl(IPersistent[] arr, IPersistent owner) { 
        this.arr = arr;
        this.owner = owner;
        used = arr.length;
    }

    IPersistent[] arr;
    int           used;
    transient IPersistent owner;
}
