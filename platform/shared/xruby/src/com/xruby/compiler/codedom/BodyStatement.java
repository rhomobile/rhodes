/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.*;

import antlr.RecognitionException;

class Rescue {
    private final ExceptionList condition_;
    private final CompoundStatement body_;

    Rescue(ExceptionList condition, CompoundStatement body) {
        condition_ = condition;
        body_ = body;
    }

    public void accept(CodeVisitor visitor, Object end_label, int excepton_var, boolean has_ensure) {

        Object next_label = condition_.accept(visitor, excepton_var);

        if (null != body_) {
            body_.accept(visitor);
        }

        if (has_ensure) {
            visitor.visitEnsure(-1);
        }

        //insert "$! = nil;"
        AssignmentOperatorExpression exp = null;
        try {
            exp = new AssignmentOperatorExpression(new GlobalVariableExpression("$!"), new NilExpression());
        } catch (RecognitionException e) {
            throw new Error(e);
        }
        exp.accept(visitor);
        visitor.visitTerminal();

        visitor.visitAfterRescueBody(next_label, end_label);
    }

    // For debugger
    int getLastLine() {
        return body_.getLastLine();
    }
}

public class BodyStatement implements Visitable {

    private final CompoundStatement compoundStatement_;
    private ArrayList<Rescue> rescues_ = new ArrayList<Rescue>();
    private CompoundStatement else_ = null;
    private CompoundStatement ensure_ = null;

    public BodyStatement(CompoundStatement compoundStatement) {
        compoundStatement_ = compoundStatement;
    }

    public int size() {
        return compoundStatement_.size();
    }

    boolean lastStatementHasReturnValue() {
        return compoundStatement_.lastStatementHasReturnValue();
    }

    public void addRescue(ExceptionList el, CompoundStatement compoundStatement) {
        if (null == compoundStatement) {
            compoundStatement = new CompoundStatement();
            compoundStatement.addStatement(new ExpressionStatement(new NilExpression()));
        }
        rescues_.add(new Rescue(el, compoundStatement));
    }

    public void addElse(CompoundStatement compoundStatement) {
        if (null == compoundStatement) {
            compoundStatement = new CompoundStatement();
            compoundStatement.addStatement(new ExpressionStatement(new NilExpression()));
        }
        else_ = compoundStatement;
    }

    public void addEnsure(CompoundStatement compoundStatement) {
        if (null == compoundStatement) {
            compoundStatement = new CompoundStatement();
            compoundStatement.addStatement(new ExpressionStatement(new NilExpression()));
        }
        ensure_ = compoundStatement;
    }

    private boolean needCatch() {
        return !rescues_.isEmpty() || null != ensure_;
    }

    private void acceptNoBody(CodeVisitor visitor) {
        //Optimazation: because there is no code to throw any exception, we only need to execute 'ensure'
        if (null != else_) {
            else_.accept(visitor);
        }

        if (null != ensure_) {
            if (null != else_) {
                visitor.visitTerminal();
            }
            ensure_.accept(visitor);
        }

        if (null == else_ && null == ensure_) {
            visitor.visitNilExpression();
        }
    }

    public void accept(CodeVisitor visitor) {
        if (null == compoundStatement_) {
            acceptNoBody(visitor);
            return;
        } else if (!needCatch()) {
            compoundStatement_.accept(visitor);
            return;
        }

        compoundStatement_.ensureVariablesAreInitialized(visitor);

        ArrayList<Block> pulled_blocks = new ArrayList<Block>();
        compoundStatement_.pullBlock(pulled_blocks);
        for (Block block : pulled_blocks) {
            block.acceptAsPulled(visitor);
        }

        final Object begin_label = visitor.visitBodyBegin(null != ensure_);
        compoundStatement_.accept(visitor);
        final Object after_label = visitor.visitBodyAfter();

        //The body of an else clause is executed only if no
        //exceptions are raised by the main body of code
        if (null != else_) {
            visitor.visitTerminal();
            else_.accept(visitor);
        }

        if (null != ensure_) {
            //do this so that ensure is executed in normal situation
            visitor.visitEnsure(-1);
        }

        final Object end_label = visitor.visitPrepareEnsure();

        final int exception_var = visitor.visitRescueBegin(begin_label, after_label);
        for (Rescue rescue : rescues_) {
            rescue.accept(visitor, end_label, exception_var, null != ensure_);
        }

        if (null != ensure_) {
            visitor.visitEnsure(exception_var);
            int var = visitor.visitEnsureBodyBegin();
            ensure_.accept(visitor);
            visitor.visitEnsureBodyEnd(var);
        }

        if (!rescues_.isEmpty()) {
            visitor.visitRescueEnd(exception_var, null != ensure_);
        }

        visitor.visitBodyEnd(end_label);
    }

    // for debugger
    public int getLastLine() {
        int lastLine = compoundStatement_.getLastLine();
        if(else_ != null && else_.getLastLine() > lastLine) {
            lastLine = else_.getLastLine();
        }

        if(ensure_ != null &&  ensure_.getLastLine() > lastLine) {
            lastLine = ensure_.getLastLine();
        }

        for(Rescue rescue: rescues_) {
            if(rescue.getLastLine() > lastLine) {
                lastLine = rescue.getLastLine();
            }
        }

        return lastLine;
    }

    void pullBlock(ArrayList<Block> result) {
        if (null != compoundStatement_) {
            compoundStatement_.pullBlock(result);
        }
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        if (null != compoundStatement_) {
            compoundStatement_.getNewlyAssignedVariables(symboltable, result);
        }
    }

}
