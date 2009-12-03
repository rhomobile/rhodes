/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.parser;

import antlr.*;
import antlr.collections.*;

/**
 * CommonAST(default) from Antlr does not store line information.
 * So we extend it, so use setASTNodeClass() to set it up in parser. 
 */
public class ASTWithLineNumber extends CommonAST {

	private static final long serialVersionUID = 1L;
	private int line_ = 0;
	private static int last_line_ = 0;//help initialize(int t, String txt)

	public static void resetLineNumber() {
		last_line_ = 0;
	}
	
	public void initialize(Token tok) {
		super.initialize(tok);
		line_ = tok.getLine();
		last_line_ = line_;
	}

	public void initialize (AST ast) {
		assert(false);//I have not see this one being used
		super.initialize(ast);
		if (ast instanceof ASTWithLineNumber){
			line_ = ((ASTWithLineNumber)ast).getLine();
			last_line_ = line_;
		}
	}
	
	public void initialize(int t, String txt) {
		super.initialize(t, txt);
		line_ = last_line_;
	}

	public int getLine() {
		return line_;
	}
}
