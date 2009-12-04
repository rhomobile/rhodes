/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class RangeOperatorExpression extends BinaryOperatorExpression {

	public RangeOperatorExpression(String operator, Expression left, Expression right) {
		super(operator, left, right);
	}

	public void accept(CodeVisitor visitor) {
		left_.accept(visitor);
		right_.accept(visitor);
		if (operator_.equals("..")) {
			visitor.visitInclusiveRangeOperator();
		} else {
			visitor.visitExclusiveRangeOperator();
		}
	}
}
