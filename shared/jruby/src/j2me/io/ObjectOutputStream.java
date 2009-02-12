/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */

package j2me.io;
import j2me.lang.UnsupportedOperationException;
import java.io.IOException;
import java.io.OutputStream;
/**
 *  Classes provided for serialization support.
 */
public class ObjectOutputStream
    extends OutputStream implements ObjectOutput {
	
    // For sub-classes reimplementing this class.
	protected ObjectOutputStream() throws IOException, SecurityException {
    }

    public ObjectOutputStream(OutputStream out) throws IOException {
        throw new UnsupportedOperationException();
    }
    
    public void defaultWriteObject() throws IOException {
        throw new UnsupportedOperationException();
    }

    public void write(int arg0) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void writeObject(Object obj) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void writeBoolean(boolean v) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void writeByte(int v) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void writeBytes(String s) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void writeChar(int v) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void writeChars(String s) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void writeInt(int v) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void writeLong(long v) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void writeShort(int v) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void writeUTF(String str) throws IOException {
        throw new UnsupportedOperationException();
    }

    /*@JVM-1.1+@

    public void writeFloat (float v) throws IOException {
        throw new UnsupportedOperationException();
    }
    
    public void writeDouble (double v) throws IOException {
        throw new UnsupportedOperationException();
    }

    /**/

}
