/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.io;
import java.io.IOException;
import java.io.InputStream;
import j2me.lang.UnsupportedOperationException;

/**
 *  Classes provided for serialization support.
 */
public class ObjectInputStream extends InputStream implements
        ObjectInput {

    // For sub-classes reimplementing this class.
	protected ObjectInputStream() throws IOException, SecurityException {
    }

    public ObjectInputStream(InputStream in) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void defaultReadObject() throws IOException, ClassNotFoundException {
        throw new UnsupportedOperationException();
    }

    public int read() throws IOException {
        throw new UnsupportedOperationException();
    }

    public Object readObject() throws ClassNotFoundException, IOException {
        throw new UnsupportedOperationException();
    }

    public boolean readBoolean() throws IOException {
        throw new UnsupportedOperationException();
    }

    public byte readByte() throws IOException {
        throw new UnsupportedOperationException();
    }

    public char readChar() throws IOException {
        throw new UnsupportedOperationException();
    }

    public void readFully(byte[] b) throws IOException {
        throw new UnsupportedOperationException();
    }

    public void readFully(byte[] b, int off, int len) throws IOException {
        throw new UnsupportedOperationException();
    }

    public int readInt() throws IOException {
        throw new UnsupportedOperationException();
    }

    public String readLine() throws IOException {
        throw new UnsupportedOperationException();
    }

    public long readLong() throws IOException {
        throw new UnsupportedOperationException();
    }

    public short readShort() throws IOException {
        throw new UnsupportedOperationException();
    }

    public int readUnsignedByte() throws IOException {
        throw new UnsupportedOperationException();
    }

    public int readUnsignedShort() throws IOException {
        throw new UnsupportedOperationException();
    }

    public String readUTF() throws IOException {
        throw new UnsupportedOperationException();
    }

    public int skipBytes(int n) throws IOException {
        throw new UnsupportedOperationException();
    }

    /*@JVM-1.1+@

    public float readFloat() throws IOException {
        throw new UnsupportedOperationException();
    }
    
    public double readDouble() throws IOException {
        throw new UnsupportedOperationException();
    }

    /**/
}