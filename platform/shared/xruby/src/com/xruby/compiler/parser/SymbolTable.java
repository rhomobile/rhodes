/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.parser;

import java.util.ArrayList;

class SymbolTable {
	private final ArrayList<String> variables_ = new ArrayList<String>();
	
	public void addVariable(String s) {
		variables_.add(s);
	}
	
	public boolean findVariable(String s) {
		return variables_.indexOf(s) >= 0;
	}
}

class SymbolTableForBlock extends SymbolTable {
	private SymbolTable owner_;
	
	public SymbolTableForBlock(SymbolTable owner) {
		owner_ = owner;
	}
	
	public boolean findVariable(String s) {
		if (super.findVariable(s)) {
			return true;
		} else {
			return owner_.findVariable(s);
		}
	}
}
