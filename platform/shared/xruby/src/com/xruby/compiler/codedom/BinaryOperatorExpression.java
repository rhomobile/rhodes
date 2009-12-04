/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public class BinaryOperatorExpression extends Expression {

    protected String operator_;
    protected Expression left_;
    protected Expression right_;

    public BinaryOperatorExpression(String operator, Expression left, Expression right) {
        operator_ = operator;
        left_ = left;
        right_ = right;
    }

    public void accept(CodeVisitor visitor) {
        left_.accept(visitor);
        right_.accept(visitor);
        visitor.visitBinaryOperator(operator_);
    }

    public void getFrequentlyUsedIntegers(ArrayList<Integer> result) {
        left_.getFrequentlyUsedIntegers(result);
        right_.getFrequentlyUsedIntegers(result);
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        left_.getNewlyAssignedVariables(symboltable, result);
        right_.getNewlyAssignedVariables(symboltable, result);
    }
}
