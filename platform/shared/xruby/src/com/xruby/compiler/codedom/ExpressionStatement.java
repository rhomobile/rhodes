/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public class ExpressionStatement extends Statement {
	private Expression expression;

	public ExpressionStatement(Expression exp) {
		expression = exp;
	}

	public Expression getExpression() {
		return expression;
	}

	public void accept(CodeVisitor visitor) {
		this.expression.addLineno(visitor);
        expression.accept(visitor);
	}

	void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
		expression.getNewlyAssignedVariables(symboltable, result);
	}

    void pullBlock(ArrayList<Block> result) {
        expression.pullBlock(result);
    }
}
