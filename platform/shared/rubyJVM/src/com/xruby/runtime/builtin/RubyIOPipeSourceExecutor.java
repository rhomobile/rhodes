/**
 * Copyright 2007 Michael Chen
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import java.io.IOException;
import java.io.InputStream;

import j2me.nio.ByteBuffer;
import j2me.nio.channels.Pipe.SourceChannel;

import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;

public class RubyIOPipeSourceExecutor implements RubyIOExecutor {

    private static final int BUFFER_SIZE = 1024;

    private final SourceChannel source;

    private boolean avaliable = true;

    public RubyIOPipeSourceExecutor(SourceChannel source) {
        this.source = source;
    }

    public void close() {
        try {
            this.source.close();
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }
    }

    public InputStream getInputStream()
    {
    	return null;
    }
    
    public boolean eof() {
        return !this.avaliable;
    }

    public void flush() {
        throw notAllowed();
    }

    private RubyException notAllowed() {
        return new RubyException(RubyRuntime.IOErrorClass, "not opened for writing");
    }

    public String gets(RubyValue separator) {
        StringBuffer result = new StringBuffer();
        ByteBuffer buffer;
        if (RubyConstant.QNIL == separator) {
            buffer = ByteBuffer.allocate(BUFFER_SIZE);
        } else {
            buffer = ByteBuffer.allocate(1);
        }

        try {
            while (avaliable = (source.read(buffer) > 0)) {
                String string = new String(buffer.array(), 0, buffer.position());
                result.append(string);
                buffer.clear();

                if (RubyConstant.QNIL == separator) {
                    continue;
                }

                if (result.toString().endsWith(separator.toString())) {
                    break;
                }
            }
            return result.toString();
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }
    }

    public void print(String s) {
        throw notAllowed();
    }

    public String read() {
        return gets(RubyConstant.QNIL);
    }

    public String read(long length) {

        if (length == 0) {
            if (avaliable) {
                return "";
            } else {
                return null;
            }
        }

        ByteBuffer buffer = ByteBuffer.allocate((int) length);
        int n = 0;
        try {
            n = source.read(buffer);
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }

        avaliable = (n > 0);
        if (!avaliable) {
            return null;
        }

        return new String(buffer.array(), 0, n);
    }

    public String read(int length, int offset) {
        ByteBuffer buffer = ByteBuffer.allocate(offset);
        try {
            avaliable = source.read(buffer) > 0;
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }

        return read(length);
    }

    public int write(String s) {
        throw notAllowed();
    }

    public void truncate(int length) {
        throw notAllowed();
    }

    public void seek(long length) {
        throw notAllowed();
    }
}
