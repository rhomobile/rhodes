/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public class UnaryOperatorExpression extends Expression {
	private String operator_;
	private Expression left_;

	public UnaryOperatorExpression(String operator, Expression left) {
		operator_ = operator;
		left_ = left;
	}

	public void accept(CodeVisitor visitor) {
		left_.accept(visitor);
		visitor.visitUnaryOperator(operator_);
	}

	public void getFrequentlyUsedIntegers(ArrayList<Integer> result) {
		left_.getFrequentlyUsedIntegers(result);
	}
}
