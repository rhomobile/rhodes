/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

import antlr.RecognitionException;

public class AssignmentOperatorExpression extends Expression {

	private VariableExpression lhs_;
	private Expression rhs_;

	public AssignmentOperatorExpression(Expression left, Expression right) throws RecognitionException {
		if (left instanceof VariableExpression) {
			lhs_ = (VariableExpression)left;
		} else {
			throw new RecognitionException("Only variable can be assigned");
		}
		
		rhs_ = right;
	}
	
	VariableExpression getLhs() {
		return lhs_;
	}
	
	Expression getRhs() {
		return rhs_;
	}
	
	public static Expression create(Expression left, Expression right) throws RecognitionException {
		if (left instanceof MethodCallExpression) {
			MethodCallExpression call = (MethodCallExpression)left;
			if (call.isElementSet()) {
				return call.convertElementAccessToElementSet(right);
			} else {
				throw new RecognitionException("Method call can not be assigned");
			}
		} else if (left instanceof BinaryOperatorExpression) {
			//deal with things like:
			//x << y = 1		#same as x<< (y = 1), but parsed as (x << y) = 1
			//false || b = 2	#same as false || (b = 2), but parsed as (false || b) = 2
			//true && b = 3		#same as true && (b = 3), but parsed as (true && b) = 3
			//otherwise there are invalid base on the operator precedence rule
			BinaryOperatorExpression e = (BinaryOperatorExpression)left;
			return new AndOrBinaryOperatorExpression(e.operator_, e.left_, new AssignmentOperatorExpression(e.right_, right));
		} else {
			return new AssignmentOperatorExpression(left, right);
		}
	}

	public void accept(CodeVisitor visitor) {
		rhs_.accept(visitor);
		this.addLineno(visitor);
		lhs_.acceptAsAssignment(visitor,
				rhs_ instanceof MethodCallExpression || rhs_ instanceof YieldExpression,
				false);
	}

    void pullBlock(ArrayList<Block> result) {
        rhs_.pullBlock(result);
    }
	
	void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
		if (lhs_ instanceof LocalVariableExpression) {
			String name = ((LocalVariableExpression)lhs_).getValue();
			if (!symboltable.isDefinedInCurrentScope(name)) {
				result.add(name);
			}
		}
	}

	public void getFrequentlyUsedIntegers(ArrayList<Integer> result) {
		rhs_.getFrequentlyUsedIntegers(result);
	}
}
