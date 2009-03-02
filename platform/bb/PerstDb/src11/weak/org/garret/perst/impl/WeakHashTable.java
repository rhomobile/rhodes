package org.garret.perst.impl;
import  org.garret.perst.*;
import  java.lang.ref.*;


public class WeakHashTable implements OidHashTable { 
    Entry table[];
    static final float loadFactor = 0.75f;
    int count;
    int threshold;
    boolean flushing;

    static final int FINALIZATION_DELAY = 100;

    void runFinalization() { 
        // System.runFinalization() is not available in CDC
        //System.runFinalization();  
        try { 
            Thread.sleep(FINALIZATION_DELAY);
        } catch (InterruptedException x) {}
    }

    public void init(int initialCapacity, int nPinnedObjects) {
        threshold = (int)(initialCapacity * loadFactor);
        table = new Entry[initialCapacity];
    }

    public synchronized boolean remove(int oid) {
        Entry tab[] = table;
        int index = (oid & 0x7FFFFFFF) % tab.length;
        for (Entry e = tab[index], prev = null; e != null; prev = e, e = e.next) {
            if (e.oid == oid) {
                if (prev != null) {
                    prev.next = e.next;
                } else {
                    tab[index] = e.next;
                }
                e.clear();
                count -= 1;
                return true;
            }
        }
        return false;
    }

    protected Reference createReference(Object obj) { 
        return new WeakReference(obj);
    }

    public synchronized void put(int oid, IPersistent obj) { 
        Reference ref = createReference(obj);
        Entry tab[] = table;
        int index = (oid & 0x7FFFFFFF) % tab.length;
        for (Entry e = tab[index]; e != null; e = e.next) {
            if (e.oid == oid) {
                e.ref = ref;
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
        tab[index] = new Entry(oid, ref, tab[index]);
        count += 1;
    }

    public IPersistent get(int oid) {
        while (true) { 
            cs:synchronized(this) { 
                Entry tab[] = table;
                int index = (oid & 0x7FFFFFFF) % tab.length;
                for (Entry e = tab[index]; e != null; e = e.next) {
                    if (e.oid == oid) {
                        IPersistent obj = (IPersistent)e.ref.get();
                        if (obj == null) { 
                            if (e.dirty != 0) { 
                                break cs;
                            }
                        } else if (obj.isDeleted()) {
                            e.ref.clear();
                            return null;
                        }
                        return obj;
                    }
                }
                return null;
            }
            runFinalization();
        } 
    }
    
    public void flush() {
        while (true) { 
            cs:synchronized(this) { 
                flushing = true;
                for (int i = 0; i < table.length; i++) { 
                    for (Entry e = table[i]; e != null; e = e.next) { 
                        IPersistent obj = (IPersistent)e.ref.get();
                        if (obj != null) { 
                            if (obj.isModified()) { 
                                obj.store();
                            }
                        } else if (e.dirty != 0) { 
                            break cs;
                        }
                    }
                }
                flushing = false;
                if (count >= threshold) {
                    // Rehash the table if the threshold is exceeded
                    rehash();
                }
                return;
            }
            runFinalization();
        }
    }

    public void invalidate() {
        while (true) { 
            cs:synchronized(this) { 
                for (int i = 0; i < table.length; i++) { 
                    for (Entry e = table[i]; e != null; e = e.next) { 
                        IPersistent obj = (IPersistent)e.ref.get();
                        if (obj != null) { 
                            if (obj.isModified()) { 
                                e.dirty = 0;
                                obj.invalidate();
                            }
                        } else if (e.dirty != 0) { 
                            break cs;
                        }
                    }
                    table[i] = null;
                }
                count = 0;
                return;
            }                                   
            runFinalization();
        }
    }

    public synchronized void clear() {
        /* Explicit cleaning is not needed for this kind of cache 
        Entry tab[] = table;
        for (int i = 0; i < tab.length; i++) { 
            tab[i] = null;
        }
        count = 0;
        */
    }

    void rehash() {
        int oldCapacity = table.length;
        Entry oldMap[] = table;
        int i;

        for (i = oldCapacity; --i >= 0;) {
            Entry e, next, prev;
            for (prev = null, e = oldMap[i]; e != null; e = next) { 
                next = e.next;
                IPersistent obj = (IPersistent)e.ref.get();
                if ((obj == null || obj.isDeleted()) && e.dirty == 0) { 
                    count -= 1;
                    e.clear();
                    if (prev == null) { 
                        oldMap[i] = next;
                    } else { 
                        prev.next = next;
                    }
                } else { 
                    prev = e;
                }
            }
        }
        if (count <= (threshold >>> 1)) {
            return;
        }
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

    public synchronized void setDirty(IPersistent obj) {
        int oid = obj.getOid();
        Entry tab[] = table;
        int index = (oid & 0x7FFFFFFF) % tab.length;
        for (Entry e = tab[index]; e != null ; e = e.next) {
            if (e.oid == oid) {
                e.dirty += 1;
                return;
            }
        }
    }

    public synchronized void clearDirty(IPersistent obj) {
        int oid = obj.getOid();
        Entry tab[] = table;
        int index = (oid & 0x7FFFFFFF) % tab.length;
        for (Entry e = tab[index]; e != null ; e = e.next) {
            if (e.oid == oid) {
                if (e.dirty > 0) { 
                    e.dirty -= 1;
                }
                return;
            }
        }
    }

    public int size() { 
        return count;
    }

    static class Entry { 
        Entry     next;
        Reference ref;
        int       oid;
        int       dirty;
        
        void clear() { 
            ref.clear();
            ref = null;
            dirty = 0;
            next = null;
        }

        Entry(int oid, Reference ref, Entry chain) { 
            next = chain;
            this.oid = oid;
            this.ref = ref;
        }
    }
}

