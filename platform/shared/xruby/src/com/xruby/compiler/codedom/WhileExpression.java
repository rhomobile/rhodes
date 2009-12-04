/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public class WhileExpression extends Expression {
    private final Expression condition_;
    private final CompoundStatement body_;
    private boolean is_until_;
    private boolean do_first_;//e.g. 'begin; print 1; end while false'

    public WhileExpression(Expression condition, CompoundStatement body, boolean is_until, boolean do_first) {
        assert(null != condition);
        condition_ = condition;
        body_ = body;
        is_until_ = is_until;
        do_first_ = do_first && body.isSingleBeginEnd();
    }

    boolean willNotBeExecuted() {
        if (!is_until_ && !do_first_ && conditionIsAlwayFalse(condition_)) {
            //while (false)
            return true;
        } else if (is_until_ && !do_first_ && conditionIsAlwayTrue(condition_)) {
            //while (true)
            return true;
        } else {
            return false;
        }
    }

    public void accept(CodeVisitor visitor) {
        //We used to generate code for body first, then condition (javac does this as well, it will help to create a little more efficient code).
        //But later we changed it back, because local variable may be first assigned in while expression's condition,  and we are replying
        //codegen backend to to maintain a symbol table -- so we should not change the consequence of visiting.

        if (null != body_) {
            body_.ensureVariablesAreInitialized(visitor);

            ArrayList<Block> pulled_blocks = new ArrayList<Block>();
            body_.pullBlock(pulled_blocks);
            for (Block block : pulled_blocks) {
                block.acceptAsPulled(visitor);
            }
        }

        visitor.visitWhileConditionBegin(do_first_);
        boolean always_true = conditionIsAlwayTrue(condition_);
        if (!always_true) {
            condition_.accept(visitor);
        }
        visitor.visitWhileConditionEnd(always_true, is_until_);

        if (null != body_) {
            body_.accept(visitor);
        }

        visitor.visitWhileBodyEnd(null != body_);
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
