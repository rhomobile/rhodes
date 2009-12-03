/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class StringGroupExpression extends Expression {
	private Object value_ = new StringExpression("", false);
	
	public void appendString(String s, boolean double_quote) {
		if (value_ instanceof StringExpression) {
			((StringExpression)value_).appendString(s, double_quote);
		} else {
			((StringExpressionWithExpressionSubstitution)value_).addString(s);
		}
	}
	
	public void addStringWithExpressionSubstituation(StringExpressionWithExpressionSubstitution value) {
		if (value_ instanceof StringExpression) {
			value_ = new StringExpressionWithExpressionSubstitution(((StringExpression)value_).getValue());
		}
		
		((StringExpressionWithExpressionSubstitution)value_).merge(value);
	}
	
	public void accept(CodeVisitor visitor) {
		if (value_ instanceof StringExpression) {
			((StringExpression)value_).accept(visitor);
		} else {
			((StringExpressionWithExpressionSubstitution)value_).accept(visitor);
		}
	}
	
}
