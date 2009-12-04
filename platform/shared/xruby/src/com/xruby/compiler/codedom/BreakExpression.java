/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class BreakExpression extends Expression {

	private ReturnArguments arguments_;
	
	public BreakExpression(ReturnArguments arguments){
		arguments_ = arguments;
	}
	
	public void accept(CodeVisitor visitor) {
		if (null != arguments_) { 
			arguments_.accept(visitor);
		} else {
			visitor.visitNilExpression();
		}

		visitor.visitBreak();
	}

}
