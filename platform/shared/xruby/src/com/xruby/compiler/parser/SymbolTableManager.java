/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.parser;

import java.util.*;

public class SymbolTableManager {
	private Stack<SymbolTable> stStack = new Stack<SymbolTable>();
	
	public SymbolTableManager() {
		enterScope();
	}
	
	public SymbolTableManager(List<String> preDefinedVar) {
		enterScope();
		
		for (String s : preDefinedVar) {
			addVariable(s);
		}
	}
	
	public void enterScope() {
		stStack.add(new SymbolTable());
	}
	
	public void enterBlockScope() {
		stStack.add(new SymbolTableForBlock(stStack.peek()));
	}
	
	public void leaveScope() {
		stStack.pop();
	}
	
	public boolean isDefinedInCurrentScope(String s) {
		return stStack.peek().findVariable(s);
	}
	
	public void addVariable(String s) {
		stStack.peek().addVariable(s);
	}
}
