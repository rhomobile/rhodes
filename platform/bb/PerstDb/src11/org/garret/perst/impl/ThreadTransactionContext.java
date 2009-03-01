package org.garret.perst.impl;

import org.garret.perst.*;
import java.util.*;

/**
 * This class store transaction context associated with thread.
 * Content of this class is opaque for application, but it can use 
 * this context to share the single transaction between multiple threads
 */
public class ThreadTransactionContext { 
    static class Proxy { 
        Object obj;

        Proxy(Object o) { 
            obj = o;
        }

        public int hashCode() { 
            return System.identityHashCode(obj);
        }

        public boolean equals(Object o) { 
            return obj == o;
        }
    }

    int       nested;
    Hashtable locked = new Hashtable();
    ArrayList modified = new ArrayList();
    ArrayList deleted = new ArrayList();
}

