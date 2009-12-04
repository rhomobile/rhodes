/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class ReturnArguments extends MethodCallArguments {

	public void accept(CodeVisitor visitor) {
		if (arguments_.size() > 1 || null != asterisk_argument_) {
			ArrayExpression a = new ArrayExpression(arguments_, asterisk_argument_);
			a.accept(visitor);
		} else if (arguments_.size() == 1){
			arguments_.get(0).accept(visitor);
		} else {
			visitor.visitNilExpression();
		}
	}
}
