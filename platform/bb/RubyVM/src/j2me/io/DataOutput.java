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

public interface DataOutput {

    void write(byte[] b) throws IOException;

    void write(byte[] b, int off, int len) throws IOException;

    void write(int b) throws IOException;

    void writeBoolean(boolean v) throws IOException;

    void writeByte(int v) throws IOException;

    void writeBytes(String s) throws IOException;

    void writeChar(int v) throws IOException;

    void writeChars(String s) throws IOException;

    void writeInt(int v) throws IOException;

    void writeLong(long v) throws IOException;

    void writeShort(int v) throws IOException;

    void writeUTF(String str) throws IOException;

    /*@JVM-1.1+@

     void writeFloat (float v) throws IOException;
     
     void writeDouble (double v) throws IOException;

     /**/
}