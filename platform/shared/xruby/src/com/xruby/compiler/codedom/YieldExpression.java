/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class YieldExpression extends Expression {

	private MethodCallArguments arguments_;

	public YieldExpression(MethodCallArguments arguments) {
		arguments_ = arguments;
	}

	public void accept(CodeVisitor visitor) {
		visitor.visitYieldBegin();

		int argc = getArgc();
		switch (argc) {
		case 0:
			break;
		case 1:
			arguments_.getFirstExpression().accept(visitor);
			break;
		default:
			arguments_.accept(visitor);
			break;
		}		
		
		visitor.visitYieldEnd(argc);
	}
	
	private int getArgc() {
		if (null == arguments_) {
			return 0;
		}
		
		if (arguments_.getAsteriskArgument() != null) {
			return -1;
		}
		
		if (arguments_.getBlockArgument() != null) {
			return -1;
		}
		
		return arguments_.size();
	}
}

