/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.*;

import antlr.RecognitionException;

public class NestedVariableExpression extends ParameterVariableExpression {
	private ArrayList<ParameterVariableExpression> mlhs_ = new ArrayList<ParameterVariableExpression>();
	private ParameterVariableExpression asterisk_lhs_ = null;

	public void addLhs(Expression e) throws RecognitionException {
		if (e instanceof ParameterVariableExpression) {
			mlhs_.add((ParameterVariableExpression)e);
		} else if (e instanceof MethodCallExpression) {
			//For inputs like 'a, b = 1', a will be recognized as MethodCall
			MethodCallExpression m = (MethodCallExpression)e;
			if (m.getArguments() == null ||
					m.getArguments().size() == 0) {//TODO more erro checking? e.g. a(), b = 1
				mlhs_.add(new LocalVariableExpression(m.getName(), false));
			} else {
				throw new RecognitionException("Only variable can be parallel assigned");
			}
		} else {
			throw new RecognitionException("Only variable can be parallel assigned");
		}
	}
	
	public void setAsteriskLhs(Expression e)throws RecognitionException {
		assert(null == asterisk_lhs_);
		if (e instanceof ParameterVariableExpression) {
			asterisk_lhs_ = (ParameterVariableExpression)e;
		} else {
			throw new RecognitionException("Only variable can be parallel assigned");
		}
	}
	
	public void accept(CodeVisitor visitor) {
		throw new Error("Nested variable should not appear in this context");
	}
	
	public void acceptAsInitializeToNil(CodeVisitor visitor) {
		for (ParameterVariableExpression var : mlhs_) {
			var.acceptAsInitializeToNil(visitor);
		}
		
		if (null != asterisk_lhs_) {
			asterisk_lhs_.acceptAsInitializeToNil(visitor);
		}
	}

	public void acceptAsAssignment(CodeVisitor visitor, boolean rhs_is_method_call, boolean is_multiple_assign) {
		assert(is_multiple_assign);
		Collections.reverse(mlhs_);
		
		int var = visitor.visitNestedVariable(false, mlhs_.size() > 0);
		
		for (int i = 0; i < mlhs_.size(); ++i) {
			visitor.visitMrhs(var, i, false);
		}
		
		if (null != asterisk_lhs_) {
			visitor.visitMrhs(var, mlhs_.size(), true);
			asterisk_lhs_.acceptAsAssignment(visitor, false, true);
		}
		
		for (ParameterVariableExpression lhs : mlhs_) {
			lhs.acceptAsAssignment(visitor, false, true);
		}
	}

	void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
		for (ParameterVariableExpression var : mlhs_) {
			var.getNewlyAssignedVariables(symboltable, result);
		}
		
		if (null != asterisk_lhs_) {
			asterisk_lhs_.getNewlyAssignedVariables(symboltable, result);
		}
	}
	
}
