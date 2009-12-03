/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public abstract class ExpressionWithExpressionSubstitution extends Expression {
	protected ArrayList<Object> stmts_ = new ArrayList<Object>();

	public void addCompoundStatement(CompoundStatement stmts) {
		stmts_.add(stmts);
	}
	
	public void addGlobalVariable(String s) {
		stmts_.add(new GlobalVariableExpression(s));
	}
	
	public void addInstanceVariable(String s) {
		stmts_.add(new InstanceVariableExpression(s));
	}
	
	public void addClassVariable(String s) {
		stmts_.add(new ClassVariableExpression(s));
	}
	
	public void addString(String s) {
		stmts_.add(s);
	}
}