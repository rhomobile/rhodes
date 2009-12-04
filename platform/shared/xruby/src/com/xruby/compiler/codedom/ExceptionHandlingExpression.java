/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public class ExceptionHandlingExpression extends Expression {
    private final BodyStatement bodyStatement_;

    public ExceptionHandlingExpression(BodyStatement bodyStatement) {
        bodyStatement_ = bodyStatement;
    }

    public void accept(CodeVisitor visitor) {
        bodyStatement_.accept(visitor);
    }

    void pullBlock(ArrayList<Block> result) {
        bodyStatement_.pullBlock(result);
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        bodyStatement_.getNewlyAssignedVariables(symboltable, result);
    }
}
