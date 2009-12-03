/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class StringExpressionWithExpressionSubstitution extends ExpressionWithExpressionSubstitution {
	
	public StringExpressionWithExpressionSubstitution(String s) {
		super.addString(StringExpression.convertDoubleQuoteString(s));
	}

	public void addString(String s) {
		super.addString(StringExpression.convertDoubleQuoteString(s));
	}
	
	void merge(StringExpressionWithExpressionSubstitution another) {
		stmts_.addAll(another.stmts_);
	}

	public void accept(CodeVisitor visitor) {
		
		visitor.visitStringExpressionWithExpressionSubstitutionBegin();
		
		for (Object o : stmts_) {
			if (o instanceof String) {
				visitor.visitStringExpressionWithExpressionSubstitution((String)o);
			} else if (o instanceof InstanceVariableExpression) {
				((InstanceVariableExpression)o).accept(visitor);
				visitor.visitStringExpressionWithExpressionSubstitution();
			} else if (o instanceof ClassVariableExpression) {
				((ClassVariableExpression)o).accept(visitor);
				visitor.visitStringExpressionWithExpressionSubstitution();
			} else if (o instanceof GlobalVariableExpression) {
				((GlobalVariableExpression)o).accept(visitor);
				visitor.visitStringExpressionWithExpressionSubstitution();
			} else if (o instanceof CompoundStatement){
				((CompoundStatement)o).accept(visitor);
				visitor.visitStringExpressionWithExpressionSubstitution();
			} else {
				assert(false);
			}
		}
		
		visitor.visitStringExpressionWithExpressionSubstitutionEnd();
	}
	
}
