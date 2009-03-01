package org.garret.perst;

import com.jcraft.jzlib.*;
import java.io.*;

public class JZlibCompressor implements ICompressor
{
    private ZStream z = new ZStream();

    public int compress(byte[] dst, byte[] src, int srcSize) throws IOException {
        ZStream z = this.z;
        z.deflateInit(JZlib.Z_DEFAULT_COMPRESSION, false);
        z.next_in = src;
        z.next_in_index = 0;
        z.avail_in = srcSize;
        z.next_out = dst;
        z.next_out_index = 0;
        z.avail_out = srcSize;
        int err = z.deflate(JZlib.Z_FINISH);
        if (err != JZlib.Z_STREAM_END || z.avail_out == 0) {
            System.arraycopy(src, 0, dst, 0, srcSize);
            return srcSize;
        }
        return srcSize - z.avail_out;
    }

    public int decompress(byte[] dst, byte[] src, int srcSize) throws IOException {
        if (srcSize == dst.length) { 
            System.arraycopy(src, 0, dst, 0, srcSize);
            return srcSize;
        } else { 
            ZStream z = this.z;
            z.inflateInit(false);
            z.next_in = src;
            z.next_in_index = 0;
            z.avail_in = srcSize;
            z.next_out = dst;
            z.next_out_index = 0;
            z.avail_out = dst.length;
            int err = z.inflate(JZlib.Z_FINISH);
            return err != JZlib.Z_STREAM_END ? -1 : dst.length - z.avail_out;
        }
    }
}