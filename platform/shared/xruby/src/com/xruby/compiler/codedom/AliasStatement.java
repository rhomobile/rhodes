/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class AliasStatement extends Statement {

	private String newName_;
	private String oldName_;
	
	public AliasStatement(String newName, String oldName) {
		newName_ = newName;
		oldName_ = oldName;
	}

	public void accept(CodeVisitor visitor) {
		if (newName_.charAt(0) == '$') {
			assert(oldName_.charAt(0) == '$');
			visitor.visitAliasGlobalVariable(newName_, oldName_);
		} else {
			assert(oldName_.charAt(0) != '$');
			visitor.visitAliasMethod(newName_, oldName_);
		}
	}
}
