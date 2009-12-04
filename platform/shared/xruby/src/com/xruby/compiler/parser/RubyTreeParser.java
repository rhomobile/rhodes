// $ANTLR 2.7.6 (2005-12-22): "src/com/xruby/compiler/parser/ruby.tree.g" -> "RubyTreeParser.java"$

package com.xruby.compiler.parser;

import com.xruby.compiler.codedom.*;

import antlr.TreeParser;
import antlr.Token;
import antlr.collections.AST;
import antlr.RecognitionException;
import antlr.ANTLRException;
import antlr.NoViableAltException;
import antlr.MismatchedTokenException;
import antlr.SemanticException;
import antlr.collections.impl.BitSet;
import antlr.ASTPair;
import antlr.collections.impl.ASTArray;


/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */
public class RubyTreeParser extends antlr.TreeParser       implements RubyTreeParserTokenTypes
 {

	private String filename_ = null;
	protected int currentLineNumber = 0;

	public Program parse(AST t, String filename) throws RecognitionException {
		filename_ = filename;

		Program p = program(t);
		if (null == p) {
			return new Program(null);
		} else {
			return p;
		}
	}

public RubyTreeParser() {
	tokenNames = _tokenNames;
}

	public final Program  program(AST _t) throws RecognitionException {
		Program p;
		
		AST program_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			p = null;
			CompoundStatement cs = null;
		
		
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		{
			cs=compoundStatement(_t);
			_t = _retTree;
			p = new Program(cs);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_retTree = _t;
		return p;
	}
	
	public final CompoundStatement  compoundStatement(AST _t) throws RecognitionException {
		CompoundStatement cs;
		
		AST compoundStatement_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Statement s = null;
			cs = new CompoundStatement();
		
		
		AST __t4 = _t;
		AST tmp1_AST_in = (AST)_t;
		match(_t,COMPSTMT);
		_t = _t.getFirstChild();
		{
		int _cnt6=0;
		_loop6:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_tokenSet_0.member(_t.getType()))) {
				s=statement(_t);
				_t = _retTree;
				cs.addStatement(s);
			}
			else {
				if ( _cnt6>=1 ) { break _loop6; } else {throw new NoViableAltException(_t);}
			}
			
			_cnt6++;
		} while (true);
		}
		_t = __t4;
		_t = _t.getNextSibling();
		_retTree = _t;
		return cs;
	}
	
	public final Statement  statement(AST _t) throws RecognitionException {
		Statement s;
		
		AST statement_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case CALL:
		case SYMBOL:
		case STRING:
		case WHILE_MODIFIER:
		case UNTIL_MODIFIER:
		case ASSIGN:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case BOR:
		case LOGICAL_OR:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case QUESTION:
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
		case INCLUSIVE_RANGE:
		case EXCLUSIVE_RANGE:
		case LOGICAL_AND:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case NOT_EQUAL:
		case MATCH:
		case NOT_MATCH:
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
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case DOT:
		case COLON2:
		case LBRACK_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_CONTENT:
		case HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION:
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
		{
			s=expressionStatement(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_alias:
		{
			s=alias(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_undef:
		{
			s=undef(_t);
			_t = _retTree;
			break;
		}
		case MULTIPLE_ASSIGN:
		case MULTIPLE_ASSIGN_WITH_EXTRA_COMMA:
		{
			s=multipleAssignment(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_BEGIN:
		{
			s=beginBlock(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_END:
		{
			s=endBlock(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return s;
	}
	
	public final ExpressionStatement  expressionStatement(AST _t) throws RecognitionException {
		ExpressionStatement s;
		
		AST expressionStatement_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression e = null;
		
		
		e=expression(_t);
		_t = _retTree;
		
						s = new ExpressionStatement(e);
					
		_retTree = _t;
		return s;
	}
	
	public final AliasStatement  alias(AST _t) throws RecognitionException {
		AliasStatement s;
		
		AST alias_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			String newname = null;
			String oldname = null;
		
		
		AST __t14 = _t;
		AST tmp2_AST_in = (AST)_t;
		match(_t,LITERAL_alias);
		_t = _t.getFirstChild();
		newname=aliasParameter(_t);
		_t = _retTree;
		oldname=aliasParameter(_t);
		_t = _retTree;
		_t = __t14;
		_t = _t.getNextSibling();
		s = new AliasStatement(newname, oldname);
		_retTree = _t;
		return s;
	}
	
	public final UndefStatement  undef(AST _t) throws RecognitionException {
		UndefStatement s;
		
		AST undef_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		String name = null;
		
		AST __t20 = _t;
		AST tmp3_AST_in = (AST)_t;
		match(_t,LITERAL_undef);
		_t = _t.getFirstChild();
		s = new UndefStatement();
		{
		int _cnt22=0;
		_loop22:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_t.getType()==SYMBOL||_t.getType()==IDENTIFIER||_t.getType()==FUNCTION)) {
				name=undefParameter(_t);
				_t = _retTree;
				s.add(name);
			}
			else {
				if ( _cnt22>=1 ) { break _loop22; } else {throw new NoViableAltException(_t);}
			}
			
			_cnt22++;
		} while (true);
		}
		_t = __t20;
		_t = _t.getNextSibling();
		_retTree = _t;
		return s;
	}
	
	public final MultipleAssignmentStatement  multipleAssignment(AST _t) throws RecognitionException {
		MultipleAssignmentStatement s;
		
		AST multipleAssignment_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression e = null;
		
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case MULTIPLE_ASSIGN:
		{
			AST __t25 = _t;
			AST tmp4_AST_in = (AST)_t;
			match(_t,MULTIPLE_ASSIGN);
			_t = _t.getFirstChild();
			s = new MultipleAssignmentStatement(false);
			multipleAssignmentBody(_t,s);
			_t = _retTree;
			_t = __t25;
			_t = _t.getNextSibling();
			break;
		}
		case MULTIPLE_ASSIGN_WITH_EXTRA_COMMA:
		{
			AST __t26 = _t;
			AST tmp5_AST_in = (AST)_t;
			match(_t,MULTIPLE_ASSIGN_WITH_EXTRA_COMMA);
			_t = _t.getFirstChild();
			s = new MultipleAssignmentStatement(true);
			multipleAssignmentBody(_t,s);
			_t = _retTree;
			_t = __t26;
			_t = _t.getNextSibling();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return s;
	}
	
	public final BEGINBlock  beginBlock(AST _t) throws RecognitionException {
		BEGINBlock b;
		
		AST beginBlock_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			CompoundStatement cs=null;
		
		
		AST tmp6_AST_in = (AST)_t;
		match(_t,LITERAL_BEGIN);
		_t = _t.getNextSibling();
		AST tmp7_AST_in = (AST)_t;
		match(_t,LCURLY_BLOCK);
		_t = _t.getNextSibling();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		{
			cs=compoundStatement(_t);
			_t = _retTree;
			break;
		}
		case RCURLY:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		AST tmp8_AST_in = (AST)_t;
		match(_t,RCURLY);
		_t = _t.getNextSibling();
		
						b = new BEGINBlock(cs);
					
		_retTree = _t;
		return b;
	}
	
	public final ENDBlock  endBlock(AST _t) throws RecognitionException {
		ENDBlock b;
		
		AST endBlock_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			CompoundStatement cs=null;
		
		
		AST tmp9_AST_in = (AST)_t;
		match(_t,LITERAL_END);
		_t = _t.getNextSibling();
		AST tmp10_AST_in = (AST)_t;
		match(_t,LCURLY_BLOCK);
		_t = _t.getNextSibling();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		{
			cs=compoundStatement(_t);
			_t = _retTree;
			break;
		}
		case RCURLY:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		AST tmp11_AST_in = (AST)_t;
		match(_t,RCURLY);
		_t = _t.getNextSibling();
		
						b = new ENDBlock(cs);
					
		_retTree = _t;
		return b;
	}
	
	public final Expression  expression(AST _t) throws RecognitionException {
		Expression e;
		
		AST expression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST constant = null;
		AST function = null;
		
			e = null;
			Expression condition = null;
			Expression left = null;
			Expression right = null;
			MethodCallArguments args = null;
			ReturnArguments return_args = null;
			CompoundStatement cs = null;
			String method_name = null;
			int lineNumber = (null == _t) ? 0 : _t.getLine();
			boolean startANewLine = false;
		
			if (lineNumber != currentLineNumber) {
		currentLineNumber = lineNumber;
		startANewLine = true;
		}
		
		
		
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LITERAL_and:
		{
			AST __t45 = _t;
			AST tmp12_AST_in = (AST)_t;
			match(_t,LITERAL_and);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t45;
			_t = _t.getNextSibling();
			e = new AndOrBinaryOperatorExpression("&&", left, right);
			break;
		}
		case LITERAL_or:
		{
			AST __t46 = _t;
			AST tmp13_AST_in = (AST)_t;
			match(_t,LITERAL_or);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t46;
			_t = _t.getNextSibling();
			e = new AndOrBinaryOperatorExpression("||", left, right);
			break;
		}
		case LITERAL_not:
		{
			AST __t47 = _t;
			AST tmp14_AST_in = (AST)_t;
			match(_t,LITERAL_not);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			_t = __t47;
			_t = _t.getNextSibling();
			e = new UnaryOperatorExpression("!", left);
			break;
		}
		case QUESTION:
		{
			AST __t48 = _t;
			AST tmp15_AST_in = (AST)_t;
			match(_t,QUESTION);
			_t = _t.getFirstChild();
			condition=expression(_t);
			_t = _retTree;
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t48;
			_t = _t.getNextSibling();
			e = new IfExpression(condition, left, right);
			break;
		}
		case ASSIGN:
		{
			AST __t49 = _t;
			AST tmp16_AST_in = (AST)_t;
			match(_t,ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t49;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, right);
			break;
		}
		case PLUS_ASSIGN:
		{
			AST __t50 = _t;
			AST tmp17_AST_in = (AST)_t;
			match(_t,PLUS_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t50;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new BinaryOperatorExpression("+", left, right));
			break;
		}
		case MINUS_ASSIGN:
		{
			AST __t51 = _t;
			AST tmp18_AST_in = (AST)_t;
			match(_t,MINUS_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t51;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new BinaryOperatorExpression("-", left, right));
			break;
		}
		case STAR_ASSIGN:
		{
			AST __t52 = _t;
			AST tmp19_AST_in = (AST)_t;
			match(_t,STAR_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t52;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new BinaryOperatorExpression("*", left, right));
			break;
		}
		case DIV_ASSIGN:
		{
			AST __t53 = _t;
			AST tmp20_AST_in = (AST)_t;
			match(_t,DIV_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t53;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new BinaryOperatorExpression("/", left, right));
			break;
		}
		case MOD_ASSIGN:
		{
			AST __t54 = _t;
			AST tmp21_AST_in = (AST)_t;
			match(_t,MOD_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t54;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new BinaryOperatorExpression("%", left, right));
			break;
		}
		case POWER_ASSIGN:
		{
			AST __t55 = _t;
			AST tmp22_AST_in = (AST)_t;
			match(_t,POWER_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t55;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new BinaryOperatorExpression("**", left, right));
			break;
		}
		case BAND_ASSIGN:
		{
			AST __t56 = _t;
			AST tmp23_AST_in = (AST)_t;
			match(_t,BAND_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t56;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new BinaryOperatorExpression("&", left, right));
			break;
		}
		case BXOR_ASSIGN:
		{
			AST __t57 = _t;
			AST tmp24_AST_in = (AST)_t;
			match(_t,BXOR_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t57;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new BinaryOperatorExpression("^", left, right));
			break;
		}
		case BOR_ASSIGN:
		{
			AST __t58 = _t;
			AST tmp25_AST_in = (AST)_t;
			match(_t,BOR_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t58;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new BinaryOperatorExpression("|", left, right));
			break;
		}
		case LEFT_SHIFT_ASSIGN:
		{
			AST __t59 = _t;
			AST tmp26_AST_in = (AST)_t;
			match(_t,LEFT_SHIFT_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t59;
			_t = _t.getNextSibling();
			e =AssignmentOperatorExpression.create(left,  new BinaryOperatorExpression("<<", left, right));
			break;
		}
		case RIGHT_SHIFT_ASSIGN:
		{
			AST __t60 = _t;
			AST tmp27_AST_in = (AST)_t;
			match(_t,RIGHT_SHIFT_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t60;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new BinaryOperatorExpression(">>", left, right));
			break;
		}
		case LOGICAL_AND_ASSIGN:
		{
			AST __t61 = _t;
			AST tmp28_AST_in = (AST)_t;
			match(_t,LOGICAL_AND_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t61;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new AndOrBinaryOperatorExpression("&&", left, right));
			break;
		}
		case LOGICAL_OR_ASSIGN:
		{
			AST __t62 = _t;
			AST tmp29_AST_in = (AST)_t;
			match(_t,LOGICAL_OR_ASSIGN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t62;
			_t = _t.getNextSibling();
			e = AssignmentOperatorExpression.create(left, new AndOrBinaryOperatorExpression("||", left, right));
			break;
		}
		case INCLUSIVE_RANGE:
		{
			AST __t63 = _t;
			AST tmp30_AST_in = (AST)_t;
			match(_t,INCLUSIVE_RANGE);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t63;
			_t = _t.getNextSibling();
			e = new RangeOperatorExpression("..", left, right);
			break;
		}
		case EXCLUSIVE_RANGE:
		{
			AST __t64 = _t;
			AST tmp31_AST_in = (AST)_t;
			match(_t,EXCLUSIVE_RANGE);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t64;
			_t = _t.getNextSibling();
			e = new RangeOperatorExpression("...", left, right);
			break;
		}
		case LOGICAL_OR:
		{
			AST __t65 = _t;
			AST tmp32_AST_in = (AST)_t;
			match(_t,LOGICAL_OR);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t65;
			_t = _t.getNextSibling();
			e = new AndOrBinaryOperatorExpression("||", left, right);
			break;
		}
		case LOGICAL_AND:
		{
			AST __t66 = _t;
			AST tmp33_AST_in = (AST)_t;
			match(_t,LOGICAL_AND);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t66;
			_t = _t.getNextSibling();
			e = new AndOrBinaryOperatorExpression("&&", left, right);
			break;
		}
		case COMPARE:
		{
			AST __t67 = _t;
			AST tmp34_AST_in = (AST)_t;
			match(_t,COMPARE);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t67;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("<=>", left, right);
			break;
		}
		case EQUAL:
		{
			AST __t68 = _t;
			AST tmp35_AST_in = (AST)_t;
			match(_t,EQUAL);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t68;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("==", left, right);
			break;
		}
		case CASE_EQUAL:
		{
			AST __t69 = _t;
			AST tmp36_AST_in = (AST)_t;
			match(_t,CASE_EQUAL);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t69;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("===", left, right);
			break;
		}
		case NOT_EQUAL:
		{
			AST __t70 = _t;
			AST tmp37_AST_in = (AST)_t;
			match(_t,NOT_EQUAL);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t70;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("!=", left, right);
			break;
		}
		case MATCH:
		{
			AST __t71 = _t;
			AST tmp38_AST_in = (AST)_t;
			match(_t,MATCH);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t71;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("=~", left, right);
			break;
		}
		case NOT_MATCH:
		{
			AST __t72 = _t;
			AST tmp39_AST_in = (AST)_t;
			match(_t,NOT_MATCH);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t72;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("!~", left, right);
			break;
		}
		case LESS_THAN:
		{
			AST __t73 = _t;
			AST tmp40_AST_in = (AST)_t;
			match(_t,LESS_THAN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t73;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("<", left, right);
			break;
		}
		case GREATER_THAN:
		{
			AST __t74 = _t;
			AST tmp41_AST_in = (AST)_t;
			match(_t,GREATER_THAN);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t74;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression(">", left, right);
			break;
		}
		case LESS_OR_EQUAL:
		{
			AST __t75 = _t;
			AST tmp42_AST_in = (AST)_t;
			match(_t,LESS_OR_EQUAL);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t75;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("<=", left, right);
			break;
		}
		case GREATER_OR_EQUAL:
		{
			AST __t76 = _t;
			AST tmp43_AST_in = (AST)_t;
			match(_t,GREATER_OR_EQUAL);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t76;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression(">=", left, right);
			break;
		}
		case BXOR:
		{
			AST __t77 = _t;
			AST tmp44_AST_in = (AST)_t;
			match(_t,BXOR);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t77;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("^", left, right);
			break;
		}
		case BOR:
		{
			AST __t78 = _t;
			AST tmp45_AST_in = (AST)_t;
			match(_t,BOR);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t78;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("|", left, right);
			break;
		}
		case BAND:
		{
			AST __t79 = _t;
			AST tmp46_AST_in = (AST)_t;
			match(_t,BAND);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t79;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("&", left, right);
			break;
		}
		case LEFT_SHIFT:
		{
			AST __t80 = _t;
			AST tmp47_AST_in = (AST)_t;
			match(_t,LEFT_SHIFT);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t80;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("<<", left, right);
			break;
		}
		case RIGHT_SHIFT:
		{
			AST __t81 = _t;
			AST tmp48_AST_in = (AST)_t;
			match(_t,RIGHT_SHIFT);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t81;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression(">>", left, right);
			break;
		}
		case PLUS:
		{
			AST __t82 = _t;
			AST tmp49_AST_in = (AST)_t;
			match(_t,PLUS);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t82;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("+", left, right);
			break;
		}
		case MINUS:
		{
			AST __t83 = _t;
			AST tmp50_AST_in = (AST)_t;
			match(_t,MINUS);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t83;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("-", left, right);
			break;
		}
		case STAR:
		{
			AST __t84 = _t;
			AST tmp51_AST_in = (AST)_t;
			match(_t,STAR);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t84;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("*", left, right);
			break;
		}
		case DIV:
		{
			AST __t85 = _t;
			AST tmp52_AST_in = (AST)_t;
			match(_t,DIV);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t85;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("/", left, right);
			break;
		}
		case MOD:
		{
			AST __t86 = _t;
			AST tmp53_AST_in = (AST)_t;
			match(_t,MOD);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t86;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("%", left, right);
			break;
		}
		case POWER:
		{
			AST __t87 = _t;
			AST tmp54_AST_in = (AST)_t;
			match(_t,POWER);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			right=expression(_t);
			_t = _retTree;
			_t = __t87;
			_t = _t.getNextSibling();
			e = new BinaryOperatorExpression("**", left, right);
			break;
		}
		case UNARY_PLUS:
		{
			AST __t88 = _t;
			AST tmp55_AST_in = (AST)_t;
			match(_t,UNARY_PLUS);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			_t = __t88;
			_t = _t.getNextSibling();
			e = new UnaryOperatorExpression("+@", left);
			break;
		}
		case UNARY_MINUS:
		{
			AST __t89 = _t;
			AST tmp56_AST_in = (AST)_t;
			match(_t,UNARY_MINUS);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			_t = __t89;
			_t = _t.getNextSibling();
			e = new UnaryOperatorExpression("-@", left);
			break;
		}
		case BNOT:
		{
			AST tmp57_AST_in = (AST)_t;
			match(_t,BNOT);
			_t = _t.getNextSibling();
			left=expression(_t);
			_t = _retTree;
			e = new UnaryOperatorExpression("~", left);
			break;
		}
		case NOT:
		{
			AST tmp58_AST_in = (AST)_t;
			match(_t,NOT);
			_t = _t.getNextSibling();
			left=expression(_t);
			_t = _retTree;
			e = new UnaryOperatorExpression("!", left);
			break;
		}
		case DOT:
		{
			AST __t90 = _t;
			AST tmp59_AST_in = (AST)_t;
			match(_t,DOT);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case CALL:
			{
				right=callExpression(_t);
				_t = _retTree;
				break;
			}
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case EMPTY_ARRAY:
			{
				method_name=methodCallName(_t);
				_t = _retTree;
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t90;
			_t = _t.getNextSibling();
			
													if (right != null) {
														MethodCallExpression mc = (MethodCallExpression)right;
														e = new MethodCallExpression(left, mc.getName(), mc.getArguments(), mc.getBlock());
													} else {
														e = new MethodCallExpression(left, method_name, null, null);
													}
												
			break;
		}
		case CALL:
		{
			e=callExpression(_t);
			_t = _retTree;
			break;
		}
		case LBRACK_ARRAY_ACCESS:
		{
			AST __t92 = _t;
			AST tmp60_AST_in = (AST)_t;
			match(_t,LBRACK_ARRAY_ACCESS);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			args=elements_as_arguments(_t);
			_t = _retTree;
			_t = __t92;
			_t = _t.getNextSibling();
			e = new MethodCallExpression(left, "[]", args, null);
			break;
		}
		case COLON2:
		{
			AST __t93 = _t;
			AST tmp61_AST_in = (AST)_t;
			match(_t,COLON2);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case CALL:
			{
				right=callExpression(_t);
				_t = _retTree;
				break;
			}
			case CONSTANT:
			{
				constant = (AST)_t;
				match(_t,CONSTANT);
				_t = _t.getNextSibling();
				break;
			}
			case FUNCTION:
			{
				function = (AST)_t;
				match(_t,FUNCTION);
				_t = _t.getNextSibling();
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t93;
			_t = _t.getNextSibling();
				if (null != right) {
														MethodCallExpression mc = (MethodCallExpression)right;
														e = new MethodCallExpression(left, mc.getName(), mc.getArguments(), mc.getBlock());
													} else if (null != constant) {
														e = new Colon2Expression(left, constant.getText());
													} else {
														e = new MethodCallExpression(left, function.getText(), null, null);
													}
												
			break;
		}
		case STRING:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_CONTENT:
		case HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION:
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
		{
			e=primaryExpression(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_def:
		{
			e=methodDefinition(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_module:
		{
			e=moduleDefinition(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_class:
		{
			e=classDefinition(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_if:
		{
			e=ifExpression(_t);
			_t = _retTree;
			break;
		}
		case WHILE_MODIFIER:
		case UNTIL_MODIFIER:
		case LITERAL_while:
		case LITERAL_until:
		{
			e=whileExpression(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_for:
		{
			e=forInExpression(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_unless:
		{
			e=unlessExpression(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_case:
		{
			e=caseExpression(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_begin:
		{
			e=exceptionHandlingExpression(_t);
			_t = _retTree;
			break;
		}
		case EMPTY_ARRAY:
		case LBRACK:
		{
			e=arrayExpression(_t);
			_t = _retTree;
			break;
		}
		case LCURLY_HASH:
		{
			e=hashExpression(_t);
			_t = _retTree;
			break;
		}
		case SYMBOL:
		{
			e=symbol(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_return:
		{
			AST __t95 = _t;
			AST tmp62_AST_in = (AST)_t;
			match(_t,LITERAL_return);
			_t = _t.getFirstChild();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case ARG:
			{
				return_args=return_arguments(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t95;
			_t = _t.getNextSibling();
			e = new ReturnExpression(return_args);
			break;
		}
		case LITERAL_break:
		{
			AST __t97 = _t;
			AST tmp63_AST_in = (AST)_t;
			match(_t,LITERAL_break);
			_t = _t.getFirstChild();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case ARG:
			{
				return_args=return_arguments(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t97;
			_t = _t.getNextSibling();
			e = new BreakExpression(return_args);
			break;
		}
		case LITERAL_next:
		{
			AST __t99 = _t;
			AST tmp64_AST_in = (AST)_t;
			match(_t,LITERAL_next);
			_t = _t.getFirstChild();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case ARG:
			{
				return_args=return_arguments(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t99;
			_t = _t.getNextSibling();
			e = new NextExpression(return_args);
			break;
		}
		case LPAREN:
		{
			AST __t101 = _t;
			AST tmp65_AST_in = (AST)_t;
			match(_t,LPAREN);
			_t = _t.getFirstChild();
			cs=compoundStatement(_t);
			_t = _retTree;
			_t = __t101;
			_t = _t.getNextSibling();
			e = new ParenthesisExpression(cs);
			break;
		}
		case LPAREN_WITH_NO_LEADING_SPACE:
		{
			AST __t102 = _t;
			AST tmp66_AST_in = (AST)_t;
			match(_t,LPAREN_WITH_NO_LEADING_SPACE);
			_t = _t.getFirstChild();
			cs=compoundStatement(_t);
			_t = _retTree;
			_t = __t102;
			_t = _t.getNextSibling();
			e = new ParenthesisExpression(cs);
			break;
		}
		case LITERAL_redo:
		{
			AST tmp67_AST_in = (AST)_t;
			match(_t,LITERAL_redo);
			_t = _t.getNextSibling();
			e = new RedoExpression();
			break;
		}
		case LITERAL_retry:
		{
			AST tmp68_AST_in = (AST)_t;
			match(_t,LITERAL_retry);
			_t = _t.getNextSibling();
			e = new RetryExpression();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		
		e.setNewLine(startANewLine);
		if(e.getPosition() < 1) {
				        e.setPosition(currentLineNumber);
				    } // To fix the linenumber problem for forInExpression
				
		_retTree = _t;
		return e;
	}
	
	public final String  aliasParameter(AST _t) throws RecognitionException {
		String s;
		
		AST aliasParameter_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST gvar = null;
		AST id = null;
		AST assign1 = null;
		AST function = null;
		AST assign2 = null;
		AST constant = null;
		AST assign3 = null;
		SymbolExpression sym = null;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case GLOBAL_VARIABLE:
		{
			gvar = (AST)_t;
			match(_t,GLOBAL_VARIABLE);
			_t = _t.getNextSibling();
			s = gvar.getText();
			break;
		}
		case IDENTIFIER:
		{
			id = (AST)_t;
			match(_t,IDENTIFIER);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				assign1 = (AST)_t;
				match(_t,ASSIGN_WITH_NO_LEADING_SPACE);
				_t = _t.getNextSibling();
				break;
			}
			case 3:
			case SYMBOL:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case LITERAL_do:
			case LITERAL_end:
			case BOR:
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
				throw new NoViableAltException(_t);
			}
			}
			}
			s = id.getText(); if (null != assign1) {s += "=";}
			break;
		}
		case FUNCTION:
		{
			function = (AST)_t;
			match(_t,FUNCTION);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				assign2 = (AST)_t;
				match(_t,ASSIGN_WITH_NO_LEADING_SPACE);
				_t = _t.getNextSibling();
				break;
			}
			case 3:
			case SYMBOL:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case LITERAL_do:
			case LITERAL_end:
			case BOR:
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
				throw new NoViableAltException(_t);
			}
			}
			}
			s = function.getText(); if (null != assign2) {s += "=";}
			break;
		}
		case CONSTANT:
		{
			constant = (AST)_t;
			match(_t,CONSTANT);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				assign3 = (AST)_t;
				match(_t,ASSIGN_WITH_NO_LEADING_SPACE);
				_t = _t.getNextSibling();
				break;
			}
			case 3:
			case SYMBOL:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case LITERAL_do:
			case LITERAL_end:
			case BOR:
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
				throw new NoViableAltException(_t);
			}
			}
			}
			s = constant.getText(); if (null != assign3) {s += "=";}
			break;
		}
		case SYMBOL:
		{
			sym=symbol(_t);
			_t = _retTree;
			s = sym.getValue();
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
			s=operator(_t);
			_t = _retTree;
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
			s=keyword(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return s;
	}
	
	public final SymbolExpression  symbol(AST _t) throws RecognitionException {
		SymbolExpression e;
		
		AST symbol_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST id = null;
		AST f = null;
		AST constant = null;
		AST g = null;
		AST i = null;
		AST c = null;
		AST u = null;
		AST ds = null;
		AST ss = null;
		
		String s = null;
		StringExpressionWithExpressionSubstitution swes = null;
		
		
		AST __t165 = _t;
		AST tmp69_AST_in = (AST)_t;
		match(_t,SYMBOL);
		_t = _t.getFirstChild();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case IDENTIFIER:
		{
			id = (AST)_t;
			match(_t,IDENTIFIER);
			_t = _t.getNextSibling();
			e= new SymbolExpression(id.getText());
			break;
		}
		case FUNCTION:
		{
			f = (AST)_t;
			match(_t,FUNCTION);
			_t = _t.getNextSibling();
			e= new SymbolExpression(f.getText());
			break;
		}
		case CONSTANT:
		{
			constant = (AST)_t;
			match(_t,CONSTANT);
			_t = _t.getNextSibling();
			e= new SymbolExpression(constant.getText());
			break;
		}
		case GLOBAL_VARIABLE:
		{
			g = (AST)_t;
			match(_t,GLOBAL_VARIABLE);
			_t = _t.getNextSibling();
			e= new SymbolExpression(g.getText());
			break;
		}
		case INSTANCE_VARIABLE:
		{
			i = (AST)_t;
			match(_t,INSTANCE_VARIABLE);
			_t = _t.getNextSibling();
			e= new SymbolExpression(i.getText());
			break;
		}
		case CLASS_VARIABLE:
		{
			c = (AST)_t;
			match(_t,CLASS_VARIABLE);
			_t = _t.getNextSibling();
			e= new SymbolExpression(c.getText());
			break;
		}
		case UNARY_PLUS_MINUS_METHOD_NAME:
		{
			u = (AST)_t;
			match(_t,UNARY_PLUS_MINUS_METHOD_NAME);
			_t = _t.getNextSibling();
			e= new SymbolExpression(u.getText());
			break;
		}
		case DOUBLE_QUOTE_STRING:
		{
			ds = (AST)_t;
			match(_t,DOUBLE_QUOTE_STRING);
			_t = _t.getNextSibling();
			e= new SymbolExpression(ds.getText());
			break;
		}
		case SINGLE_QUOTE_STRING:
		{
			ss = (AST)_t;
			match(_t,SINGLE_QUOTE_STRING);
			_t = _t.getNextSibling();
			e= new SymbolExpression(ss.getText());
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
			s=keyword(_t);
			_t = _retTree;
			e = new SymbolExpression(s);
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
			s=operator(_t);
			_t = _retTree;
			e = new SymbolExpression(s);
			break;
		}
		case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			swes=stringWithExpressionSubstituation(_t);
			_t = _retTree;
			e = new SymbolExpression(swes);
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t165;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	public final String  operator(AST _t) throws RecognitionException {
		String s;
		
		AST operator_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LEFT_SHIFT:
		{
			AST tmp70_AST_in = (AST)_t;
			match(_t,LEFT_SHIFT);
			_t = _t.getNextSibling();
			s = "<<";
			break;
		}
		case RIGHT_SHIFT:
		{
			AST tmp71_AST_in = (AST)_t;
			match(_t,RIGHT_SHIFT);
			_t = _t.getNextSibling();
			s = ">>";
			break;
		}
		case EQUAL:
		{
			AST tmp72_AST_in = (AST)_t;
			match(_t,EQUAL);
			_t = _t.getNextSibling();
			s = "==";
			break;
		}
		case CASE_EQUAL:
		{
			AST tmp73_AST_in = (AST)_t;
			match(_t,CASE_EQUAL);
			_t = _t.getNextSibling();
			s = "===";
			break;
		}
		case GREATER_THAN:
		{
			AST tmp74_AST_in = (AST)_t;
			match(_t,GREATER_THAN);
			_t = _t.getNextSibling();
			s = ">";
			break;
		}
		case GREATER_OR_EQUAL:
		{
			AST tmp75_AST_in = (AST)_t;
			match(_t,GREATER_OR_EQUAL);
			_t = _t.getNextSibling();
			s = ">=";
			break;
		}
		case LESS_THAN:
		{
			AST tmp76_AST_in = (AST)_t;
			match(_t,LESS_THAN);
			_t = _t.getNextSibling();
			s = "<";
			break;
		}
		case LESS_OR_EQUAL:
		{
			AST tmp77_AST_in = (AST)_t;
			match(_t,LESS_OR_EQUAL);
			_t = _t.getNextSibling();
			s = "<=";
			break;
		}
		case PLUS:
		{
			AST tmp78_AST_in = (AST)_t;
			match(_t,PLUS);
			_t = _t.getNextSibling();
			s = "+";
			break;
		}
		case MINUS:
		{
			AST tmp79_AST_in = (AST)_t;
			match(_t,MINUS);
			_t = _t.getNextSibling();
			s = "-";
			break;
		}
		case STAR:
		{
			AST tmp80_AST_in = (AST)_t;
			match(_t,STAR);
			_t = _t.getNextSibling();
			s = "*";
			break;
		}
		case DIV:
		{
			AST tmp81_AST_in = (AST)_t;
			match(_t,DIV);
			_t = _t.getNextSibling();
			s = "/";
			break;
		}
		case MOD:
		{
			AST tmp82_AST_in = (AST)_t;
			match(_t,MOD);
			_t = _t.getNextSibling();
			s = "%";
			break;
		}
		case POWER:
		{
			AST tmp83_AST_in = (AST)_t;
			match(_t,POWER);
			_t = _t.getNextSibling();
			s = "**";
			break;
		}
		case BAND:
		{
			AST tmp84_AST_in = (AST)_t;
			match(_t,BAND);
			_t = _t.getNextSibling();
			s = "&";
			break;
		}
		case BOR:
		{
			AST tmp85_AST_in = (AST)_t;
			match(_t,BOR);
			_t = _t.getNextSibling();
			s = "|";
			break;
		}
		case BXOR:
		{
			AST tmp86_AST_in = (AST)_t;
			match(_t,BXOR);
			_t = _t.getNextSibling();
			s = "^";
			break;
		}
		case EMPTY_ARRAY_ACCESS:
		case EMPTY_ARRAY:
		{
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case EMPTY_ARRAY:
			{
				AST tmp87_AST_in = (AST)_t;
				match(_t,EMPTY_ARRAY);
				_t = _t.getNextSibling();
				break;
			}
			case EMPTY_ARRAY_ACCESS:
			{
				AST tmp88_AST_in = (AST)_t;
				match(_t,EMPTY_ARRAY_ACCESS);
				_t = _t.getNextSibling();
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			s = "[]";
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				AST tmp89_AST_in = (AST)_t;
				match(_t,ASSIGN_WITH_NO_LEADING_SPACE);
				_t = _t.getNextSibling();
				s = "[]=";
				break;
			}
			case 3:
			case BODY:
			case SYMBOL:
			case REST_ARG_PREFIX:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case LITERAL_undef:
			case LITERAL_alias:
			case GLOBAL_VARIABLE:
			case LITERAL_do:
			case LITERAL_end:
			case BOR:
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
			case BLOCK_ARG_PREFIX:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
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
				throw new NoViableAltException(_t);
			}
			}
			}
			break;
		}
		case MATCH:
		{
			AST tmp90_AST_in = (AST)_t;
			match(_t,MATCH);
			_t = _t.getNextSibling();
			s = "=~";
			break;
		}
		case COMPARE:
		{
			AST tmp91_AST_in = (AST)_t;
			match(_t,COMPARE);
			_t = _t.getNextSibling();
			s = "<=>";
			break;
		}
		case BNOT:
		{
			AST tmp92_AST_in = (AST)_t;
			match(_t,BNOT);
			_t = _t.getNextSibling();
			s = "~";
			break;
		}
		case SINGLE_QUOTE:
		{
			AST tmp93_AST_in = (AST)_t;
			match(_t,SINGLE_QUOTE);
			_t = _t.getNextSibling();
			s = "`";
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return s;
	}
	
	public final String  keyword(AST _t) throws RecognitionException {
		String s;
		
		AST keyword_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LITERAL_and:
		{
			AST tmp94_AST_in = (AST)_t;
			match(_t,LITERAL_and);
			_t = _t.getNextSibling();
			s = "and";
			break;
		}
		case LITERAL_def:
		{
			AST tmp95_AST_in = (AST)_t;
			match(_t,LITERAL_def);
			_t = _t.getNextSibling();
			s = "def";
			break;
		}
		case LITERAL_end:
		{
			AST tmp96_AST_in = (AST)_t;
			match(_t,LITERAL_end);
			_t = _t.getNextSibling();
			s = "end";
			break;
		}
		case LITERAL_in:
		{
			AST tmp97_AST_in = (AST)_t;
			match(_t,LITERAL_in);
			_t = _t.getNextSibling();
			s = "in";
			break;
		}
		case LITERAL_or:
		{
			AST tmp98_AST_in = (AST)_t;
			match(_t,LITERAL_or);
			_t = _t.getNextSibling();
			s = "or";
			break;
		}
		case LITERAL_unless:
		{
			AST tmp99_AST_in = (AST)_t;
			match(_t,LITERAL_unless);
			_t = _t.getNextSibling();
			s = "unless";
			break;
		}
		case LITERAL_begin:
		{
			AST tmp100_AST_in = (AST)_t;
			match(_t,LITERAL_begin);
			_t = _t.getNextSibling();
			s = "begin";
			break;
		}
		case 155:
		{
			AST tmp101_AST_in = (AST)_t;
			match(_t,155);
			_t = _t.getNextSibling();
			s = "defined?";
			break;
		}
		case LITERAL_ensure:
		{
			AST tmp102_AST_in = (AST)_t;
			match(_t,LITERAL_ensure);
			_t = _t.getNextSibling();
			s = "ensure";
			break;
		}
		case LITERAL_module:
		{
			AST tmp103_AST_in = (AST)_t;
			match(_t,LITERAL_module);
			_t = _t.getNextSibling();
			s = "module";
			break;
		}
		case LITERAL_redo:
		{
			AST tmp104_AST_in = (AST)_t;
			match(_t,LITERAL_redo);
			_t = _t.getNextSibling();
			s = "redo";
			break;
		}
		case LITERAL_super:
		{
			AST tmp105_AST_in = (AST)_t;
			match(_t,LITERAL_super);
			_t = _t.getNextSibling();
			s = "super";
			break;
		}
		case LITERAL_until:
		{
			AST tmp106_AST_in = (AST)_t;
			match(_t,LITERAL_until);
			_t = _t.getNextSibling();
			s = "until";
			break;
		}
		case LITERAL_BEGIN:
		{
			AST tmp107_AST_in = (AST)_t;
			match(_t,LITERAL_BEGIN);
			_t = _t.getNextSibling();
			s = "BEGIN";
			break;
		}
		case LITERAL_break:
		{
			AST tmp108_AST_in = (AST)_t;
			match(_t,LITERAL_break);
			_t = _t.getNextSibling();
			s = "break";
			break;
		}
		case LITERAL_do:
		{
			AST tmp109_AST_in = (AST)_t;
			match(_t,LITERAL_do);
			_t = _t.getNextSibling();
			s = "do";
			break;
		}
		case LITERAL_next:
		{
			AST tmp110_AST_in = (AST)_t;
			match(_t,LITERAL_next);
			_t = _t.getNextSibling();
			s = "next";
			break;
		}
		case LITERAL_rescue:
		{
			AST tmp111_AST_in = (AST)_t;
			match(_t,LITERAL_rescue);
			_t = _t.getNextSibling();
			s = "rescue";
			break;
		}
		case LITERAL_then:
		{
			AST tmp112_AST_in = (AST)_t;
			match(_t,LITERAL_then);
			_t = _t.getNextSibling();
			s = "then";
			break;
		}
		case LITERAL_when:
		{
			AST tmp113_AST_in = (AST)_t;
			match(_t,LITERAL_when);
			_t = _t.getNextSibling();
			s = "when";
			break;
		}
		case LITERAL_END:
		{
			AST tmp114_AST_in = (AST)_t;
			match(_t,LITERAL_END);
			_t = _t.getNextSibling();
			s = "END";
			break;
		}
		case LITERAL_case:
		{
			AST tmp115_AST_in = (AST)_t;
			match(_t,LITERAL_case);
			_t = _t.getNextSibling();
			s = "case";
			break;
		}
		case LITERAL_else:
		{
			AST tmp116_AST_in = (AST)_t;
			match(_t,LITERAL_else);
			_t = _t.getNextSibling();
			s = "else";
			break;
		}
		case LITERAL_for:
		{
			AST tmp117_AST_in = (AST)_t;
			match(_t,LITERAL_for);
			_t = _t.getNextSibling();
			s = "for";
			break;
		}
		case LITERAL_retry:
		{
			AST tmp118_AST_in = (AST)_t;
			match(_t,LITERAL_retry);
			_t = _t.getNextSibling();
			s = "retry";
			break;
		}
		case LITERAL_while:
		{
			AST tmp119_AST_in = (AST)_t;
			match(_t,LITERAL_while);
			_t = _t.getNextSibling();
			s = "while";
			break;
		}
		case LITERAL_alias:
		{
			AST tmp120_AST_in = (AST)_t;
			match(_t,LITERAL_alias);
			_t = _t.getNextSibling();
			s = "alias";
			break;
		}
		case LITERAL_class:
		{
			AST tmp121_AST_in = (AST)_t;
			match(_t,LITERAL_class);
			_t = _t.getNextSibling();
			s = "class";
			break;
		}
		case LITERAL_elsif:
		{
			AST tmp122_AST_in = (AST)_t;
			match(_t,LITERAL_elsif);
			_t = _t.getNextSibling();
			s = "elsif";
			break;
		}
		case LITERAL_if:
		{
			AST tmp123_AST_in = (AST)_t;
			match(_t,LITERAL_if);
			_t = _t.getNextSibling();
			s = "if";
			break;
		}
		case LITERAL_not:
		{
			AST tmp124_AST_in = (AST)_t;
			match(_t,LITERAL_not);
			_t = _t.getNextSibling();
			s = "not";
			break;
		}
		case LITERAL_return:
		{
			AST tmp125_AST_in = (AST)_t;
			match(_t,LITERAL_return);
			_t = _t.getNextSibling();
			s = "return";
			break;
		}
		case LITERAL_undef:
		{
			AST tmp126_AST_in = (AST)_t;
			match(_t,LITERAL_undef);
			_t = _t.getNextSibling();
			s = "undef";
			break;
		}
		case LITERAL_yield:
		{
			AST tmp127_AST_in = (AST)_t;
			match(_t,LITERAL_yield);
			_t = _t.getNextSibling();
			s = "yield";
			break;
		}
		case LITERAL_true:
		{
			AST tmp128_AST_in = (AST)_t;
			match(_t,LITERAL_true);
			_t = _t.getNextSibling();
			s = "true";
			break;
		}
		case LITERAL_false:
		{
			AST tmp129_AST_in = (AST)_t;
			match(_t,LITERAL_false);
			_t = _t.getNextSibling();
			s = "false";
			break;
		}
		case LITERAL_self:
		{
			AST tmp130_AST_in = (AST)_t;
			match(_t,LITERAL_self);
			_t = _t.getNextSibling();
			s = "self";
			break;
		}
		case LITERAL_nil:
		{
			AST tmp131_AST_in = (AST)_t;
			match(_t,LITERAL_nil);
			_t = _t.getNextSibling();
			s = "nil";
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return s;
	}
	
	public final String  undefParameter(AST _t) throws RecognitionException {
		String s;
		
		AST undefParameter_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST func = null;
		AST id = null;
		SymbolExpression sym = null;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case FUNCTION:
		{
			func = (AST)_t;
			match(_t,FUNCTION);
			_t = _t.getNextSibling();
			s = func.getText();
			break;
		}
		case IDENTIFIER:
		{
			id = (AST)_t;
			match(_t,IDENTIFIER);
			_t = _t.getNextSibling();
			s = id.getText();
			break;
		}
		case SYMBOL:
		{
			sym=symbol(_t);
			_t = _retTree;
			s = sym.getValue();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return s;
	}
	
	protected final void multipleAssignmentBody(AST _t,
		MultipleAssignmentStatement s
	) throws RecognitionException {
		
		AST multipleAssignmentBody_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression e = null;
		
		
		{
		_loop29:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_tokenSet_1.member(_t.getType()))) {
				e=lhs(_t);
				_t = _retTree;
				s.addLhs(e);e=null;
			}
			else {
				break _loop29;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case REST_ARG_PREFIX:
		{
			AST tmp132_AST_in = (AST)_t;
			match(_t,REST_ARG_PREFIX);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case CALL:
			case SYMBOL:
			case STRING:
			case WHILE_MODIFIER:
			case UNTIL_MODIFIER:
			case ASSIGN:
			case LPAREN_WITH_NO_LEADING_SPACE:
			case LPAREN:
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case GLOBAL_VARIABLE:
			case BOR:
			case LOGICAL_OR:
			case INSTANCE_VARIABLE:
			case CLASS_VARIABLE:
			case LITERAL_and:
			case LITERAL_or:
			case LITERAL_not:
			case QUESTION:
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
			case INCLUSIVE_RANGE:
			case EXCLUSIVE_RANGE:
			case LOGICAL_AND:
			case COMPARE:
			case EQUAL:
			case CASE_EQUAL:
			case NOT_EQUAL:
			case MATCH:
			case NOT_MATCH:
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
			case NOT:
			case LITERAL_return:
			case LITERAL_break:
			case LITERAL_next:
			case DOT:
			case COLON2:
			case LBRACK_ARRAY_ACCESS:
			case UNARY_PLUS:
			case UNARY_MINUS:
			case LITERAL_nil:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL___FILE__:
			case LITERAL___LINE__:
			case REGEX:
			case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
			case COMMAND_OUTPUT:
			case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
			case HERE_DOC_CONTENT:
			case HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION:
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
			{
				e=expression(_t);
				_t = _retTree;
				break;
			}
			case 3:
			case MRHS:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			s.setAsteriskLhs(e);
			break;
		}
		case 3:
		case MRHS:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case MRHS:
		{
			AST __t33 = _t;
			AST tmp133_AST_in = (AST)_t;
			match(_t,MRHS);
			_t = _t.getFirstChild();
			{
			_loop35:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_tokenSet_2.member(_t.getType()))) {
					e=expression(_t);
					_t = _retTree;
					s.addRhs(e);
				}
				else {
					break _loop35;
				}
				
			} while (true);
			}
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case REST_ARG_PREFIX:
			{
				AST tmp134_AST_in = (AST)_t;
				match(_t,REST_ARG_PREFIX);
				_t = _t.getNextSibling();
				e=expression(_t);
				_t = _retTree;
				s.setAsteriskRhs(e);
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t33;
			_t = _t.getNextSibling();
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_retTree = _t;
	}
	
	public final Expression  lhs(AST _t) throws RecognitionException {
		Expression e;
		
		AST lhs_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case CALL:
		case SYMBOL:
		case STRING:
		case WHILE_MODIFIER:
		case UNTIL_MODIFIER:
		case ASSIGN:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case BOR:
		case LOGICAL_OR:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case QUESTION:
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
		case INCLUSIVE_RANGE:
		case EXCLUSIVE_RANGE:
		case LOGICAL_AND:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case NOT_EQUAL:
		case MATCH:
		case NOT_MATCH:
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
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case DOT:
		case COLON2:
		case LBRACK_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_CONTENT:
		case HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION:
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
		{
			e=expression(_t);
			_t = _retTree;
			break;
		}
		case NESTED_LHS:
		{
			e=nestedLhs(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return e;
	}
	
	public final NestedVariableExpression  nestedLhs(AST _t) throws RecognitionException {
		NestedVariableExpression e;
		
		AST nestedLhs_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression exp = null;
		
		
		AST __t39 = _t;
		AST tmp135_AST_in = (AST)_t;
		match(_t,NESTED_LHS);
		_t = _t.getFirstChild();
		e = new NestedVariableExpression();
		{
		_loop41:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_tokenSet_2.member(_t.getType()))) {
				exp=expression(_t);
				_t = _retTree;
				e.addLhs(exp);
			}
			else {
				break _loop41;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case REST_ARG_PREFIX:
		{
			AST tmp136_AST_in = (AST)_t;
			match(_t,REST_ARG_PREFIX);
			_t = _t.getNextSibling();
			exp=expression(_t);
			_t = _retTree;
			e.setAsteriskLhs(exp);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t39;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	public final Expression  callExpression(AST _t) throws RecognitionException {
		Expression e;
		
		AST callExpression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST yield = null;
		AST sup = null;
		AST defined = null;
		
			String method_name = null;
			String method_name2 = null;
			MethodCallArguments args = null;
			Block block = null;
			Expression left = null;
			Expression right = null;
		
		
		AST __t104 = _t;
		AST tmp137_AST_in = (AST)_t;
		match(_t,CALL);
		_t = _t.getFirstChild();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case UNARY_PLUS_MINUS_METHOD_NAME:
		case EMPTY_ARRAY:
		{
			method_name=methodCallName(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_yield:
		{
			yield = (AST)_t;
			match(_t,LITERAL_yield);
			_t = _t.getNextSibling();
			break;
		}
		case LITERAL_super:
		{
			sup = (AST)_t;
			match(_t,LITERAL_super);
			_t = _t.getNextSibling();
			break;
		}
		case 155:
		{
			defined = (AST)_t;
			match(_t,155);
			_t = _t.getNextSibling();
			break;
		}
		case DOT:
		{
			AST __t106 = _t;
			AST tmp138_AST_in = (AST)_t;
			match(_t,DOT);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case CALL:
			{
				right=callExpression(_t);
				_t = _retTree;
				break;
			}
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case EMPTY_ARRAY:
			{
				method_name2=methodCallName(_t);
				_t = _retTree;
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t106;
			_t = _t.getNextSibling();
			break;
		}
		case COLON2:
		{
			AST __t108 = _t;
			AST tmp139_AST_in = (AST)_t;
			match(_t,COLON2);
			_t = _t.getFirstChild();
			left=expression(_t);
			_t = _retTree;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case CALL:
			{
				right=callExpression(_t);
				_t = _retTree;
				break;
			}
			case IDENTIFIER:
			case CONSTANT:
			case FUNCTION:
			case UNARY_PLUS_MINUS_METHOD_NAME:
			case EMPTY_ARRAY:
			{
				method_name2=methodCallName(_t);
				_t = _retTree;
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t108;
			_t = _t.getNextSibling();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case ARG:
		{
			args=arguments(_t);
			_t = _retTree;
			break;
		}
		case 3:
		case BLOCK:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case BLOCK:
		{
			block=codeBlock(_t);
			_t = _retTree;
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t104;
		_t = _t.getNextSibling();
		
						if (null != yield) {
							if (null != block) {
								throw new RecognitionException("block can not be passed into yield");
							}
							e = new YieldExpression(args);
						} else if (null != sup) {
							e = new SuperExpression(args, block);
						} else if (null != defined) {
							if (null != block) {
								throw new RecognitionException("block can not be passed into defined?");
							}
							e = new DefinedExpression(args);
						} else if (null != method_name) {
							e = new MethodCallExpression(null, method_name, args, block);
						} else if (right != null) {
							MethodCallExpression mc = (MethodCallExpression)right;
							e = new MethodCallExpression(left, mc.getName(), mc.getArguments(), mc.getBlock());
						} else {
							e = new MethodCallExpression(left, method_name2, args, block);
						}
					
		_retTree = _t;
		return e;
	}
	
	public final String  methodCallName(AST _t) throws RecognitionException {
		String s;
		
		AST methodCallName_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST constant = null;
		AST id = null;
		AST function = null;
		AST unary = null;
		AST empty_array = null;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case CONSTANT:
		{
			constant = (AST)_t;
			match(_t,CONSTANT);
			_t = _t.getNextSibling();
			s = constant.getText();
			break;
		}
		case IDENTIFIER:
		{
			id = (AST)_t;
			match(_t,IDENTIFIER);
			_t = _t.getNextSibling();
			s = id.getText();
			break;
		}
		case FUNCTION:
		{
			function = (AST)_t;
			match(_t,FUNCTION);
			_t = _t.getNextSibling();
			s = function.getText();
			break;
		}
		case UNARY_PLUS_MINUS_METHOD_NAME:
		{
			unary = (AST)_t;
			match(_t,UNARY_PLUS_MINUS_METHOD_NAME);
			_t = _t.getNextSibling();
			s = unary.getText();
			break;
		}
		case EMPTY_ARRAY:
		{
			empty_array = (AST)_t;
			match(_t,EMPTY_ARRAY);
			_t = _t.getNextSibling();
			s = empty_array.getText();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return s;
	}
	
	public final MethodCallArguments  elements_as_arguments(AST _t) throws RecognitionException {
		MethodCallArguments args;
		
		AST elements_as_arguments_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			args = new MethodCallArguments();
			Expression e = null;
		
		
		{
		_loop131:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_tokenSet_2.member(_t.getType()))) {
				e=expression(_t);
				_t = _retTree;
				args.addArgument(e);
			}
			else {
				break _loop131;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case REST_ARG_PREFIX:
		{
			AST tmp140_AST_in = (AST)_t;
			match(_t,REST_ARG_PREFIX);
			_t = _t.getNextSibling();
			e=expression(_t);
			_t = _retTree;
			args.setAsteriskArgument(e);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_retTree = _t;
		return args;
	}
	
	public final Expression  primaryExpression(AST _t) throws RecognitionException {
		Expression e;
		
		AST primaryExpression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST integer = null;
		AST hex = null;
		AST binary = null;
		AST octal = null;
		AST float_ = null;
		AST ascii = null;
		AST command_output = null;
		AST heredoc = null;
		AST regex = null;
		AST warray = null;
		AST constant = null;
		AST constant2 = null;
		AST gvar = null;
		AST class_variable = null;
		AST instance_variable = null;
		AST line = null;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case INTEGER:
		{
			integer = (AST)_t;
			match(_t,INTEGER);
			_t = _t.getNextSibling();
			e = new IntegerExpression(integer.getText(), 10);
			break;
		}
		case HEX:
		{
			hex = (AST)_t;
			match(_t,HEX);
			_t = _t.getNextSibling();
			e = new IntegerExpression(hex.getText(), 16);
			break;
		}
		case BINARY:
		{
			binary = (AST)_t;
			match(_t,BINARY);
			_t = _t.getNextSibling();
			e = new IntegerExpression(binary.getText(), 2);
			break;
		}
		case OCTAL:
		{
			octal = (AST)_t;
			match(_t,OCTAL);
			_t = _t.getNextSibling();
			e = new IntegerExpression(octal.getText(), 8);
			break;
		}
		case FLOAT:
		{
			float_ = (AST)_t;
			match(_t,FLOAT);
			_t = _t.getNextSibling();
			e = new FloatExpression(float_.getText());
			break;
		}
		case ASCII_VALUE:
		{
			ascii = (AST)_t;
			match(_t,ASCII_VALUE);
			_t = _t.getNextSibling();
			e = new AsciiValueExpression(ascii.getText());
			break;
		}
		case COMMAND_OUTPUT:
		{
			command_output = (AST)_t;
			match(_t,COMMAND_OUTPUT);
			_t = _t.getNextSibling();
			e = new CommandOutputExpression(command_output.getText());
			break;
		}
		case HERE_DOC_CONTENT:
		{
			heredoc = (AST)_t;
			match(_t,HERE_DOC_CONTENT);
			_t = _t.getNextSibling();
			e = new StringExpression(heredoc.getText(), true);
			break;
		}
		case REGEX:
		{
			regex = (AST)_t;
			match(_t,REGEX);
			_t = _t.getNextSibling();
			e = new RegexpExpression(regex.getText());
			break;
		}
		case W_ARRAY:
		{
			warray = (AST)_t;
			match(_t,W_ARRAY);
			_t = _t.getNextSibling();
			e = new WArrayExpression(warray.getText());
			break;
		}
		case IDENTIFIER:
		case FUNCTION:
		{
			e=local_variable(_t);
			_t = _retTree;
			break;
		}
		case CONSTANT:
		{
			constant = (AST)_t;
			match(_t,CONSTANT);
			_t = _t.getNextSibling();
			e = new CurrentNamespaceConstantExpression(constant.getText());
			break;
		}
		case LEADING_COLON2:
		{
			AST tmp141_AST_in = (AST)_t;
			match(_t,LEADING_COLON2);
			_t = _t.getNextSibling();
			constant2 = (AST)_t;
			match(_t,CONSTANT);
			_t = _t.getNextSibling();
			e = new Colon2Expression(null, constant2.getText());
			break;
		}
		case GLOBAL_VARIABLE:
		{
			gvar = (AST)_t;
			match(_t,GLOBAL_VARIABLE);
			_t = _t.getNextSibling();
			e = new GlobalVariableExpression(gvar.getText());
			break;
		}
		case LITERAL_true:
		{
			AST tmp142_AST_in = (AST)_t;
			match(_t,LITERAL_true);
			_t = _t.getNextSibling();
			e = new TrueExpression();
			break;
		}
		case LITERAL_false:
		{
			AST tmp143_AST_in = (AST)_t;
			match(_t,LITERAL_false);
			_t = _t.getNextSibling();
			e = new FalseExpression();
			break;
		}
		case LITERAL_self:
		{
			AST tmp144_AST_in = (AST)_t;
			match(_t,LITERAL_self);
			_t = _t.getNextSibling();
			e = new SelfExpression();
			break;
		}
		case LITERAL_nil:
		{
			AST tmp145_AST_in = (AST)_t;
			match(_t,LITERAL_nil);
			_t = _t.getNextSibling();
			e = new NilExpression();
			break;
		}
		case LITERAL_super:
		{
			AST tmp146_AST_in = (AST)_t;
			match(_t,LITERAL_super);
			_t = _t.getNextSibling();
			e = new SuperExpression(null, null);
			break;
		}
		case CLASS_VARIABLE:
		{
			class_variable = (AST)_t;
			match(_t,CLASS_VARIABLE);
			_t = _t.getNextSibling();
			e = new ClassVariableExpression(class_variable.getText());
			break;
		}
		case INSTANCE_VARIABLE:
		{
			instance_variable = (AST)_t;
			match(_t,INSTANCE_VARIABLE);
			_t = _t.getNextSibling();
			e = new InstanceVariableExpression(instance_variable.getText());
			break;
		}
		case LITERAL___FILE__:
		{
			AST tmp147_AST_in = (AST)_t;
			match(_t,LITERAL___FILE__);
			_t = _t.getNextSibling();
			e = new StringExpression((null != filename_) ? filename_ : "-", false);
			break;
		}
		case LITERAL___LINE__:
		{
			line = (AST)_t;
			match(_t,LITERAL___LINE__);
			_t = _t.getNextSibling();
			e = new IntegerExpression(line.getLine());
			break;
		}
		case STRING:
		{
			e=string(_t);
			_t = _retTree;
			break;
		}
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			e=regexWithExpressionSubstituation(_t);
			_t = _retTree;
			break;
		}
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			e=commandOutputWithExpressionSubstituation(_t);
			_t = _retTree;
			break;
		}
		case HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION:
		{
			e=heredocWithExpressionSubstituation(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return e;
	}
	
	public final MethodDefinitionExpression  methodDefinition(AST _t) throws RecognitionException {
		MethodDefinitionExpression e;
		
		AST methodDefinition_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST id = null;
		AST func = null;
		AST id2 = null;
		AST func2 = null;
		AST id3 = null;
		AST func3 = null;
		
			String name = null;
			BodyStatement body = null;
			Expression exp = null;
			int lineNumber = _t.getLine();
		
		
		AST __t233 = _t;
		AST tmp148_AST_in = (AST)_t;
		match(_t,LITERAL_def);
		_t = _t.getFirstChild();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LITERAL_undef:
		case LITERAL_alias:
		case LITERAL_do:
		case LITERAL_end:
		case BOR:
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
			name=methodName(_t);
			_t = _retTree;
			e = new MethodDefinitionExpression(name);
			break;
		}
		case SINGLETON_METHOD:
		{
			AST __t235 = _t;
			AST tmp149_AST_in = (AST)_t;
			match(_t,SINGLETON_METHOD);
			_t = _t.getFirstChild();
			exp=expression(_t);
			_t = _retTree;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case DOT:
			{
				AST tmp150_AST_in = (AST)_t;
				match(_t,DOT);
				_t = _t.getNextSibling();
				break;
			}
			case COLON2:
			{
				AST tmp151_AST_in = (AST)_t;
				match(_t,COLON2);
				_t = _t.getNextSibling();
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			name=methodName(_t);
			_t = _retTree;
			e = new MethodDefinitionExpression(name, exp);
			_t = __t235;
			_t = _t.getNextSibling();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		_loop241:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_t.getType()==IDENTIFIER||_t.getType()==FUNCTION)) {
				id=null;func=null;exp=null;
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case IDENTIFIER:
				{
					id = (AST)_t;
					match(_t,IDENTIFIER);
					_t = _t.getNextSibling();
					break;
				}
				case FUNCTION:
				{
					func = (AST)_t;
					match(_t,FUNCTION);
					_t = _t.getNextSibling();
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case ASSIGN:
				case ASSIGN_WITH_NO_LEADING_SPACE:
				{
					{
					if (_t==null) _t=ASTNULL;
					switch ( _t.getType()) {
					case ASSIGN:
					{
						AST tmp152_AST_in = (AST)_t;
						match(_t,ASSIGN);
						_t = _t.getNextSibling();
						break;
					}
					case ASSIGN_WITH_NO_LEADING_SPACE:
					{
						AST tmp153_AST_in = (AST)_t;
						match(_t,ASSIGN_WITH_NO_LEADING_SPACE);
						_t = _t.getNextSibling();
						break;
					}
					default:
					{
						throw new NoViableAltException(_t);
					}
					}
					}
					exp=expression(_t);
					_t = _retTree;
					break;
				}
				case 3:
				case BODY:
				case REST_ARG_PREFIX:
				case IDENTIFIER:
				case FUNCTION:
				case BLOCK_ARG_PREFIX:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				e.addParameter((null != id) ? id.getText() : func.getText(), exp);
			}
			else {
				break _loop241;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case REST_ARG_PREFIX:
		{
			AST tmp154_AST_in = (AST)_t;
			match(_t,REST_ARG_PREFIX);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case IDENTIFIER:
			{
				id2 = (AST)_t;
				match(_t,IDENTIFIER);
				_t = _t.getNextSibling();
				break;
			}
			case FUNCTION:
			{
				func2 = (AST)_t;
				match(_t,FUNCTION);
				_t = _t.getNextSibling();
				break;
			}
			case 3:
			case BODY:
			case BLOCK_ARG_PREFIX:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			e.setAsteriskParameter((null != id2) ? id2.getText() : ((null != func2) ? func2.getText() : null));
			break;
		}
		case 3:
		case BODY:
		case BLOCK_ARG_PREFIX:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case BLOCK_ARG_PREFIX:
		{
			AST tmp155_AST_in = (AST)_t;
			match(_t,BLOCK_ARG_PREFIX);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case IDENTIFIER:
			{
				id3 = (AST)_t;
				match(_t,IDENTIFIER);
				_t = _t.getNextSibling();
				break;
			}
			case FUNCTION:
			{
				func3 = (AST)_t;
				match(_t,FUNCTION);
				_t = _t.getNextSibling();
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			e.setBlockParameter((null != id3) ? id3.getText() : func3.getText());
			break;
		}
		case 3:
		case BODY:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case BODY:
		{
			body=bodyStatement(_t);
			_t = _retTree;
			e.setBody(body);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t233;
		_t = _t.getNextSibling();
		
					     e.setPosition(lineNumber);
					
		_retTree = _t;
		return e;
	}
	
	public final ModuleDefinitionExpression  moduleDefinition(AST _t) throws RecognitionException {
		ModuleDefinitionExpression e;
		
		AST moduleDefinition_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			String name = null;
			Expression scope = null;
			BodyStatement body = null;
		
		
		AST __t227 = _t;
		AST tmp156_AST_in = (AST)_t;
		match(_t,LITERAL_module);
		_t = _t.getFirstChild();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LEADING_COLON2:
		{
			AST tmp157_AST_in = (AST)_t;
			match(_t,LEADING_COLON2);
			_t = _t.getNextSibling();
			scope = new CurrentNamespaceConstantExpression("Object");
			break;
		}
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case COLON2:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		{
			name=moduleName(_t);
			_t = _retTree;
			break;
		}
		case COLON2:
		{
			AST __t230 = _t;
			AST tmp158_AST_in = (AST)_t;
			match(_t,COLON2);
			_t = _t.getFirstChild();
			scope=expression(_t);
			_t = _retTree;
			name=moduleName(_t);
			_t = _retTree;
			_t = __t230;
			_t = _t.getNextSibling();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case BODY:
		{
			body=bodyStatement(_t);
			_t = _retTree;
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t227;
		_t = _t.getNextSibling();
		
						e = new ModuleDefinitionExpression(scope, name, body);
					
		_retTree = _t;
		return e;
	}
	
	public final ClassDefinitionExpression  classDefinition(AST _t) throws RecognitionException {
		ClassDefinitionExpression e;
		
		AST classDefinition_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression scope = null;
			String name = null;
			Expression exp = null;
			Expression super_class = null;
			BodyStatement body = null;
		
		
		AST __t219 = _t;
		AST tmp159_AST_in = (AST)_t;
		match(_t,LITERAL_class);
		_t = _t.getFirstChild();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LEADING_COLON2:
		{
			AST tmp160_AST_in = (AST)_t;
			match(_t,LEADING_COLON2);
			_t = _t.getNextSibling();
			scope = new CurrentNamespaceConstantExpression("Object");
			break;
		}
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case LEFT_SHIFT:
		case COLON2:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		{
			name=className(_t);
			_t = _retTree;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case LESS_THAN:
			{
				AST tmp161_AST_in = (AST)_t;
				match(_t,LESS_THAN);
				_t = _t.getNextSibling();
				super_class=expression(_t);
				_t = _retTree;
				break;
			}
			case 3:
			case BODY:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			break;
		}
		case COLON2:
		{
			AST __t223 = _t;
			AST tmp162_AST_in = (AST)_t;
			match(_t,COLON2);
			_t = _t.getFirstChild();
			scope=expression(_t);
			_t = _retTree;
			name=className(_t);
			_t = _retTree;
			_t = __t223;
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case LESS_THAN:
			{
				AST tmp163_AST_in = (AST)_t;
				match(_t,LESS_THAN);
				_t = _t.getNextSibling();
				super_class=expression(_t);
				_t = _retTree;
				break;
			}
			case 3:
			case BODY:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			break;
		}
		case LEFT_SHIFT:
		{
			AST tmp164_AST_in = (AST)_t;
			match(_t,LEFT_SHIFT);
			_t = _t.getNextSibling();
			exp=expression(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case BODY:
		{
			body=bodyStatement(_t);
			_t = _retTree;
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t219;
		_t = _t.getNextSibling();
		
						if (name != null) {
							e = new ClassDefinitionExpression(scope, name, super_class, body);
						} else {
							e = new ClassDefinitionExpression(exp, super_class, body);
						}
					
		_retTree = _t;
		return e;
	}
	
	public final IfExpression  ifExpression(AST _t) throws RecognitionException {
		IfExpression e;
		
		AST ifExpression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression condition = null;
			CompoundStatement body = null;
		
		
		AST __t180 = _t;
		AST tmp165_AST_in = (AST)_t;
		match(_t,LITERAL_if);
		_t = _t.getFirstChild();
		condition=expression(_t);
		_t = _retTree;
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		{
			body=compoundStatement(_t);
			_t = _retTree;
			break;
		}
		case 3:
		case LITERAL_else:
		case LITERAL_elsif:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		e = new IfExpression(condition, body);condition=null;body=null;
		{
		_loop185:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_t.getType()==LITERAL_elsif)) {
				AST tmp166_AST_in = (AST)_t;
				match(_t,LITERAL_elsif);
				_t = _t.getNextSibling();
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case CALL:
				case SYMBOL:
				case STRING:
				case WHILE_MODIFIER:
				case UNTIL_MODIFIER:
				case ASSIGN:
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case BOR:
				case LOGICAL_OR:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case LITERAL_and:
				case LITERAL_or:
				case LITERAL_not:
				case QUESTION:
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
				case INCLUSIVE_RANGE:
				case EXCLUSIVE_RANGE:
				case LOGICAL_AND:
				case COMPARE:
				case EQUAL:
				case CASE_EQUAL:
				case NOT_EQUAL:
				case MATCH:
				case NOT_MATCH:
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
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case DOT:
				case COLON2:
				case LBRACK_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_CONTENT:
				case HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION:
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
				{
					condition=expression(_t);
					_t = _retTree;
					break;
				}
				case 3:
				case COMPSTMT:
				case LITERAL_else:
				case LITERAL_elsif:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case COMPSTMT:
				{
					body=compoundStatement(_t);
					_t = _retTree;
					break;
				}
				case 3:
				case LITERAL_else:
				case LITERAL_elsif:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				e.addElsif(condition, body);condition=null;body=null;
			}
			else {
				break _loop185;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LITERAL_else:
		{
			AST tmp167_AST_in = (AST)_t;
			match(_t,LITERAL_else);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case COMPSTMT:
			{
				body=compoundStatement(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			e.addElse(body);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t180;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	public final WhileExpression  whileExpression(AST _t) throws RecognitionException {
		WhileExpression e;
		
		AST whileExpression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression condition = null;
			CompoundStatement body = null;
		
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LITERAL_while:
		{
			AST __t189 = _t;
			AST tmp168_AST_in = (AST)_t;
			match(_t,LITERAL_while);
			_t = _t.getFirstChild();
			condition=expression(_t);
			_t = _retTree;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case COMPSTMT:
			{
				body=compoundStatement(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			e = new WhileExpression(condition, body, false, false);
			_t = __t189;
			_t = _t.getNextSibling();
			break;
		}
		case LITERAL_until:
		{
			AST __t191 = _t;
			AST tmp169_AST_in = (AST)_t;
			match(_t,LITERAL_until);
			_t = _t.getFirstChild();
			condition=expression(_t);
			_t = _retTree;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case COMPSTMT:
			{
				body=compoundStatement(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			e = new WhileExpression(condition, body, true, false);
			_t = __t191;
			_t = _t.getNextSibling();
			break;
		}
		case WHILE_MODIFIER:
		{
			AST __t193 = _t;
			AST tmp170_AST_in = (AST)_t;
			match(_t,WHILE_MODIFIER);
			_t = _t.getFirstChild();
			condition=expression(_t);
			_t = _retTree;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case COMPSTMT:
			{
				body=compoundStatement(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			e = new WhileExpression(condition, body, false, true);
			_t = __t193;
			_t = _t.getNextSibling();
			break;
		}
		case UNTIL_MODIFIER:
		{
			AST __t195 = _t;
			AST tmp171_AST_in = (AST)_t;
			match(_t,UNTIL_MODIFIER);
			_t = _t.getFirstChild();
			condition=expression(_t);
			_t = _retTree;
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case COMPSTMT:
			{
				body=compoundStatement(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			e = new WhileExpression(condition, body, true, true);
			_t = __t195;
			_t = _t.getNextSibling();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return e;
	}
	
	public final ForInExpression  forInExpression(AST _t) throws RecognitionException {
		ForInExpression e;
		
		AST forInExpression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
		// TODO: Here's a line number issue, after forInExpression is called. Line number is added to one
		// So, the final line number is the line next to it.
			Expression exp = null;
			Block b = null;
			CompoundStatement cs = null;
			ParameterVariableExpression var = null;
			int lineNumber = _t.getLine();
		
		
		AST __t248 = _t;
		AST tmp172_AST_in = (AST)_t;
		match(_t,LITERAL_for);
		_t = _t.getFirstChild();
		b = new Block();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case BLOCK_ARG:
		{
			AST __t250 = _t;
			AST tmp173_AST_in = (AST)_t;
			match(_t,BLOCK_ARG);
			_t = _t.getFirstChild();
			{
			int _cnt253=0;
			_loop253:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_t.getType()==NESTED_LHS||_t.getType()==IDENTIFIER||_t.getType()==FUNCTION)) {
					{
					if (_t==null) _t=ASTNULL;
					switch ( _t.getType()) {
					case IDENTIFIER:
					case FUNCTION:
					{
						var=local_variable(_t);
						_t = _retTree;
						break;
					}
					case NESTED_LHS:
					{
						var=nestedLhs(_t);
						_t = _retTree;
						break;
					}
					default:
					{
						throw new NoViableAltException(_t);
					}
					}
					}
					b.addParameter(var, null);
				}
				else {
					if ( _cnt253>=1 ) { break _loop253; } else {throw new NoViableAltException(_t);}
				}
				
				_cnt253++;
			} while (true);
			}
			_t = __t250;
			_t = _t.getNextSibling();
			break;
		}
		case BLOCK_ARG_WITH_EXTRA_COMMA:
		{
			AST __t254 = _t;
			AST tmp174_AST_in = (AST)_t;
			match(_t,BLOCK_ARG_WITH_EXTRA_COMMA);
			_t = _t.getFirstChild();
			{b.setHasExtraComma();}
			{
			int _cnt257=0;
			_loop257:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_t.getType()==NESTED_LHS||_t.getType()==IDENTIFIER||_t.getType()==FUNCTION)) {
					{
					if (_t==null) _t=ASTNULL;
					switch ( _t.getType()) {
					case IDENTIFIER:
					case FUNCTION:
					{
						var=local_variable(_t);
						_t = _retTree;
						break;
					}
					case NESTED_LHS:
					{
						var=nestedLhs(_t);
						_t = _retTree;
						break;
					}
					default:
					{
						throw new NoViableAltException(_t);
					}
					}
					}
					b.addParameter(var, null);
				}
				else {
					if ( _cnt257>=1 ) { break _loop257; } else {throw new NoViableAltException(_t);}
				}
				
				_cnt257++;
			} while (true);
			}
			_t = __t254;
			_t = _t.getNextSibling();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		AST tmp175_AST_in = (AST)_t;
		match(_t,LITERAL_in);
		_t = _t.getNextSibling();
		exp=expression(_t);
		_t = _retTree;
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		{
			cs=compoundStatement(_t);
			_t = _retTree;
			b.setBody(cs);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		e = new ForInExpression(exp, b);
		_t = __t248;
		_t = _t.getNextSibling();
		
					    e.setPosition(lineNumber);
					
		_retTree = _t;
		return e;
	}
	
	public final UnlessExpression  unlessExpression(AST _t) throws RecognitionException {
		UnlessExpression e;
		
		AST unlessExpression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression condition = null;
			CompoundStatement body = null;
			CompoundStatement else_body = null;
		
		
		AST __t198 = _t;
		AST tmp176_AST_in = (AST)_t;
		match(_t,LITERAL_unless);
		_t = _t.getFirstChild();
		condition=expression(_t);
		_t = _retTree;
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		{
			body=compoundStatement(_t);
			_t = _retTree;
			break;
		}
		case 3:
		case LITERAL_else:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LITERAL_else:
		{
			AST tmp177_AST_in = (AST)_t;
			match(_t,LITERAL_else);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case COMPSTMT:
			{
				else_body=compoundStatement(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		e = new UnlessExpression(condition, body, else_body);
		_t = __t198;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	public final CaseExpression  caseExpression(AST _t) throws RecognitionException {
		CaseExpression e;
		
		AST caseExpression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression condition = null;
			CompoundStatement body = null;
			When w = null;
		
		
		AST __t203 = _t;
		AST tmp178_AST_in = (AST)_t;
		match(_t,LITERAL_case);
		_t = _t.getFirstChild();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case CALL:
		case SYMBOL:
		case STRING:
		case WHILE_MODIFIER:
		case UNTIL_MODIFIER:
		case ASSIGN:
		case LPAREN_WITH_NO_LEADING_SPACE:
		case LPAREN:
		case IDENTIFIER:
		case CONSTANT:
		case FUNCTION:
		case GLOBAL_VARIABLE:
		case BOR:
		case LOGICAL_OR:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		case LITERAL_and:
		case LITERAL_or:
		case LITERAL_not:
		case QUESTION:
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
		case INCLUSIVE_RANGE:
		case EXCLUSIVE_RANGE:
		case LOGICAL_AND:
		case COMPARE:
		case EQUAL:
		case CASE_EQUAL:
		case NOT_EQUAL:
		case MATCH:
		case NOT_MATCH:
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
		case NOT:
		case LITERAL_return:
		case LITERAL_break:
		case LITERAL_next:
		case DOT:
		case COLON2:
		case LBRACK_ARRAY_ACCESS:
		case UNARY_PLUS:
		case UNARY_MINUS:
		case LITERAL_nil:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL___FILE__:
		case LITERAL___LINE__:
		case REGEX:
		case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
		case COMMAND_OUTPUT:
		case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_CONTENT:
		case HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION:
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
		{
			condition=expression(_t);
			_t = _retTree;
			break;
		}
		case 3:
		case LITERAL_else:
		case LITERAL_when:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		e=null;
		e = new CaseExpression(condition);
		{
		_loop206:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_t.getType()==LITERAL_when)) {
				w=when(_t);
				_t = _retTree;
				e.addWhen(w);
			}
			else {
				break _loop206;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LITERAL_else:
		{
			AST tmp179_AST_in = (AST)_t;
			match(_t,LITERAL_else);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case COMPSTMT:
			{
				body=compoundStatement(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			e.addElse(body);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t203;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	public final ExceptionHandlingExpression  exceptionHandlingExpression(AST _t) throws RecognitionException {
		ExceptionHandlingExpression e;
		
		AST exceptionHandlingExpression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			String name = null;
			BodyStatement body = null;
		
		
		AST __t216 = _t;
		AST tmp180_AST_in = (AST)_t;
		match(_t,LITERAL_begin);
		_t = _t.getFirstChild();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case BODY:
		{
			body=bodyStatement(_t);
			_t = _retTree;
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t216;
		_t = _t.getNextSibling();
		
						e = new ExceptionHandlingExpression(body);
					
		_retTree = _t;
		return e;
	}
	
	public final ArrayExpression  arrayExpression(AST _t) throws RecognitionException {
		ArrayExpression e;
		
		AST arrayExpression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression element = null;
		
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LBRACK:
		{
			AST __t170 = _t;
			AST tmp181_AST_in = (AST)_t;
			match(_t,LBRACK);
			_t = _t.getFirstChild();
			e = new ArrayExpression();
			{
			_loop172:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_tokenSet_2.member(_t.getType()))) {
					element=expression(_t);
					_t = _retTree;
					e.addElement(element);
				}
				else {
					break _loop172;
				}
				
			} while (true);
			}
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case REST_ARG_PREFIX:
			{
				AST tmp182_AST_in = (AST)_t;
				match(_t,REST_ARG_PREFIX);
				_t = _t.getNextSibling();
				element=expression(_t);
				_t = _retTree;
				e.setAsteriskElement(element);
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t170;
			_t = _t.getNextSibling();
			break;
		}
		case EMPTY_ARRAY:
		{
			AST tmp183_AST_in = (AST)_t;
			match(_t,EMPTY_ARRAY);
			_t = _t.getNextSibling();
			e = new ArrayExpression();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return e;
	}
	
	public final HashExpression  hashExpression(AST _t) throws RecognitionException {
		HashExpression e;
		
		AST hashExpression_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression key = null;
			Expression value = null;
		
		
		AST __t175 = _t;
		AST tmp184_AST_in = (AST)_t;
		match(_t,LCURLY_HASH);
		_t = _t.getFirstChild();
		e = new HashExpression();
		{
		_loop178:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_tokenSet_2.member(_t.getType()))) {
				key=expression(_t);
				_t = _retTree;
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case ASSOC:
				{
					AST tmp185_AST_in = (AST)_t;
					match(_t,ASSOC);
					_t = _t.getNextSibling();
					value=expression(_t);
					_t = _retTree;
					break;
				}
				case 3:
				case CALL:
				case SYMBOL:
				case STRING:
				case WHILE_MODIFIER:
				case UNTIL_MODIFIER:
				case ASSIGN:
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case BOR:
				case LOGICAL_OR:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case LITERAL_and:
				case LITERAL_or:
				case LITERAL_not:
				case QUESTION:
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
				case INCLUSIVE_RANGE:
				case EXCLUSIVE_RANGE:
				case LOGICAL_AND:
				case COMPARE:
				case EQUAL:
				case CASE_EQUAL:
				case NOT_EQUAL:
				case MATCH:
				case NOT_MATCH:
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
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case DOT:
				case COLON2:
				case LBRACK_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_CONTENT:
				case HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION:
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
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				e.addElement(key, value);value=null;
			}
			else {
				break _loop178;
			}
			
		} while (true);
		}
		_t = __t175;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	public final ReturnArguments  return_arguments(AST _t) throws RecognitionException {
		ReturnArguments args;
		
		AST return_arguments_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			args = new ReturnArguments();
			Expression e = null;
		
		
		AST __t125 = _t;
		AST tmp186_AST_in = (AST)_t;
		match(_t,ARG);
		_t = _t.getFirstChild();
		{
		_loop127:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_tokenSet_2.member(_t.getType()))) {
				e=expression(_t);
				_t = _retTree;
				args.addArgument(e);
			}
			else {
				break _loop127;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case REST_ARG_PREFIX:
		{
			AST tmp187_AST_in = (AST)_t;
			match(_t,REST_ARG_PREFIX);
			_t = _t.getNextSibling();
			e=expression(_t);
			_t = _retTree;
			args.setAsteriskArgument(e);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t125;
		_t = _t.getNextSibling();
		_retTree = _t;
		return args;
	}
	
	public final MethodCallArguments  arguments(AST _t) throws RecognitionException {
		MethodCallArguments args;
		
		AST arguments_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			args = new MethodCallArguments();
			Expression e = null;
		
		
		AST __t113 = _t;
		AST tmp188_AST_in = (AST)_t;
		match(_t,ARG);
		_t = _t.getFirstChild();
		{
		_loop115:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_tokenSet_2.member(_t.getType()))) {
				e=expression(_t);
				_t = _retTree;
				args.addArgument(e);
			}
			else {
				break _loop115;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case ASSOC:
		{
			e=implicitHash(_t);
			_t = _retTree;
			args.addArgument(e);
			break;
		}
		case 3:
		case REST_ARG_PREFIX:
		case BLOCK_ARG_PREFIX:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case REST_ARG_PREFIX:
		{
			AST tmp189_AST_in = (AST)_t;
			match(_t,REST_ARG_PREFIX);
			_t = _t.getNextSibling();
			e=expression(_t);
			_t = _retTree;
			args.setAsteriskArgument(e);
			break;
		}
		case 3:
		case BLOCK_ARG_PREFIX:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case BLOCK_ARG_PREFIX:
		{
			AST tmp190_AST_in = (AST)_t;
			match(_t,BLOCK_ARG_PREFIX);
			_t = _t.getNextSibling();
			e=expression(_t);
			_t = _retTree;
			args.setBlockArgument(e);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t113;
		_t = _t.getNextSibling();
		_retTree = _t;
		return args;
	}
	
	public final Block  codeBlock(AST _t) throws RecognitionException {
		Block b;
		
		AST codeBlock_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			CompoundStatement cs = null;
			Expression default_value = null;
			ParameterVariableExpression var = null;
		
		
		AST __t260 = _t;
		AST tmp191_AST_in = (AST)_t;
		match(_t,BLOCK);
		_t = _t.getFirstChild();
		b = new Block();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case BOR:
		case LOGICAL_OR:
		{
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case BOR:
			{
				AST tmp192_AST_in = (AST)_t;
				match(_t,BOR);
				_t = _t.getNextSibling();
				break;
			}
			case LOGICAL_OR:
			{
				AST tmp193_AST_in = (AST)_t;
				match(_t,LOGICAL_OR);
				_t = _t.getNextSibling();
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			b.setShouldValidateArgumentLength();
			break;
		}
		case 3:
		case COMPSTMT:
		case BLOCK_ARG:
		case BLOCK_ARG_WITH_EXTRA_COMMA:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case BLOCK_ARG:
		{
			AST __t264 = _t;
			AST tmp194_AST_in = (AST)_t;
			match(_t,BLOCK_ARG);
			_t = _t.getFirstChild();
			{
			_loop269:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_t.getType()==NESTED_LHS||_t.getType()==IDENTIFIER||_t.getType()==FUNCTION)) {
					{
					if (_t==null) _t=ASTNULL;
					switch ( _t.getType()) {
					case IDENTIFIER:
					case FUNCTION:
					{
						var=local_variable(_t);
						_t = _retTree;
						break;
					}
					case NESTED_LHS:
					{
						var=nestedLhs(_t);
						_t = _retTree;
						break;
					}
					default:
					{
						throw new NoViableAltException(_t);
					}
					}
					}
					{
					if (_t==null) _t=ASTNULL;
					switch ( _t.getType()) {
					case ASSIGN:
					case ASSIGN_WITH_NO_LEADING_SPACE:
					{
						{
						if (_t==null) _t=ASTNULL;
						switch ( _t.getType()) {
						case ASSIGN:
						{
							AST tmp195_AST_in = (AST)_t;
							match(_t,ASSIGN);
							_t = _t.getNextSibling();
							break;
						}
						case ASSIGN_WITH_NO_LEADING_SPACE:
						{
							AST tmp196_AST_in = (AST)_t;
							match(_t,ASSIGN_WITH_NO_LEADING_SPACE);
							_t = _t.getNextSibling();
							break;
						}
						default:
						{
							throw new NoViableAltException(_t);
						}
						}
						}
						default_value=expression(_t);
						_t = _retTree;
						break;
					}
					case 3:
					case NESTED_LHS:
					case REST_ARG_PREFIX:
					case IDENTIFIER:
					case FUNCTION:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(_t);
					}
					}
					}
					b.addParameter(var, default_value);
				}
				else {
					break _loop269;
				}
				
			} while (true);
			}
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case REST_ARG_PREFIX:
			{
				AST tmp197_AST_in = (AST)_t;
				match(_t,REST_ARG_PREFIX);
				_t = _t.getNextSibling();
				b.setAsterisk();
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case IDENTIFIER:
				case FUNCTION:
				{
					var=local_variable(_t);
					_t = _retTree;
					b.setAsteriskParameter(var);
					break;
				}
				case 3:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t264;
			_t = _t.getNextSibling();
			break;
		}
		case BLOCK_ARG_WITH_EXTRA_COMMA:
		{
			AST __t272 = _t;
			AST tmp198_AST_in = (AST)_t;
			match(_t,BLOCK_ARG_WITH_EXTRA_COMMA);
			_t = _t.getFirstChild();
			{b.setHasExtraComma();}
			{
			_loop277:
			do {
				if (_t==null) _t=ASTNULL;
				if ((_t.getType()==NESTED_LHS||_t.getType()==IDENTIFIER||_t.getType()==FUNCTION)) {
					{
					if (_t==null) _t=ASTNULL;
					switch ( _t.getType()) {
					case IDENTIFIER:
					case FUNCTION:
					{
						var=local_variable(_t);
						_t = _retTree;
						break;
					}
					case NESTED_LHS:
					{
						var=nestedLhs(_t);
						_t = _retTree;
						break;
					}
					default:
					{
						throw new NoViableAltException(_t);
					}
					}
					}
					{
					if (_t==null) _t=ASTNULL;
					switch ( _t.getType()) {
					case ASSIGN:
					case ASSIGN_WITH_NO_LEADING_SPACE:
					{
						{
						if (_t==null) _t=ASTNULL;
						switch ( _t.getType()) {
						case ASSIGN:
						{
							AST tmp199_AST_in = (AST)_t;
							match(_t,ASSIGN);
							_t = _t.getNextSibling();
							break;
						}
						case ASSIGN_WITH_NO_LEADING_SPACE:
						{
							AST tmp200_AST_in = (AST)_t;
							match(_t,ASSIGN_WITH_NO_LEADING_SPACE);
							_t = _t.getNextSibling();
							break;
						}
						default:
						{
							throw new NoViableAltException(_t);
						}
						}
						}
						default_value=expression(_t);
						_t = _retTree;
						break;
					}
					case 3:
					case NESTED_LHS:
					case REST_ARG_PREFIX:
					case IDENTIFIER:
					case FUNCTION:
					{
						break;
					}
					default:
					{
						throw new NoViableAltException(_t);
					}
					}
					}
					b.addParameter(var, default_value);
				}
				else {
					break _loop277;
				}
				
			} while (true);
			}
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case REST_ARG_PREFIX:
			{
				AST tmp201_AST_in = (AST)_t;
				match(_t,REST_ARG_PREFIX);
				_t = _t.getNextSibling();
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case IDENTIFIER:
				case FUNCTION:
				{
					var=local_variable(_t);
					_t = _retTree;
					b.setAsteriskParameter(var);
					break;
				}
				case 3:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			_t = __t272;
			_t = _t.getNextSibling();
			break;
		}
		case 3:
		case COMPSTMT:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		{
			cs=compoundStatement(_t);
			_t = _retTree;
			b.setBody(cs);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t260;
		_t = _t.getNextSibling();
		_retTree = _t;
		return b;
	}
	
	public final HashExpression  implicitHash(AST _t) throws RecognitionException {
		HashExpression e;
		
		AST implicitHash_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression key = null;
			Expression value = null;
			e = new HashExpression();
		
		
		{
		int _cnt123=0;
		_loop123:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_t.getType()==ASSOC)) {
				AST __t121 = _t;
				AST tmp202_AST_in = (AST)_t;
				match(_t,ASSOC);
				_t = _t.getFirstChild();
				key=expression(_t);
				_t = _retTree;
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case CALL:
				case SYMBOL:
				case STRING:
				case WHILE_MODIFIER:
				case UNTIL_MODIFIER:
				case ASSIGN:
				case LPAREN_WITH_NO_LEADING_SPACE:
				case LPAREN:
				case IDENTIFIER:
				case CONSTANT:
				case FUNCTION:
				case GLOBAL_VARIABLE:
				case BOR:
				case LOGICAL_OR:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				case LITERAL_and:
				case LITERAL_or:
				case LITERAL_not:
				case QUESTION:
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
				case INCLUSIVE_RANGE:
				case EXCLUSIVE_RANGE:
				case LOGICAL_AND:
				case COMPARE:
				case EQUAL:
				case CASE_EQUAL:
				case NOT_EQUAL:
				case MATCH:
				case NOT_MATCH:
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
				case NOT:
				case LITERAL_return:
				case LITERAL_break:
				case LITERAL_next:
				case DOT:
				case COLON2:
				case LBRACK_ARRAY_ACCESS:
				case UNARY_PLUS:
				case UNARY_MINUS:
				case LITERAL_nil:
				case LITERAL_true:
				case LITERAL_false:
				case LITERAL___FILE__:
				case LITERAL___LINE__:
				case REGEX:
				case REGEX_BEFORE_EXPRESSION_SUBSTITUTION:
				case COMMAND_OUTPUT:
				case COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_CONTENT:
				case HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION:
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
				{
					value=expression(_t);
					_t = _retTree;
					break;
				}
				case 3:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				e.addElement(key, value);value=null;
				_t = __t121;
				_t = _t.getNextSibling();
			}
			else {
				if ( _cnt123>=1 ) { break _loop123; } else {throw new NoViableAltException(_t);}
			}
			
			_cnt123++;
		} while (true);
		}
		_retTree = _t;
		return e;
	}
	
	public final LocalVariableExpression  local_variable(AST _t) throws RecognitionException {
		LocalVariableExpression e;
		
		AST local_variable_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST id = null;
		AST function = null;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case IDENTIFIER:
		{
			id = (AST)_t;
			match(_t,IDENTIFIER);
			_t = _t.getNextSibling();
			e = new LocalVariableExpression(id.getText(), false);
			break;
		}
		case FUNCTION:
		{
			function = (AST)_t;
			match(_t,FUNCTION);
			_t = _t.getNextSibling();
			e = new LocalVariableExpression(function.getText(), true);
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return e;
	}
	
	public final StringGroupExpression  string(AST _t) throws RecognitionException {
		StringGroupExpression e;
		
		AST string_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST double_quote_string = null;
		AST single_quote_string = null;
		
			e = new StringGroupExpression();
			StringExpressionWithExpressionSubstitution s = null;
		
		
		AST __t135 = _t;
		AST tmp203_AST_in = (AST)_t;
		match(_t,STRING);
		_t = _t.getFirstChild();
		{
		int _cnt137=0;
		_loop137:
		do {
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case DOUBLE_QUOTE_STRING:
			{
				double_quote_string = (AST)_t;
				match(_t,DOUBLE_QUOTE_STRING);
				_t = _t.getNextSibling();
				e.appendString(double_quote_string.getText(), true);
				break;
			}
			case SINGLE_QUOTE_STRING:
			{
				single_quote_string = (AST)_t;
				match(_t,SINGLE_QUOTE_STRING);
				_t = _t.getNextSibling();
				e.appendString(single_quote_string.getText(), false);
				break;
			}
			case STRING_BEFORE_EXPRESSION_SUBSTITUTION:
			{
				s=stringWithExpressionSubstituation(_t);
				_t = _retTree;
				e.addStringWithExpressionSubstituation(s);
				break;
			}
			default:
			{
				if ( _cnt137>=1 ) { break _loop137; } else {throw new NoViableAltException(_t);}
			}
			}
			_cnt137++;
		} while (true);
		}
		_t = __t135;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	public final RegexpExpressionWithExpressionSubstitution  regexWithExpressionSubstituation(AST _t) throws RecognitionException {
		RegexpExpressionWithExpressionSubstitution e;
		
		AST regexWithExpressionSubstituation_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST b = null;
		AST m = null;
		AST a = null;
		
		AST __t151 = _t;
		b = _t==ASTNULL ? null :(AST)_t;
		match(_t,REGEX_BEFORE_EXPRESSION_SUBSTITUTION);
		_t = _t.getFirstChild();
		e = new RegexpExpressionWithExpressionSubstitution(b.getText());
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		case GLOBAL_VARIABLE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		{
			expression_substituation(_t,e);
			_t = _retTree;
			break;
		}
		case STRING_BETWEEN_EXPRESSION_SUBSTITUTION:
		case STRING_AFTER_EXPRESSION_SUBSTITUTION:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		_loop155:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_t.getType()==STRING_BETWEEN_EXPRESSION_SUBSTITUTION)) {
				m = (AST)_t;
				match(_t,STRING_BETWEEN_EXPRESSION_SUBSTITUTION);
				_t = _t.getNextSibling();
				e.addString(m.getText());
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case COMPSTMT:
				case GLOBAL_VARIABLE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				{
					expression_substituation(_t,e);
					_t = _retTree;
					break;
				}
				case STRING_BETWEEN_EXPRESSION_SUBSTITUTION:
				case STRING_AFTER_EXPRESSION_SUBSTITUTION:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
			}
			else {
				break _loop155;
			}
			
		} while (true);
		}
		a = (AST)_t;
		match(_t,STRING_AFTER_EXPRESSION_SUBSTITUTION);
		_t = _t.getNextSibling();
		e.addLastString(a.getText());
		_t = __t151;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	public final CommandOutputExpressionWithExpressionSubstitution  commandOutputWithExpressionSubstituation(AST _t) throws RecognitionException {
		CommandOutputExpressionWithExpressionSubstitution e;
		
		AST commandOutputWithExpressionSubstituation_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST b = null;
		AST m = null;
		AST a = null;
		
		AST __t157 = _t;
		b = _t==ASTNULL ? null :(AST)_t;
		match(_t,COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION);
		_t = _t.getFirstChild();
		e = new CommandOutputExpressionWithExpressionSubstitution(b.getText());
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		case GLOBAL_VARIABLE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		{
			expression_substituation(_t,e);
			_t = _retTree;
			break;
		}
		case STRING_BETWEEN_EXPRESSION_SUBSTITUTION:
		case STRING_AFTER_EXPRESSION_SUBSTITUTION:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		_loop161:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_t.getType()==STRING_BETWEEN_EXPRESSION_SUBSTITUTION)) {
				m = (AST)_t;
				match(_t,STRING_BETWEEN_EXPRESSION_SUBSTITUTION);
				_t = _t.getNextSibling();
				e.addString(m.getText());
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case COMPSTMT:
				case GLOBAL_VARIABLE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				{
					expression_substituation(_t,e);
					_t = _retTree;
					break;
				}
				case STRING_BETWEEN_EXPRESSION_SUBSTITUTION:
				case STRING_AFTER_EXPRESSION_SUBSTITUTION:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
			}
			else {
				break _loop161;
			}
			
		} while (true);
		}
		a = (AST)_t;
		match(_t,STRING_AFTER_EXPRESSION_SUBSTITUTION);
		_t = _t.getNextSibling();
		e.addString(a.getText());
		_t = __t157;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	protected final StringExpressionWithExpressionSubstitution  heredocWithExpressionSubstituation(AST _t) throws RecognitionException {
		StringExpressionWithExpressionSubstitution e;
		
		AST heredocWithExpressionSubstituation_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST b = null;
		AST m = null;
		AST a = null;
		
		AST __t145 = _t;
		b = _t==ASTNULL ? null :(AST)_t;
		match(_t,HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION);
		_t = _t.getFirstChild();
		e = new StringExpressionWithExpressionSubstitution(b.getText());
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		case GLOBAL_VARIABLE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		{
			expression_substituation(_t,e);
			_t = _retTree;
			break;
		}
		case HERE_DOC_BETWEEN_EXPRESSION_SUBSTITUTION:
		case HERE_DOC_AFTER_EXPRESSION_SUBSTITUTION:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		_loop149:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_t.getType()==HERE_DOC_BETWEEN_EXPRESSION_SUBSTITUTION)) {
				m = (AST)_t;
				match(_t,HERE_DOC_BETWEEN_EXPRESSION_SUBSTITUTION);
				_t = _t.getNextSibling();
				e.addString(m.getText());
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case COMPSTMT:
				case GLOBAL_VARIABLE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				{
					expression_substituation(_t,e);
					_t = _retTree;
					break;
				}
				case HERE_DOC_BETWEEN_EXPRESSION_SUBSTITUTION:
				case HERE_DOC_AFTER_EXPRESSION_SUBSTITUTION:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
			}
			else {
				break _loop149;
			}
			
		} while (true);
		}
		a = (AST)_t;
		match(_t,HERE_DOC_AFTER_EXPRESSION_SUBSTITUTION);
		_t = _t.getNextSibling();
		e.addString(a.getText());
		_t = __t145;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	protected final StringExpressionWithExpressionSubstitution  stringWithExpressionSubstituation(AST _t) throws RecognitionException {
		StringExpressionWithExpressionSubstitution e;
		
		AST stringWithExpressionSubstituation_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST b = null;
		AST m = null;
		AST a = null;
		
		AST __t139 = _t;
		b = _t==ASTNULL ? null :(AST)_t;
		match(_t,STRING_BEFORE_EXPRESSION_SUBSTITUTION);
		_t = _t.getFirstChild();
		e = new StringExpressionWithExpressionSubstitution(b.getText());
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		case GLOBAL_VARIABLE:
		case INSTANCE_VARIABLE:
		case CLASS_VARIABLE:
		{
			expression_substituation(_t,e);
			_t = _retTree;
			break;
		}
		case STRING_BETWEEN_EXPRESSION_SUBSTITUTION:
		case STRING_AFTER_EXPRESSION_SUBSTITUTION:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		_loop143:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_t.getType()==STRING_BETWEEN_EXPRESSION_SUBSTITUTION)) {
				m = (AST)_t;
				match(_t,STRING_BETWEEN_EXPRESSION_SUBSTITUTION);
				_t = _t.getNextSibling();
				e.addString(m.getText());
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case COMPSTMT:
				case GLOBAL_VARIABLE:
				case INSTANCE_VARIABLE:
				case CLASS_VARIABLE:
				{
					expression_substituation(_t,e);
					_t = _retTree;
					break;
				}
				case STRING_BETWEEN_EXPRESSION_SUBSTITUTION:
				case STRING_AFTER_EXPRESSION_SUBSTITUTION:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
			}
			else {
				break _loop143;
			}
			
		} while (true);
		}
		a = (AST)_t;
		match(_t,STRING_AFTER_EXPRESSION_SUBSTITUTION);
		_t = _t.getNextSibling();
		e.addString(a.getText());
		_t = __t139;
		_t = _t.getNextSibling();
		_retTree = _t;
		return e;
	}
	
	public final void expression_substituation(AST _t,
		ExpressionWithExpressionSubstitution e
	) throws RecognitionException {
		
		AST expression_substituation_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST g = null;
		AST i = null;
		AST c = null;
		
			CompoundStatement cs = null;
		
		
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		{
			cs=compoundStatement(_t);
			_t = _retTree;
			e.addCompoundStatement(cs);
			break;
		}
		case GLOBAL_VARIABLE:
		{
			g = (AST)_t;
			match(_t,GLOBAL_VARIABLE);
			_t = _t.getNextSibling();
			e.addGlobalVariable(g.getText());
			break;
		}
		case INSTANCE_VARIABLE:
		{
			i = (AST)_t;
			match(_t,INSTANCE_VARIABLE);
			_t = _t.getNextSibling();
			e.addInstanceVariable(i.getText());
			break;
		}
		case CLASS_VARIABLE:
		{
			c = (AST)_t;
			match(_t,CLASS_VARIABLE);
			_t = _t.getNextSibling();
			e.addClassVariable(c.getText());
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_retTree = _t;
	}
	
	public final When  when(AST _t) throws RecognitionException {
		When w;
		
		AST when_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression e = null;
			CompoundStatement body = null;
		
		
		AST tmp204_AST_in = (AST)_t;
		match(_t,LITERAL_when);
		_t = _t.getNextSibling();
		w = new When();
		AST __t210 = _t;
		AST tmp205_AST_in = (AST)_t;
		match(_t,MRHS);
		_t = _t.getFirstChild();
		{
		_loop212:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_tokenSet_2.member(_t.getType()))) {
				e=expression(_t);
				_t = _retTree;
				w.addCondition(e);
			}
			else {
				break _loop212;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case REST_ARG_PREFIX:
		{
			AST tmp206_AST_in = (AST)_t;
			match(_t,REST_ARG_PREFIX);
			_t = _t.getNextSibling();
			e=expression(_t);
			_t = _retTree;
			w.setAsteriskCondition(e);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t210;
		_t = _t.getNextSibling();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		{
			body=compoundStatement(_t);
			_t = _retTree;
			break;
		}
		case 3:
		case LITERAL_else:
		case LITERAL_when:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		w.setBody(body);
		_retTree = _t;
		return w;
	}
	
	public final BodyStatement  bodyStatement(AST _t) throws RecognitionException {
		BodyStatement bs;
		
		AST bodyStatement_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			ExceptionList el;
			CompoundStatement cs = null;
			CompoundStatement rescue_cs = null;
			CompoundStatement else_cs = null;
			CompoundStatement ensure_cs = null;
		
		
		AST __t291 = _t;
		AST tmp207_AST_in = (AST)_t;
		match(_t,BODY);
		_t = _t.getFirstChild();
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case COMPSTMT:
		{
			cs=compoundStatement(_t);
			_t = _retTree;
			break;
		}
		case 3:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		bs = new BodyStatement(cs);
		{
		_loop295:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_t.getType()==LITERAL_rescue)) {
				AST tmp208_AST_in = (AST)_t;
				match(_t,LITERAL_rescue);
				_t = _t.getNextSibling();
				el=exceptionList(_t);
				_t = _retTree;
				{
				if (_t==null) _t=ASTNULL;
				switch ( _t.getType()) {
				case COMPSTMT:
				{
					rescue_cs=compoundStatement(_t);
					_t = _retTree;
					break;
				}
				case 3:
				case LITERAL_rescue:
				case LITERAL_else:
				case LITERAL_ensure:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(_t);
				}
				}
				}
				bs.addRescue(el, rescue_cs);
			}
			else {
				break _loop295;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LITERAL_else:
		{
			AST tmp209_AST_in = (AST)_t;
			match(_t,LITERAL_else);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case COMPSTMT:
			{
				else_cs=compoundStatement(_t);
				_t = _retTree;
				break;
			}
			case 3:
			case LITERAL_ensure:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			bs.addElse(else_cs);
			break;
		}
		case 3:
		case LITERAL_ensure:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case LITERAL_ensure:
		{
			AST tmp210_AST_in = (AST)_t;
			match(_t,LITERAL_ensure);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case COMPSTMT:
			{
				ensure_cs=compoundStatement(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			bs.addEnsure(ensure_cs);
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_t = __t291;
		_t = _t.getNextSibling();
		_retTree = _t;
		return bs;
	}
	
	public final String  className(AST _t) throws RecognitionException {
		String s;
		
		AST className_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST id = null;
		AST function = null;
		AST constant = null;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case IDENTIFIER:
		{
			id = (AST)_t;
			match(_t,IDENTIFIER);
			_t = _t.getNextSibling();
			s = id.getText();
			break;
		}
		case FUNCTION:
		{
			function = (AST)_t;
			match(_t,FUNCTION);
			_t = _t.getNextSibling();
			s = function.getText();
			break;
		}
		case CONSTANT:
		{
			constant = (AST)_t;
			match(_t,CONSTANT);
			_t = _t.getNextSibling();
			s = constant.getText();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return s;
	}
	
	public final String  moduleName(AST _t) throws RecognitionException {
		String s;
		
		AST moduleName_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST id = null;
		AST function = null;
		AST constant = null;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case IDENTIFIER:
		{
			id = (AST)_t;
			match(_t,IDENTIFIER);
			_t = _t.getNextSibling();
			s = id.getText();
			break;
		}
		case FUNCTION:
		{
			function = (AST)_t;
			match(_t,FUNCTION);
			_t = _t.getNextSibling();
			s = function.getText();
			break;
		}
		case CONSTANT:
		{
			constant = (AST)_t;
			match(_t,CONSTANT);
			_t = _t.getNextSibling();
			s = constant.getText();
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return s;
	}
	
	public final String  methodName(AST _t) throws RecognitionException {
		String s;
		
		AST methodName_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		AST id = null;
		AST assign1 = null;
		AST function = null;
		AST assign2 = null;
		AST constant = null;
		AST unary = null;
		
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case IDENTIFIER:
		{
			id = (AST)_t;
			match(_t,IDENTIFIER);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				assign1 = (AST)_t;
				match(_t,ASSIGN_WITH_NO_LEADING_SPACE);
				_t = _t.getNextSibling();
				break;
			}
			case 3:
			case BODY:
			case REST_ARG_PREFIX:
			case IDENTIFIER:
			case FUNCTION:
			case BLOCK_ARG_PREFIX:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			s = id.getText(); if (null != assign1) {s += "=";}
			break;
		}
		case FUNCTION:
		{
			function = (AST)_t;
			match(_t,FUNCTION);
			_t = _t.getNextSibling();
			{
			if (_t==null) _t=ASTNULL;
			switch ( _t.getType()) {
			case ASSIGN_WITH_NO_LEADING_SPACE:
			{
				assign2 = (AST)_t;
				match(_t,ASSIGN_WITH_NO_LEADING_SPACE);
				_t = _t.getNextSibling();
				break;
			}
			case 3:
			case BODY:
			case REST_ARG_PREFIX:
			case IDENTIFIER:
			case FUNCTION:
			case BLOCK_ARG_PREFIX:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(_t);
			}
			}
			}
			s = function.getText(); if (null != assign2) {s += "=";}
			break;
		}
		case CONSTANT:
		{
			constant = (AST)_t;
			match(_t,CONSTANT);
			_t = _t.getNextSibling();
			s = constant.getText();
			break;
		}
		case UNARY_PLUS_MINUS_METHOD_NAME:
		{
			unary = (AST)_t;
			match(_t,UNARY_PLUS_MINUS_METHOD_NAME);
			_t = _t.getNextSibling();
			s = unary.getText();
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
			s=keyword(_t);
			_t = _retTree;
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
			s=operator(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		_retTree = _t;
		return s;
	}
	
	public final ExceptionList  exceptionList(AST _t) throws RecognitionException {
		ExceptionList el;
		
		AST exceptionList_AST_in = (_t == ASTNULL) ? null : (AST)_t;
		
			Expression e;
			LocalVariableExpression v;
		
		
		el = new ExceptionList();
		{
		_loop302:
		do {
			if (_t==null) _t=ASTNULL;
			if ((_tokenSet_2.member(_t.getType()))) {
				e=expression(_t);
				_t = _retTree;
				el.addArgument(e);
			}
			else {
				break _loop302;
			}
			
		} while (true);
		}
		{
		if (_t==null) _t=ASTNULL;
		switch ( _t.getType()) {
		case ASSOC:
		{
			AST tmp211_AST_in = (AST)_t;
			match(_t,ASSOC);
			_t = _t.getNextSibling();
			v=local_variable(_t);
			_t = _retTree;
			el.addExceptionVariable(v);
			break;
		}
		case 3:
		case COMPSTMT:
		case LITERAL_rescue:
		case LITERAL_else:
		case LITERAL_ensure:
		{
			break;
		}
		default:
		{
			throw new NoViableAltException(_t);
		}
		}
		}
		_retTree = _t;
		return el;
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
	
	private static final long[] mk_tokenSet_0() {
		long[] data = { -9330476048967104L, -451451554445131777L, 2481899455L, 0L, 0L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_0 = new BitSet(mk_tokenSet_0());
	private static final long[] mk_tokenSet_1() {
		long[] data = { -9331300682628544L, -451451554445131777L, 65980351L, 0L, 0L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_1 = new BitSet(mk_tokenSet_1());
	private static final long[] mk_tokenSet_2() {
		long[] data = { -9331300682694080L, -451451554445131777L, 65980351L, 0L, 0L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_2 = new BitSet(mk_tokenSet_2());
	}
	
