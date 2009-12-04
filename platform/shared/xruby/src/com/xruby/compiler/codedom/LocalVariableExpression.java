/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

import antlr.RecognitionException;

public class LocalVariableExpression extends ParameterVariableExpression {
    private final String value_;
    private final boolean isFunction_;

    public LocalVariableExpression(String value, boolean isFunction) {
        value_ = value;
        isFunction_ = isFunction;
    }

    String getValue() {
        return value_;
    }

    private boolean isFunction(CodeVisitor visitor) {
        if (visitor.isDefinedInCurrentScope(value_)) {
            return false;
        } else {
            return isFunction_;
        }
    }

    public void acceptAsInitializeToNil(CodeVisitor visitor) {
        AssignmentOperatorExpression assign;
        try {
            assign = new AssignmentOperatorExpression(new LocalVariableExpression(value_, false), new NilExpression());
        } catch (RecognitionException e) {
            throw new Error(e);
        }
        assign.accept(visitor);
        visitor.visitTerminal();
    }

    public void accept(CodeVisitor visitor) {
        //TODO replace the old symbol table with the one used in codegen, so that
        //function/variable ambiguity can be resolved in parser.
        if (isFunction(visitor)) {
            Expression exp;
            try {
                exp = new MethodCallExpression(null, value_, null, null);
            } catch (RecognitionException e) {
                throw new Error(e);
            }
            exp.accept(visitor);
        } else {
            visitor.visitLocalVariableExpression(value_);
        }
    }

    public void acceptAsAssignment(CodeVisitor visitor, boolean rhs_is_method_call, boolean is_multiple_assign) {
        visitor.visitLocalVariableAssignmentOperator(value_, rhs_is_method_call, is_multiple_assign);
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        //Do nothing
    }

}
