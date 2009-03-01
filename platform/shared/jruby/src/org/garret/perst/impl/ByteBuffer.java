package org.garret.perst.impl;

import org.garret.perst.StorageError;
import java.io.*;

public class ByteBuffer {
    public final void append(int size) {  
        size += used;
        if (size > arr.length) { 
            int newLen = size > arr.length*2 ? size : arr.length*2;
            byte[] newArr = new byte[newLen];
            System.arraycopy(arr, 0, newArr, 0, used); 
            arr = newArr;
        }
    }

    public final void extend(int size) {  
        if (size > arr.length) { 
            int newLen = size > arr.length*2 ? size : arr.length*2;
            byte[] newArr = new byte[newLen];
            System.arraycopy(arr, 0, newArr, 0, used); 
            arr = newArr;
        }
        used = size;
    }
    
    final byte[] toArray() { 
        byte[] result = new byte[used];
        System.arraycopy(arr, 0, result, 0, used); 
        return result;
    }

    final int size() { 
        return used;
    }

    class ByteBufferOutputStream extends OutputStream { 
        public void write(int b) {
            write(new byte[]{(byte)b}, 0, 1);
        }

        public void write(byte b[], int off, int len) {
            int pos = used;
            extend(pos + len);
            System.arraycopy(b, off, arr, pos, len);
        }
    }

    public OutputStream getOutputStream() { 
        return new ByteBufferOutputStream();
    }

    ByteBuffer() { 
        arr = new byte[64];
    }

    public byte[] arr;
    public int    used;
}




