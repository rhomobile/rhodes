package org.garret.perst;

public interface ICompressor
{
    public int compress(byte[] dst, byte[] src, int srcSize) throws java.io.IOException;    
    public int decompress(byte[] dst, byte[] src, int srcSize) throws java.io.IOException;
}