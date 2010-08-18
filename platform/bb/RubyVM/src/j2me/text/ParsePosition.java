/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.text;

// Stub for javolution.lang.TextFormat.Cursor to be compatible with ParsePosition.
public class ParsePosition {
    
    int _index = 0;
    int _errorIndex = -1;
    
    public ParsePosition(int index) {
    }
    
    public int getIndex() {
        return _index;
    }
    
    public void setIndex(int index) {
        _index = index;
    }
    
    public int getErrorIndex() {
        return _errorIndex;
    }
    
    public void setErrorIndex(int index) {
        _errorIndex = index;
    }
    
}
