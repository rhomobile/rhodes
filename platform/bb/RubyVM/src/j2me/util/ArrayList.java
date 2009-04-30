/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util;
import javolution.util.FastTable;
public class ArrayList extends FastTable {
    
    public ArrayList() {
        super();
    }
    public ArrayList(int capacity) {
        super(capacity);
    }
    public ArrayList(List copy) {
        super();
        super.addAll(copy);
    }
    
}
