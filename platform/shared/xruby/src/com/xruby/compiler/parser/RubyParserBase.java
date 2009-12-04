// $ANTLR 2.7.6 (2005-12-22): "src/com/xruby/compiler/parser/ruby.g" -> "RubyParserBase.java"$

package com.xruby.compiler.parser;

import antlr.TokenBuffer;
import antlr.TokenStreamException;
import antlr.TokenStreamIOException;
import antlr.ANTLRException;
import antlr.LLkParser;
import antlr.Token;
import antlr.TokenStream;
import antlr.RecognitionException;
import antlr.NoViableAltException;
import antlr.MismatchedTokenException;
import antlr.SemanticException;
import antlr.ParserSharedInputState;
import antlr.collections.impl.BitSet;
import antlr.collections.AST;
import java.util.Hashtable;
import antlr.ASTFactory;
import antlr.ASTPair;
import antlr.collections.impl.ASTArray;

/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */
public class RubyParserBase extends antlr.LLkParser       implements RubyTokenTypes
 {

	private int can_be_command_ = 0;
	private int is_in_method_definition = 0;

	protected void enterScope()	{assert(false);}
	protected void enterBlockScope()	{assert(false);}
	protected void leaveScope()	{assert(false);}
	protected void addVariable(Token id)	{assert(false);}
	protected void setIsInNestedMultipleAssign(boolean v)	{assert(false);}
	protected void tellLexerWeHaveFinishedParsingMethodparameters()	{assert(false);}
	protected void tellLexerWeHaveFinishedParsingSymbol()	{assert(false);}
	protected void tellLexerWeHaveFinishedParsingStringExpressionSubstituation()	{assert(false);}
	protected void tellLexerWeHaveFinishedParsingRegexExpressionSubstituation()	{assert(false);}
	protected void tellLexerWeHaveFinishedParsingHeredocExpressionSubstituation()	{assert(false);}

	boolean isNotAssign(int token_type) {
		switch (token_type) {
			case ASSIGN:
			case ASSIGN_WITH_NO_LEADING_SPACE:
			case PLUS_ASSIGN:
			case MINUS_ASSIGN:
			case STAR_ASSIGN:
			case DIV_ASSIGN:
			case MOD_ASSIGN:
			case POWER_ASSIGN:
			case BAND_ASSIGN:
			case BXOR_ASSIGN:
			case BOR_ASSIGN:
			case LEFT_SHIFT_ASSIGN:
			case RIGHT_SHIFT_ASSIGN:
			case LOGICAL_AND_ASSIGN:
			case LOGICAL_OR_ASSIGN:
				return false;
			default:
				return true;
		}
	}

protected RubyParserBase(TokenBuffer tokenBuf, int k) {
  super(tokenBuf,k);
  tokenNames = _tokenNames;
  buildTokenTypeASTClassMap();
  astFactory = new ASTFactory(getTokenTypeToASTClassMap());
}

public RubyParserBase(TokenBuffer tokenBuf) {
  this(tokenBuf,1);
}

protected RubyParserBase(TokenStream lexer, int k) {
  super(lexer,k);
  tokenNames = _tokenNames;
  buildTokenTypeASTClassMap();
  astFactory = new ASTFactory(getTokenTypeToASTClassMap());
}

public RubyParserBase(TokenStream lexer) {
  this(lexer,1);
}

public RubyParserBase(ParserSharedInputState state) {
  super(state,1);
  tokenNames = _tokenNames;
  buildTokenTypeASTClassMap();
  astFactory = new ASTFactory(getTokenTypeToASTClassMap());
}

	public final void program() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST program_AST = null;
		
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			compoundStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(Token.EOF_TYPE);
		program_AST = (AST)currentAST.root;
		returnAST = program_AST;
	}
	
	public final void compoundStatement() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST compoundStatement_AST = null;
		boolean empty = true;
		
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		{
			terminal();
			{
			switch ( LA(1)) {
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_END:
			{
				statements();
				astFactory.addASTChild(currentAST, returnAST);
				if ( inputState.guessing==0 ) {
					empty = false;
				}
				break;
			}
			case EOF:
			case RCURLY:
			case RPAREN:
			case LITERAL_end:
			case LITERAL_rescue:
			case LITERAL_else:
			case LITERAL_ensure:
			case LITERAL_elsif:
			case LITERAL_when:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			statements();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				empty = false;
			}
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState.guessing==0 ) {
			compoundStatement_AST = (AST)currentAST.root;
			if (!empty)	compoundStatement_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(COMPSTMT,"COMPSTMT")).add(compoundStatement_AST));
			currentAST.root = compoundStatement_AST;
			currentAST.child = compoundStatement_AST!=null &&compoundStatement_AST.getFirstChild()!=null ?
				compoundStatement_AST.getFirstChild() : compoundStatement_AST;
			currentAST.advanceChildToEnd();
		}
		compoundStatement_AST = (AST)currentAST.root;
		returnAST = compoundStatement_AST;
	}
	
	public final void terminal() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST terminal_AST = null;
		
		switch ( LA(1)) {
		case SEMI:
		{
			match(SEMI);
			terminal_AST = (AST)currentAST.root;
			break;
		}
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			terminal_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = terminal_AST;
	}
	
	public final void statements() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST statements_AST = null;
		
		statement();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop8:
		do {
			if ((LA(1)==SEMI||LA(1)==LINE_BREAK)) {
				terminal();
				astFactory.addASTChild(currentAST, returnAST);
				if ( inputState.guessing==0 ) {
					
										if (	(EOF == LA(1))	||	//script end
											(RCURLY == LA(1))	||	(LITERAL_end == LA(1))	||	//block end
											(RPAREN == LA(1))	||
											(LITERAL_else == LA(1)) || (LITERAL_elsif == LA(1))	||
											(LITERAL_rescue == LA(1)) || (LITERAL_ensure == LA(1))	||
											(LITERAL_when == LA(1))
										)
										break;
									
				}
				statement();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop8;
			}
			
		} while (true);
		}
		statements_AST = (AST)currentAST.root;
		returnAST = statements_AST;
	}
	
	public final void statement() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST statement_AST = null;
		AST body_AST = null;
		AST if_condition_AST = null;
		AST unless_condition_AST = null;
		AST while_condition_AST = null;
		AST until_condition_AST = null;
		AST rescue_condition_AST = null;
		
		statementWithoutModifier();
		body_AST = (AST)returnAST;
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop12:
		do {
			switch ( LA(1)) {
			case IF_MODIFIER:
			{
				AST tmp4_AST = null;
				tmp4_AST = astFactory.create(LT(1));
				match(IF_MODIFIER);
				expression();
				if_condition_AST = (AST)returnAST;
				if ( inputState.guessing==0 ) {
					statement_AST = (AST)currentAST.root;
					statement_AST = (AST)astFactory.make( (new ASTArray(3)).add(astFactory.create(LITERAL_if,"if")).add(if_condition_AST).add((AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(COMPSTMT,"COMPSTMT")).add(body_AST))));
					currentAST.root = statement_AST;
					currentAST.child = statement_AST!=null &&statement_AST.getFirstChild()!=null ?
						statement_AST.getFirstChild() : statement_AST;
					currentAST.advanceChildToEnd();
				}
				break;
			}
			case UNLESS_MODIFIER:
			{
				AST tmp5_AST = null;
				tmp5_AST = astFactory.create(LT(1));
				match(UNLESS_MODIFIER);
				expression();
				unless_condition_AST = (AST)returnAST;
				if ( inputState.guessing==0 ) {
					statement_AST = (AST)currentAST.root;
					statement_AST = (AST)astFactory.make( (new ASTArray(3)).add(astFactory.create(LITERAL_unless,"unless")).add(unless_condition_AST).add((AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(COMPSTMT,"COMPSTMT")).add(body_AST))));
					currentAST.root = statement_AST;
					currentAST.child = statement_AST!=null &&statement_AST.getFirstChild()!=null ?
						statement_AST.getFirstChild() : statement_AST;
					currentAST.advanceChildToEnd();
				}
				break;
			}
			case WHILE_MODIFIER:
			{
				AST tmp6_AST = null;
				tmp6_AST = astFactory.create(LT(1));
				match(WHILE_MODIFIER);
				expression();
				while_condition_AST = (AST)returnAST;
				if ( inputState.guessing==0 ) {
					statement_AST = (AST)currentAST.root;
					statement_AST = (AST)astFactory.make( (new ASTArray(3)).add(tmp6_AST).add(while_condition_AST).add((AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(COMPSTMT,"COMPSTMT")).add(body_AST))));
					currentAST.root = statement_AST;
					currentAST.child = statement_AST!=null &&statement_AST.getFirstChild()!=null ?
						statement_AST.getFirstChild() : statement_AST;
					currentAST.advanceChildToEnd();
				}
				break;
			}
			case UNTIL_MODIFIER:
			{
				AST tmp7_AST = null;
				tmp7_AST = astFactory.create(LT(1));
				match(UNTIL_MODIFIER);
				expression();
				until_condition_AST = (AST)returnAST;
				if ( inputState.guessing==0 ) {
					statement_AST = (AST)currentAST.root;
					statement_AST = (AST)astFactory.make( (new ASTArray(3)).add(tmp7_AST).add(until_condition_AST).add((AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(COMPSTMT,"COMPSTMT")).add(body_AST))));
					currentAST.root = statement_AST;
					currentAST.child = statement_AST!=null &&statement_AST.getFirstChild()!=null ?
						statement_AST.getFirstChild() : statement_AST;
					currentAST.advanceChildToEnd();
				}
				break;
			}
			case RESCUE_MODIFIER:
			{
				AST tmp8_AST = null;
				tmp8_AST = astFactory.create(LT(1));
				match(RESCUE_MODIFIER);
				expression();
				rescue_condition_AST = (AST)returnAST;
				if ( inputState.guessing==0 ) {
					statement_AST = (AST)currentAST.root;
					statement_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(LITERAL_begin,"begin")).add((AST)astFactory.make( (new ASTArray(4)).add(astFactory.create(BODY,"BODY")).add((AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(COMPSTMT,"COMPSTMT")).add(body_AST))).add(astFactory.create(LITERAL_rescue,"rescue")).add(rescue_condition_AST))));
					currentAST.root = statement_AST;
					currentAST.child = statement_AST!=null &&statement_AST.getFirstChild()!=null ?
						statement_AST.getFirstChild() : statement_AST;
					currentAST.advanceChildToEnd();
				}
				break;
			}
			default:
			{
				break _loop12;
			}
			}
		} while (true);
		}
		statement_AST = (AST)currentAST.root;
		returnAST = statement_AST;
	}
	
	public final void statementWithoutModifier() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST statementWithoutModifier_AST = null;
		Token  seen_star = null;
		AST seen_star_AST = null;
		
			boolean is_parallel_assignment = false;
			boolean has_extra_comma = false;
		
		
		{
		switch ( LA(1)) {
		case LITERAL_alias:
		{
			aliasStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_undef:
		{
			undefStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_BEGIN:
		{
			keyword_BEGIN();
			astFactory.addASTChild(currentAST, returnAST);
			AST tmp9_AST = null;
			tmp9_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp9_AST);
			match(LCURLY_BLOCK);
			{
			switch ( LA(1)) {
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_END:
			{
				compoundStatement();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case RCURLY:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			AST tmp10_AST = null;
			tmp10_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp10_AST);
			match(RCURLY);
			if ( inputState.guessing==0 ) {
				if (is_in_method_definition>0) {throw new RecognitionException("BEGIN in method");}
			}
			break;
		}
		case LITERAL_END:
		{
			keyword_END();
			astFactory.addASTChild(currentAST, returnAST);
			AST tmp11_AST = null;
			tmp11_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp11_AST);
			match(LCURLY_BLOCK);
			{
			switch ( LA(1)) {
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_END:
			{
				compoundStatement();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case RCURLY:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			AST tmp12_AST = null;
			tmp12_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp12_AST);
			match(RCURLY);
			if ( inputState.guessing==0 ) {
				if (is_in_method_definition>0) {throw new RecognitionException("END in method");}
			}
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			expression();
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop20:
			do {
				if ((LA(1)==COMMA)) {
					match(COMMA);
					if ( inputState.guessing==0 ) {
						if (ASSIGN == LA(1) || ASSIGN_WITH_NO_LEADING_SPACE == LA(1)) {has_extra_comma=true;break;}
					}
					{
					switch ( LA(1)) {
					case REST_ARG_PREFIX:
					{
						seen_star = LT(1);
						seen_star_AST = astFactory.create(seen_star);
						astFactory.addASTChild(currentAST, seen_star_AST);
						match(REST_ARG_PREFIX);
						break;
					}
					case EOF:
					case SEMI:
					case LINE_BREAK:
					case IF_MODIFIER:
					case UNLESS_MODIFIER:
					case WHILE_MODIFIER:
					case UNTIL_MODIFIER:
					case RESCUE_MODIFIER:
					case RCURLY:
					case COMMA:
					case ASSIGN:
					case ASSIGN_WITH_NO_LEADING_SPACE:
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case RPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case LITERAL_end:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_rescue:
					case LITERAL_else:
					case LITERAL_ensure:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_elsif:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					case LITERAL_when:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					{
					switch ( LA(1)) {
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						mlhs_item();
						astFactory.addASTChild(currentAST, returnAST);
						break;
					}
					case EOF:
					case SEMI:
					case LINE_BREAK:
					case IF_MODIFIER:
					case UNLESS_MODIFIER:
					case WHILE_MODIFIER:
					case UNTIL_MODIFIER:
					case RESCUE_MODIFIER:
					case RCURLY:
					case COMMA:
					case ASSIGN:
					case ASSIGN_WITH_NO_LEADING_SPACE:
					case RPAREN:
					case LITERAL_end:
					case LITERAL_rescue:
					case LITERAL_else:
					case LITERAL_ensure:
					case LITERAL_elsif:
					case LITERAL_when:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					if ( inputState.guessing==0 ) {
						if (null != seen_star) break;
					}
					if ( inputState.guessing==0 ) {
						is_parallel_assignment = true;
					}
				}
				else {
					break _loop20;
				}
				
			} while (true);
			}
			{
			switch ( LA(1)) {
			case ASSIGN:
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				{
				switch ( LA(1)) {
				case ASSIGN:
				{
					match(ASSIGN);
					break;
				}
				case ASSIGN_WITH_NO_LEADING_SPACE:
				{
					match(ASSIGN_WITH_NO_LEADING_SPACE);
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				{
				switch ( LA(1)) {
				case LINE_BREAK:
				{
					match(LINE_BREAK);
					break;
				}
				case REST_ARG_PREFIX:
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case LITERAL_not:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				mrhs();
				astFactory.addASTChild(currentAST, returnAST);
				if ( inputState.guessing==0 ) {
					is_parallel_assignment = true;
				}
				break;
			}
			case EOF:
			case SEMI:
			case LINE_BREAK:
			case IF_MODIFIER:
			case UNLESS_MODIFIER:
			case WHILE_MODIFIER:
			case UNTIL_MODIFIER:
			case RESCUE_MODIFIER:
			case RCURLY:
			case RPAREN:
			case LITERAL_end:
			case LITERAL_rescue:
			case LITERAL_else:
			case LITERAL_ensure:
			case LITERAL_elsif:
			case LITERAL_when:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case REST_ARG_PREFIX:
		{
			AST tmp17_AST = null;
			tmp17_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp17_AST);
			match(REST_ARG_PREFIX);
			mlhs_item();
			astFactory.addASTChild(currentAST, returnAST);
			{
			switch ( LA(1)) {
			case ASSIGN:
			{
				match(ASSIGN);
				break;
			}
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				match(ASSIGN_WITH_NO_LEADING_SPACE);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case LINE_BREAK:
			{
				match(LINE_BREAK);
				break;
			}
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			mrhs();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				is_parallel_assignment = true;
			}
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState.guessing==0 ) {
			statementWithoutModifier_AST = (AST)currentAST.root;
			
							if (is_parallel_assignment && !has_extra_comma) {
								statementWithoutModifier_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(MULTIPLE_ASSIGN,"MULTIPLE_ASSIGN")).add(statementWithoutModifier_AST));
							} else if (is_parallel_assignment && has_extra_comma) {
								statementWithoutModifier_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(MULTIPLE_ASSIGN_WITH_EXTRA_COMMA,"MULTIPLE_ASSIGN_WITH_EXTRA_COMMA")).add(statementWithoutModifier_AST));
							}
						
			currentAST.root = statementWithoutModifier_AST;
			currentAST.child = statementWithoutModifier_AST!=null &&statementWithoutModifier_AST.getFirstChild()!=null ?
				statementWithoutModifier_AST.getFirstChild() : statementWithoutModifier_AST;
			currentAST.advanceChildToEnd();
		}
		statementWithoutModifier_AST = (AST)currentAST.root;
		returnAST = statementWithoutModifier_AST;
	}
	
	public final void expression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST expression_AST = null;
		
		andorExpression();
		astFactory.addASTChild(currentAST, returnAST);
		expression_AST = (AST)currentAST.root;
		returnAST = expression_AST;
	}
	
	public final void aliasStatement() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST aliasStatement_AST = null;
		
		AST tmp21_AST = null;
		tmp21_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp21_AST);
		match(LITERAL_alias);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case LITERAL_do:
		case LITERAL_end:
		case BOR:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case MATCH:
		case LESS_THAN:
		case GREATER_THAN:
		case LESS_OR_EQUAL:
		case GREATER_OR_EQUAL:
		case BXOR:
		case BAND:
		case LEFT_SHIFT:
		case RIGHT_SHIFT:
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
		case MOD:
		case POWER:
		case BNOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case LITERAL_super:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_elsif:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_in:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case SINGLE_QUOTE:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_then:
		case LITERAL_when:
		case LITERAL_END:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case GLOBAL_VARIABLE:
		{
			AST tmp23_AST = null;
			tmp23_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp23_AST);
			match(GLOBAL_VARIABLE);
			{
			switch ( LA(1)) {
			case LINE_BREAK:
			{
				match(LINE_BREAK);
				break;
			}
			case GLOBAL_VARIABLE:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			AST tmp25_AST = null;
			tmp25_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp25_AST);
			match(GLOBAL_VARIABLE);
			break;
		}
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case LITERAL_do:
		case LITERAL_end:
		case BOR:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case MATCH:
		case LESS_THAN:
		case GREATER_THAN:
		case LESS_OR_EQUAL:
		case GREATER_OR_EQUAL:
		case BXOR:
		case BAND:
		case LEFT_SHIFT:
		case RIGHT_SHIFT:
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
		case MOD:
		case POWER:
		case BNOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case LITERAL_super:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_elsif:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_in:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case SINGLE_QUOTE:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_then:
		case LITERAL_when:
		case LITERAL_END:
		{
			undef_parameter();
			astFactory.addASTChild(currentAST, returnAST);
			{
			switch ( LA(1)) {
			case LINE_BREAK:
			{
				match(LINE_BREAK);
				break;
			}
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case LITERAL_do:
			case LITERAL_end:
			case BOR:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case LITERAL_and:
			case LITERAL_or:
			case LITERAL_not:
			case COMPARE:
			case EQUAL:
			case CASE_EQUAL:
			case MATCH:
			case LESS_THAN:
			case GREATER_THAN:
			case LESS_OR_EQUAL:
			case GREATER_OR_EQUAL:
			case BXOR:
			case BAND:
			case LEFT_SHIFT:
			case RIGHT_SHIFT:
			case PLUS:
			case MINUS:
			case STAR:
			case DIV:
			case MOD:
			case POWER:
			case BNOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case LITERAL_super:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LITERAL_rescue:
			case LITERAL_else:
			case LITERAL_ensure:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_elsif:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_in:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case SINGLE_QUOTE:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_then:
			case LITERAL_when:
			case LITERAL_END:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			undef_parameter();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		aliasStatement_AST = (AST)currentAST.root;
		returnAST = aliasStatement_AST;
	}
	
	public final void undefStatement() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST undefStatement_AST = null;
		
		AST tmp27_AST = null;
		tmp27_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp27_AST);
		match(LITERAL_undef);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case LITERAL_do:
		case LITERAL_end:
		case BOR:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case MATCH:
		case LESS_THAN:
		case GREATER_THAN:
		case LESS_OR_EQUAL:
		case GREATER_OR_EQUAL:
		case BXOR:
		case BAND:
		case LEFT_SHIFT:
		case RIGHT_SHIFT:
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
		case MOD:
		case POWER:
		case BNOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case LITERAL_super:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_elsif:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_in:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case SINGLE_QUOTE:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_then:
		case LITERAL_when:
		case LITERAL_END:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		undef_parameter();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop45:
		do {
			if ((LA(1)==COMMA)) {
				match(COMMA);
				undef_parameter();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop45;
			}
			
		} while (true);
		}
		undefStatement_AST = (AST)currentAST.root;
		returnAST = undefStatement_AST;
	}
	
	public final void keyword_BEGIN() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_BEGIN_AST = null;
		
		AST tmp30_AST = null;
		tmp30_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp30_AST);
		match(LITERAL_BEGIN);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case LCURLY_BLOCK:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_BEGIN_AST = (AST)currentAST.root;
		returnAST = keyword_BEGIN_AST;
	}
	
	public final void keyword_END() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_END_AST = null;
		
		AST tmp32_AST = null;
		tmp32_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp32_AST);
		match(LITERAL_END);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case LCURLY_BLOCK:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_END_AST = (AST)currentAST.root;
		returnAST = keyword_END_AST;
	}
	
	public final void mlhs_item() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST mlhs_item_AST = null;
		Token  seen_star = null;
		AST seen_star_AST = null;
		
		boolean synPredMatched29 = false;
		if (((LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN))) {
			int _m29 = mark();
			synPredMatched29 = true;
			inputState.guessing++;
			try {
				{
				lparen();
				dotColonOrArrayAccess();
				match(COMMA);
				}
			}
			catch (RecognitionException pe) {
				synPredMatched29 = false;
			}
			rewind(_m29);
inputState.guessing--;
		}
		if ( synPredMatched29 ) {
			lparen();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				setIsInNestedMultipleAssign(true);
			}
			dotColonOrArrayAccess();
			astFactory.addASTChild(currentAST, returnAST);
			{
			int _cnt32=0;
			_loop32:
			do {
				if ((LA(1)==COMMA)) {
					match(COMMA);
					if ( inputState.guessing==0 ) {
						if (RPAREN == LA(1)) break;
					}
					{
					switch ( LA(1)) {
					case REST_ARG_PREFIX:
					{
						seen_star = LT(1);
						seen_star_AST = astFactory.create(seen_star);
						astFactory.addASTChild(currentAST, seen_star_AST);
						match(REST_ARG_PREFIX);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					dotColonOrArrayAccess();
					astFactory.addASTChild(currentAST, returnAST);
					if ( inputState.guessing==0 ) {
						if (null != seen_star) break;
					}
				}
				else {
					if ( _cnt32>=1 ) { break _loop32; } else {throw new NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt32++;
			} while (true);
			}
			match(RPAREN);
			if ( inputState.guessing==0 ) {
				setIsInNestedMultipleAssign(false);
			}
			if ( inputState.guessing==0 ) {
				mlhs_item_AST = (AST)currentAST.root;
				mlhs_item_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(NESTED_LHS,"NESTED_LHS")).add(mlhs_item_AST));
				currentAST.root = mlhs_item_AST;
				currentAST.child = mlhs_item_AST!=null &&mlhs_item_AST.getFirstChild()!=null ?
					mlhs_item_AST.getFirstChild() : mlhs_item_AST;
				currentAST.advanceChildToEnd();
			}
			mlhs_item_AST = (AST)currentAST.root;
		}
		else if ((LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==GLOBAL_VARIABLE||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==155)) {
			dotColonOrArrayAccess();
			astFactory.addASTChild(currentAST, returnAST);
			mlhs_item_AST = (AST)currentAST.root;
		}
		else {
			throw new NoViableAltException(LT(1), getFilename());
		}
		
		returnAST = mlhs_item_AST;
	}
	
	public final void mrhs() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST mrhs_AST = null;
		Token  seen_star = null;
		AST seen_star_AST = null;
		
		{
		switch ( LA(1)) {
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			expression();
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop37:
			do {
				if ((LA(1)==COMMA)) {
					match(COMMA);
					if ( inputState.guessing==0 ) {
						if (ASSIGN == LA(1)||RBRACK == LA(1)) break;
					}
					{
					switch ( LA(1)) {
					case REST_ARG_PREFIX:
					{
						seen_star = LT(1);
						seen_star_AST = astFactory.create(seen_star);
						astFactory.addASTChild(currentAST, seen_star_AST);
						match(REST_ARG_PREFIX);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case LITERAL_not:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					expression();
					astFactory.addASTChild(currentAST, returnAST);
					if ( inputState.guessing==0 ) {
						if (null != seen_star) break;
					}
				}
				else {
					break _loop37;
				}
				
			} while (true);
			}
			break;
		}
		case REST_ARG_PREFIX:
		{
			AST tmp37_AST = null;
			tmp37_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp37_AST);
			match(REST_ARG_PREFIX);
			expression();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState.guessing==0 ) {
			mrhs_AST = (AST)currentAST.root;
			mrhs_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(MRHS,"MRHS")).add(mrhs_AST));
			currentAST.root = mrhs_AST;
			currentAST.child = mrhs_AST!=null &&mrhs_AST.getFirstChild()!=null ?
				mrhs_AST.getFirstChild() : mrhs_AST;
			currentAST.advanceChildToEnd();
		}
		mrhs_AST = (AST)currentAST.root;
		returnAST = mrhs_AST;
	}
	
	protected final void lparen() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST lparen_AST = null;
		
		switch ( LA(1)) {
		case LPAREN_WITH_NO_LEADING_SPACE:
		{
			match(LPAREN_WITH_NO_LEADING_SPACE);
			lparen_AST = (AST)currentAST.root;
			break;
		}
		case LPAREN:
		{
			match(LPAREN);
			lparen_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = lparen_AST;
	}
	
	public final void dotColonOrArrayAccess() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST dotColonOrArrayAccess_AST = null;
		
		unaryExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop182:
		do {
			if ((LA(1)==DOT||LA(1)==COLON2||LA(1)==LBRACK_ARRAY_ACCESS||LA(1)==EMPTY_ARRAY_ACCESS)) {
				{
				switch ( LA(1)) {
				case DOT:
				{
					AST tmp40_AST = null;
					tmp40_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp40_AST);
					match(DOT);
					methodCall();
					astFactory.addASTChild(currentAST, returnAST);
					if ( inputState.guessing==0 ) {
						can_be_command_ = 3;
					}
					break;
				}
				case COLON2:
				{
					AST tmp41_AST = null;
					tmp41_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp41_AST);
					match(COLON2);
					methodCall();
					astFactory.addASTChild(currentAST, returnAST);
					if ( inputState.guessing==0 ) {
						can_be_command_ = 1;
					}
					break;
				}
				case LBRACK_ARRAY_ACCESS:
				{
					AST tmp42_AST = null;
					tmp42_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp42_AST);
					match(LBRACK_ARRAY_ACCESS);
					{
					switch ( LA(1)) {
					case REST_ARG_PREFIX:
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case LITERAL_not:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						arrayReferenceArgument();
						astFactory.addASTChild(currentAST, returnAST);
						break;
					}
					case RBRACK:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					match(RBRACK);
					if ( inputState.guessing==0 ) {
						can_be_command_ = 1;
					}
					break;
				}
				case EMPTY_ARRAY_ACCESS:
				{
					AST tmp44_AST = null;
					tmp44_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp44_AST);
					match(EMPTY_ARRAY_ACCESS);
					if ( inputState.guessing==0 ) {
						can_be_command_ = 1;
					}
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
			}
			else {
				break _loop182;
			}
			
		} while (true);
		}
		dotColonOrArrayAccess_AST = (AST)currentAST.root;
		returnAST = dotColonOrArrayAccess_AST;
	}
	
	protected final void undef_parameter() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST undef_parameter_AST = null;
		
		switch ( LA(1)) {
		case IDENTIFIER:
		{
			AST tmp45_AST = null;
			tmp45_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp45_AST);
			match(IDENTIFIER);
			{
			switch ( LA(1)) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				AST tmp46_AST = null;
				tmp46_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp46_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
				break;
			}
			case EOF:
			case SEMI:
			case LINE_BREAK:
			case IF_MODIFIER:
			case UNLESS_MODIFIER:
			case WHILE_MODIFIER:
			case UNTIL_MODIFIER:
			case RESCUE_MODIFIER:
			case RCURLY:
			case COMMA:
			case RPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case LITERAL_do:
			case LITERAL_end:
			case BOR:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case LITERAL_and:
			case LITERAL_or:
			case LITERAL_not:
			case COMPARE:
			case EQUAL:
			case CASE_EQUAL:
			case MATCH:
			case LESS_THAN:
			case GREATER_THAN:
			case LESS_OR_EQUAL:
			case GREATER_OR_EQUAL:
			case BXOR:
			case BAND:
			case LEFT_SHIFT:
			case RIGHT_SHIFT:
			case PLUS:
			case MINUS:
			case STAR:
			case DIV:
			case MOD:
			case POWER:
			case BNOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case LITERAL_super:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LITERAL_rescue:
			case LITERAL_else:
			case LITERAL_ensure:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_elsif:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_in:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case SINGLE_QUOTE:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_then:
			case LITERAL_when:
			case LITERAL_END:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			undef_parameter_AST = (AST)currentAST.root;
			break;
		}
		case CONSTANT:
		{
			AST tmp47_AST = null;
			tmp47_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp47_AST);
			match(CONSTANT);
			{
			switch ( LA(1)) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				AST tmp48_AST = null;
				tmp48_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp48_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
				break;
			}
			case EOF:
			case SEMI:
			case LINE_BREAK:
			case IF_MODIFIER:
			case UNLESS_MODIFIER:
			case WHILE_MODIFIER:
			case UNTIL_MODIFIER:
			case RESCUE_MODIFIER:
			case RCURLY:
			case COMMA:
			case RPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case LITERAL_do:
			case LITERAL_end:
			case BOR:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case LITERAL_and:
			case LITERAL_or:
			case LITERAL_not:
			case COMPARE:
			case EQUAL:
			case CASE_EQUAL:
			case MATCH:
			case LESS_THAN:
			case GREATER_THAN:
			case LESS_OR_EQUAL:
			case GREATER_OR_EQUAL:
			case BXOR:
			case BAND:
			case LEFT_SHIFT:
			case RIGHT_SHIFT:
			case PLUS:
			case MINUS:
			case STAR:
			case DIV:
			case MOD:
			case POWER:
			case BNOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case LITERAL_super:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LITERAL_rescue:
			case LITERAL_else:
			case LITERAL_ensure:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_elsif:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_in:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case SINGLE_QUOTE:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_then:
			case LITERAL_when:
			case LITERAL_END:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			undef_parameter_AST = (AST)currentAST.root;
			break;
		}
		case FUNCTION:
		{
			AST tmp49_AST = null;
			tmp49_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp49_AST);
			match(FUNCTION);
			{
			switch ( LA(1)) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				AST tmp50_AST = null;
				tmp50_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp50_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
				break;
			}
			case EOF:
			case SEMI:
			case LINE_BREAK:
			case IF_MODIFIER:
			case UNLESS_MODIFIER:
			case WHILE_MODIFIER:
			case UNTIL_MODIFIER:
			case RESCUE_MODIFIER:
			case RCURLY:
			case COMMA:
			case RPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case LITERAL_do:
			case LITERAL_end:
			case BOR:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case LITERAL_and:
			case LITERAL_or:
			case LITERAL_not:
			case COMPARE:
			case EQUAL:
			case CASE_EQUAL:
			case MATCH:
			case LESS_THAN:
			case GREATER_THAN:
			case LESS_OR_EQUAL:
			case GREATER_OR_EQUAL:
			case BXOR:
			case BAND:
			case LEFT_SHIFT:
			case RIGHT_SHIFT:
			case PLUS:
			case MINUS:
			case STAR:
			case DIV:
			case MOD:
			case POWER:
			case BNOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case LITERAL_super:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LITERAL_rescue:
			case LITERAL_else:
			case LITERAL_ensure:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_elsif:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_in:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case SINGLE_QUOTE:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_then:
			case LITERAL_when:
			case LITERAL_END:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			undef_parameter_AST = (AST)currentAST.root;
			break;
		}
		case COLON_WITH_NO_FOLLOWING_SPACE:
		{
			symbol();
			astFactory.addASTChild(currentAST, returnAST);
			undef_parameter_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_undef:
		case LITERAL_alias:
		case LITERAL_do:
		case LITERAL_end:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case LITERAL_super:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_elsif:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_in:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_then:
		case LITERAL_when:
		case LITERAL_END:
		{
			keywordAsMethodName();
			astFactory.addASTChild(currentAST, returnAST);
			undef_parameter_AST = (AST)currentAST.root;
			break;
		}
		case BOR:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case MATCH:
		case LESS_THAN:
		case GREATER_THAN:
		case LESS_OR_EQUAL:
		case GREATER_OR_EQUAL:
		case BXOR:
		case BAND:
		case LEFT_SHIFT:
		case RIGHT_SHIFT:
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
		case MOD:
		case POWER:
		case BNOT:
		case EMPTY_ARRAY_ACCESS:
		case EMPTY_ARRAY:
		case SINGLE_QUOTE:
		{
			operatorAsMethodname();
			astFactory.addASTChild(currentAST, returnAST);
			undef_parameter_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = undef_parameter_AST;
	}
	
	public final void symbol() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST symbol_AST = null;
		
		match(COLON_WITH_NO_FOLLOWING_SPACE);
		{
		switch ( LA(1)) {
		case IDENTIFIER:
		{
			AST tmp52_AST = null;
			tmp52_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp52_AST);
			match(IDENTIFIER);
			{
			if ((LA(1)==ASSIGN_WITH_NO_LEADING_SPACE)) {
				AST tmp53_AST = null;
				tmp53_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp53_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
			}
			else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==RPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==LITERAL_undef||LA(1)==LITERAL_alias||LA(1)==GLOBAL_VARIABLE||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==LITERAL_not||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==DOT||LA(1)==COLON2||LA(1)==LBRACK_ARRAY_ACCESS||LA(1)==RBRACK||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_elsif||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_in||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==SINGLE_QUOTE||LA(1)==155||LA(1)==LITERAL_BEGIN||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==LITERAL_END||LA(1)==DO_IN_CONDITION)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			break;
		}
		case FUNCTION:
		{
			AST tmp54_AST = null;
			tmp54_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp54_AST);
			match(FUNCTION);
			{
			if ((LA(1)==ASSIGN_WITH_NO_LEADING_SPACE)) {
				AST tmp55_AST = null;
				tmp55_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp55_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
			}
			else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==RPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==LITERAL_undef||LA(1)==LITERAL_alias||LA(1)==GLOBAL_VARIABLE||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==LITERAL_not||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==DOT||LA(1)==COLON2||LA(1)==LBRACK_ARRAY_ACCESS||LA(1)==RBRACK||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_elsif||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_in||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==SINGLE_QUOTE||LA(1)==155||LA(1)==LITERAL_BEGIN||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==LITERAL_END||LA(1)==DO_IN_CONDITION)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			break;
		}
		case CONSTANT:
		{
			AST tmp56_AST = null;
			tmp56_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp56_AST);
			match(CONSTANT);
			{
			if ((LA(1)==ASSIGN_WITH_NO_LEADING_SPACE)) {
				AST tmp57_AST = null;
				tmp57_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp57_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
			}
			else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==RPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==LITERAL_undef||LA(1)==LITERAL_alias||LA(1)==GLOBAL_VARIABLE||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==LITERAL_not||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==DOT||LA(1)==COLON2||LA(1)==LBRACK_ARRAY_ACCESS||LA(1)==RBRACK||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_elsif||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_in||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==SINGLE_QUOTE||LA(1)==155||LA(1)==LITERAL_BEGIN||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==LITERAL_END||LA(1)==DO_IN_CONDITION)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			break;
		}
		case GLOBAL_VARIABLE:
		{
			AST tmp58_AST = null;
			tmp58_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp58_AST);
			match(GLOBAL_VARIABLE);
			break;
		}
		case INSTANCE_VARIABLE:
		{
			AST tmp59_AST = null;
			tmp59_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp59_AST);
			match(INSTANCE_VARIABLE);
			break;
		}
		case CLASS_VARIABLE:
		{
			AST tmp60_AST = null;
			tmp60_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp60_AST);
			match(CLASS_VARIABLE);
			break;
		}
		case UNARY_PLUS_MINUS_METHOD_NAME:
		{
			AST tmp61_AST = null;
			tmp61_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp61_AST);
			match(UNARY_PLUS_MINUS_METHOD_NAME);
			break;
		}
		case BOR:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case MATCH:
		case LESS_THAN:
		case GREATER_THAN:
		case LESS_OR_EQUAL:
		case GREATER_OR_EQUAL:
		case BXOR:
		case BAND:
		case LEFT_SHIFT:
		case RIGHT_SHIFT:
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
		case MOD:
		case POWER:
		case BNOT:
		case EMPTY_ARRAY_ACCESS:
		case EMPTY_ARRAY:
		case SINGLE_QUOTE:
		{
			operatorAsMethodname();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_undef:
		case LITERAL_alias:
		case LITERAL_do:
		case LITERAL_end:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case LITERAL_self:
		case LITERAL_super:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_elsif:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_in:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_then:
		case LITERAL_when:
		case LITERAL_END:
		{
			keyword();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			string();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState.guessing==0 ) {
			tellLexerWeHaveFinishedParsingSymbol();
		}
		if ( inputState.guessing==0 ) {
			symbol_AST = (AST)currentAST.root;
			symbol_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(SYMBOL,"SYMBOL")).add(symbol_AST));
			currentAST.root = symbol_AST;
			currentAST.child = symbol_AST!=null &&symbol_AST.getFirstChild()!=null ?
				symbol_AST.getFirstChild() : symbol_AST;
			currentAST.advanceChildToEnd();
		}
		symbol_AST = (AST)currentAST.root;
		returnAST = symbol_AST;
	}
	
	public final void keywordAsMethodName() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keywordAsMethodName_AST = null;
		
		switch ( LA(1)) {
		case LITERAL_and:
		{
			AST tmp62_AST = null;
			tmp62_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp62_AST);
			match(LITERAL_and);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_def:
		{
			AST tmp63_AST = null;
			tmp63_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp63_AST);
			match(LITERAL_def);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_end:
		{
			AST tmp64_AST = null;
			tmp64_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp64_AST);
			match(LITERAL_end);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_in:
		{
			AST tmp65_AST = null;
			tmp65_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp65_AST);
			match(LITERAL_in);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_or:
		{
			AST tmp66_AST = null;
			tmp66_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp66_AST);
			match(LITERAL_or);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_unless:
		{
			AST tmp67_AST = null;
			tmp67_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp67_AST);
			match(LITERAL_unless);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_begin:
		{
			AST tmp68_AST = null;
			tmp68_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp68_AST);
			match(LITERAL_begin);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case 155:
		{
			AST tmp69_AST = null;
			tmp69_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp69_AST);
			match(155);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_ensure:
		{
			AST tmp70_AST = null;
			tmp70_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp70_AST);
			match(LITERAL_ensure);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_module:
		{
			AST tmp71_AST = null;
			tmp71_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp71_AST);
			match(LITERAL_module);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_redo:
		{
			AST tmp72_AST = null;
			tmp72_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp72_AST);
			match(LITERAL_redo);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_super:
		{
			AST tmp73_AST = null;
			tmp73_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp73_AST);
			match(LITERAL_super);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_until:
		{
			AST tmp74_AST = null;
			tmp74_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp74_AST);
			match(LITERAL_until);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_BEGIN:
		{
			AST tmp75_AST = null;
			tmp75_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp75_AST);
			match(LITERAL_BEGIN);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_break:
		{
			AST tmp76_AST = null;
			tmp76_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp76_AST);
			match(LITERAL_break);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_do:
		{
			AST tmp77_AST = null;
			tmp77_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp77_AST);
			match(LITERAL_do);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_next:
		{
			AST tmp78_AST = null;
			tmp78_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp78_AST);
			match(LITERAL_next);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_rescue:
		{
			AST tmp79_AST = null;
			tmp79_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp79_AST);
			match(LITERAL_rescue);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_then:
		{
			AST tmp80_AST = null;
			tmp80_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp80_AST);
			match(LITERAL_then);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_when:
		{
			AST tmp81_AST = null;
			tmp81_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp81_AST);
			match(LITERAL_when);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_END:
		{
			AST tmp82_AST = null;
			tmp82_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp82_AST);
			match(LITERAL_END);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_case:
		{
			AST tmp83_AST = null;
			tmp83_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp83_AST);
			match(LITERAL_case);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_else:
		{
			AST tmp84_AST = null;
			tmp84_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp84_AST);
			match(LITERAL_else);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_for:
		{
			AST tmp85_AST = null;
			tmp85_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp85_AST);
			match(LITERAL_for);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_retry:
		{
			AST tmp86_AST = null;
			tmp86_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp86_AST);
			match(LITERAL_retry);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_while:
		{
			AST tmp87_AST = null;
			tmp87_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp87_AST);
			match(LITERAL_while);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_alias:
		{
			AST tmp88_AST = null;
			tmp88_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp88_AST);
			match(LITERAL_alias);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_class:
		{
			AST tmp89_AST = null;
			tmp89_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp89_AST);
			match(LITERAL_class);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_elsif:
		{
			AST tmp90_AST = null;
			tmp90_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp90_AST);
			match(LITERAL_elsif);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_if:
		{
			AST tmp91_AST = null;
			tmp91_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp91_AST);
			match(LITERAL_if);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_not:
		{
			AST tmp92_AST = null;
			tmp92_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp92_AST);
			match(LITERAL_not);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_return:
		{
			AST tmp93_AST = null;
			tmp93_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp93_AST);
			match(LITERAL_return);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_undef:
		{
			AST tmp94_AST = null;
			tmp94_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp94_AST);
			match(LITERAL_undef);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_yield:
		{
			AST tmp95_AST = null;
			tmp95_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp95_AST);
			match(LITERAL_yield);
			keywordAsMethodName_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = keywordAsMethodName_AST;
	}
	
	public final void operatorAsMethodname() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST operatorAsMethodname_AST = null;
		
		switch ( LA(1)) {
		case LEFT_SHIFT:
		{
			AST tmp96_AST = null;
			tmp96_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp96_AST);
			match(LEFT_SHIFT);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case RIGHT_SHIFT:
		{
			AST tmp97_AST = null;
			tmp97_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp97_AST);
			match(RIGHT_SHIFT);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case EQUAL:
		{
			AST tmp98_AST = null;
			tmp98_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp98_AST);
			match(EQUAL);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case CASE_EQUAL:
		{
			AST tmp99_AST = null;
			tmp99_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp99_AST);
			match(CASE_EQUAL);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case GREATER_THAN:
		{
			AST tmp100_AST = null;
			tmp100_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp100_AST);
			match(GREATER_THAN);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case GREATER_OR_EQUAL:
		{
			AST tmp101_AST = null;
			tmp101_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp101_AST);
			match(GREATER_OR_EQUAL);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case LESS_THAN:
		{
			AST tmp102_AST = null;
			tmp102_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp102_AST);
			match(LESS_THAN);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case LESS_OR_EQUAL:
		{
			AST tmp103_AST = null;
			tmp103_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp103_AST);
			match(LESS_OR_EQUAL);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case PLUS:
		{
			AST tmp104_AST = null;
			tmp104_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp104_AST);
			match(PLUS);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case MINUS:
		{
			AST tmp105_AST = null;
			tmp105_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp105_AST);
			match(MINUS);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case STAR:
		{
			AST tmp106_AST = null;
			tmp106_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp106_AST);
			match(STAR);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case DIV:
		{
			AST tmp107_AST = null;
			tmp107_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp107_AST);
			match(DIV);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case MOD:
		{
			AST tmp108_AST = null;
			tmp108_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp108_AST);
			match(MOD);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case POWER:
		{
			AST tmp109_AST = null;
			tmp109_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp109_AST);
			match(POWER);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case BAND:
		{
			AST tmp110_AST = null;
			tmp110_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp110_AST);
			match(BAND);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case BOR:
		{
			AST tmp111_AST = null;
			tmp111_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp111_AST);
			match(BOR);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case BXOR:
		{
			AST tmp112_AST = null;
			tmp112_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp112_AST);
			match(BXOR);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case EMPTY_ARRAY_ACCESS:
		case EMPTY_ARRAY:
		{
			{
			switch ( LA(1)) {
			case EMPTY_ARRAY:
			{
				AST tmp113_AST = null;
				tmp113_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp113_AST);
				match(EMPTY_ARRAY);
				break;
			}
			case EMPTY_ARRAY_ACCESS:
			{
				AST tmp114_AST = null;
				tmp114_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp114_AST);
				match(EMPTY_ARRAY_ACCESS);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			if ((LA(1)==ASSIGN_WITH_NO_LEADING_SPACE)) {
				AST tmp115_AST = null;
				tmp115_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp115_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
			}
			else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==RPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==LITERAL_undef||LA(1)==LITERAL_alias||LA(1)==GLOBAL_VARIABLE||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==LITERAL_not||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==DOT||LA(1)==COLON2||LA(1)==LBRACK_ARRAY_ACCESS||LA(1)==RBRACK||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_elsif||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_in||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==SINGLE_QUOTE||LA(1)==155||LA(1)==LITERAL_BEGIN||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==LITERAL_END||LA(1)==DO_IN_CONDITION)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case MATCH:
		{
			AST tmp116_AST = null;
			tmp116_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp116_AST);
			match(MATCH);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case COMPARE:
		{
			AST tmp117_AST = null;
			tmp117_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp117_AST);
			match(COMPARE);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case BNOT:
		{
			AST tmp118_AST = null;
			tmp118_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp118_AST);
			match(BNOT);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		case SINGLE_QUOTE:
		{
			AST tmp119_AST = null;
			tmp119_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp119_AST);
			match(SINGLE_QUOTE);
			operatorAsMethodname_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = operatorAsMethodname_AST;
	}
	
	public final void block_vars() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST block_vars_AST = null;
		Token  seen_star = null;
		AST seen_star_AST = null;
		
			boolean has_extra_comma = false;
		
		
		{
		switch ( LA(1)) {
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			mlhs_item();
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop56:
			do {
				if ((LA(1)==COMMA)) {
					match(COMMA);
					if ( inputState.guessing==0 ) {
						if (BOR == LA(1) || LITERAL_in == LA(1)) {has_extra_comma = true; break;}
					}
					{
					switch ( LA(1)) {
					case REST_ARG_PREFIX:
					{
						seen_star = LT(1);
						seen_star_AST = astFactory.create(seen_star);
						astFactory.addASTChild(currentAST, seen_star_AST);
						match(REST_ARG_PREFIX);
						break;
					}
					case COMMA:
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case BOR:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_in:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					{
					switch ( LA(1)) {
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						mlhs_item();
						astFactory.addASTChild(currentAST, returnAST);
						break;
					}
					case COMMA:
					case BOR:
					case LITERAL_in:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					if ( inputState.guessing==0 ) {
						if (null != seen_star) break;
					}
				}
				else {
					break _loop56;
				}
				
			} while (true);
			}
			break;
		}
		case REST_ARG_PREFIX:
		{
			AST tmp121_AST = null;
			tmp121_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp121_AST);
			match(REST_ARG_PREFIX);
			{
			switch ( LA(1)) {
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			{
				dotColonOrArrayAccess();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case BOR:
			case LITERAL_in:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState.guessing==0 ) {
			block_vars_AST = (AST)currentAST.root;
				if (has_extra_comma) {
								block_vars_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(BLOCK_ARG_WITH_EXTRA_COMMA,"BLOCK_ARG_WITH_EXTRA_COMMA")).add(block_vars_AST));
							} else {
								block_vars_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(BLOCK_ARG,"BLOCK_ARG")).add(block_vars_AST));
							}
						
			currentAST.root = block_vars_AST;
			currentAST.child = block_vars_AST!=null &&block_vars_AST.getFirstChild()!=null ?
				block_vars_AST.getFirstChild() : block_vars_AST;
			currentAST.advanceChildToEnd();
		}
		block_vars_AST = (AST)currentAST.root;
		returnAST = block_vars_AST;
	}
	
	public final void codeBlock() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST codeBlock_AST = null;
		
		{
		switch ( LA(1)) {
		case LITERAL_do:
		{
			match(LITERAL_do);
			{
			if ((LA(1)==LINE_BREAK)) {
				match(LINE_BREAK);
			}
			else if ((LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==REST_ARG_PREFIX||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==LITERAL_undef||LA(1)==LITERAL_alias||LA(1)==GLOBAL_VARIABLE||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_not||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==155||LA(1)==LITERAL_BEGIN||LA(1)==LITERAL_END)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			if ( inputState.guessing==0 ) {
				enterBlockScope();
			}
			blockContent();
			astFactory.addASTChild(currentAST, returnAST);
			match(LITERAL_end);
			if ( inputState.guessing==0 ) {
				leaveScope();
			}
			break;
		}
		case LCURLY_BLOCK:
		{
			match(LCURLY_BLOCK);
			if ( inputState.guessing==0 ) {
				enterBlockScope();
			}
			blockContent();
			astFactory.addASTChild(currentAST, returnAST);
			match(RCURLY);
			if ( inputState.guessing==0 ) {
				leaveScope();
			}
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState.guessing==0 ) {
			codeBlock_AST = (AST)currentAST.root;
			codeBlock_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(BLOCK,"BLOCK")).add(codeBlock_AST));
			currentAST.root = codeBlock_AST;
			currentAST.child = codeBlock_AST!=null &&codeBlock_AST.getFirstChild()!=null ?
				codeBlock_AST.getFirstChild() : codeBlock_AST;
			currentAST.advanceChildToEnd();
		}
		codeBlock_AST = (AST)currentAST.root;
		returnAST = codeBlock_AST;
	}
	
	protected final void blockContent() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST blockContent_AST = null;
		
		{
		switch ( LA(1)) {
		case BOR:
		{
			AST tmp127_AST = null;
			tmp127_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp127_AST);
			match(BOR);
			{
			switch ( LA(1)) {
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			{
				block_vars();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case BOR:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(BOR);
			break;
		}
		case LOGICAL_OR:
		{
			AST tmp129_AST = null;
			tmp129_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp129_AST);
			match(LOGICAL_OR);
			break;
		}
		case SEMI:
		case LINE_BREAK:
		case RCURLY:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case LITERAL_end:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			compoundStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case RCURLY:
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		blockContent_AST = (AST)currentAST.root;
		returnAST = blockContent_AST;
	}
	
	public final void keyword() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_AST = null;
		
		switch ( LA(1)) {
		case LITERAL_undef:
		case LITERAL_alias:
		case LITERAL_do:
		case LITERAL_end:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case LITERAL_super:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_elsif:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_in:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_then:
		case LITERAL_when:
		case LITERAL_END:
		{
			keywordAsMethodName();
			astFactory.addASTChild(currentAST, returnAST);
			keyword_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_nil:
		{
			AST tmp130_AST = null;
			tmp130_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp130_AST);
			match(LITERAL_nil);
			keyword_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_self:
		{
			AST tmp131_AST = null;
			tmp131_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp131_AST);
			match(LITERAL_self);
			keyword_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_true:
		{
			AST tmp132_AST = null;
			tmp132_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp132_AST);
			match(LITERAL_true);
			keyword_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_false:
		{
			AST tmp133_AST = null;
			tmp133_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp133_AST);
			match(LITERAL_false);
			keyword_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL___FILE__:
		{
			AST tmp134_AST = null;
			tmp134_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp134_AST);
			match(LITERAL___FILE__);
			keyword_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL___LINE__:
		{
			AST tmp135_AST = null;
			tmp135_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp135_AST);
			match(LITERAL___LINE__);
			keyword_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = keyword_AST;
	}
	
	public final void string() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST string_AST = null;
		
		switch ( LA(1)) {
		case DOUBLE_QUOTE_STRING:
		{
			AST tmp136_AST = null;
			tmp136_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp136_AST);
			match(DOUBLE_QUOTE_STRING);
			string_AST = (AST)currentAST.root;
			break;
		}
		case SINGLE_QUOTE_STRING:
		{
			AST tmp137_AST = null;
			tmp137_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp137_AST);
			match(SINGLE_QUOTE_STRING);
			string_AST = (AST)currentAST.root;
			break;
		}
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			AST tmp138_AST = null;
			tmp138_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, tmp138_AST);
			match(STRING_BEFORE_EXPRESSION_SUBSTITUTION);
			expression_substituation();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				tellLexerWeHaveFinishedParsingStringExpressionSubstituation();
			}
			{
			_loop209:
			do {
				if ((LA(1)==STRING_BETWEEN_EXPRESSION_SUBSTITUTION)) {
					AST tmp139_AST = null;
					tmp139_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp139_AST);
					match(STRING_BETWEEN_EXPRESSION_SUBSTITUTION);
					expression_substituation();
					astFactory.addASTChild(currentAST, returnAST);
					if ( inputState.guessing==0 ) {
						tellLexerWeHaveFinishedParsingStringExpressionSubstituation();
					}
				}
				else {
					break _loop209;
				}
				
			} while (true);
			}
			AST tmp140_AST = null;
			tmp140_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp140_AST);
			match(STRING_AFTER_EXPRESSION_SUBSTITUTION);
			string_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = string_AST;
	}
	
	public final void andorExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST andorExpression_AST = null;
		
		notExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop76:
		do {
			if ((LA(1)==LITERAL_and||LA(1)==LITERAL_or)) {
				{
				switch ( LA(1)) {
				case LITERAL_and:
				{
					AST tmp141_AST = null;
					tmp141_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp141_AST);
					match(LITERAL_and);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case LITERAL_not:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case LITERAL_or:
				{
					AST tmp143_AST = null;
					tmp143_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp143_AST);
					match(LITERAL_or);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case LITERAL_not:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				notExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop76;
			}
			
		} while (true);
		}
		andorExpression_AST = (AST)currentAST.root;
		returnAST = andorExpression_AST;
	}
	
	public final void notExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST notExpression_AST = null;
		
		switch ( LA(1)) {
		case LITERAL_not:
		{
			AST tmp145_AST = null;
			tmp145_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, tmp145_AST);
			match(LITERAL_not);
			{
			switch ( LA(1)) {
			case LINE_BREAK:
			{
				match(LINE_BREAK);
				break;
			}
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			notExpression();
			astFactory.addASTChild(currentAST, returnAST);
			notExpression_AST = (AST)currentAST.root;
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			ternaryIfThenElseExpression();
			astFactory.addASTChild(currentAST, returnAST);
			notExpression_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = notExpression_AST;
	}
	
	public final void ternaryIfThenElseExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST ternaryIfThenElseExpression_AST = null;
		
		assignmentExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		if ((LA(1)==QUESTION)) {
			AST tmp147_AST = null;
			tmp147_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, tmp147_AST);
			match(QUESTION);
			{
			switch ( LA(1)) {
			case LINE_BREAK:
			{
				match(LINE_BREAK);
				break;
			}
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			ternaryIfThenElseExpression();
			astFactory.addASTChild(currentAST, returnAST);
			{
			switch ( LA(1)) {
			case COLON:
			{
				match(COLON);
				break;
			}
			case COLON_WITH_NO_FOLLOWING_SPACE:
			{
				match(COLON_WITH_NO_FOLLOWING_SPACE);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case LINE_BREAK:
			{
				match(LINE_BREAK);
				break;
			}
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			ternaryIfThenElseExpression();
			astFactory.addASTChild(currentAST, returnAST);
		}
		else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==RPAREN||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==RBRACK||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_elsif||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
		}
		else {
			throw new NoViableAltException(LT(1), getFilename());
		}
		
		}
		ternaryIfThenElseExpression_AST = (AST)currentAST.root;
		returnAST = ternaryIfThenElseExpression_AST;
	}
	
	public final void assignmentExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST assignmentExpression_AST = null;
		Token  a = null;
		AST a_AST = null;
		
		rangeExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop102:
		do {
			if (((LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN))&&(REST_ARG_PREFIX != LA(2))) {
				{
				switch ( LA(1)) {
				case ASSIGN:
				{
					AST tmp152_AST = null;
					tmp152_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp152_AST);
					match(ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case ASSIGN_WITH_NO_LEADING_SPACE:
				{
					a = LT(1);
					a_AST = astFactory.create(a);
					astFactory.makeASTRoot(currentAST, a_AST);
					match(ASSIGN_WITH_NO_LEADING_SPACE);
					if ( inputState.guessing==0 ) {
						{a_AST.setType(ASSIGN);}
					}
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case PLUS_ASSIGN:
				{
					AST tmp155_AST = null;
					tmp155_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp155_AST);
					match(PLUS_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case MINUS_ASSIGN:
				{
					AST tmp157_AST = null;
					tmp157_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp157_AST);
					match(MINUS_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case STAR_ASSIGN:
				{
					AST tmp159_AST = null;
					tmp159_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp159_AST);
					match(STAR_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case DIV_ASSIGN:
				{
					AST tmp161_AST = null;
					tmp161_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp161_AST);
					match(DIV_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case MOD_ASSIGN:
				{
					AST tmp163_AST = null;
					tmp163_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp163_AST);
					match(MOD_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case POWER_ASSIGN:
				{
					AST tmp165_AST = null;
					tmp165_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp165_AST);
					match(POWER_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case BAND_ASSIGN:
				{
					AST tmp167_AST = null;
					tmp167_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp167_AST);
					match(BAND_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case BXOR_ASSIGN:
				{
					AST tmp169_AST = null;
					tmp169_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp169_AST);
					match(BXOR_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case BOR_ASSIGN:
				{
					AST tmp171_AST = null;
					tmp171_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp171_AST);
					match(BOR_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case LEFT_SHIFT_ASSIGN:
				{
					AST tmp173_AST = null;
					tmp173_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp173_AST);
					match(LEFT_SHIFT_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case RIGHT_SHIFT_ASSIGN:
				{
					AST tmp175_AST = null;
					tmp175_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp175_AST);
					match(RIGHT_SHIFT_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case LOGICAL_AND_ASSIGN:
				{
					AST tmp177_AST = null;
					tmp177_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp177_AST);
					match(LOGICAL_AND_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case LOGICAL_OR_ASSIGN:
				{
					AST tmp179_AST = null;
					tmp179_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp179_AST);
					match(LOGICAL_OR_ASSIGN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				ternaryIfThenElseExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop102;
			}
			
		} while (true);
		}
		assignmentExpression_AST = (AST)currentAST.root;
		returnAST = assignmentExpression_AST;
	}
	
	public final void rangeExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST rangeExpression_AST = null;
		
		logicalOrExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop108:
		do {
			if ((LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE)) {
				{
				switch ( LA(1)) {
				case INCLUSIVE_RANGE:
				{
					AST tmp181_AST = null;
					tmp181_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp181_AST);
					match(INCLUSIVE_RANGE);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case EXCLUSIVE_RANGE:
				{
					AST tmp183_AST = null;
					tmp183_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp183_AST);
					match(EXCLUSIVE_RANGE);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				logicalOrExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop108;
			}
			
		} while (true);
		}
		rangeExpression_AST = (AST)currentAST.root;
		returnAST = rangeExpression_AST;
	}
	
	public final void logicalOrExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST logicalOrExpression_AST = null;
		
		logicalAndExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop112:
		do {
			if ((LA(1)==LOGICAL_OR)) {
				AST tmp185_AST = null;
				tmp185_AST = astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp185_AST);
				match(LOGICAL_OR);
				{
				switch ( LA(1)) {
				case LINE_BREAK:
				{
					match(LINE_BREAK);
					break;
				}
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				logicalAndExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop112;
			}
			
		} while (true);
		}
		logicalOrExpression_AST = (AST)currentAST.root;
		returnAST = logicalOrExpression_AST;
	}
	
	public final void logicalAndExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST logicalAndExpression_AST = null;
		
		equalityExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop116:
		do {
			if ((LA(1)==LOGICAL_AND)) {
				AST tmp187_AST = null;
				tmp187_AST = astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp187_AST);
				match(LOGICAL_AND);
				{
				switch ( LA(1)) {
				case LINE_BREAK:
				{
					match(LINE_BREAK);
					break;
				}
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				equalityExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop116;
			}
			
		} while (true);
		}
		logicalAndExpression_AST = (AST)currentAST.root;
		returnAST = logicalAndExpression_AST;
	}
	
	public final void equalityExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST equalityExpression_AST = null;
		
		relationalExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop126:
		do {
			if (((LA(1) >= COMPARE && LA(1) <= NOT_MATCH))) {
				{
				switch ( LA(1)) {
				case COMPARE:
				{
					AST tmp189_AST = null;
					tmp189_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp189_AST);
					match(COMPARE);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case EQUAL:
				{
					AST tmp191_AST = null;
					tmp191_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp191_AST);
					match(EQUAL);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case CASE_EQUAL:
				{
					AST tmp193_AST = null;
					tmp193_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp193_AST);
					match(CASE_EQUAL);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case NOT_EQUAL:
				{
					AST tmp195_AST = null;
					tmp195_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp195_AST);
					match(NOT_EQUAL);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case MATCH:
				{
					AST tmp197_AST = null;
					tmp197_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp197_AST);
					match(MATCH);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case NOT_MATCH:
				{
					AST tmp199_AST = null;
					tmp199_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp199_AST);
					match(NOT_MATCH);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				relationalExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop126;
			}
			
		} while (true);
		}
		equalityExpression_AST = (AST)currentAST.root;
		returnAST = equalityExpression_AST;
	}
	
	public final void relationalExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST relationalExpression_AST = null;
		
		orExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop134:
		do {
			if (((LA(1) >= LESS_THAN && LA(1) <= GREATER_OR_EQUAL))) {
				{
				switch ( LA(1)) {
				case LESS_THAN:
				{
					AST tmp201_AST = null;
					tmp201_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp201_AST);
					match(LESS_THAN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case GREATER_THAN:
				{
					AST tmp203_AST = null;
					tmp203_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp203_AST);
					match(GREATER_THAN);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case LESS_OR_EQUAL:
				{
					AST tmp205_AST = null;
					tmp205_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp205_AST);
					match(LESS_OR_EQUAL);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case GREATER_OR_EQUAL:
				{
					AST tmp207_AST = null;
					tmp207_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp207_AST);
					match(GREATER_OR_EQUAL);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				orExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop134;
			}
			
		} while (true);
		}
		relationalExpression_AST = (AST)currentAST.root;
		returnAST = relationalExpression_AST;
	}
	
	public final void orExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST orExpression_AST = null;
		
		andExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop140:
		do {
			if ((LA(1)==BOR||LA(1)==BXOR)) {
				{
				switch ( LA(1)) {
				case BXOR:
				{
					AST tmp209_AST = null;
					tmp209_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp209_AST);
					match(BXOR);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case BOR:
				{
					AST tmp211_AST = null;
					tmp211_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp211_AST);
					match(BOR);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				andExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop140;
			}
			
		} while (true);
		}
		orExpression_AST = (AST)currentAST.root;
		returnAST = orExpression_AST;
	}
	
	public final void andExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST andExpression_AST = null;
		
		shiftExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop144:
		do {
			if ((LA(1)==BAND)) {
				AST tmp213_AST = null;
				tmp213_AST = astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp213_AST);
				match(BAND);
				{
				switch ( LA(1)) {
				case LINE_BREAK:
				{
					match(LINE_BREAK);
					break;
				}
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				shiftExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop144;
			}
			
		} while (true);
		}
		andExpression_AST = (AST)currentAST.root;
		returnAST = andExpression_AST;
	}
	
	public final void shiftExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST shiftExpression_AST = null;
		
		additiveExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop150:
		do {
			if ((LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT)) {
				{
				switch ( LA(1)) {
				case LEFT_SHIFT:
				{
					AST tmp215_AST = null;
					tmp215_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp215_AST);
					match(LEFT_SHIFT);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case RIGHT_SHIFT:
				{
					AST tmp217_AST = null;
					tmp217_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp217_AST);
					match(RIGHT_SHIFT);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				additiveExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop150;
			}
			
		} while (true);
		}
		shiftExpression_AST = (AST)currentAST.root;
		returnAST = shiftExpression_AST;
	}
	
	public final void additiveExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST additiveExpression_AST = null;
		
		multiplicativeExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop156:
		do {
			if ((LA(1)==PLUS||LA(1)==MINUS)) {
				{
				switch ( LA(1)) {
				case PLUS:
				{
					AST tmp219_AST = null;
					tmp219_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp219_AST);
					match(PLUS);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case MINUS:
				{
					AST tmp221_AST = null;
					tmp221_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp221_AST);
					match(MINUS);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				multiplicativeExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop156;
			}
			
		} while (true);
		}
		additiveExpression_AST = (AST)currentAST.root;
		returnAST = additiveExpression_AST;
	}
	
	public final void multiplicativeExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST multiplicativeExpression_AST = null;
		
		powerExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop163:
		do {
			if (((LA(1) >= STAR && LA(1) <= MOD))) {
				{
				switch ( LA(1)) {
				case STAR:
				{
					AST tmp223_AST = null;
					tmp223_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp223_AST);
					match(STAR);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case DIV:
				{
					AST tmp225_AST = null;
					tmp225_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp225_AST);
					match(DIV);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				case MOD:
				{
					AST tmp227_AST = null;
					tmp227_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp227_AST);
					match(MOD);
					{
					switch ( LA(1)) {
					case LINE_BREAK:
					{
						match(LINE_BREAK);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				powerExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop163;
			}
			
		} while (true);
		}
		multiplicativeExpression_AST = (AST)currentAST.root;
		returnAST = multiplicativeExpression_AST;
	}
	
	public final void powerExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST powerExpression_AST = null;
		
		bnotExpression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		_loop167:
		do {
			if ((LA(1)==POWER)) {
				AST tmp229_AST = null;
				tmp229_AST = astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp229_AST);
				match(POWER);
				{
				switch ( LA(1)) {
				case LINE_BREAK:
				{
					match(LINE_BREAK);
					break;
				}
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				bnotExpression();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else {
				break _loop167;
			}
			
		} while (true);
		}
		powerExpression_AST = (AST)currentAST.root;
		returnAST = powerExpression_AST;
	}
	
	public final void bnotExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST bnotExpression_AST = null;
		
		{
		_loop172:
		do {
			switch ( LA(1)) {
			case BNOT:
			{
				AST tmp231_AST = null;
				tmp231_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp231_AST);
				match(BNOT);
				{
				switch ( LA(1)) {
				case LINE_BREAK:
				{
					match(LINE_BREAK);
					break;
				}
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case NOT:
			{
				AST tmp233_AST = null;
				tmp233_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp233_AST);
				match(NOT);
				{
				switch ( LA(1)) {
				case LINE_BREAK:
				{
					match(LINE_BREAK);
					break;
				}
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			default:
			{
				break _loop172;
			}
			}
		} while (true);
		}
		command();
		astFactory.addASTChild(currentAST, returnAST);
		bnotExpression_AST = (AST)currentAST.root;
		returnAST = bnotExpression_AST;
	}
	
	public final void command() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST command_AST = null;
		
			boolean seen_arguments = false;
			boolean seen_block = false;
			int can_be_command = 0;
		
		
		switch ( LA(1)) {
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			dotColonOrArrayAccess();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				can_be_command = can_be_command_;
			}
			{
			if (((LA(1)==REST_ARG_PREFIX||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==GLOBAL_VARIABLE||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_not||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==155))&&(can_be_command > 0)) {
				methodInvocationArgumentWithoutParen(false);
				astFactory.addASTChild(currentAST, returnAST);
				if ( inputState.guessing==0 ) {
					seen_arguments = true;
				}
			}
			else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==RPAREN||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==RBRACK||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_elsif||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			{
			if (((LA(1)==LCURLY_BLOCK||LA(1)==LITERAL_do))&&(can_be_command > 1)) {
				codeBlock();
				astFactory.addASTChild(currentAST, returnAST);
				if ( inputState.guessing==0 ) {
					seen_block = true;
				}
			}
			else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==RPAREN||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==RBRACK||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_elsif||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			if ( inputState.guessing==0 ) {
				command_AST = (AST)currentAST.root;
				
								if (seen_arguments || seen_block || ((2 == can_be_command) && isNotAssign(LA(1))))
								{
									command_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(CALL,"CALL")).add(command_AST));
								}
							
				currentAST.root = command_AST;
				currentAST.child = command_AST!=null &&command_AST.getFirstChild()!=null ?
					command_AST.getFirstChild() : command_AST;
				currentAST.advanceChildToEnd();
			}
			command_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		{
			{
			switch ( LA(1)) {
			case LITERAL_return:
			{
				AST tmp235_AST = null;
				tmp235_AST = astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp235_AST);
				match(LITERAL_return);
				break;
			}
			case LITERAL_break:
			{
				AST tmp236_AST = null;
				tmp236_AST = astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp236_AST);
				match(LITERAL_break);
				break;
			}
			case LITERAL_next:
			{
				AST tmp237_AST = null;
				tmp237_AST = astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp237_AST);
				match(LITERAL_next);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			if ((LA(1)==REST_ARG_PREFIX||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==GLOBAL_VARIABLE||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_not||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==155)) {
				methodInvocationArgumentWithoutParen(false);
				astFactory.addASTChild(currentAST, returnAST);
			}
			else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==RPAREN||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==RBRACK||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_elsif||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			command_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = command_AST;
	}
	
	public final void methodInvocationArgumentWithoutParen(
		boolean should_ignore_line_break
	) throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST methodInvocationArgumentWithoutParen_AST = null;
		
		boolean seen_star_or_band = false;
		
		
		{
		switch ( LA(1)) {
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			normalMethodInvocationArgument(should_ignore_line_break);
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop198:
			do {
				if ((LA(1)==COMMA)) {
					match(COMMA);
					if ( inputState.guessing==0 ) {
						if (REST_ARG_PREFIX == LA(1) || BLOCK_ARG_PREFIX == LA(1))	{seen_star_or_band = true;break;}
					}
					normalMethodInvocationArgument(should_ignore_line_break);
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop198;
				}
				
			} while (true);
			}
			{
			if (((LA(1)==REST_ARG_PREFIX))&&(seen_star_or_band)) {
				restMethodInvocationArgument();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else if (((LA(1)==BLOCK_ARG_PREFIX))&&(seen_star_or_band)) {
				blockMethodInvocationArgument();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==RPAREN||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==RBRACK||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_elsif||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			break;
		}
		case REST_ARG_PREFIX:
		{
			restMethodInvocationArgument();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case BLOCK_ARG_PREFIX:
		{
			blockMethodInvocationArgument();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState.guessing==0 ) {
			methodInvocationArgumentWithoutParen_AST = (AST)currentAST.root;
			methodInvocationArgumentWithoutParen_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(ARG,"ARG")).add(methodInvocationArgumentWithoutParen_AST));
			currentAST.root = methodInvocationArgumentWithoutParen_AST;
			currentAST.child = methodInvocationArgumentWithoutParen_AST!=null &&methodInvocationArgumentWithoutParen_AST.getFirstChild()!=null ?
				methodInvocationArgumentWithoutParen_AST.getFirstChild() : methodInvocationArgumentWithoutParen_AST;
			currentAST.advanceChildToEnd();
		}
		methodInvocationArgumentWithoutParen_AST = (AST)currentAST.root;
		returnAST = methodInvocationArgumentWithoutParen_AST;
	}
	
	public final void unaryExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST unaryExpression_AST = null;
		
		{
		_loop187:
		do {
			switch ( LA(1)) {
			case UNARY_PLUS:
			{
				AST tmp239_AST = null;
				tmp239_AST = astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp239_AST);
				match(UNARY_PLUS);
				{
				switch ( LA(1)) {
				case LINE_BREAK:
				{
					match(LINE_BREAK);
					break;
				}
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case UNARY_MINUS:
			{
				AST tmp241_AST = null;
				tmp241_AST = astFactory.create(LT(1));
				astFactory.makeASTRoot(currentAST, tmp241_AST);
				match(UNARY_MINUS);
				{
				switch ( LA(1)) {
				case LINE_BREAK:
				{
					match(LINE_BREAK);
					break;
				}
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			default:
			{
				break _loop187;
			}
			}
		} while (true);
		}
		{
		switch ( LA(1)) {
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case EMPTY_ARRAY_ACCESS:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case 155:
		{
			methodCall();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		{
			primaryExpressionThatCanNotBeMethodName();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				can_be_command_ = 0;
			}
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		unaryExpression_AST = (AST)currentAST.root;
		returnAST = unaryExpression_AST;
	}
	
	public final void methodCall() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST methodCall_AST = null;
		AST name_AST = null;
		AST arg_AST = null;
		
			can_be_command_ = 1;
			boolean seen_arguments = false;
			boolean seen_block = false;
			boolean is_function = false;
		
		
		primaryExpressionThatCanBeMethodName();
		name_AST = (AST)returnAST;
		astFactory.addASTChild(currentAST, returnAST);
		if ( inputState.guessing==0 ) {
			is_function = (2 == can_be_command_);
		}
		{
		if ((LA(1)==LPAREN_WITH_NO_LEADING_SPACE)) {
			methodInvocationArgumentWithParen();
			arg_AST = (AST)returnAST;
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				seen_arguments = true;
			}
		}
		else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==RPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==GLOBAL_VARIABLE||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==LITERAL_not||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==DOT||LA(1)==COLON2||LA(1)==LBRACK_ARRAY_ACCESS||LA(1)==RBRACK||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_elsif||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_in||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==155||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
		}
		else {
			throw new NoViableAltException(LT(1), getFilename());
		}
		
		}
		{
		if ((LA(1)==LCURLY_BLOCK||LA(1)==LITERAL_do)) {
			codeBlock();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				seen_block = true;
			}
		}
		else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==RPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==GLOBAL_VARIABLE||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==LITERAL_not||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==DOT||LA(1)==COLON2||LA(1)==LBRACK_ARRAY_ACCESS||LA(1)==RBRACK||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_elsif||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_in||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==155||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
		}
		else {
			throw new NoViableAltException(LT(1), getFilename());
		}
		
		}
		if ( inputState.guessing==0 ) {
			methodCall_AST = (AST)currentAST.root;
			
							if (seen_arguments || seen_block || (is_function && (LA(1) == LBRACK_ARRAY_ACCESS)/*this is for a[1]*/))
							{
								methodCall_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(CALL,"CALL")).add(methodCall_AST));
								can_be_command_ = 0;
							}
						
			currentAST.root = methodCall_AST;
			currentAST.child = methodCall_AST!=null &&methodCall_AST.getFirstChild()!=null ?
				methodCall_AST.getFirstChild() : methodCall_AST;
			currentAST.advanceChildToEnd();
		}
		methodCall_AST = (AST)currentAST.root;
		returnAST = methodCall_AST;
	}
	
	public final void arrayReferenceArgument() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST arrayReferenceArgument_AST = null;
		Token  seen_star = null;
		AST seen_star_AST = null;
		
		switch ( LA(1)) {
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			keyValuePair();
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop233:
			do {
				if ((LA(1)==COMMA)) {
					match(COMMA);
					if ( inputState.guessing==0 ) {
						if (ASSIGN == LA(1)||RBRACK == LA(1)) break;
					}
					{
					switch ( LA(1)) {
					case REST_ARG_PREFIX:
					{
						seen_star = LT(1);
						seen_star_AST = astFactory.create(seen_star);
						astFactory.addASTChild(currentAST, seen_star_AST);
						match(REST_ARG_PREFIX);
						break;
					}
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case LITERAL_not:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					keyValuePair();
					astFactory.addASTChild(currentAST, returnAST);
					if ( inputState.guessing==0 ) {
						if (null != seen_star) break;
					}
				}
				else {
					break _loop233;
				}
				
			} while (true);
			}
			arrayReferenceArgument_AST = (AST)currentAST.root;
			break;
		}
		case REST_ARG_PREFIX:
		{
			AST tmp244_AST = null;
			tmp244_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp244_AST);
			match(REST_ARG_PREFIX);
			expression();
			astFactory.addASTChild(currentAST, returnAST);
			{
			switch ( LA(1)) {
			case LINE_BREAK:
			{
				match(LINE_BREAK);
				break;
			}
			case RBRACK:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			arrayReferenceArgument_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = arrayReferenceArgument_AST;
	}
	
	public final void primaryExpressionThatCanNotBeMethodName() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST primaryExpressionThatCanNotBeMethodName_AST = null;
		
		switch ( LA(1)) {
		case INSTANCE_VARIABLE:
		{
			AST tmp246_AST = null;
			tmp246_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp246_AST);
			match(INSTANCE_VARIABLE);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case GLOBAL_VARIABLE:
		{
			AST tmp247_AST = null;
			tmp247_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp247_AST);
			match(GLOBAL_VARIABLE);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case CLASS_VARIABLE:
		{
			AST tmp248_AST = null;
			tmp248_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp248_AST);
			match(CLASS_VARIABLE);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		{
			predefinedValue();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		{
			numeric();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		{
			literal();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LBRACK:
		{
			arrayExpression();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LCURLY_HASH:
		{
			hashExpression();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LPAREN:
		{
			AST tmp249_AST = null;
			tmp249_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, tmp249_AST);
			match(LPAREN);
			compoundStatement();
			astFactory.addASTChild(currentAST, returnAST);
			match(RPAREN);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		{
			AST tmp251_AST = null;
			tmp251_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, tmp251_AST);
			match(LPAREN_WITH_NO_LEADING_SPACE);
			compoundStatement();
			astFactory.addASTChild(currentAST, returnAST);
			match(RPAREN);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_if:
		{
			ifExpression();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_unless:
		{
			unlessExpression();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_while:
		{
			whileExpression();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_until:
		{
			untilExpression();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_case:
		{
			caseExpression();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_for:
		{
			forExpression();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_begin:
		{
			exceptionHandlingExpression();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_module:
		{
			moduleDefinition();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_class:
		{
			classDefinition();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_def:
		{
			methodDefinition();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanNotBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = primaryExpressionThatCanNotBeMethodName_AST;
	}
	
	public final void primaryExpressionThatCanBeMethodName() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST primaryExpressionThatCanBeMethodName_AST = null;
		
		switch ( LA(1)) {
		case FUNCTION:
		{
			AST tmp253_AST = null;
			tmp253_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp253_AST);
			match(FUNCTION);
			if ( inputState.guessing==0 ) {
				can_be_command_ = 2;
			}
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case IDENTIFIER:
		{
			AST tmp254_AST = null;
			tmp254_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp254_AST);
			match(IDENTIFIER);
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_self:
		{
			AST tmp255_AST = null;
			tmp255_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp255_AST);
			match(LITERAL_self);
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_super:
		{
			AST tmp256_AST = null;
			tmp256_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp256_AST);
			match(LITERAL_super);
			if ( inputState.guessing==0 ) {
				can_be_command_ = 2;
			}
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case CONSTANT:
		{
			AST tmp257_AST = null;
			tmp257_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp257_AST);
			match(CONSTANT);
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LEADING_COLON2:
		{
			AST tmp258_AST = null;
			tmp258_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp258_AST);
			match(LEADING_COLON2);
			AST tmp259_AST = null;
			tmp259_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp259_AST);
			match(CONSTANT);
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_retry:
		{
			AST tmp260_AST = null;
			tmp260_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp260_AST);
			match(LITERAL_retry);
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_yield:
		{
			AST tmp261_AST = null;
			tmp261_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp261_AST);
			match(LITERAL_yield);
			if ( inputState.guessing==0 ) {
				can_be_command_ = 2;
			}
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case 155:
		{
			keyword_defined();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_redo:
		{
			AST tmp262_AST = null;
			tmp262_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp262_AST);
			match(LITERAL_redo);
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case EMPTY_ARRAY:
		{
			AST tmp263_AST = null;
			tmp263_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp263_AST);
			match(EMPTY_ARRAY);
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case EMPTY_ARRAY_ACCESS:
		{
			AST tmp264_AST = null;
			tmp264_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp264_AST);
			match(EMPTY_ARRAY_ACCESS);
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		case UNARY_PLUS_MINUS_METHOD_NAME:
		{
			AST tmp265_AST = null;
			tmp265_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp265_AST);
			match(UNARY_PLUS_MINUS_METHOD_NAME);
			primaryExpressionThatCanBeMethodName_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = primaryExpressionThatCanBeMethodName_AST;
	}
	
	public final void methodInvocationArgumentWithParen() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST methodInvocationArgumentWithParen_AST = null;
		
		//super and super() are different, the former takes implicit parameters from the calling method
		boolean has_arg = false;
		
		
		match(LPAREN_WITH_NO_LEADING_SPACE);
		{
		switch ( LA(1)) {
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case BLOCK_ARG_PREFIX:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			methodInvocationArgumentWithoutParen(true);
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				has_arg = true;
			}
			break;
		}
		case LINE_BREAK:
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(RPAREN);
		if ( inputState.guessing==0 ) {
			methodInvocationArgumentWithParen_AST = (AST)currentAST.root;
			if (!has_arg) {methodInvocationArgumentWithParen_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(ARG,"ARG")).add(methodInvocationArgumentWithParen_AST));}
			currentAST.root = methodInvocationArgumentWithParen_AST;
			currentAST.child = methodInvocationArgumentWithParen_AST!=null &&methodInvocationArgumentWithParen_AST.getFirstChild()!=null ?
				methodInvocationArgumentWithParen_AST.getFirstChild() : methodInvocationArgumentWithParen_AST;
			currentAST.advanceChildToEnd();
		}
		methodInvocationArgumentWithParen_AST = (AST)currentAST.root;
		returnAST = methodInvocationArgumentWithParen_AST;
	}
	
	public final void normalMethodInvocationArgument(
		boolean should_ignore_line_break
	) throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST normalMethodInvocationArgument_AST = null;
		
		expression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		if ((LA(1)==ASSOC)) {
			AST tmp269_AST = null;
			tmp269_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, tmp269_AST);
			match(ASSOC);
			expression();
			astFactory.addASTChild(currentAST, returnAST);
		}
		else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==RPAREN||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==RBRACK||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_elsif||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
		}
		else {
			throw new NoViableAltException(LT(1), getFilename());
		}
		
		}
		{
		if (((LA(1)==LINE_BREAK))&&(should_ignore_line_break)) {
			match(LINE_BREAK);
		}
		else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==RPAREN||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==RBRACK||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_elsif||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
		}
		else {
			throw new NoViableAltException(LT(1), getFilename());
		}
		
		}
		normalMethodInvocationArgument_AST = (AST)currentAST.root;
		returnAST = normalMethodInvocationArgument_AST;
	}
	
	public final void restMethodInvocationArgument() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST restMethodInvocationArgument_AST = null;
		
		AST tmp271_AST = null;
		tmp271_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp271_AST);
		match(REST_ARG_PREFIX);
		expression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		if ((LA(1)==COMMA)) {
			match(COMMA);
			blockMethodInvocationArgument();
			astFactory.addASTChild(currentAST, returnAST);
		}
		else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==RPAREN||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==RBRACK||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_elsif||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
		}
		else {
			throw new NoViableAltException(LT(1), getFilename());
		}
		
		}
		restMethodInvocationArgument_AST = (AST)currentAST.root;
		returnAST = restMethodInvocationArgument_AST;
	}
	
	public final void blockMethodInvocationArgument() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST blockMethodInvocationArgument_AST = null;
		
		AST tmp273_AST = null;
		tmp273_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp273_AST);
		match(BLOCK_ARG_PREFIX);
		expression();
		astFactory.addASTChild(currentAST, returnAST);
		blockMethodInvocationArgument_AST = (AST)currentAST.root;
		returnAST = blockMethodInvocationArgument_AST;
	}
	
	public final void predefinedValue() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST predefinedValue_AST = null;
		
		switch ( LA(1)) {
		case LITERAL_nil:
		{
			AST tmp274_AST = null;
			tmp274_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp274_AST);
			match(LITERAL_nil);
			predefinedValue_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_true:
		{
			AST tmp275_AST = null;
			tmp275_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp275_AST);
			match(LITERAL_true);
			predefinedValue_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL_false:
		{
			AST tmp276_AST = null;
			tmp276_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp276_AST);
			match(LITERAL_false);
			predefinedValue_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL___FILE__:
		{
			AST tmp277_AST = null;
			tmp277_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp277_AST);
			match(LITERAL___FILE__);
			predefinedValue_AST = (AST)currentAST.root;
			break;
		}
		case LITERAL___LINE__:
		{
			AST tmp278_AST = null;
			tmp278_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp278_AST);
			match(LITERAL___LINE__);
			predefinedValue_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = predefinedValue_AST;
	}
	
	protected final void expression_substituation() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST expression_substituation_AST = null;
		
		{
		switch ( LA(1)) {
		case LCURLY_BLOCK:
		{
			match(LCURLY_BLOCK);
			{
			switch ( LA(1)) {
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_END:
			{
				compoundStatement();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case RCURLY:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(RCURLY);
			break;
		}
		case GLOBAL_VARIABLE:
		{
			AST tmp281_AST = null;
			tmp281_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp281_AST);
			match(GLOBAL_VARIABLE);
			break;
		}
		case INSTANCE_VARIABLE:
		{
			AST tmp282_AST = null;
			tmp282_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp282_AST);
			match(INSTANCE_VARIABLE);
			break;
		}
		case CLASS_VARIABLE:
		{
			AST tmp283_AST = null;
			tmp283_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp283_AST);
			match(CLASS_VARIABLE);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		expression_substituation_AST = (AST)currentAST.root;
		returnAST = expression_substituation_AST;
	}
	
	public final void regex() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST regex_AST = null;
		
		switch ( LA(1)) {
		case REGEX:
		{
			AST tmp284_AST = null;
			tmp284_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp284_AST);
			match(REGEX);
			regex_AST = (AST)currentAST.root;
			break;
		}
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			AST tmp285_AST = null;
			tmp285_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, tmp285_AST);
			match(REGEX_BEFORE_EXPRESSION_SUBSTITUTION);
			expression_substituation();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				tellLexerWeHaveFinishedParsingRegexExpressionSubstituation();
			}
			{
			_loop215:
			do {
				if ((LA(1)==STRING_BETWEEN_EXPRESSION_SUBSTITUTION)) {
					AST tmp286_AST = null;
					tmp286_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp286_AST);
					match(STRING_BETWEEN_EXPRESSION_SUBSTITUTION);
					expression_substituation();
					astFactory.addASTChild(currentAST, returnAST);
					if ( inputState.guessing==0 ) {
						tellLexerWeHaveFinishedParsingRegexExpressionSubstituation();
					}
				}
				else {
					break _loop215;
				}
				
			} while (true);
			}
			AST tmp287_AST = null;
			tmp287_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp287_AST);
			match(STRING_AFTER_EXPRESSION_SUBSTITUTION);
			regex_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = regex_AST;
	}
	
	public final void command_output() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST command_output_AST = null;
		
		switch ( LA(1)) {
		case COMMAND_OUTPUT:
		{
			AST tmp288_AST = null;
			tmp288_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp288_AST);
			match(COMMAND_OUTPUT);
			command_output_AST = (AST)currentAST.root;
			break;
		}
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			AST tmp289_AST = null;
			tmp289_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, tmp289_AST);
			match(COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION);
			expression_substituation();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				tellLexerWeHaveFinishedParsingStringExpressionSubstituation();
			}
			{
			_loop218:
			do {
				if ((LA(1)==STRING_BETWEEN_EXPRESSION_SUBSTITUTION)) {
					AST tmp290_AST = null;
					tmp290_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp290_AST);
					match(STRING_BETWEEN_EXPRESSION_SUBSTITUTION);
					expression_substituation();
					astFactory.addASTChild(currentAST, returnAST);
					if ( inputState.guessing==0 ) {
						tellLexerWeHaveFinishedParsingStringExpressionSubstituation();
					}
				}
				else {
					break _loop218;
				}
				
			} while (true);
			}
			AST tmp291_AST = null;
			tmp291_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp291_AST);
			match(STRING_AFTER_EXPRESSION_SUBSTITUTION);
			command_output_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = command_output_AST;
	}
	
	public final void heredoc() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST heredoc_AST = null;
		
		match(HERE_DOC_BEGIN);
		{
		switch ( LA(1)) {
		case HERE_DOC_CONTENT:
		{
			AST tmp293_AST = null;
			tmp293_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp293_AST);
			match(HERE_DOC_CONTENT);
			break;
		}
		case HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			AST tmp294_AST = null;
			tmp294_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, tmp294_AST);
			match(HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION);
			expression_substituation();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				tellLexerWeHaveFinishedParsingHeredocExpressionSubstituation();
			}
			{
			_loop222:
			do {
				if ((LA(1)==HERE_DOC_BETWEEN_EXPRESSION_SUBSTITUTION)) {
					AST tmp295_AST = null;
					tmp295_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp295_AST);
					match(HERE_DOC_BETWEEN_EXPRESSION_SUBSTITUTION);
					expression_substituation();
					astFactory.addASTChild(currentAST, returnAST);
					if ( inputState.guessing==0 ) {
						tellLexerWeHaveFinishedParsingHeredocExpressionSubstituation();
					}
				}
				else {
					break _loop222;
				}
				
			} while (true);
			}
			AST tmp296_AST = null;
			tmp296_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp296_AST);
			match(HERE_DOC_AFTER_EXPRESSION_SUBSTITUTION);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		heredoc_AST = (AST)currentAST.root;
		returnAST = heredoc_AST;
	}
	
	public final void literal() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST literal_AST = null;
		
		switch ( LA(1)) {
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			regex();
			astFactory.addASTChild(currentAST, returnAST);
			literal_AST = (AST)currentAST.root;
			break;
		}
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			{
			int _cnt225=0;
			_loop225:
			do {
				if (((LA(1) >= DOUBLE_QUOTE_STRING && LA(1) <= STRING_BEFORE_EXPRESSION_SUBSTITUTION))) {
					string();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					if ( _cnt225>=1 ) { break _loop225; } else {throw new NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt225++;
			} while (true);
			}
			if ( inputState.guessing==0 ) {
				literal_AST = (AST)currentAST.root;
				literal_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(STRING,"STRING")).add(literal_AST));
				currentAST.root = literal_AST;
				currentAST.child = literal_AST!=null &&literal_AST.getFirstChild()!=null ?
					literal_AST.getFirstChild() : literal_AST;
				currentAST.advanceChildToEnd();
			}
			literal_AST = (AST)currentAST.root;
			break;
		}
		case HERE_DOC_BEGIN:
		{
			heredoc();
			astFactory.addASTChild(currentAST, returnAST);
			literal_AST = (AST)currentAST.root;
			break;
		}
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			command_output();
			astFactory.addASTChild(currentAST, returnAST);
			literal_AST = (AST)currentAST.root;
			break;
		}
		case COLON_WITH_NO_FOLLOWING_SPACE:
		{
			symbol();
			astFactory.addASTChild(currentAST, returnAST);
			literal_AST = (AST)currentAST.root;
			break;
		}
		case W_ARRAY:
		{
			AST tmp297_AST = null;
			tmp297_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp297_AST);
			match(W_ARRAY);
			literal_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = literal_AST;
	}
	
	public final void numeric() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST numeric_AST = null;
		
		switch ( LA(1)) {
		case INTEGER:
		{
			AST tmp298_AST = null;
			tmp298_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp298_AST);
			match(INTEGER);
			numeric_AST = (AST)currentAST.root;
			break;
		}
		case HEX:
		{
			AST tmp299_AST = null;
			tmp299_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp299_AST);
			match(HEX);
			numeric_AST = (AST)currentAST.root;
			break;
		}
		case BINARY:
		{
			AST tmp300_AST = null;
			tmp300_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp300_AST);
			match(BINARY);
			numeric_AST = (AST)currentAST.root;
			break;
		}
		case OCTAL:
		{
			AST tmp301_AST = null;
			tmp301_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp301_AST);
			match(OCTAL);
			numeric_AST = (AST)currentAST.root;
			break;
		}
		case FLOAT:
		{
			AST tmp302_AST = null;
			tmp302_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp302_AST);
			match(FLOAT);
			numeric_AST = (AST)currentAST.root;
			break;
		}
		case ASCII_VALUE:
		{
			AST tmp303_AST = null;
			tmp303_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp303_AST);
			match(ASCII_VALUE);
			numeric_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = numeric_AST;
	}
	
	public final void keyword_defined() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_defined_AST = null;
		
		AST tmp304_AST = null;
		tmp304_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp304_AST);
		match(155);
		{
		if ((LA(1)==LINE_BREAK)) {
			match(LINE_BREAK);
		}
		else if ((LA(1)==EOF||LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==IF_MODIFIER||LA(1)==UNLESS_MODIFIER||LA(1)==WHILE_MODIFIER||LA(1)==UNTIL_MODIFIER||LA(1)==RESCUE_MODIFIER||LA(1)==LCURLY_BLOCK||LA(1)==RCURLY||LA(1)==COMMA||LA(1)==REST_ARG_PREFIX||LA(1)==ASSIGN||LA(1)==ASSIGN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==RPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==GLOBAL_VARIABLE||LA(1)==LITERAL_do||LA(1)==LITERAL_end||LA(1)==BOR||LA(1)==LOGICAL_OR||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_and||LA(1)==LITERAL_or||LA(1)==LITERAL_not||LA(1)==QUESTION||LA(1)==COLON||LA(1)==PLUS_ASSIGN||LA(1)==MINUS_ASSIGN||LA(1)==STAR_ASSIGN||LA(1)==DIV_ASSIGN||LA(1)==MOD_ASSIGN||LA(1)==POWER_ASSIGN||LA(1)==BAND_ASSIGN||LA(1)==BXOR_ASSIGN||LA(1)==BOR_ASSIGN||LA(1)==LEFT_SHIFT_ASSIGN||LA(1)==RIGHT_SHIFT_ASSIGN||LA(1)==LOGICAL_AND_ASSIGN||LA(1)==LOGICAL_OR_ASSIGN||LA(1)==INCLUSIVE_RANGE||LA(1)==EXCLUSIVE_RANGE||LA(1)==LOGICAL_AND||LA(1)==COMPARE||LA(1)==EQUAL||LA(1)==CASE_EQUAL||LA(1)==NOT_EQUAL||LA(1)==MATCH||LA(1)==NOT_MATCH||LA(1)==LESS_THAN||LA(1)==GREATER_THAN||LA(1)==LESS_OR_EQUAL||LA(1)==GREATER_OR_EQUAL||LA(1)==BXOR||LA(1)==BAND||LA(1)==LEFT_SHIFT||LA(1)==RIGHT_SHIFT||LA(1)==PLUS||LA(1)==MINUS||LA(1)==STAR||LA(1)==DIV||LA(1)==MOD||LA(1)==POWER||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==DOT||LA(1)==COLON2||LA(1)==LBRACK_ARRAY_ACCESS||LA(1)==RBRACK||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==ASSOC||LA(1)==BLOCK_ARG_PREFIX||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_rescue||LA(1)==LITERAL_else||LA(1)==LITERAL_ensure||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_elsif||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_in||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==155||LA(1)==LITERAL_then||LA(1)==LITERAL_when||LA(1)==DO_IN_CONDITION)) {
		}
		else {
			throw new NoViableAltException(LT(1), getFilename());
		}
		
		}
		keyword_defined_AST = (AST)currentAST.root;
		returnAST = keyword_defined_AST;
	}
	
	public final void arrayExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST arrayExpression_AST = null;
		
		AST tmp306_AST = null;
		tmp306_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp306_AST);
		match(LBRACK);
		{
		switch ( LA(1)) {
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			arrayReferenceArgument();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case RBRACK:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(RBRACK);
		arrayExpression_AST = (AST)currentAST.root;
		returnAST = arrayExpression_AST;
	}
	
	public final void hashExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST hashExpression_AST = null;
		
		AST tmp308_AST = null;
		tmp308_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp308_AST);
		match(LCURLY_HASH);
		{
		switch ( LA(1)) {
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			keyValuePair();
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop243:
			do {
				if (((LA(1)==COMMA))&&(RCURLY != LA(2))) {
					match(COMMA);
					keyValuePair();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop243;
				}
				
			} while (true);
			}
			{
			switch ( LA(1)) {
			case COMMA:
			{
				match(COMMA);
				break;
			}
			case RCURLY:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case RCURLY:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(RCURLY);
		hashExpression_AST = (AST)currentAST.root;
		returnAST = hashExpression_AST;
	}
	
	public final void ifExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST ifExpression_AST = null;
		
		AST tmp312_AST = null;
		tmp312_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp312_AST);
		match(LITERAL_if);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		expression();
		astFactory.addASTChild(currentAST, returnAST);
		thenOrTermialOrColon();
		astFactory.addASTChild(currentAST, returnAST);
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			compoundStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_end:
		case LITERAL_else:
		case LITERAL_elsif:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		_loop278:
		do {
			if ((LA(1)==LITERAL_elsif)) {
				AST tmp314_AST = null;
				tmp314_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp314_AST);
				match(LITERAL_elsif);
				{
				switch ( LA(1)) {
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case LITERAL_not:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				{
					expression();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				case SEMI:
				case LINE_BREAK:
				case COLON:
				case LITERAL_then:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				thenOrTermialOrColon();
				astFactory.addASTChild(currentAST, returnAST);
				{
				switch ( LA(1)) {
				case SEMI:
				case LINE_BREAK:
				case REST_ARG_PREFIX:
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case LITERAL_undef:
				case LITERAL_alias:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case LITERAL_not:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				case LITERAL_BEGIN:
				case LITERAL_END:
				{
					compoundStatement();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				case LITERAL_end:
				case LITERAL_else:
				case LITERAL_elsif:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
			}
			else {
				break _loop278;
			}
			
		} while (true);
		}
		{
		switch ( LA(1)) {
		case LITERAL_else:
		{
			AST tmp315_AST = null;
			tmp315_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp315_AST);
			match(LITERAL_else);
			{
			switch ( LA(1)) {
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_END:
			{
				compoundStatement();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case LITERAL_end:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LITERAL_end);
		ifExpression_AST = (AST)currentAST.root;
		returnAST = ifExpression_AST;
	}
	
	public final void unlessExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST unlessExpression_AST = null;
		
		AST tmp317_AST = null;
		tmp317_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp317_AST);
		match(LITERAL_unless);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		expression();
		astFactory.addASTChild(currentAST, returnAST);
		thenOrTermialOrColon();
		astFactory.addASTChild(currentAST, returnAST);
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			compoundStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_end:
		case LITERAL_else:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case LITERAL_else:
		{
			AST tmp319_AST = null;
			tmp319_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp319_AST);
			match(LITERAL_else);
			{
			switch ( LA(1)) {
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_END:
			{
				compoundStatement();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case LITERAL_end:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LITERAL_end);
		unlessExpression_AST = (AST)currentAST.root;
		returnAST = unlessExpression_AST;
	}
	
	public final void whileExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST whileExpression_AST = null;
		
		AST tmp321_AST = null;
		tmp321_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp321_AST);
		match(LITERAL_while);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		expression();
		astFactory.addASTChild(currentAST, returnAST);
		doOrTermialOrColon();
		astFactory.addASTChild(currentAST, returnAST);
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			compoundStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LITERAL_end);
		whileExpression_AST = (AST)currentAST.root;
		returnAST = whileExpression_AST;
	}
	
	public final void untilExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST untilExpression_AST = null;
		
		AST tmp324_AST = null;
		tmp324_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp324_AST);
		match(LITERAL_until);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		expression();
		astFactory.addASTChild(currentAST, returnAST);
		doOrTermialOrColon();
		astFactory.addASTChild(currentAST, returnAST);
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			compoundStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LITERAL_end);
		untilExpression_AST = (AST)currentAST.root;
		returnAST = untilExpression_AST;
	}
	
	public final void caseExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST caseExpression_AST = null;
		
		AST tmp327_AST = null;
		tmp327_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp327_AST);
		match(LITERAL_case);
		{
		switch ( LA(1)) {
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			expression();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case SEMI:
		case LINE_BREAK:
		case COLON:
		case LITERAL_then:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		thenOrTermialOrColon();
		astFactory.addASTChild(currentAST, returnAST);
		{
		int _cnt290=0;
		_loop290:
		do {
			if ((LA(1)==LITERAL_when)) {
				keyword_when();
				astFactory.addASTChild(currentAST, returnAST);
				mrhs();
				astFactory.addASTChild(currentAST, returnAST);
				thenOrTermialOrColon();
				astFactory.addASTChild(currentAST, returnAST);
				{
				switch ( LA(1)) {
				case SEMI:
				case LINE_BREAK:
				case REST_ARG_PREFIX:
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case LITERAL_undef:
				case LITERAL_alias:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case LITERAL_not:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				case LITERAL_BEGIN:
				case LITERAL_END:
				{
					compoundStatement();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				case LITERAL_end:
				case LITERAL_else:
				case LITERAL_when:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
			}
			else {
				if ( _cnt290>=1 ) { break _loop290; } else {throw new NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt290++;
		} while (true);
		}
		{
		switch ( LA(1)) {
		case LITERAL_else:
		{
			AST tmp328_AST = null;
			tmp328_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp328_AST);
			match(LITERAL_else);
			{
			switch ( LA(1)) {
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_END:
			{
				compoundStatement();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case LITERAL_end:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LITERAL_end);
		caseExpression_AST = (AST)currentAST.root;
		returnAST = caseExpression_AST;
	}
	
	public final void forExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST forExpression_AST = null;
		
		AST tmp330_AST = null;
		tmp330_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp330_AST);
		match(LITERAL_for);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		block_vars();
		astFactory.addASTChild(currentAST, returnAST);
		AST tmp332_AST = null;
		tmp332_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp332_AST);
		match(LITERAL_in);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		expression();
		astFactory.addASTChild(currentAST, returnAST);
		doOrTermialOrColon();
		astFactory.addASTChild(currentAST, returnAST);
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			compoundStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LITERAL_end);
		forExpression_AST = (AST)currentAST.root;
		returnAST = forExpression_AST;
	}
	
	public final void exceptionHandlingExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST exceptionHandlingExpression_AST = null;
		
		AST tmp335_AST = null;
		tmp335_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp335_AST);
		match(LITERAL_begin);
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_rescue:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			bodyStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LITERAL_end);
		exceptionHandlingExpression_AST = (AST)currentAST.root;
		returnAST = exceptionHandlingExpression_AST;
	}
	
	public final void moduleDefinition() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST moduleDefinition_AST = null;
		
		AST tmp337_AST = null;
		tmp337_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp337_AST);
		match(LITERAL_module);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case CONSTANT:
		case LEADING_COLON2:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		moduleName();
		astFactory.addASTChild(currentAST, returnAST);
		{
		if ((LA(1)==SEMI||LA(1)==LINE_BREAK)) {
			terminal();
			astFactory.addASTChild(currentAST, returnAST);
		}
		else if ((LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==REST_ARG_PREFIX||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==LITERAL_undef||LA(1)==LITERAL_alias||LA(1)==GLOBAL_VARIABLE||LA(1)==LITERAL_end||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_not||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_rescue||LA(1)==LITERAL_ensure||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==155||LA(1)==LITERAL_BEGIN||LA(1)==LITERAL_END)) {
		}
		else {
			throw new NoViableAltException(LT(1), getFilename());
		}
		
		}
		if ( inputState.guessing==0 ) {
			enterScope();
		}
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_rescue:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			bodyStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LITERAL_end);
		if ( inputState.guessing==0 ) {
			leaveScope();
		}
		moduleDefinition_AST = (AST)currentAST.root;
		returnAST = moduleDefinition_AST;
	}
	
	public final void classDefinition() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST classDefinition_AST = null;
		
		AST tmp340_AST = null;
		tmp340_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp340_AST);
		match(LITERAL_class);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case CONSTANT:
		case FUNCTION:
		case LEFT_SHIFT:
		case LEADING_COLON2:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case CONSTANT:
		case FUNCTION:
		case LEADING_COLON2:
		{
			className();
			astFactory.addASTChild(currentAST, returnAST);
			{
			switch ( LA(1)) {
			case LESS_THAN:
			{
				AST tmp342_AST = null;
				tmp342_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp342_AST);
				match(LESS_THAN);
				expression();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case SEMI:
			case LINE_BREAK:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LEFT_SHIFT:
		{
			AST tmp343_AST = null;
			tmp343_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp343_AST);
			match(LEFT_SHIFT);
			expression();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		terminal();
		astFactory.addASTChild(currentAST, returnAST);
		if ( inputState.guessing==0 ) {
			enterScope();
		}
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_rescue:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			bodyStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LITERAL_end);
		if ( inputState.guessing==0 ) {
			leaveScope();
		}
		classDefinition_AST = (AST)currentAST.root;
		returnAST = classDefinition_AST;
	}
	
	public final void methodDefinition() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST methodDefinition_AST = null;
		
		AST tmp345_AST = null;
		tmp345_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, tmp345_AST);
		match(LITERAL_def);
		if ( inputState.guessing==0 ) {
			++is_in_method_definition;
		}
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case LITERAL_do:
		case LITERAL_end:
		case BOR:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case MATCH:
		case LESS_THAN:
		case GREATER_THAN:
		case LESS_OR_EQUAL:
		case GREATER_OR_EQUAL:
		case BXOR:
		case BAND:
		case LEFT_SHIFT:
		case RIGHT_SHIFT:
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
		case MOD:
		case POWER:
		case BNOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case LITERAL_self:
		case LITERAL_super:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_elsif:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_in:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case SINGLE_QUOTE:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_then:
		case LITERAL_when:
		case LITERAL_END:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		methodName();
		astFactory.addASTChild(currentAST, returnAST);
		if ( inputState.guessing==0 ) {
			enterScope();
		}
		methodDefinitionArgument();
		astFactory.addASTChild(currentAST, returnAST);
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_rescue:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			bodyStatement();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_end:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LITERAL_end);
		if ( inputState.guessing==0 ) {
			leaveScope();--is_in_method_definition;
		}
		methodDefinition_AST = (AST)currentAST.root;
		returnAST = methodDefinition_AST;
	}
	
	public final void primaryExpression() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST primaryExpression_AST = null;
		
		switch ( LA(1)) {
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		{
			primaryExpressionThatCanNotBeMethodName();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpression_AST = (AST)currentAST.root;
			break;
		}
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case EMPTY_ARRAY_ACCESS:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case 155:
		{
			primaryExpressionThatCanBeMethodName();
			astFactory.addASTChild(currentAST, returnAST);
			primaryExpression_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = primaryExpression_AST;
	}
	
	public final void keyValuePair() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyValuePair_AST = null;
		
		expression();
		astFactory.addASTChild(currentAST, returnAST);
		{
		switch ( LA(1)) {
		case ASSOC:
		{
			AST tmp348_AST = null;
			tmp348_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp348_AST);
			match(ASSOC);
			expression();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LINE_BREAK:
		case RCURLY:
		case COMMA:
		case RBRACK:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case RCURLY:
		case COMMA:
		case RBRACK:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyValuePair_AST = (AST)currentAST.root;
		returnAST = keyValuePair_AST;
	}
	
	public final void bodyStatement() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST bodyStatement_AST = null;
		
		{
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_END:
		{
			compoundStatement();
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop249:
			do {
				if ((LA(1)==LITERAL_rescue)) {
					AST tmp350_AST = null;
					tmp350_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp350_AST);
					match(LITERAL_rescue);
					exceptionList();
					astFactory.addASTChild(currentAST, returnAST);
					thenOrTermialOrColon();
					astFactory.addASTChild(currentAST, returnAST);
					{
					switch ( LA(1)) {
					case SEMI:
					case LINE_BREAK:
					case REST_ARG_PREFIX:
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case LITERAL_undef:
					case LITERAL_alias:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case LITERAL_not:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					case LITERAL_BEGIN:
					case LITERAL_END:
					{
						compoundStatement();
						astFactory.addASTChild(currentAST, returnAST);
						break;
					}
					case LITERAL_end:
					case LITERAL_rescue:
					case LITERAL_else:
					case LITERAL_ensure:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
				}
				else {
					break _loop249;
				}
				
			} while (true);
			}
			{
			switch ( LA(1)) {
			case LITERAL_else:
			{
				AST tmp351_AST = null;
				tmp351_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp351_AST);
				match(LITERAL_else);
				{
				switch ( LA(1)) {
				case SEMI:
				case LINE_BREAK:
				case REST_ARG_PREFIX:
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case LITERAL_undef:
				case LITERAL_alias:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case LITERAL_not:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				case LITERAL_BEGIN:
				case LITERAL_END:
				{
					compoundStatement();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				case LITERAL_end:
				case LITERAL_ensure:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case LITERAL_end:
			case LITERAL_ensure:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case LITERAL_ensure:
			{
				AST tmp352_AST = null;
				tmp352_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp352_AST);
				match(LITERAL_ensure);
				{
				switch ( LA(1)) {
				case SEMI:
				case LINE_BREAK:
				case REST_ARG_PREFIX:
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case LITERAL_undef:
				case LITERAL_alias:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case LITERAL_not:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				case LITERAL_BEGIN:
				case LITERAL_END:
				{
					compoundStatement();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				case LITERAL_end:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case LITERAL_end:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LITERAL_rescue:
		{
			{
			int _cnt256=0;
			_loop256:
			do {
				if ((LA(1)==LITERAL_rescue)) {
					AST tmp353_AST = null;
					tmp353_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp353_AST);
					match(LITERAL_rescue);
					exceptionList();
					astFactory.addASTChild(currentAST, returnAST);
					thenOrTermialOrColon();
					astFactory.addASTChild(currentAST, returnAST);
					{
					switch ( LA(1)) {
					case SEMI:
					case LINE_BREAK:
					case REST_ARG_PREFIX:
					case LPAREN_WITH_NO_LEADING_SPACE:
					case LPAREN:
					case IDENTIFIER:
					case CONSTANT:
					case FUNCTION:
					case LITERAL_undef:
					case LITERAL_alias:
					case GLOBAL_VARIABLE:
					case COLON_WITH_NO_FOLLOWING_SPACE:
					case INSTANCE_VARIABLE:
					case CLASS_VARIABLE:
					case UNARY_PLUS_MINUS_METHOD_NAME:
					case LITERAL_not:
					case BNOT:
					case NOT:
					case LITERAL_return:
					case LITERAL_break:
					case LITERAL_next:
					case EMPTY_ARRAY_ACCESS:
					case UNARY_PLUS:
					case UNARY_MINUS:
					case LITERAL_nil:
					case LITERAL_true:
					case LITERAL_false:
					case LITERAL___FILE__:
					case LITERAL___LINE__:
					case DOUBLE_QUOTE_STRING:
					case SINGLE_QUOTE_STRING:
					case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
					case REGEX:
					case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
					case COMMAND_OUTPUT:
					case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
					case HERE_DOC_BEGIN:
					case W_ARRAY:
					case INTEGER:
					case HEX:
					case BINARY:
					case OCTAL:
					case FLOAT:
					case ASCII_VALUE:
					case LITERAL_self:
					case LITERAL_super:
					case LEADING_COLON2:
					case LITERAL_retry:
					case LITERAL_yield:
					case LITERAL_redo:
					case EMPTY_ARRAY:
					case LBRACK:
					case LCURLY_HASH:
					case LITERAL_begin:
					case LITERAL_if:
					case LITERAL_unless:
					case LITERAL_case:
					case LITERAL_for:
					case LITERAL_while:
					case LITERAL_until:
					case LITERAL_module:
					case LITERAL_class:
					case LITERAL_def:
					case 155:
					case LITERAL_BEGIN:
					case LITERAL_END:
					{
						compoundStatement();
						astFactory.addASTChild(currentAST, returnAST);
						break;
					}
					case LITERAL_end:
					case LITERAL_rescue:
					case LITERAL_else:
					case LITERAL_ensure:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
				}
				else {
					if ( _cnt256>=1 ) { break _loop256; } else {throw new NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt256++;
			} while (true);
			}
			{
			switch ( LA(1)) {
			case LITERAL_else:
			{
				AST tmp354_AST = null;
				tmp354_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp354_AST);
				match(LITERAL_else);
				{
				switch ( LA(1)) {
				case SEMI:
				case LINE_BREAK:
				case REST_ARG_PREFIX:
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case LITERAL_undef:
				case LITERAL_alias:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case LITERAL_not:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				case LITERAL_BEGIN:
				case LITERAL_END:
				{
					compoundStatement();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				case LITERAL_end:
				case LITERAL_ensure:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case LITERAL_end:
			case LITERAL_ensure:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case LITERAL_ensure:
			{
				AST tmp355_AST = null;
				tmp355_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp355_AST);
				match(LITERAL_ensure);
				{
				switch ( LA(1)) {
				case SEMI:
				case LINE_BREAK:
				case REST_ARG_PREFIX:
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case LITERAL_undef:
				case LITERAL_alias:
				case GLOBAL_VARIABLE:
				case COLON_WITH_NO_FOLLOWING_SPACE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case UNARY_PLUS_MINUS_METHOD_NAME:
				case LITERAL_not:
				case BNOT:
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case EMPTY_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
				case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_BEGIN:
				case W_ARRAY:
				case INTEGER:
				case HEX:
				case BINARY:
				case OCTAL:
				case FLOAT:
				case ASCII_VALUE:
				case LITERAL_self:
				case LITERAL_super:
				case LEADING_COLON2:
				case LITERAL_retry:
				case LITERAL_yield:
				case LITERAL_redo:
				case EMPTY_ARRAY:
				case LBRACK:
				case LCURLY_HASH:
				case LITERAL_begin:
				case LITERAL_if:
				case LITERAL_unless:
				case LITERAL_case:
				case LITERAL_for:
				case LITERAL_while:
				case LITERAL_until:
				case LITERAL_module:
				case LITERAL_class:
				case LITERAL_def:
				case 155:
				case LITERAL_BEGIN:
				case LITERAL_END:
				{
					compoundStatement();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				case LITERAL_end:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case LITERAL_end:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LITERAL_ensure:
		{
			{
			AST tmp356_AST = null;
			tmp356_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp356_AST);
			match(LITERAL_ensure);
			{
			switch ( LA(1)) {
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_END:
			{
				compoundStatement();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case LITERAL_end:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			}
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState.guessing==0 ) {
			bodyStatement_AST = (AST)currentAST.root;
			bodyStatement_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(BODY,"BODY")).add(bodyStatement_AST));
			currentAST.root = bodyStatement_AST;
			currentAST.child = bodyStatement_AST!=null &&bodyStatement_AST.getFirstChild()!=null ?
				bodyStatement_AST.getFirstChild() : bodyStatement_AST;
			currentAST.advanceChildToEnd();
		}
		bodyStatement_AST = (AST)currentAST.root;
		returnAST = bodyStatement_AST;
	}
	
	public final void exceptionList() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST exceptionList_AST = null;
		
		switch ( LA(1)) {
		case SEMI:
		case LINE_BREAK:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case INSTANCE_VARIABLE:
		case COLON:
		case ASSOC:
		case LEADING_COLON2:
		case LITERAL_then:
		{
			{
			switch ( LA(1)) {
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case INSTANCE_VARIABLE:
			case LEADING_COLON2:
			{
				{
				switch ( LA(1)) {
				case CONSTANT:
				case FUNCTION:
				case LEADING_COLON2:
				{
					className();
					astFactory.addASTChild(currentAST, returnAST);
					break;
				}
				case INSTANCE_VARIABLE:
				{
					AST tmp357_AST = null;
					tmp357_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp357_AST);
					match(INSTANCE_VARIABLE);
					break;
				}
				case IDENTIFIER:
				{
					AST tmp358_AST = null;
					tmp358_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp358_AST);
					match(IDENTIFIER);
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				{
				_loop270:
				do {
					if ((LA(1)==COMMA)) {
						match(COMMA);
						{
						switch ( LA(1)) {
						case CONSTANT:
						case FUNCTION:
						case LEADING_COLON2:
						{
							className();
							astFactory.addASTChild(currentAST, returnAST);
							break;
						}
						case INSTANCE_VARIABLE:
						{
							AST tmp360_AST = null;
							tmp360_AST = astFactory.create(LT(1));
							astFactory.addASTChild(currentAST, tmp360_AST);
							match(INSTANCE_VARIABLE);
							break;
						}
						case IDENTIFIER:
						{
							AST tmp361_AST = null;
							tmp361_AST = astFactory.create(LT(1));
							astFactory.addASTChild(currentAST, tmp361_AST);
							match(IDENTIFIER);
							break;
						}
						default:
						{
							throw new NoViableAltException(LT(1), getFilename());
						}
						}
						}
					}
					else {
						break _loop270;
					}
					
				} while (true);
				}
				break;
			}
			case SEMI:
			case LINE_BREAK:
			case COLON:
			case ASSOC:
			case LITERAL_then:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case ASSOC:
			{
				AST tmp362_AST = null;
				tmp362_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp362_AST);
				match(ASSOC);
				variable();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case SEMI:
			case LINE_BREAK:
			case COLON:
			case LITERAL_then:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			exceptionList_AST = (AST)currentAST.root;
			break;
		}
		case GLOBAL_VARIABLE:
		{
			AST tmp363_AST = null;
			tmp363_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp363_AST);
			match(GLOBAL_VARIABLE);
			exceptionList_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = exceptionList_AST;
	}
	
	public final void thenOrTermialOrColon() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST thenOrTermialOrColon_AST = null;
		
		switch ( LA(1)) {
		case LITERAL_then:
		{
			match(LITERAL_then);
			thenOrTermialOrColon_AST = (AST)currentAST.root;
			break;
		}
		case SEMI:
		case LINE_BREAK:
		{
			terminal();
			astFactory.addASTChild(currentAST, returnAST);
			{
			switch ( LA(1)) {
			case LITERAL_then:
			{
				match(LITERAL_then);
				break;
			}
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case LITERAL_end:
			case COLON_WITH_NO_FOLLOWING_SPACE:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case LITERAL_not:
			case BNOT:
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case EMPTY_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_BEGIN:
			case W_ARRAY:
			case INTEGER:
			case HEX:
			case BINARY:
			case OCTAL:
			case FLOAT:
			case ASCII_VALUE:
			case LITERAL_self:
			case LITERAL_super:
			case LEADING_COLON2:
			case LITERAL_retry:
			case LITERAL_yield:
			case LITERAL_redo:
			case EMPTY_ARRAY:
			case LBRACK:
			case LCURLY_HASH:
			case LITERAL_rescue:
			case LITERAL_else:
			case LITERAL_ensure:
			case LITERAL_begin:
			case LITERAL_if:
			case LITERAL_elsif:
			case LITERAL_unless:
			case LITERAL_case:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_until:
			case LITERAL_module:
			case LITERAL_class:
			case LITERAL_def:
			case 155:
			case LITERAL_BEGIN:
			case LITERAL_when:
			case LITERAL_END:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			thenOrTermialOrColon_AST = (AST)currentAST.root;
			break;
		}
		case COLON:
		{
			match(COLON);
			thenOrTermialOrColon_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = thenOrTermialOrColon_AST;
	}
	
	public final void className() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST className_AST = null;
		
		switch ( LA(1)) {
		case CONSTANT:
		case FUNCTION:
		{
			{
			switch ( LA(1)) {
			case CONSTANT:
			{
				AST tmp367_AST = null;
				tmp367_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp367_AST);
				match(CONSTANT);
				break;
			}
			case FUNCTION:
			{
				AST tmp368_AST = null;
				tmp368_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp368_AST);
				match(FUNCTION);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			_loop321:
			do {
				if ((LA(1)==COLON2)) {
					AST tmp369_AST = null;
					tmp369_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp369_AST);
					match(COLON2);
					AST tmp370_AST = null;
					tmp370_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp370_AST);
					match(CONSTANT);
				}
				else {
					break _loop321;
				}
				
			} while (true);
			}
			className_AST = (AST)currentAST.root;
			break;
		}
		case LEADING_COLON2:
		{
			{
			AST tmp371_AST = null;
			tmp371_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp371_AST);
			match(LEADING_COLON2);
			AST tmp372_AST = null;
			tmp372_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp372_AST);
			match(CONSTANT);
			}
			{
			_loop324:
			do {
				if ((LA(1)==COLON2)) {
					AST tmp373_AST = null;
					tmp373_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp373_AST);
					match(COLON2);
					AST tmp374_AST = null;
					tmp374_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp374_AST);
					match(CONSTANT);
				}
				else {
					break _loop324;
				}
				
			} while (true);
			}
			className_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = className_AST;
	}
	
	public final void variable() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST variable_AST = null;
		Token  id1 = null;
		AST id1_AST = null;
		Token  id2 = null;
		AST id2_AST = null;
		
		switch ( LA(1)) {
		case IDENTIFIER:
		{
			id1 = LT(1);
			id1_AST = astFactory.create(id1);
			astFactory.addASTChild(currentAST, id1_AST);
			match(IDENTIFIER);
			if ( inputState.guessing==0 ) {
				addVariable(id1);
			}
			variable_AST = (AST)currentAST.root;
			break;
		}
		case FUNCTION:
		{
			id2 = LT(1);
			id2_AST = astFactory.create(id2);
			astFactory.addASTChild(currentAST, id2_AST);
			match(FUNCTION);
			if ( inputState.guessing==0 ) {
				addVariable(id2);
			}
			variable_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = variable_AST;
	}
	
	public final void keyword_when() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_when_AST = null;
		
		AST tmp375_AST = null;
		tmp375_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp375_AST);
		match(LITERAL_when);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case REST_ARG_PREFIX:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case COLON_WITH_NO_FOLLOWING_SPACE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case LITERAL_not:
		case BNOT:
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case EMPTY_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case DOUBLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING:
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_BEGIN:
		case W_ARRAY:
		case INTEGER:
		case HEX:
		case BINARY:
		case OCTAL:
		case FLOAT:
		case ASCII_VALUE:
		case LITERAL_self:
		case LITERAL_super:
		case LEADING_COLON2:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case EMPTY_ARRAY:
		case LBRACK:
		case LCURLY_HASH:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_when_AST = (AST)currentAST.root;
		returnAST = keyword_when_AST;
	}
	
	public final void doOrTermialOrColon() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST doOrTermialOrColon_AST = null;
		
		switch ( LA(1)) {
		case DO_IN_CONDITION:
		{
			match(DO_IN_CONDITION);
			doOrTermialOrColon_AST = (AST)currentAST.root;
			break;
		}
		case SEMI:
		case LINE_BREAK:
		{
			terminal();
			astFactory.addASTChild(currentAST, returnAST);
			doOrTermialOrColon_AST = (AST)currentAST.root;
			break;
		}
		case COLON:
		{
			match(COLON);
			doOrTermialOrColon_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = doOrTermialOrColon_AST;
	}
	
	public final void moduleName() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST moduleName_AST = null;
		
		switch ( LA(1)) {
		case CONSTANT:
		{
			AST tmp379_AST = null;
			tmp379_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp379_AST);
			match(CONSTANT);
			{
			_loop309:
			do {
				if ((LA(1)==COLON2)) {
					AST tmp380_AST = null;
					tmp380_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp380_AST);
					match(COLON2);
					AST tmp381_AST = null;
					tmp381_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp381_AST);
					match(CONSTANT);
				}
				else {
					break _loop309;
				}
				
			} while (true);
			}
			moduleName_AST = (AST)currentAST.root;
			break;
		}
		case LEADING_COLON2:
		{
			{
			AST tmp382_AST = null;
			tmp382_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp382_AST);
			match(LEADING_COLON2);
			AST tmp383_AST = null;
			tmp383_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp383_AST);
			match(CONSTANT);
			}
			{
			_loop312:
			do {
				if ((LA(1)==COLON2)) {
					AST tmp384_AST = null;
					tmp384_AST = astFactory.create(LT(1));
					astFactory.makeASTRoot(currentAST, tmp384_AST);
					match(COLON2);
					AST tmp385_AST = null;
					tmp385_AST = astFactory.create(LT(1));
					astFactory.addASTChild(currentAST, tmp385_AST);
					match(CONSTANT);
				}
				else {
					break _loop312;
				}
				
			} while (true);
			}
			moduleName_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = moduleName_AST;
	}
	
	public final void methodName() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST methodName_AST = null;
		Token  id = null;
		AST id_AST = null;
		Token  function = null;
		AST function_AST = null;
		Token  constant = null;
		AST constant_AST = null;
		boolean is_singleton_method = false;
		
		{
		switch ( LA(1)) {
		case BOR:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case MATCH:
		case LESS_THAN:
		case GREATER_THAN:
		case LESS_OR_EQUAL:
		case GREATER_OR_EQUAL:
		case BXOR:
		case BAND:
		case LEFT_SHIFT:
		case RIGHT_SHIFT:
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
		case MOD:
		case POWER:
		case BNOT:
		case EMPTY_ARRAY_ACCESS:
		case EMPTY_ARRAY:
		case SINGLE_QUOTE:
		{
			operatorAsMethodname();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_undef:
		case LITERAL_alias:
		case LITERAL_do:
		case LITERAL_end:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case LITERAL_super:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_elsif:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_in:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_then:
		case LITERAL_when:
		case LITERAL_END:
		{
			keywordAsMethodName();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		{
			{
			switch ( LA(1)) {
			case IDENTIFIER:
			{
				id = LT(1);
				id_AST = astFactory.create(id);
				astFactory.addASTChild(currentAST, id_AST);
				match(IDENTIFIER);
				break;
			}
			case FUNCTION:
			{
				function = LT(1);
				function_AST = astFactory.create(function);
				astFactory.addASTChild(currentAST, function_AST);
				match(FUNCTION);
				break;
			}
			case CONSTANT:
			{
				constant = LT(1);
				constant_AST = astFactory.create(constant);
				astFactory.addASTChild(currentAST, constant_AST);
				match(CONSTANT);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case DOT:
			case COLON2:
			{
				methodNameSupplement();
				astFactory.addASTChild(currentAST, returnAST);
				if ( inputState.guessing==0 ) {
					is_singleton_method = true;
				}
				break;
			}
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				AST tmp386_AST = null;
				tmp386_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp386_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
				break;
			}
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case FUNCTION:
			case BLOCK_ARG_PREFIX:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		{
			{
			switch ( LA(1)) {
			case LPAREN:
			{
				match(LPAREN);
				break;
			}
			case LPAREN_WITH_NO_LEADING_SPACE:
			{
				match(LPAREN_WITH_NO_LEADING_SPACE);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			expression();
			astFactory.addASTChild(currentAST, returnAST);
			{
			switch ( LA(1)) {
			case LINE_BREAK:
			{
				match(LINE_BREAK);
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(RPAREN);
			methodNameSupplement();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				is_singleton_method = true;
			}
			break;
		}
		case GLOBAL_VARIABLE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		{
			{
			switch ( LA(1)) {
			case INSTANCE_VARIABLE:
			{
				AST tmp391_AST = null;
				tmp391_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp391_AST);
				match(INSTANCE_VARIABLE);
				break;
			}
			case CLASS_VARIABLE:
			{
				AST tmp392_AST = null;
				tmp392_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp392_AST);
				match(CLASS_VARIABLE);
				break;
			}
			case GLOBAL_VARIABLE:
			{
				AST tmp393_AST = null;
				tmp393_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp393_AST);
				match(GLOBAL_VARIABLE);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			methodNameSupplement();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				is_singleton_method = true;
			}
			break;
		}
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case LITERAL_self:
		{
			{
			switch ( LA(1)) {
			case LITERAL_nil:
			{
				AST tmp394_AST = null;
				tmp394_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp394_AST);
				match(LITERAL_nil);
				break;
			}
			case LITERAL_self:
			{
				AST tmp395_AST = null;
				tmp395_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp395_AST);
				match(LITERAL_self);
				break;
			}
			case LITERAL_true:
			{
				AST tmp396_AST = null;
				tmp396_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp396_AST);
				match(LITERAL_true);
				break;
			}
			case LITERAL_false:
			{
				AST tmp397_AST = null;
				tmp397_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp397_AST);
				match(LITERAL_false);
				break;
			}
			case LITERAL___FILE__:
			{
				AST tmp398_AST = null;
				tmp398_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp398_AST);
				match(LITERAL___FILE__);
				break;
			}
			case LITERAL___LINE__:
			{
				AST tmp399_AST = null;
				tmp399_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp399_AST);
				match(LITERAL___LINE__);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			methodNameSupplement();
			astFactory.addASTChild(currentAST, returnAST);
			if ( inputState.guessing==0 ) {
				is_singleton_method = true;
			}
			break;
		}
		case UNARY_PLUS_MINUS_METHOD_NAME:
		{
			AST tmp400_AST = null;
			tmp400_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp400_AST);
			match(UNARY_PLUS_MINUS_METHOD_NAME);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState.guessing==0 ) {
			methodName_AST = (AST)currentAST.root;
			
							if (is_singleton_method) {
								methodName_AST = (AST)astFactory.make( (new ASTArray(2)).add(astFactory.create(SINGLETON_METHOD,"SINGLETON_METHOD")).add(methodName_AST));
							}
						
			currentAST.root = methodName_AST;
			currentAST.child = methodName_AST!=null &&methodName_AST.getFirstChild()!=null ?
				methodName_AST.getFirstChild() : methodName_AST;
			currentAST.advanceChildToEnd();
		}
		methodName_AST = (AST)currentAST.root;
		returnAST = methodName_AST;
	}
	
	public final void methodDefinitionArgument() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST methodDefinitionArgument_AST = null;
		
		switch ( LA(1)) {
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		{
			lparen();
			astFactory.addASTChild(currentAST, returnAST);
			{
			switch ( LA(1)) {
			case REST_ARG_PREFIX:
			case IDENTIFIER:
			case FUNCTION:
			case BLOCK_ARG_PREFIX:
			{
				methodDefinitionArgumentWithoutParen();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(RPAREN);
			if ( inputState.guessing==0 ) {
				tellLexerWeHaveFinishedParsingMethodparameters();
			}
			{
			if ((LA(1)==SEMI||LA(1)==LINE_BREAK)) {
				terminal();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else if ((LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==REST_ARG_PREFIX||LA(1)==LPAREN_WITH_NO_LEADING_SPACE||LA(1)==LPAREN||LA(1)==IDENTIFIER||LA(1)==CONSTANT||LA(1)==FUNCTION||LA(1)==LITERAL_undef||LA(1)==LITERAL_alias||LA(1)==GLOBAL_VARIABLE||LA(1)==LITERAL_end||LA(1)==COLON_WITH_NO_FOLLOWING_SPACE||LA(1)==INSTANCE_VARIABLE||LA(1)==CLASS_VARIABLE||LA(1)==UNARY_PLUS_MINUS_METHOD_NAME||LA(1)==LITERAL_not||LA(1)==BNOT||LA(1)==NOT||LA(1)==LITERAL_return||LA(1)==LITERAL_break||LA(1)==LITERAL_next||LA(1)==EMPTY_ARRAY_ACCESS||LA(1)==UNARY_PLUS||LA(1)==UNARY_MINUS||LA(1)==LITERAL_nil||LA(1)==LITERAL_true||LA(1)==LITERAL_false||LA(1)==LITERAL___FILE__||LA(1)==LITERAL___LINE__||LA(1)==DOUBLE_QUOTE_STRING||LA(1)==SINGLE_QUOTE_STRING||LA(1)==STRING_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==REGEX||LA(1)==REGEX_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==COMMAND_OUTPUT||LA(1)==COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION||LA(1)==HERE_DOC_BEGIN||LA(1)==W_ARRAY||LA(1)==INTEGER||LA(1)==HEX||LA(1)==BINARY||LA(1)==OCTAL||LA(1)==FLOAT||LA(1)==ASCII_VALUE||LA(1)==LITERAL_self||LA(1)==LITERAL_super||LA(1)==LEADING_COLON2||LA(1)==LITERAL_retry||LA(1)==LITERAL_yield||LA(1)==LITERAL_redo||LA(1)==EMPTY_ARRAY||LA(1)==LBRACK||LA(1)==LCURLY_HASH||LA(1)==LITERAL_rescue||LA(1)==LITERAL_ensure||LA(1)==LITERAL_begin||LA(1)==LITERAL_if||LA(1)==LITERAL_unless||LA(1)==LITERAL_case||LA(1)==LITERAL_for||LA(1)==LITERAL_while||LA(1)==LITERAL_until||LA(1)==LITERAL_module||LA(1)==LITERAL_class||LA(1)==LITERAL_def||LA(1)==155||LA(1)==LITERAL_BEGIN||LA(1)==LITERAL_END)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			methodDefinitionArgument_AST = (AST)currentAST.root;
			break;
		}
		case SEMI:
		case LINE_BREAK:
		case REST_ARG_PREFIX:
		case IDENTIFIER:
		case FUNCTION:
		case BLOCK_ARG_PREFIX:
		{
			{
			switch ( LA(1)) {
			case REST_ARG_PREFIX:
			case IDENTIFIER:
			case FUNCTION:
			case BLOCK_ARG_PREFIX:
			{
				methodDefinitionArgumentWithoutParen();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case SEMI:
			case LINE_BREAK:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			terminal();
			astFactory.addASTChild(currentAST, returnAST);
			methodDefinitionArgument_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = methodDefinitionArgument_AST;
	}
	
	protected final void methodNameSupplement() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST methodNameSupplement_AST = null;
		
		{
		switch ( LA(1)) {
		case DOT:
		{
			AST tmp402_AST = null;
			tmp402_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp402_AST);
			match(DOT);
			break;
		}
		case COLON2:
		{
			AST tmp403_AST = null;
			tmp403_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp403_AST);
			match(COLON2);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case IDENTIFIER:
		{
			AST tmp404_AST = null;
			tmp404_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp404_AST);
			match(IDENTIFIER);
			{
			switch ( LA(1)) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				AST tmp405_AST = null;
				tmp405_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp405_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
				break;
			}
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case FUNCTION:
			case BLOCK_ARG_PREFIX:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case FUNCTION:
		{
			AST tmp406_AST = null;
			tmp406_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp406_AST);
			match(FUNCTION);
			{
			switch ( LA(1)) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				AST tmp407_AST = null;
				tmp407_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp407_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
				break;
			}
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case FUNCTION:
			case BLOCK_ARG_PREFIX:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case CONSTANT:
		{
			AST tmp408_AST = null;
			tmp408_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp408_AST);
			match(CONSTANT);
			{
			switch ( LA(1)) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				AST tmp409_AST = null;
				tmp409_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp409_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
				break;
			}
			case SEMI:
			case LINE_BREAK:
			case REST_ARG_PREFIX:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case FUNCTION:
			case BLOCK_ARG_PREFIX:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case BOR:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case MATCH:
		case LESS_THAN:
		case GREATER_THAN:
		case LESS_OR_EQUAL:
		case GREATER_OR_EQUAL:
		case BXOR:
		case BAND:
		case LEFT_SHIFT:
		case RIGHT_SHIFT:
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
		case MOD:
		case POWER:
		case BNOT:
		case EMPTY_ARRAY_ACCESS:
		case EMPTY_ARRAY:
		case SINGLE_QUOTE:
		{
			operatorAsMethodname();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case LITERAL_undef:
		case LITERAL_alias:
		case LITERAL_do:
		case LITERAL_end:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case LITERAL_self:
		case LITERAL_super:
		case LITERAL_retry:
		case LITERAL_yield:
		case LITERAL_redo:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		case LITERAL_begin:
		case LITERAL_if:
		case LITERAL_elsif:
		case LITERAL_unless:
		case LITERAL_case:
		case LITERAL_for:
		case LITERAL_in:
		case LITERAL_while:
		case LITERAL_until:
		case LITERAL_module:
		case LITERAL_class:
		case LITERAL_def:
		case 155:
		case LITERAL_BEGIN:
		case LITERAL_then:
		case LITERAL_when:
		case LITERAL_END:
		{
			keyword();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		methodNameSupplement_AST = (AST)currentAST.root;
		returnAST = methodNameSupplement_AST;
	}
	
	public final void methodDefinitionArgumentWithoutParen() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST methodDefinitionArgumentWithoutParen_AST = null;
		boolean seen_star_or_band = false;
		
		switch ( LA(1)) {
		case IDENTIFIER:
		case FUNCTION:
		{
			normalMethodDefinitionArgument();
			astFactory.addASTChild(currentAST, returnAST);
			{
			_loop353:
			do {
				if ((LA(1)==COMMA)) {
					match(COMMA);
					if ( inputState.guessing==0 ) {
						if (REST_ARG_PREFIX == LA(1) || BLOCK_ARG_PREFIX == LA(1))	{seen_star_or_band = true;break;}
					}
					normalMethodDefinitionArgument();
					astFactory.addASTChild(currentAST, returnAST);
				}
				else {
					break _loop353;
				}
				
			} while (true);
			}
			{
			if (((LA(1)==REST_ARG_PREFIX))&&(seen_star_or_band)) {
				restMethodDefinitionArgument();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else if (((LA(1)==BLOCK_ARG_PREFIX))&&(seen_star_or_band)) {
				blockMethodDefinitionArgument();
				astFactory.addASTChild(currentAST, returnAST);
			}
			else if ((LA(1)==SEMI||LA(1)==LINE_BREAK||LA(1)==RPAREN)) {
			}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			
			}
			methodDefinitionArgumentWithoutParen_AST = (AST)currentAST.root;
			break;
		}
		case REST_ARG_PREFIX:
		{
			restMethodDefinitionArgument();
			astFactory.addASTChild(currentAST, returnAST);
			methodDefinitionArgumentWithoutParen_AST = (AST)currentAST.root;
			break;
		}
		case BLOCK_ARG_PREFIX:
		{
			blockMethodDefinitionArgument();
			astFactory.addASTChild(currentAST, returnAST);
			methodDefinitionArgumentWithoutParen_AST = (AST)currentAST.root;
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		returnAST = methodDefinitionArgumentWithoutParen_AST;
	}
	
	public final void normalMethodDefinitionArgument() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST normalMethodDefinitionArgument_AST = null;
		
		variable();
		astFactory.addASTChild(currentAST, returnAST);
		{
		switch ( LA(1)) {
		case ASSIGN:
		case ASSIGN_WITH_NO_LEADING_SPACE:
		{
			{
			switch ( LA(1)) {
			case ASSIGN:
			{
				AST tmp411_AST = null;
				tmp411_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp411_AST);
				match(ASSIGN);
				break;
			}
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				AST tmp412_AST = null;
				tmp412_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, tmp412_AST);
				match(ASSIGN_WITH_NO_LEADING_SPACE);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			expression();
			astFactory.addASTChild(currentAST, returnAST);
			break;
		}
		case SEMI:
		case LINE_BREAK:
		case COMMA:
		case REST_ARG_PREFIX:
		case RPAREN:
		case BLOCK_ARG_PREFIX:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		normalMethodDefinitionArgument_AST = (AST)currentAST.root;
		returnAST = normalMethodDefinitionArgument_AST;
	}
	
	public final void restMethodDefinitionArgument() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST restMethodDefinitionArgument_AST = null;
		
		AST tmp413_AST = null;
		tmp413_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp413_AST);
		match(REST_ARG_PREFIX);
		{
		switch ( LA(1)) {
		case IDENTIFIER:
		case FUNCTION:
		{
			variable();
			astFactory.addASTChild(currentAST, returnAST);
			{
			switch ( LA(1)) {
			case COMMA:
			{
				match(COMMA);
				blockMethodDefinitionArgument();
				astFactory.addASTChild(currentAST, returnAST);
				break;
			}
			case SEMI:
			case LINE_BREAK:
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case SEMI:
		case LINE_BREAK:
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		restMethodDefinitionArgument_AST = (AST)currentAST.root;
		returnAST = restMethodDefinitionArgument_AST;
	}
	
	public final void blockMethodDefinitionArgument() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST blockMethodDefinitionArgument_AST = null;
		
		AST tmp415_AST = null;
		tmp415_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp415_AST);
		match(BLOCK_ARG_PREFIX);
		variable();
		astFactory.addASTChild(currentAST, returnAST);
		blockMethodDefinitionArgument_AST = (AST)currentAST.root;
		returnAST = blockMethodDefinitionArgument_AST;
	}
	
	public final void operator_ASSIGN() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST operator_ASSIGN_AST = null;
		
		{
		switch ( LA(1)) {
		case ASSIGN:
		{
			AST tmp416_AST = null;
			tmp416_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp416_AST);
			match(ASSIGN);
			break;
		}
		case ASSIGN_WITH_NO_LEADING_SPACE:
		{
			AST tmp417_AST = null;
			tmp417_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, tmp417_AST);
			match(ASSIGN_WITH_NO_LEADING_SPACE);
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		operator_ASSIGN_AST = (AST)currentAST.root;
		returnAST = operator_ASSIGN_AST;
	}
	
	public final void keyword___FILE__() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword___FILE___AST = null;
		
		AST tmp419_AST = null;
		tmp419_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp419_AST);
		match(LITERAL___FILE__);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword___FILE___AST = (AST)currentAST.root;
		returnAST = keyword___FILE___AST;
	}
	
	public final void keyword___LINE__() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword___LINE___AST = null;
		
		AST tmp421_AST = null;
		tmp421_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp421_AST);
		match(LITERAL___LINE__);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword___LINE___AST = (AST)currentAST.root;
		returnAST = keyword___LINE___AST;
	}
	
	public final void keyword_and() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_and_AST = null;
		
		AST tmp423_AST = null;
		tmp423_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp423_AST);
		match(LITERAL_and);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_and_AST = (AST)currentAST.root;
		returnAST = keyword_and_AST;
	}
	
	public final void keyword_begin() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_begin_AST = null;
		
		AST tmp425_AST = null;
		tmp425_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp425_AST);
		match(LITERAL_begin);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_begin_AST = (AST)currentAST.root;
		returnAST = keyword_begin_AST;
	}
	
	public final void keyword_break() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_break_AST = null;
		
		AST tmp427_AST = null;
		tmp427_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp427_AST);
		match(LITERAL_break);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_break_AST = (AST)currentAST.root;
		returnAST = keyword_break_AST;
	}
	
	public final void keyword_case() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_case_AST = null;
		
		AST tmp429_AST = null;
		tmp429_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp429_AST);
		match(LITERAL_case);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_case_AST = (AST)currentAST.root;
		returnAST = keyword_case_AST;
	}
	
	public final void keyword_class() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_class_AST = null;
		
		AST tmp431_AST = null;
		tmp431_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp431_AST);
		match(LITERAL_class);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_class_AST = (AST)currentAST.root;
		returnAST = keyword_class_AST;
	}
	
	public final void keyword_def() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_def_AST = null;
		
		AST tmp433_AST = null;
		tmp433_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp433_AST);
		match(LITERAL_def);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_def_AST = (AST)currentAST.root;
		returnAST = keyword_def_AST;
	}
	
	public final void keyword_do() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_do_AST = null;
		
		AST tmp435_AST = null;
		tmp435_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp435_AST);
		match(LITERAL_do);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_do_AST = (AST)currentAST.root;
		returnAST = keyword_do_AST;
	}
	
	public final void keyword_else() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_else_AST = null;
		
		AST tmp437_AST = null;
		tmp437_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp437_AST);
		match(LITERAL_else);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_else_AST = (AST)currentAST.root;
		returnAST = keyword_else_AST;
	}
	
	public final void keyword_elsif() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_elsif_AST = null;
		
		AST tmp439_AST = null;
		tmp439_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp439_AST);
		match(LITERAL_elsif);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_elsif_AST = (AST)currentAST.root;
		returnAST = keyword_elsif_AST;
	}
	
	public final void keyword_end() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_end_AST = null;
		
		AST tmp441_AST = null;
		tmp441_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp441_AST);
		match(LITERAL_end);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_end_AST = (AST)currentAST.root;
		returnAST = keyword_end_AST;
	}
	
	public final void keyword_ensure() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_ensure_AST = null;
		
		AST tmp443_AST = null;
		tmp443_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp443_AST);
		match(LITERAL_ensure);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_ensure_AST = (AST)currentAST.root;
		returnAST = keyword_ensure_AST;
	}
	
	public final void keyword_false() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_false_AST = null;
		
		AST tmp445_AST = null;
		tmp445_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp445_AST);
		match(LITERAL_false);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_false_AST = (AST)currentAST.root;
		returnAST = keyword_false_AST;
	}
	
	public final void keyword_module() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_module_AST = null;
		
		AST tmp447_AST = null;
		tmp447_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp447_AST);
		match(LITERAL_module);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_module_AST = (AST)currentAST.root;
		returnAST = keyword_module_AST;
	}
	
	public final void keyword_next() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_next_AST = null;
		
		AST tmp449_AST = null;
		tmp449_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp449_AST);
		match(LITERAL_next);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_next_AST = (AST)currentAST.root;
		returnAST = keyword_next_AST;
	}
	
	public final void keyword_nil() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_nil_AST = null;
		
		AST tmp451_AST = null;
		tmp451_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp451_AST);
		match(LITERAL_nil);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_nil_AST = (AST)currentAST.root;
		returnAST = keyword_nil_AST;
	}
	
	public final void keyword_not() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_not_AST = null;
		
		AST tmp453_AST = null;
		tmp453_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp453_AST);
		match(LITERAL_not);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_not_AST = (AST)currentAST.root;
		returnAST = keyword_not_AST;
	}
	
	public final void keyword_or() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_or_AST = null;
		
		AST tmp455_AST = null;
		tmp455_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp455_AST);
		match(LITERAL_or);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_or_AST = (AST)currentAST.root;
		returnAST = keyword_or_AST;
	}
	
	public final void keyword_redo() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_redo_AST = null;
		
		AST tmp457_AST = null;
		tmp457_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp457_AST);
		match(LITERAL_redo);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_redo_AST = (AST)currentAST.root;
		returnAST = keyword_redo_AST;
	}
	
	public final void keyword_rescue() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_rescue_AST = null;
		
		AST tmp459_AST = null;
		tmp459_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp459_AST);
		match(LITERAL_rescue);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_rescue_AST = (AST)currentAST.root;
		returnAST = keyword_rescue_AST;
	}
	
	public final void keyword_retry() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_retry_AST = null;
		
		AST tmp461_AST = null;
		tmp461_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp461_AST);
		match(LITERAL_retry);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_retry_AST = (AST)currentAST.root;
		returnAST = keyword_retry_AST;
	}
	
	public final void keyword_return() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_return_AST = null;
		
		AST tmp463_AST = null;
		tmp463_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp463_AST);
		match(LITERAL_return);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_return_AST = (AST)currentAST.root;
		returnAST = keyword_return_AST;
	}
	
	public final void keyword_self() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_self_AST = null;
		
		AST tmp465_AST = null;
		tmp465_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp465_AST);
		match(LITERAL_self);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_self_AST = (AST)currentAST.root;
		returnAST = keyword_self_AST;
	}
	
	public final void keyword_super() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_super_AST = null;
		
		AST tmp467_AST = null;
		tmp467_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp467_AST);
		match(LITERAL_super);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_super_AST = (AST)currentAST.root;
		returnAST = keyword_super_AST;
	}
	
	public final void keyword_then() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_then_AST = null;
		
		AST tmp469_AST = null;
		tmp469_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp469_AST);
		match(LITERAL_then);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_then_AST = (AST)currentAST.root;
		returnAST = keyword_then_AST;
	}
	
	public final void keyword_true() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_true_AST = null;
		
		AST tmp471_AST = null;
		tmp471_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp471_AST);
		match(LITERAL_true);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_true_AST = (AST)currentAST.root;
		returnAST = keyword_true_AST;
	}
	
	public final void keyword_undef() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_undef_AST = null;
		
		AST tmp473_AST = null;
		tmp473_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp473_AST);
		match(LITERAL_undef);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_undef_AST = (AST)currentAST.root;
		returnAST = keyword_undef_AST;
	}
	
	public final void keyword_yield() throws RecognitionException, TokenStreamException {
		
		returnAST = null;
		ASTPair currentAST = new ASTPair();
		AST keyword_yield_AST = null;
		
		AST tmp475_AST = null;
		tmp475_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, tmp475_AST);
		match(LITERAL_yield);
		{
		switch ( LA(1)) {
		case LINE_BREAK:
		{
			match(LINE_BREAK);
			break;
		}
		case EOF:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		}
		keyword_yield_AST = (AST)currentAST.root;
		returnAST = keyword_yield_AST;
	}
	
	
	public static final String[] _tokenNames = {
		"<0>",
		"EOF",
		"<2>",
		"NULL_TREE_LOOKAHEAD",
		"RPAREN_IN_METHOD_DEFINATION",
		"BODY",
		"CALL",
		"ARG",
		"COMPSTMT",
		"SYMBOL",
		"BLOCK",
		"MULTIPLE_ASSIGN",
		"MULTIPLE_ASSIGN_WITH_EXTRA_COMMA",
		"BLOCK_ARG",
		"BLOCK_ARG_WITH_EXTRA_COMMA",
		"MRHS",
		"NESTED_LHS",
		"SINGLETON_METHOD",
		"STRING",
		"SEMI",
		"LINE_BREAK",
		"IF_MODIFIER",
		"UNLESS_MODIFIER",
		"WHILE_MODIFIER",
		"UNTIL_MODIFIER",
		"RESCUE_MODIFIER",
		"LCURLY_BLOCK",
		"RCURLY",
		"COMMA",
		"REST_ARG_PREFIX",
		"ASSIGN",
		"ASSIGN_WITH_NO_LEADING_SPACE",
		"LPAREN_WITH_NO_LEADING_SPACE",
		"LPAREN",
		"RPAREN",
		"IDENTIFIER",
		"CONSTANT",
		"FUNCTION",
		"\"undef\"",
		"\"alias\"",
		"GLOBAL_VARIABLE",
		"\"do\"",
		"\"end\"",
		"BOR",
		"LOGICAL_OR",
		"COLON_WITH_NO_FOLLOWING_SPACE",
		"INSTANCE_VARIABLE",
		"CLASS_VARIABLE",
		"UNARY_PLUS_MINUS_METHOD_NAME",
		"\"and\"",
		"\"or\"",
		"\"not\"",
		"QUESTION",
		"COLON",
		"PLUS_ASSIGN",
		"MINUS_ASSIGN",
		"STAR_ASSIGN",
		"DIV_ASSIGN",
		"MOD_ASSIGN",
		"POWER_ASSIGN",
		"BAND_ASSIGN",
		"BXOR_ASSIGN",
		"BOR_ASSIGN",
		"LEFT_SHIFT_ASSIGN",
		"RIGHT_SHIFT_ASSIGN",
		"LOGICAL_AND_ASSIGN",
		"LOGICAL_OR_ASSIGN",
		"INCLUSIVE_RANGE",
		"EXCLUSIVE_RANGE",
		"LOGICAL_AND",
		"COMPARE",
		"EQUAL",
		"CASE_EQUAL",
		"NOT_EQUAL",
		"MATCH",
		"NOT_MATCH",
		"LESS_THAN",
		"GREATER_THAN",
		"LESS_OR_EQUAL",
		"GREATER_OR_EQUAL",
		"BXOR",
		"BAND",
		"LEFT_SHIFT",
		"RIGHT_SHIFT",
		"PLUS",
		"MINUS",
		"STAR",
		"DIV",
		"MOD",
		"POWER",
		"BNOT",
		"NOT",
		"\"return\"",
		"\"break\"",
		"\"next\"",
		"DOT",
		"COLON2",
		"LBRACK_ARRAY_ACCESS",
		"RBRACK",
		"EMPTY_ARRAY_ACCESS",
		"UNARY_PLUS",
		"UNARY_MINUS",
		"ASSOC",
		"BLOCK_ARG_PREFIX",
		"\"nil\"",
		"\"true\"",
		"\"false\"",
		"\"__FILE__\"",
		"\"__LINE__\"",
		"DOUBLE_QUOTE_STRING",
		"SINGLE_QUOTE_STRING",
		"STRING_BEFORE_EXPRESSION_SUBSTITUTION",
		"STRING_BETWEEN_EXPRESSION_SUBSTITUTION",
		"STRING_AFTER_EXPRESSION_SUBSTITUTION",
		"REGEX",
		"REGEX_BEFORE_EXPRESSION_SUBSTITUTION",
		"COMMAND_OUTPUT",
		"COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION",
		"HERE_DOC_BEGIN",
		"HERE_DOC_CONTENT",
		"HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION",
		"HERE_DOC_BETWEEN_EXPRESSION_SUBSTITUTION",
		"HERE_DOC_AFTER_EXPRESSION_SUBSTITUTION",
		"W_ARRAY",
		"INTEGER",
		"HEX",
		"BINARY",
		"OCTAL",
		"FLOAT",
		"ASCII_VALUE",
		"\"self\"",
		"\"super\"",
		"LEADING_COLON2",
		"\"retry\"",
		"\"yield\"",
		"\"redo\"",
		"EMPTY_ARRAY",
		"LBRACK",
		"LCURLY_HASH",
		"\"rescue\"",
		"\"else\"",
		"\"ensure\"",
		"\"begin\"",
		"\"if\"",
		"\"elsif\"",
		"\"unless\"",
		"\"case\"",
		"\"for\"",
		"\"in\"",
		"\"while\"",
		"\"until\"",
		"\"module\"",
		"\"class\"",
		"\"def\"",
		"SINGLE_QUOTE",
		"\"defined?\"",
		"\"BEGIN\"",
		"\"then\"",
		"\"when\"",
		"\"END\"",
		"DO_IN_CONDITION",
		"PURE_LINE_BREAK",
		"LINE_FEED",
		"REGEX_MODIFIER",
		"SPECIAL_STRING",
		"STRING_CHAR",
		"ANYTHING_OTHER_THAN_LINE_FEED_AND_POUND",
		"HERE_DOC_DELIMITER",
		"RDOC",
		"ANYTHING_OTHER_THAN_LINE_FEED",
		"LINE",
		"ESC",
		"IDENTIFIER_CONSTANT_AND_KEYWORD",
		"UNDER_SCORE",
		"FLOAT_WITH_LEADING_DOT",
		"NON_ZERO_DECIMAL",
		"OCTAL_CONTENT",
		"HEX_CONTENT",
		"BINARY_CONTENT",
		"EXPONENT",
		"COMMENT",
		"WHITE_SPACE_CHAR",
		"WHITE_SPACE",
		"LINE_CONTINUATION",
		"END_OF_FILE"
	};
	
	protected void buildTokenTypeASTClassMap() {
		tokenTypeToASTClassMap=null;
	};
	
	
	}
