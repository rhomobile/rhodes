/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import antlr.RecognitionException;

public class DefinedExpression extends Expression {

	private Expression expression_;
	
	public DefinedExpression(MethodCallArguments arguments) throws RecognitionException {
		if (arguments.size() > 1) {
			throw new RecognitionException("defined? can only has one parameter");
		} else if (arguments.size() == 0) {
			throw new RecognitionException("defined? should has one parameter");
		}

		expression_ = arguments.getFirstExpression();
	}
	
	// defined? expression is compile time computable in some cases
	public void accept(CodeVisitor visitor) {
		if (expression_ instanceof NilExpression) {
			visitor.visitStringExpression("nil");
		} else if (expression_ instanceof GlobalVariableExpression) {
			visitor.visitStringExpression("global-variable");
		} else if (expression_ instanceof CurrentNamespaceConstantExpression) {
			((CurrentNamespaceConstantExpression)expression_).acceptAsDefined(visitor);
		} else if (expression_ instanceof Colon2Expression) {
			((Colon2Expression)expression_).acceptAsDefined(visitor);
		} else if (expression_ instanceof TrueExpression) {
			visitor.visitStringExpression("true");
		} else if (expression_ instanceof FalseExpression) {
			visitor.visitStringExpression("falses");
		} else if (expression_ instanceof MethodCallExpression) {
			MethodCallExpression call = (MethodCallExpression)expression_;
			if (null != call.getReceiver()) {
				call.getReceiver().accept(visitor);
				visitor.visitDefinedPublicMethod(call.getName());
			} else {
				visitor.visitSelfExpression();
				visitor.visitDefinedMethod(call.getName());
			}
		} else if (expression_ instanceof SuperExpression) {
			visitor.visitDefinedSuperMethod();
		} else if (expression_ instanceof BinaryOperatorExpression) {
			visitor.visitStringExpression("method");
		} else if (expression_ instanceof YieldExpression) {
			visitor.visitDefinedYield();
		} else if (expression_ instanceof LocalVariableExpression) {
			visitor.visitDefinedLocalVariable(((LocalVariableExpression)expression_).getValue());
		} else if (expression_ instanceof InstanceVariableExpression) {
			visitor.visitDefinedInstanceVariable(((InstanceVariableExpression)expression_).getValue());
		} else {
			visitor.visitStringExpression("expression");
		}
		
	}

}
