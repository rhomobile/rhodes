/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;
import java.util.List;

import antlr.RecognitionException;

public class CompoundStatement implements Visitable {
    protected List<Statement> statements_ = new ArrayList<Statement>();
    private List<BEGINBlock> beginblocks_ = null;
    private List<ENDBlock> endblocks_ = null;
    private int lastLine = 0;

    public void addStatement(Statement statement) {
        if (null != statement) {
            //optimazation: remove dead code
            if (statement instanceof ExpressionStatement) {
                Expression expression = ((ExpressionStatement) statement).getExpression();
                if (!expression.willNotBeExecuted()) {
                    statements_.add(statement);
                }
                if(expression.shouldlabelNewLine() && expression.getPosition() > 0) {
                    lastLine = expression.getPosition();
                }
            } else if (statement instanceof BEGINBlock) {
            	this.addBeginBlock((BEGINBlock)statement);
            } else if (statement instanceof ENDBlock) {
            	this.addEndBlock((ENDBlock)statement);
            } else {
                statements_.add(statement);
            }
        }
    }
    
    private void addBeginBlock(BEGINBlock block) {
    	if (this.beginblocks_ == null) {
    		this.beginblocks_ = new ArrayList<BEGINBlock>();
    	}
    	
    	this.beginblocks_.add(block);
    }
    
    private void addEndBlock(ENDBlock block) {
    	if (this.endblocks_ == null) {
    		this.endblocks_ = new ArrayList<ENDBlock>();
    	}
    	
    	this.endblocks_.add(0, block);
    }

    int size() {
        return statements_.size();
    }

    // begin; ... ; end
    boolean isSingleBeginEnd() {
        return (this.statements_.size() == 1) &&
            (statements_.get(0) instanceof ExpressionStatement) &&
            (((ExpressionStatement)statements_.get(0)).getExpression() instanceof ExceptionHandlingExpression);
    }

    public int getLastLine() {
        return lastLine;
    }

    boolean lastStatementHasReturnValue() {
        if (statements_.isEmpty()) {
            return false;
        } else if (statements_.get(statements_.size() - 1) instanceof ExpressionStatement) {
            ExpressionStatement e = (ExpressionStatement)statements_.get(statements_.size() - 1);
            if (e.getExpression() instanceof ReturnExpression ||
                e.getExpression() instanceof BreakExpression ||
                e.getExpression() instanceof NextExpression ||
                e.getExpression() instanceof RedoExpression) {
                return false;
            } else {
                return true;
            }
        } else if (statements_.get(statements_.size() - 1) instanceof MultipleAssignmentStatement) {
            return true;
        } else {
            return false;
        }
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        for (Statement s : statements_) {
            s.getNewlyAssignedVariables(symboltable, result);
        }
    }

    void pullBlock(ArrayList<Block> result) {
        for (Statement s : statements_) {
            s.pullBlock(result);
        }
    }

    void ensureVariablesAreInitialized(CodeVisitor visitor) {
        //some vairiables appeare first in if's body, and they may not be excuted.
        ArrayList<String> vars = new ArrayList<String>();
        getNewlyAssignedVariables(visitor, vars);
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
    	if (this.beginblocks_ != null) {
    		statements_.addAll(0, beginblocks_);
    	}
    	
    	if (this.endblocks_ != null) {
    		statements_.addAll(endblocks_);
    	}

        int i = 0;
        for (Statement statement : statements_) {
            ++i;
            statement.accept(visitor);

            if ((statement instanceof ExpressionStatement ||
                statement instanceof MultipleAssignmentStatement ||
                statement instanceof BEGINBlock||
                statement instanceof ENDBlock) && (i != statements_.size())) {
                visitor.visitTerminal();
            }
        }
    }
}
