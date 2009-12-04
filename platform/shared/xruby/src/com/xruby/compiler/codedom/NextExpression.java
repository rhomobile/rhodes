/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class NextExpression extends Expression {

	private ReturnArguments arguments_;
	
	public NextExpression(ReturnArguments arguments) {
		arguments_ = arguments;
	}
	
	public void accept(CodeVisitor visitor) {
		if (null != arguments_) { 
			arguments_.accept(visitor);
		} else {
			visitor.visitNilExpression();
		}

		visitor.visitNext();
	}
}
