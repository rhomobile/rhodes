package org.garret.perst;

/**
 * Array list wrapper
 */
public class ArrayList extends j2me.util.ArrayList { 
    
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
           
    public Iterator iterator() { 
        return new ArrayListIterator();
    }
}
    