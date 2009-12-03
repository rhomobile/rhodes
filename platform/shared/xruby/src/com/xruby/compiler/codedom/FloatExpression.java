/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class FloatExpression extends Expression {
	private double value_;

	public FloatExpression(String value) {
		value_ = Double.parseDouble(value);
	}

	public void accept(CodeVisitor visitor) {
		visitor.visitFloatExpression(value_);
	}
}

