/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.parser;

import antlr.*;
import java.io.*;

public class InputBufferWithHereDocSupport extends InputBuffer {

	private Reader reader_;
	private StringBuilder buffer_ = new StringBuilder();
	private StringBuilder backup_ = new StringBuilder();
	
	public InputBufferWithHereDocSupport(Reader reader) {
		reader_ = reader;
	}
	
	public void consume() {
		buffer_.deleteCharAt(0);
	}
	
	public char LA(int index) throws CharStreamException {
		try {
			int need_fecth = index - buffer_.length();
			for (int i = 0; i < need_fecth; ++i) {
				buffer_.append((char)reader_.read());
			}
			
			return buffer_.charAt(index - 1);
		} catch (IOException e) {
			throw new CharStreamIOException(e);
		}
	}
	
	void skipToLineBreak() throws TokenStreamException {
		
		for (int i = 0; i < buffer_.length(); ++i) {
			if ('\n' == buffer_.charAt(i)) {
				backup_.append(buffer_, 0, i + 1);
				buffer_.delete(0, i + 1);
				return;
			}
		}
		
		backup_.append(buffer_);
		buffer_.setLength(0);
		
		while (true) {
			char c;
			try {
				c = (char)reader_.read();
			} catch (IOException e) {
				throw new TokenStreamException(e); 
			}
			backup_.append(c);
			if ('\n' == c) {
				break;
			}
		}
	}
	
	void finishedHereDoc() {
		buffer_.insert(0, backup_);
		backup_.setLength(0);
	}
	
	public void commit() {
		throw new Error("Do not use me!");
	}
	
	public void fill(int arg0) throws CharStreamException {
		throw new Error("Do not use me!");
	}
	
	public String getLAChars() {
		throw new Error("Do not use me!");
	}
	
	public String getMarkedChars() {
		throw new Error("Do not use me!");
	}
	
	public boolean isMarked() {
		throw new Error("Do not use me!");
	}
	
	public int mark() {
		throw new Error("Do not use me!");
	}
	
	public void rewind(int mark) {
		throw new Error("Do not use me!");
	}
	
	public void reset() {
		throw new Error("Do not use me!");
	}
}
