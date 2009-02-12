package org.garret.perst;

/**
 * This class is used to make it possible to iterate collection without locking it and 
 * when cokkection can be currently updated by other threads.
 * Please notice that if other threads are deleting elements from the tree, then
 * deleting current elemet of the iterator can still cause the problem.
 * Also this iterator should be used only inside one thread - sharing iterator between multiple
 * threads will not work correctly.
 */
public class ThreadSafeIterator extends Iterator { 
    private IResource   collection;
    private Iterator    iterator;
    private Object      next;
    private int         nextOid;

    public boolean hasNext() { 
        boolean result;
        if (next == null) {
            collection.sharedLock();
            if (iterator.hasNext()) { 
                next = iterator.next();
                result = true;
            } else { 
                result = false; 
            }
            collection.unlock();
        } else { 
            result = true;
        }
        return result;
    }
    
    public int nextOid() { 
        int oid = nextOid;
        if (oid == 0) { 
            collection.sharedLock();
            oid = iterator.nextOid();
            collection.unlock();
        } else { 
            nextOid = 0;
        }
        return oid;
    }
        
    public Object next() { 
        Object obj = next;
        if (obj == null) { 
            collection.sharedLock();
            obj = iterator.next();
            collection.unlock();
        } else { 
            next = null;
        }
        return obj;
    }

    public ThreadSafeIterator(IResource collection, Iterator iterator) { 
        this.collection = collection;
        this.iterator = iterator;
    }

    public void remove() {
        collection.exclusiveLock();
        iterator.remove();
        collection.unlock();
        next = null;
    }
}
