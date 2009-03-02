package org.garret.perst.impl;
import  org.garret.perst.*;

public class StrongHashTable implements OidHashTable { 
    Entry table[];
    static final float loadFactor = 1f;
    int nPinnedObjects;
    int count;
    int tick;
    int threshold;
    boolean flushing;

    public void init(int initialCapacity, int nPinnedObjects) {
        this.nPinnedObjects = nPinnedObjects;
        threshold = (int)(initialCapacity * loadFactor);
        tick = 0;
        if (initialCapacity != 0) { 
            table = new Entry[initialCapacity];
        }
    }

    public synchronized boolean remove(int oid) {
        Entry tab[] = table;
        int index = (oid & 0x7FFFFFFF) % tab.length;
        for (Entry e = tab[index], prev = null; e != null; prev = e, e = e.next) {
            if (e.oid == oid) {
                e.obj = null;
                count -= 1;
                if (prev != null) {
                    prev.next = e.next;
                } else {
                    tab[index] = e.next;
                }
                return true;
            }
        }
        return false;
    }

    public synchronized void put(int oid, IPersistent obj) { 
        Entry tab[] = table;
        int index = (oid & 0x7FFFFFFF) % tab.length;
        for (Entry e = tab[index]; e != null; e = e.next) {
            if (e.oid == oid) {
                e.obj = obj;
                e.tick = ++tick;
                return;
            }
        }
        if (count >= threshold && !flushing) {
            // Rehash the table if the threshold is exceeded
            rehash();
            tab = table;
            index = (oid & 0x7FFFFFFF) % tab.length;
        } 

        // Creates the new entry.
        tab[index] = new Entry(oid, obj, tab[index], ++tick);
        count++;
    }

    public synchronized IPersistent get(int oid) {
        Entry tab[] = table;
        int index = (oid & 0x7FFFFFFF) % tab.length;
        for (Entry e = tab[index] ; e != null ; e = e.next) {
            if (e.oid == oid) {
                e.tick = ++tick;
                return e.obj;
            }
        }
        return null;
    }
    
    void rehash() {
        int oldCapacity = table.length;
        Entry oldMap[] = table;
        int i;

        int newCapacity = oldCapacity * 2 + 1;
        Entry newMap[] = new Entry[newCapacity];

        threshold = (int)(newCapacity * loadFactor);
        table = newMap;

        for (i = oldCapacity; --i >= 0 ;) {
            for (Entry old = oldMap[i]; old != null; ) {
                Entry e = old;
                old = old.next;

                int index = (e.oid & 0x7FFFFFFF) % newCapacity;
                e.next = newMap[index];
                newMap[index] = e;
            }
        }
    }

    public synchronized void flush() {
        Entry tab[] = table;
        flushing = true;
        for (int i = 0; i < tab.length; i++) { 
            for (Entry e = tab[i]; e != null; e = e.next) { 
                if (e.obj.isModified()) { 
                    e.obj.store();
                }
            }
        }
        flushing = false;
    }

    public synchronized void clear() {
        Entry tab[] = table;
        int tick = this.tick;
        for (int i = 0; i < tab.length; i++) {
            Entry prev = null;
            for (Entry e = tab[i]; e != null; e = e.next) { 
                if (tick - e.tick < nPinnedObjects && e.obj.isPinnable()) { 
                    if (prev == null) { 
                        tab[i] = e;
                    } else { 
                        prev.next = e;
                    }
                    prev = e;
                } else { 
                    count -= 1;
                }
            }
            if (prev == null) { 
                tab[i] = null;
            } else { 
                prev.next = null;
            }
        }
    }

    public synchronized void invalidate() {
        for (int i = 0; i < table.length; i++) { 
            for (Entry e = table[i]; e != null; e = e.next) { 
                if (e.obj.isModified()) { 
                    e.obj.invalidate();
                }
            }
            table[i] = null;
        }
        count = 0;
    }

    public int size() { 
        return count;
    }

    static class Entry { 
        Entry         next;
        IPersistent   obj;
        int           oid;
        int           tick;
        
        Entry(int oid, IPersistent obj, Entry chain, int tick) { 
            next = chain;
            this.oid = oid;
            this.obj = obj;
            this.tick = tick;
        }
    }

    public void clearDirty(IPersistent obj) {}
    public void setDirty(IPersistent obj) {}
}







