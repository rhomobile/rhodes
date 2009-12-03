/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public class ParenthesisExpression extends Expression {

    CompoundStatement compstmt_;

    public ParenthesisExpression(CompoundStatement compstmt) {
        compstmt_ = compstmt;
    }

    public void accept(CodeVisitor visitor) {
        compstmt_.accept(visitor);
    }

    void ensureVariablesAreInitialized(CodeVisitor visitor) {
        compstmt_.ensureVariablesAreInitialized(visitor);
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        compstmt_.getNewlyAssignedVariables(symboltable, result);
    }

    void pullBlock(ArrayList<Block> result) {
        compstmt_.pullBlock(result);
    }
}
