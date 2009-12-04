/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public class MethodCallArguments implements Visitable {

    protected final ArrayList<Expression> arguments_ = new ArrayList<Expression>();
    protected Expression asterisk_argument_ = null;
    protected Expression block_argument_ = null;

    public MethodCallArguments() {
    }

    public MethodCallArguments(Expression argument) {
        arguments_.add(argument);
    }

    Expression getAsteriskArgument() {
        return asterisk_argument_;
    }

    Expression getBlockArgument() {
        return block_argument_;
    }

    public int size() {
        return arguments_.size();
    }

    Expression getFirstExpression() {
        return arguments_.get(0);
    }

    Expression getSecondExpression() {
        return arguments_.get(1);
    }

    public MethodCallArguments clone() {
        MethodCallArguments r = new MethodCallArguments();
        for (Expression exp : arguments_) {
            r.addArgument(exp);
        }
        return r;
    }

    public void setAsteriskArgument(Expression argument) {
        assert(null == asterisk_argument_);
        asterisk_argument_ = argument;
    }

    public void setBlockArgument(Expression argument) {
        assert(null == block_argument_);
        block_argument_ = argument;
    }

    public void addArgument(Expression argument) {
        arguments_.add(argument);
    }

    public void accept(CodeVisitor visitor) {
        if (arguments_.isEmpty() && null == asterisk_argument_) {
            visitor.visitNoParameter();
        } else {
            ArrayExpression to_a = new ArrayExpression(arguments_, asterisk_argument_);
            to_a.accept(visitor);
        }
    }

    public void getFrequentlyUsedIntegers(ArrayList<Integer> result) {
        for (Expression exp : arguments_) {
            exp.getFrequentlyUsedIntegers(result);
        }
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        for (Expression exp : arguments_) {
            exp.getNewlyAssignedVariables(symboltable, result);
        }
    }
}
