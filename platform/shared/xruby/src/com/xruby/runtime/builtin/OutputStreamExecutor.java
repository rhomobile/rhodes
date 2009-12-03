package com.xruby.runtime.builtin;

import java.io.OutputStream;
import java.io.PrintStream;

import com.xruby.runtime.lang.RubyValue;

public class OutputStreamExecutor implements RubyIOExecutor {
	private PrintStream ps;
	
	public OutputStreamExecutor(OutputStream os) {
		if (os instanceof PrintStream) {
			this.ps = (PrintStream)os;
		} else {
			this.ps = new PrintStream(os);
		}
	}

	public void close() {
		this.ps.close();
	}

	public boolean eof() {
		return this.eof();
	}

	public void flush() {
		this.ps.flush();
	}

	public String gets(RubyValue seperator) {
		throw new Error("should reach here");
	}

	public void print(String s) {
		this.ps.print(s);
	}

	public String read() {
		throw new Error("should reach here");
	}

	public String read(long length) {
		throw new Error("should reach here");
	}

	public String read(int length, int offset) {
		throw new Error("should reach here");
	}

	public void seek(long pos) {
		throw new Error("should reach here");
	}

	public void truncate(int length) {
		throw new Error("should reach here");
	}

	public int write(String s) {
		this.ps.print(s);
		return s.length();
	}
}
