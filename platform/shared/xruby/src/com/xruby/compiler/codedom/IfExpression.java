/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.*;

public class IfExpression extends Expression {

    private class Elseif {

        private final Expression condition_;
        private final CompoundStatement body_;

        Elseif(Expression condition, CompoundStatement body) {
            condition_ = condition;
        	if ( body == null )
        	{
        		body_ = new CompoundStatement();
        		body_.addStatement(new ExpressionStatement(new NilExpression()));
        	}else
                body_ = body;
        }

        public void accept(CodeVisitor visitor, Object end_label) {
            if (null == condition_) {
                visitor.visitTrueExpression();
            } else {
                condition_.accept(visitor);
            }
            
            Object next_label = visitor.visitAfterIfCondition();
            if (null != body_) {
                body_.accept(visitor);
            } else {
                visitor.visitNilExpression();
            }
            visitor.visitAfterIfBody(next_label, end_label);
        }

        void pullBlock(ArrayList<Block> result) {
            condition_.pullBlock(result);
           	body_.pullBlock(result);
        }

        void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
            condition_.getNewlyAssignedVariables(symboltable, result);
           	body_.getNewlyAssignedVariables(symboltable, result);
        }
    }

    private final Expression if_condition_;
    private CompoundStatement if_body_;
    private ArrayList<Elseif> elsifs = new ArrayList<Elseif>();
    private CompoundStatement else_body_;

    public IfExpression(Expression condition, Expression left, Expression right) {
        if_condition_ = condition;
        if_body_ = new CompoundStatement();
        if_body_.addStatement(new ExpressionStatement(left));
        else_body_ = new CompoundStatement();
        else_body_.addStatement(new ExpressionStatement(right));
    }

    public IfExpression(Expression condition, CompoundStatement body) {
        if_condition_ = condition;
        if_body_ = body;
    }

    public void addElsif(Expression condition, CompoundStatement body) {
        elsifs.add(new Elseif(condition, body));
    }

    public void addElse(CompoundStatement body) {
        else_body_ = body;
    }

    private void ensureIfBodyAndElseBodyAreNotEmpty() {
        if (null == if_body_) {
            if_body_ = new CompoundStatement();
            if_body_.addStatement(new ExpressionStatement(new NilExpression()));
        }

        if (null == else_body_) {
            else_body_ = new CompoundStatement();
            else_body_.addStatement(new ExpressionStatement(new NilExpression()));
        }
    }

    public void accept(CodeVisitor visitor) {
        ensureIfBodyAndElseBodyAreNotEmpty();

        if (if_condition_ instanceof ParenthesisExpression) {
            ((ParenthesisExpression)if_condition_).ensureVariablesAreInitialized(visitor);
        }
        if_body_.ensureVariablesAreInitialized(visitor);
        for (Elseif elsif : elsifs) {
       		elsif.body_.ensureVariablesAreInitialized(visitor);
        }
        else_body_.ensureVariablesAreInitialized(visitor);

        ArrayList<Block> pulled_blocks = new ArrayList<Block>();
        if_body_.pullBlock(pulled_blocks);
        for (Elseif elsif : elsifs) {
       		elsif.body_.pullBlock(pulled_blocks);
        }
        else_body_.pullBlock(pulled_blocks);
        for (Block block : pulled_blocks) {
            block.acceptAsPulled(visitor);
        }

        //optimazation
        //TODO add more optimazation
        //TODO maybe we should do this in treewalker?
        if (conditionIsAlwayTrue(if_condition_)) {
            if_body_.accept(visitor);
            if (!if_body_.lastStatementHasReturnValue()) {
                visitor.visitNilExpression();
            }
            return;
        }

        accept_with_no_optimazation(visitor);
    }

    private void accept_with_no_optimazation(CodeVisitor visitor) {
        if_condition_.accept(visitor);
        Object next_label = visitor.visitAfterIfCondition();

        if_body_.accept(visitor);
        if (!if_body_.lastStatementHasReturnValue()) {
            visitor.visitNilExpression();
        }

        final Object end_label = visitor.visitAfterIfBody(next_label, null);

        for (Elseif elsif : elsifs) {
            elsif.accept(visitor, end_label);
        }

        else_body_.accept(visitor);
        visitor.visitAfterIfBody(null, end_label);
    }

    void pullBlock(ArrayList<Block> result) {
        if_condition_.pullBlock(result);
        if (null != if_body_) {
            if_body_.pullBlock(result);
        }
        for (Elseif elsif : elsifs) {
            elsif.pullBlock(result);
        }
        if (null != else_body_) {
            else_body_.pullBlock(result);
        }
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        if_condition_.getNewlyAssignedVariables(symboltable, result);
        if (null != if_body_) {
            if_body_.getNewlyAssignedVariables(symboltable, result);
        }
        for (Elseif elsif : elsifs) {
            elsif.getNewlyAssignedVariables(symboltable, result);
        }
        if (null != else_body_) {
            else_body_.getNewlyAssignedVariables(symboltable, result);
        }
    }
}
