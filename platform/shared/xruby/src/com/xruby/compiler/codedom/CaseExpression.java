/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public class CaseExpression extends Expression {
    private final Expression condition_;
    private ArrayList<When> whens_ = new ArrayList<When>();
    private CompoundStatement else_body_;

    public CaseExpression(Expression condition) {
        condition_ = condition;
    }

    public void addWhen(When when) {
        whens_.add(when);
    }

    public void addElse(CompoundStatement body) {
        else_body_ = body;
    }

    private void ensureElseBodyIsNotEmpty() {
        if (null == else_body_) {
            else_body_ = new CompoundStatement();
            else_body_.addStatement(new ExpressionStatement(new NilExpression()));
        }
    }

    public void accept(CodeVisitor visitor) {
        ensureElseBodyIsNotEmpty();

        for (When when : whens_) {
            when.ensureVariablesAreInitialized(visitor);
        }
        else_body_.ensureVariablesAreInitialized(visitor);

        ArrayList<Block> pulled_blocks = new ArrayList<Block>();
        for (When when : whens_) {
            when.pullBlock(pulled_blocks);
        }
        else_body_.pullBlock(pulled_blocks);
        for (Block block : pulled_blocks) {
            block.acceptAsPulled(visitor);
        }

        if (null == condition_) {
            visitor.visitTrueExpression();
        } else {
            condition_.accept(visitor);
        }
        Object var = visitor.visitAfterCaseCondition();

        Object end_label = null;
        for (When when : whens_) {
            end_label = when.accept(visitor, var, end_label, null == condition_);
        }

        else_body_.accept(visitor);
        visitor.visitAfterWhenBody(null, end_label);
    }

    void pullBlock(ArrayList<Block> result) {
        if (null != condition_) {
            condition_.pullBlock(result);
        }
        for (When when : whens_) {
            when.pullBlock(result);
        }
        if (null != else_body_) {
            else_body_.pullBlock(result);
        }
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        if (null != condition_) {
            condition_.getNewlyAssignedVariables(symboltable, result);
        }
        for (When when : whens_) {
            when.getNewlyAssignedVariables(symboltable, result);
        }
        if (null != else_body_) {
            else_body_.getNewlyAssignedVariables(symboltable, result);
        }
    }
}
