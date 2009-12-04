/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

import antlr.RecognitionException;

public class InstanceVariableExpression extends ParameterVariableExpression {
	private final String value_;
	
	public String getValue() {
		return value_.toString();
	}

	public InstanceVariableExpression(String value) {
		value_ = value;
	}

	public void accept(CodeVisitor visitor) {
		visitor.visitInstanceVariableExpression(value_);
	}

	public void acceptAsAssignment(CodeVisitor visitor, boolean rhs_is_method_call, boolean is_multiple_assign) {
		visitor.visitInstanceVariableAssignmentOperator(value_, rhs_is_method_call, is_multiple_assign);
	}
	
	public void acceptAsInitializeToNil(CodeVisitor visitor) {
        AssignmentOperatorExpression assign;
        try {
            assign = new AssignmentOperatorExpression(new InstanceVariableExpression(value_), new NilExpression());
        } catch (RecognitionException e) {
            throw new Error(e);
        }
        assign.accept(visitor);
        visitor.visitTerminal();
    }
	
	void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
	}
}
