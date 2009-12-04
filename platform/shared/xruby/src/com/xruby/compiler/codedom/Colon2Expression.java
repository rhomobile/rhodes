/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class Colon2Expression extends VariableExpression {

	private Expression left_;
	private String name_;
	
	public Colon2Expression(Expression left, String name) {
		left_ = left;
		name_ = name;
	}
	
	public void accept(CodeVisitor visitor) {
		if (null != left_) {
			//e.g. "M::C"
			left_.accept(visitor);
			visitor.visitConstant(name_);
		} else {
			//e.g. "::C"
			visitor.visitTopLevelConstant(name_);
		}
	}

	public void acceptAsAssignment(CodeVisitor visitor, boolean rhs_is_method_call, boolean is_multiple_assign) {
		if (null != left_) {
			//e.g. "M::C"
			left_.accept(visitor);
			visitor.visitConstantAssignmentOperator(name_, rhs_is_method_call, is_multiple_assign);
		} else {
			//e.g. "::C"
			visitor.visitTopLevelConstantAssignmentOperator(name_, rhs_is_method_call, is_multiple_assign);
		}
	}

	public void acceptAsDefined(CodeVisitor visitor) {
		if (null != left_) {
			left_.accept(visitor);
			visitor.visitDefinedConstant(name_);
		} else {
			visitor.visitDefinedTopLevelConstant(name_);
		}
	}

}
