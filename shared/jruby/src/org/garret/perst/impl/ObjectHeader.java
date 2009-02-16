package org.garret.perst.impl;

class ObjectHeader { 
    static final int sizeof = 8;
    
    static int getSize(byte[] arr, int offs) { 
        return Bytes.unpack4(arr, offs);   
    }
    static void setSize(byte[] arr, int offs, int size) { 
        Bytes.pack4(arr, offs, size);   
    }
    static int getType(byte[] arr, int offs) { 
        return Bytes.unpack4(arr, offs+4);   
    }
    static void setType(byte[] arr, int offs, int type) { 
        Bytes.pack4(arr, offs+4, type);   
    }
}
    
