/**
 * Copyright 2007 Michael Chen
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import j2me.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import j2me.io.File;
//import j2me.io.RandomAccessFile;
import com.rho.file.RandomAccessFile;

import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;

import j2me.lang.AssertMe;

class RubyIOFileExecutor implements RubyIOExecutor {

    private RandomAccessFile file_ = null;

    public RubyIOFileExecutor(String filename, String mode) {
        open(filename, mode);
    }

    private String translateMode(String mode) {
        if (mode.equals("w")) {
            return "rw";
        } else if (mode.equals("a")) {
            return "rw";
        } else if (mode.equals("rb")) {
            return "r";
        } else if (mode.equals("r+")) {
            return "r";
        } else {
            return mode;
        }
    }

    public InputStream getInputStream()
    {
    	return null;
    }
    
    private boolean open(String filename, String mode) {
        AssertMe.rho_assert (null == file_);
		File f = new File(filename);
		long length =  f.length();
        try {
            file_ = new RandomAccessFile(f, translateMode(mode), true);
            if (mode.equals("w")) {
                file_.setLength(0);
            } else if (mode.equals("a")) {
                file_.seek(length);
			}
            return true;
        } catch (FileNotFoundException e) {
            return false;
        } catch (IOException e) {
            file_ = null;
            return false;
        }
    }

    public boolean eof() {
        try {
            return file_.length() == file_.getFilePointer();
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }
    }

    public void close() {
        if (null != file_) {
            try {
                file_.close();
                file_ = null;
            } catch (IOException e) {
                // Do not care
            }
        }
    }

    public void flush() {
        try {
            file_.getChannel().force(false);
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public void print(String s) {
        if (null == file_) {
            throw new RubyException(RubyRuntime.SystemCallErrorClass, "file is not opened");
        }

        try {
            file_.writeBytes(s);
        } catch (IOException e) {
            // FIXME: deal with the exception
        }
    }

    private String readsTheEntireContents() throws IOException {
        int size = (int) file_.length();// TODO converted long to int
        byte[] buffer = new byte[size];
        if (file_.read(buffer) < 0) {
            return "";
        } else {
            return new String(buffer);
        }
    }

    private String readUntilSeperator(String separator) throws IOException {
        // FIXME This is cheating, should read until separator
        String s = file_.readLine();
        if (null == s) {
            return null;
        } else {
            return s + "\n";
        }
    }

    public String gets(RubyValue separator) {
        if (null == file_) {
            throw new RubyException(RubyRuntime.SystemCallErrorClass, "file is not opened");
        }

        try {
            if (RubyConstant.QNIL == separator) {
                return readsTheEntireContents();
            }

            if (!(separator instanceof RubyString)) {
                throw new RubyException(RubyRuntime.TypeErrorClass, "can't convert "
                        + separator.getRubyClass().getName() + " into String");
            }

            RubyString s = (RubyString) separator;

            return readUntilSeperator(s.toString());
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }
    }

    public String read() {
        if (null == file_) {
            throw new RubyException(RubyRuntime.SystemCallErrorClass, "file is not opened");
        }

        try {
            return readsTheEntireContents();
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }
    }

    public String read(long length) {
        if (null == file_) {
            throw new RubyException(RubyRuntime.SystemCallErrorClass, "file is not opened");
        }

        try {
            long size = file_.length();
            size = Math.min(length, size);
            byte[] buffer = new byte[(int) size];
            if (file_.read(buffer) < 0) {
                return null;
            } else {
                return new String(buffer);
            }
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }
    }

    public String read(int length, int offset) {
        if (null == file_) {
            throw new RubyException(RubyRuntime.SystemCallErrorClass, "file is not opened");
        }

        try {
            file_.seek(offset);
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }

        return read(length);
    }

    public int write(String s) {
        print(s);
        return s.length();
    }

    public void truncate(int length) {
        try {
            file_.setLength(length);
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }
    }

    public void seek(long pos) {
        try {
            file_.seek(pos);
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }
    }
}
