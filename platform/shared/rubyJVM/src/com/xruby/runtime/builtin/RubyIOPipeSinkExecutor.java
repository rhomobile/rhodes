/**
 * Copyright 2007 Michael Chen
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import java.io.IOException;
import java.io.InputStream;

import j2me.nio.ByteBuffer;
import j2me.nio.channels.Pipe.SinkChannel;

import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;

public class RubyIOPipeSinkExecutor implements RubyIOExecutor {

    private final SinkChannel sink;

    public RubyIOPipeSinkExecutor(SinkChannel sink) {
        this.sink = sink;
    }

    public void close() {
        try {
            this.sink.close();
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }
    }

    public InputStream getInputStream()
    {
    	return null;
    }
    
    public boolean eof() {
        throw notAllowed();
    }

    private RubyException notAllowed() {
        return new RubyException(RubyRuntime.IOErrorClass, "not opened for reading");
    }

    public void flush() {
        // FIXME: may be not used for the java NIO pipe implementation..
    }

    public String gets(RubyValue seperator) {
        throw notAllowed();
    }

    public void print(String s) {
        ByteBuffer bytebuffer = ByteBuffer.wrap(s.getBytes());
        try {
            this.sink.write(bytebuffer);
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }
    }

    public String read() {
        throw notAllowed();
    }

    public String read(long length) {
        throw notAllowed();
    }

    public String read(int length, int offset) {
        throw notAllowed();
    }

    public int write(String s) {
        print(s);
        return s.length();
    }

    public void truncate(int length) {
        throw notAllowed();
    }

    public void seek(long length) {
        throw notAllowed();
    }
}
