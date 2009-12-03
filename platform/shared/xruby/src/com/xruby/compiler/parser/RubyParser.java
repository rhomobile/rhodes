/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.parser;

import com.xruby.compiler.codedom.Program;

import java.io.Reader;
import java.util.List;

import antlr.Token;
import antlr.RecognitionException;
import antlr.TokenStreamException;
import antlr.collections.AST;

public class RubyParser extends RubyParserBase {
	private RubyLexer lexer_;

	private RubyParser(RubyLexer lexer) {
		super(lexer);
		lexer_ = lexer;
	}
	
	public RubyParser(Reader in, List<String> pre_defined, boolean strip) {
		this(new RubyLexer(in, new SymbolTableManager(pre_defined), strip));
	}
	
	/// pre_defined can be empty
	public RubyParser(Reader in, boolean strip) {
		this(new RubyLexer(in, strip));
	}

	/// @return AST
	AST createAST() throws RecognitionException, TokenStreamException {
		ASTWithLineNumber.resetLineNumber();
		setASTNodeClass("com.xruby.compiler.parser.ASTWithLineNumber");
		program();
		return getAST();
	}

	public Program parse(String filename) throws RecognitionException, TokenStreamException {
		RubyTreeParser treeparser = new RubyTreeParser();
		return treeparser.parse(createAST(), filename);
	}

	protected void setIsInNestedMultipleAssign(boolean v) {
		lexer_.setIsInNestedMultipleAssign(v);
	}

	protected void tellLexerWeHaveFinishedParsingMethodparameters() {
		lexer_.setLastTokenToBe_RPAREN_IN_METHOD_DEFINATION();
	}

	protected void tellLexerWeHaveFinishedParsingSymbol() {
		lexer_.setJustFinishedParsingSymbol();
	}

	protected void tellLexerWeHaveFinishedParsingStringExpressionSubstituation() {
		lexer_.setJustFinishedParsingStringExpressionSubstituation();
	}

	protected void tellLexerWeHaveFinishedParsingRegexExpressionSubstituation() {
		lexer_.setJustFinishedParsingRegexExpressionSubstituation();
	}

	protected void tellLexerWeHaveFinishedParsingHeredocExpressionSubstituation() {
		lexer_.setJustFinishedParsingHeredocExpressionSubstituation();
	}
	
	protected void enterScope() {
		lexer_.getSymbolTableManager().enterScope();
	}

	protected void enterBlockScope() {
		lexer_.getSymbolTableManager().enterBlockScope();
	}

	protected void leaveScope() {
		lexer_.getSymbolTableManager().leaveScope();
	}

	protected void addVariable(Token id) {
		lexer_.getSymbolTableManager().addVariable(id.getText());
	}
}
