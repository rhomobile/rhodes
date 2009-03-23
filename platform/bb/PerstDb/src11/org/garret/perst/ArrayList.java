package org.garret.perst;

import java.util.Vector;

/**
 * Array list wrapper
 */
public class ArrayList extends Vector { 
    public Object get(int i) { 
        return elementAt(i);
    }

    public void set(int i, Object obj) { 
        setElementAt(obj, i);
    }


    public void add(Object o) { 
        addElement(o);
    }
    
    public Object[] toArray(Object[] arr) { 
        copyInto(arr);
        return arr;
    }

    public void clear() { 
        removeAllElements();
    }

    public Object[] toArray() {
        return toArray(new Object[size()]);
    }

    public class ArrayListIterator extends Iterator { 
        int curr = -1;
        
        public boolean hasNext() { 
            return curr + 1 < ArrayList.this.size();
        }

        public Object next() { 
            return ArrayList.this.get(++curr);
        }

        public int nextOid() {            
            return ((IPersistent)ArrayList.this.get(++curr)).getOid();
        }

        public void remove() {
            ArrayList.this.remove(curr);
        }
    }

    public void remove(int index) {         
        removeElementAt(index);
    }
           
    public Iterator iterator() { 
        return new ArrayListIterator();
    }
}
    