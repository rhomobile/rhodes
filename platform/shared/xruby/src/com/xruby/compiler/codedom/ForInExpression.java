/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import antlr.RecognitionException;
import java.util.ArrayList;

public class ForInExpression extends Expression {

    private Expression exp_;
    private Block block_;

    public ForInExpression(Expression exp, Block block) {
        exp_ = exp;
        block_= block;
        block_.setIsForInExpression();
    }

    public void accept(CodeVisitor visitor) {
        block_.initAllParametersToNil(visitor);
        block_.setStartPosition(position);

        MethodCallExpression e;
        try {
            e = new MethodCallExpression(exp_, "each", null, block_);
        } catch (RecognitionException e1) {
            throw new Error(e1);
        }
        e.accept(visitor);
    }

    void pullBlock(ArrayList<Block> result) {
        if (null != block_) {
            result.add(block_);
        }
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        block_.getNewlyAssignedVariables(symboltable, result);
    }
}
