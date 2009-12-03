/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class RetryExpression extends Expression {

	public void accept(CodeVisitor visitor) {
		visitor.visitRetry();
	}

}
