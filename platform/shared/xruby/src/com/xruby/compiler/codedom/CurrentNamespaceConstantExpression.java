/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class CurrentNamespaceConstantExpression extends VariableExpression {

	private String name_;
	
	public CurrentNamespaceConstantExpression(String name) {
		name_ = name;
	}

	public void accept(CodeVisitor visitor) {
		visitor.visitCurrentNamespaceConstant(name_);
	}
	
	public void acceptAsAssignment(CodeVisitor visitor, boolean rhs_is_method_call, boolean is_multiple_assign) {
		visitor.visitCurrentNamespaceConstantAssignmentOperator(name_, rhs_is_method_call, is_multiple_assign);
	}

	public void acceptAsDefined(CodeVisitor visitor) {
		visitor.visitDefinedCurrentNamespaceConstant(name_);
	}

}
