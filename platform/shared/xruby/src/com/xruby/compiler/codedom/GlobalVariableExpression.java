/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class GlobalVariableExpression extends VariableExpression {
	private final String value_;
	
	public String getValue() {
		return value_.toString();
	}

	public GlobalVariableExpression(String value) {
		value_ = value;
	}

	public void accept(CodeVisitor visitor) {
		visitor.visitGlobalVariableExpression(value_);
	}

	public void acceptAsAssignment(CodeVisitor visitor, boolean rhs_is_method_call, boolean is_multiple_assign) {
		visitor.visitGlobalVariableAssignmentOperator(value_, rhs_is_method_call, is_multiple_assign);
	}
}
