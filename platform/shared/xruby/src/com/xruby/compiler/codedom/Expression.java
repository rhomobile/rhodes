/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public abstract class Expression implements Visitable {
	protected int position;
	protected boolean newLine = false;

    protected boolean conditionIsAlwayTrue(Expression e) {
		if (e instanceof IntegerExpression) {
			return true;
		} else if (e instanceof FloatExpression) {
			return true;
		} else if (e instanceof StringExpression) {
			return true;
		} else if (e instanceof TrueExpression) {
			return true;
		}

		return false;
	}

	/// Used for optimazation, so we will not compile dead code
	boolean willNotBeExecuted() {
		return false;
	}

	protected boolean conditionIsAlwayFalse(Expression e) {
		if (e instanceof FalseExpression) {
			return true;
		} else if (e instanceof NilExpression) {
			return true;
		}

		return false;
	}

	void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
		//Default behavior is do nothing
	}

    void pullBlock(ArrayList<Block> result) {
        //Default behavior is do nothing
    }

	public void getFrequentlyUsedIntegers(ArrayList<Integer> result) {
		//Default behavior is do nothing
	}


	public void setPosition(int position) {
		this.position = position;
	}

	public int getPosition() {
		return position;
	}


    public boolean shouldlabelNewLine() {
        return newLine;
    }

    public void setNewLine(boolean newLine) {
        this.newLine = newLine;
    }
    
    public void addLineno(CodeVisitor visitor) {
    	if(this.shouldlabelNewLine()) {
    		// TODO: Add Line Number info
    		int lineNumber = this.getPosition();
    		visitor.visitLineLabel(lineNumber);
    	}
    }
}
