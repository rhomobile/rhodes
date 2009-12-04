/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;
import antlr.RecognitionException;

// && and || are not as same as other binaray operators, they
//are not method calls and can not be overiden.
public class AndOrBinaryOperatorExpression extends BinaryOperatorExpression {

    public AndOrBinaryOperatorExpression(String operator, Expression left, Expression right) {
        super(operator, left, right);
    }

    private void ensureVariablesAreInitialized(CodeVisitor visitor) {
        //e.g. false or a = 1; print a
        ArrayList<String> vars = new ArrayList<String>();
        right_.getNewlyAssignedVariables(visitor, vars);
        if (vars.isEmpty()) {
            return;
        }

        CompoundStatement comp = new CompoundStatement();
        for (String var : vars) {
            try {
                comp.addStatement(new ExpressionStatement(new AssignmentOperatorExpression(new LocalVariableExpression(var, false), new NilExpression())));
            } catch (RecognitionException e) {
                throw new Error(e);
            }
        }
        comp.accept(visitor);
        visitor.visitTerminal();
    }

    public void accept(CodeVisitor visitor) {
        if (operator_.equals("&&")) {
            left_.accept(visitor);
            Object label = visitor.visitAndBinaryOperatorLeft();
            right_.accept(visitor);
            visitor.visitAndBinaryOperatorRight(label);
        } else {
            ensureVariablesAreInitialized(visitor);

            ArrayList<Block> pulled_blocks = new ArrayList<Block>();
            right_.pullBlock(pulled_blocks);
            for (Block block : pulled_blocks) {
                block.acceptAsPulled(visitor);
            }
            
            left_.accept(visitor);
            Object label = visitor.visitOrBinaryOperatorLeft();
            right_.accept(visitor);
            visitor.visitOrBinaryOperatorRight(label);
        }
    }
}
