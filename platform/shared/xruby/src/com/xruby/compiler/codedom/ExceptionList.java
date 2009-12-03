/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.*;

public class ExceptionList {

	private ArrayList<Expression> arguments_ = new ArrayList<Expression>();
	private LocalVariableExpression var_ = null;
	
	public void addArgument(Expression v) {
		arguments_.add(v);
	}
	
	public void addExceptionVariable(LocalVariableExpression v) {
		var_ = v;
	}
	
	public Object accept(CodeVisitor visitor, int excepton_var) {

		if (arguments_.isEmpty()) {
			arguments_.add(new Colon2Expression(null, "StandardError"));
		}
		
		ArrayExpression a = new ArrayExpression(arguments_, null);
		a.accept(visitor);

		return visitor.visitRescueVariable((null != var_) ? var_.getValue() : null, excepton_var);
	}

}
