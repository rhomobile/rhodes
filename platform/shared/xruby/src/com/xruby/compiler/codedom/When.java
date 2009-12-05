/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */
package com.xruby.compiler.codedom;

import java.util.ArrayList;

public class When {

    private final ArrayList<Expression> condition_ = new ArrayList<Expression>();
    private Expression asterisk_condition_ = null;// when *[[1]]
    private CompoundStatement body_ = null;

    public void addCondition(Expression condition) {
        condition_.add(condition);
    }

    public void setAsteriskCondition(Expression asterisk_condition) {
        asterisk_condition_ = asterisk_condition;
    }

    public void setBody(CompoundStatement body) {
        body_ = body;
    }

    public Object accept(CodeVisitor visitor, Object var, Object end_label, boolean isEmptyCase) {
        Object next_label;

        if (condition_.size() == 1 && null == asterisk_condition_) {
            condition_.get(0).accept(visitor);
            if ( !isEmptyCase )
            	next_label = visitor.visitAfterWhenCondition(var, false);
            else
            	next_label = visitor.visitAfterWhenConditionNotNil(var);
        } else {
            Expression to_a = new ArrayExpression(condition_, asterisk_condition_);
            to_a.accept(visitor);
            next_label = visitor.visitAfterWhenCondition(var, true);
        }

        if (null != body_) {
            body_.accept(visitor);
        } else {
            visitor.visitNilExpression();
        }
        return visitor.visitAfterWhenBody(next_label, end_label);
    }

    void ensureVariablesAreInitialized(CodeVisitor visitor) {
        if (null != body_) {
            body_.ensureVariablesAreInitialized(visitor);
        }
    }

    void pullBlock(ArrayList<Block> result) {
        for (Expression e : condition_) {
            e.pullBlock(result);
        }
        if (null != asterisk_condition_) {
            asterisk_condition_.pullBlock(result);
        }
        if (null != body_) {
            body_.pullBlock(result);
        }
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        for (Expression e : condition_) {
            e.getNewlyAssignedVariables(symboltable, result);
        }
        if (null != asterisk_condition_) {
            asterisk_condition_.getNewlyAssignedVariables(symboltable, result);
        }
        if (null != body_) {
            body_.getNewlyAssignedVariables(symboltable, result);
        }
    }
}
