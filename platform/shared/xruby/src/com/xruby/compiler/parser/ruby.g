header {
package com.xruby.compiler.parser;
}

/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

//----------------------------------------------------------------------------
// The Ruby parser. Do not use this one, use RubyParser instead!
//----------------------------------------------------------------------------
class RubyParserBase extends Parser;
options {
	k = 1;                           // Will add prediction manually if we need k > 1.
	exportVocab=Ruby;
	codeGenMakeSwitchThreshold = 2;
	codeGenBitsetTestThreshold = 999;
	defaultErrorHandler = false;
	buildAST = true;
	//analyzerDebug = true;
}

tokens {
	// "imaginary" tokens
	RPAREN_IN_METHOD_DEFINATION;
	BODY;
	CALL;
	ARG;
	COMPSTMT;
	SYMBOL;
	BLOCK;
	MULTIPLE_ASSIGN;
	MULTIPLE_ASSIGN_WITH_EXTRA_COMMA;
	BLOCK_ARG;
	BLOCK_ARG_WITH_EXTRA_COMMA;
	MRHS;
	NESTED_LHS;
	SINGLETON_METHOD;
	STRING;
}

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
}

program
		:	(compoundStatement)? EOF!
		;

compoundStatement
{boolean empty = true;}
		:	(terminal!	(statements	{empty = false;})?
			|	statements	{empty = false;})
			{if (!empty)	#compoundStatement = #(#[COMPSTMT, "COMPSTMT"], #compoundStatement);}
		;

statements
		:	statement
			(
				terminal
				{
					if (	(EOF == LA(1))	||	//script end
						(RCURLY == LA(1))	||	(LITERAL_end == LA(1))	||	//block end
						(RPAREN == LA(1))	||
						(LITERAL_else == LA(1)) || (LITERAL_elsif == LA(1))	||
						(LITERAL_rescue == LA(1)) || (LITERAL_ensure == LA(1))	||
						(LITERAL_when == LA(1))
					)
					break;
				}
				statement
			)*
		;

terminal
		:	SEMI!
		|	LINE_BREAK!
		;

statement
		:	body:statementWithoutModifier
									(!IF_MODIFIER^		if_condition:expression	{#statement = #(#[LITERAL_if, "if"], if_condition, #(#[COMPSTMT, "COMPSTMT"], #body));}
									|!UNLESS_MODIFIER^	unless_condition:expression	{#statement = #(#[LITERAL_unless, "unless"], unless_condition, #(#[COMPSTMT, "COMPSTMT"], #body));}
									|!WHILE_MODIFIER^	while_condition:expression	{#statement = #(#WHILE_MODIFIER, while_condition, #(#[COMPSTMT, "COMPSTMT"], #body));}
									|!UNTIL_MODIFIER^	until_condition:expression	{#statement = #(#UNTIL_MODIFIER, until_condition, #(#[COMPSTMT, "COMPSTMT"], #body));}
									|!RESCUE_MODIFIER^	rescue_condition:expression	{#statement = #(#[LITERAL_begin, "begin"], #(#[BODY, "BODY"], #(#[COMPSTMT, "COMPSTMT"], #body), #[LITERAL_rescue, "rescue"], rescue_condition));}
									)*
		;

statementWithoutModifier
{
	boolean is_parallel_assignment = false;
	boolean has_extra_comma = false;
}
		:	(aliasStatement
			|	undefStatement
			|	keyword_BEGIN 	LCURLY_BLOCK (compoundStatement)? RCURLY	{if (is_in_method_definition>0) {throw new RecognitionException("BEGIN in method");}}
			|	keyword_END 	LCURLY_BLOCK (compoundStatement)? RCURLY		{if (is_in_method_definition>0) {throw new RecognitionException("END in method");}}
			|	expression
				(options{greedy=true;}:
					COMMA!
					{if (ASSIGN == LA(1) || ASSIGN_WITH_NO_LEADING_SPACE == LA(1)) {has_extra_comma=true;break;}}
					(seen_star:REST_ARG_PREFIX)?
					(mlhs_item)?	{if (null != seen_star) break;}
					{is_parallel_assignment = true;}
				)*
				(	(ASSIGN!|ASSIGN_WITH_NO_LEADING_SPACE!)
					(options{greedy=true;}:LINE_BREAK!)?
					mrhs
					{is_parallel_assignment = true;}
				)?
			|	REST_ARG_PREFIX
				mlhs_item
				(ASSIGN!|ASSIGN_WITH_NO_LEADING_SPACE!)
				(options{greedy=true;}:LINE_BREAK!)?
				mrhs
				{is_parallel_assignment = true;}
			)
			{
				if (is_parallel_assignment && !has_extra_comma) {
					#statementWithoutModifier = #(#[MULTIPLE_ASSIGN, "MULTIPLE_ASSIGN"], #statementWithoutModifier);
				} else if (is_parallel_assignment && has_extra_comma) {
					#statementWithoutModifier = #(#[MULTIPLE_ASSIGN_WITH_EXTRA_COMMA, "MULTIPLE_ASSIGN_WITH_EXTRA_COMMA"], #statementWithoutModifier);
				}
			}
		;

protected
lparen
		:	LPAREN_WITH_NO_LEADING_SPACE!
		|	LPAREN!
		;

//LPAREN can start primaryExpression, have to use syntactic predicate here.
mlhs_item
		:	(lparen	dotColonOrArrayAccess	COMMA)=>
			lparen	{setIsInNestedMultipleAssign(true);}
			dotColonOrArrayAccess
			(COMMA!	{if (RPAREN == LA(1)) break;}
			(seen_star:REST_ARG_PREFIX)?	dotColonOrArrayAccess	{if (null != seen_star) break;}
			)+
			RPAREN!	{setIsInNestedMultipleAssign(false);}
			{#mlhs_item = #(#[NESTED_LHS, "NESTED_LHS"], #mlhs_item);}
		|	dotColonOrArrayAccess
		;

mrhs
		:	(expression	(COMMA! {if (ASSIGN == LA(1)||RBRACK == LA(1)) break;}	(seen_star:REST_ARG_PREFIX)?	expression	{if (null != seen_star) break;})*
			|	REST_ARG_PREFIX	expression
			)
			{#mrhs = #(#[MRHS, "MRHS"], #mrhs);}
		;

protected
undef_parameter
		:	IDENTIFIER	(ASSIGN_WITH_NO_LEADING_SPACE)?
		|	CONSTANT	(ASSIGN_WITH_NO_LEADING_SPACE)?
		|	FUNCTION	(ASSIGN_WITH_NO_LEADING_SPACE)?
		|	symbol
		|	keywordAsMethodName
		|	operatorAsMethodname
		;

undefStatement
		:	"undef"^	(options{greedy=true;}:LINE_BREAK!)?
			undef_parameter	(COMMA!	undef_parameter)*
		;

aliasStatement
		:	"alias"^	(options{greedy=true;}:LINE_BREAK!)?
			(GLOBAL_VARIABLE 	(LINE_BREAK!)?	GLOBAL_VARIABLE
			|undef_parameter	(LINE_BREAK!)?	undef_parameter
			)
		;

//TODO should be mlhs
block_vars
{
	boolean has_extra_comma = false;
}
		:	(mlhs_item		(COMMA!	{if (BOR == LA(1) || LITERAL_in == LA(1)) {has_extra_comma = true; break;}}	(seen_star:REST_ARG_PREFIX)?	(mlhs_item)?	{if (null != seen_star) break;})*
			|	REST_ARG_PREFIX	(dotColonOrArrayAccess)?
			)
			{	if (has_extra_comma) {
					#block_vars = #(#[BLOCK_ARG_WITH_EXTRA_COMMA, "BLOCK_ARG_WITH_EXTRA_COMMA"], #block_vars);
				} else {
					#block_vars = #(#[BLOCK_ARG, "BLOCK_ARG"], #block_vars);
				}
			}
		;

codeBlock
		:	("do"!	(options{greedy=true;}:LINE_BREAK!)?		{enterBlockScope();}
				blockContent
				"end"!			{leaveScope();}
			|	LCURLY_BLOCK!	{enterBlockScope();}
				blockContent
				RCURLY!			{leaveScope();}
			)
			{#codeBlock = #(#[BLOCK, "BLOCK"], #codeBlock);}
		;

protected
blockContent
		:	(BOR	(block_vars)?	BOR!
			|LOGICAL_OR
			)?	(compoundStatement)?
		;

symbol
		:	COLON_WITH_NO_FOLLOWING_SPACE!
					(IDENTIFIER	(options{greedy=true;}:ASSIGN_WITH_NO_LEADING_SPACE)?
					|FUNCTION	(options{greedy=true;}:ASSIGN_WITH_NO_LEADING_SPACE)?
					|CONSTANT	(options{greedy=true;}:ASSIGN_WITH_NO_LEADING_SPACE)?
					|GLOBAL_VARIABLE
					|INSTANCE_VARIABLE
					|CLASS_VARIABLE
					|UNARY_PLUS_MINUS_METHOD_NAME
					|operatorAsMethodname
					|keyword
					|string)
			{tellLexerWeHaveFinishedParsingSymbol();}
			{#symbol = #(#[SYMBOL, "SYMBOL"], #symbol);}
		;

/*
 * operator expression precedence
 * lowest->
 *		and or
 *		not
 *		? :
 *		= += -= *= /= %= &= ^= |= <<= >>= **=
 *		.. ...
 *		||
 *		&&
 *		<=> ==  === !=  =~  !~
 *		>  >=  <  <=
 *		|	 ^
 *		&
 *		<<  >>
 *		+  -
 *		*  /  %
 *		-(unary)  +(unary)  !  ~
 *		**
 *		[]
 *		::
 * highest<-
 */

expression
		:	andorExpression
		;

//and or
andorExpression
		:	notExpression
			(options{greedy=true;/*caused by command*/}:
				(	"and"^		(options{greedy=true;}:LINE_BREAK!)?
				|	"or"^		(options{greedy=true;}:LINE_BREAK!)?
				)
				notExpression
			)*
		;

//not
notExpression
		:	"not"^
			(options{greedy=true;}:LINE_BREAK!)?
			notExpression
		|	ternaryIfThenElseExpression
		;

ternaryIfThenElseExpression
		:	assignmentExpression
			(options{greedy=true;/*caused by command*/}:
				QUESTION^			(options{greedy=true;}:LINE_BREAK!)?
				ternaryIfThenElseExpression
				(COLON!|COLON_WITH_NO_FOLLOWING_SPACE!)				(options{greedy=true;}:LINE_BREAK!)?
				ternaryIfThenElseExpression
			)?
		;

//= += -= *= /= %= **= &= ^= |= <<= >>= &&= ||=
assignmentExpression
		:	rangeExpression
			(options{greedy=true;/*caused by command*/}:
			{REST_ARG_PREFIX != LA(2)}?
				(	ASSIGN^				(options{greedy=true;}:LINE_BREAK!)?
				|	a:ASSIGN_WITH_NO_LEADING_SPACE^	{{#a.setType(ASSIGN);}}	(options{greedy=true;}:LINE_BREAK!)?
				|	PLUS_ASSIGN^		(options{greedy=true;}:LINE_BREAK!)?
				|	MINUS_ASSIGN^		(options{greedy=true;}:LINE_BREAK!)?
				|	STAR_ASSIGN^		(options{greedy=true;}:LINE_BREAK!)?
				|	DIV_ASSIGN^		(options{greedy=true;}:LINE_BREAK!)?
				|	MOD_ASSIGN^		(options{greedy=true;}:LINE_BREAK!)?
				|	POWER_ASSIGN^		(options{greedy=true;}:LINE_BREAK!)?
				|	BAND_ASSIGN^		(options{greedy=true;}:LINE_BREAK!)?
				|	BXOR_ASSIGN^		(options{greedy=true;}:LINE_BREAK!)?
				|	BOR_ASSIGN^		(options{greedy=true;}:LINE_BREAK!)?
				|	LEFT_SHIFT_ASSIGN^	(options{greedy=true;}:LINE_BREAK!)?
				|	RIGHT_SHIFT_ASSIGN^	(options{greedy=true;}:LINE_BREAK!)?
				|	LOGICAL_AND_ASSIGN^	(options{greedy=true;}:LINE_BREAK!)?
				|	LOGICAL_OR_ASSIGN^	(options{greedy=true;}:LINE_BREAK!)?
				)
				ternaryIfThenElseExpression
			)*
		;


//.. ...
rangeExpression
		:	logicalOrExpression
			(options{greedy=true;/*caused by command*/}:
				(	INCLUSIVE_RANGE^	(options{greedy=true;}:LINE_BREAK!)?
				|	EXCLUSIVE_RANGE^	(options{greedy=true;}:LINE_BREAK!)?
				)
				logicalOrExpression
			)*
		;

//||
logicalOrExpression
		:	logicalAndExpression
			(options{greedy=true;/*caused by command*/}:
				LOGICAL_OR^		(options{greedy=true;}:LINE_BREAK!)?
				logicalAndExpression
			)*
		;

//&&
logicalAndExpression
		:	equalityExpression
			(options{greedy=true;/*caused by command*/}:
				LOGICAL_AND^		(options{greedy=true;}:LINE_BREAK!)?
				equalityExpression
			)*
		;

//<=> ==  === !=  =~  !~
equalityExpression
		:	relationalExpression
			(options{greedy=true;/*caused by command*/}:
				(	COMPARE^		(options{greedy=true;}:LINE_BREAK!)?
				|	EQUAL^			(options{greedy=true;}:LINE_BREAK!)?
				|	CASE_EQUAL^	(options{greedy=true;}:LINE_BREAK!)?
				|	NOT_EQUAL^		(options{greedy=true;}:LINE_BREAK!)?
				|	MATCH^			(options{greedy=true;}:LINE_BREAK!)?
				|	NOT_MATCH^		(options{greedy=true;}:LINE_BREAK!)?
				)
				relationalExpression
			)*
		;


//>  >=  <  <=
relationalExpression
		:	orExpression
			(options{greedy=true;/*caused by command*/}:
				(	LESS_THAN^			(options{greedy=true;}:LINE_BREAK!)?
				|	GREATER_THAN^		(options{greedy=true;}:LINE_BREAK!)?
				|	LESS_OR_EQUAL^		(options{greedy=true;}:LINE_BREAK!)?
				|	GREATER_OR_EQUAL^	(options{greedy=true;}:LINE_BREAK!)?
				)
				orExpression
			)*
		;

//|  ^
orExpression
		:	andExpression
			(options{greedy=true;/*caused by command*/}:
				(	BXOR^			(options{greedy=true;}:LINE_BREAK!)?
				|	BOR^			(options{greedy=true;}:LINE_BREAK!)?
				)
				andExpression
			)*
		;

//&
andExpression
		:	shiftExpression
			(options{greedy=true;/*caused by command*/}:
				BAND^			(options{greedy=true;}:LINE_BREAK!)?
				shiftExpression
			)*
		;



//<<  >>
shiftExpression
		:	additiveExpression
			(options{greedy=true;/*caused by command*/}:
				(	LEFT_SHIFT^		(options{greedy=true;}:LINE_BREAK!)?
				|	RIGHT_SHIFT^	(options{greedy=true;}:LINE_BREAK!)?
				)
				additiveExpression
			)*
		;



//+  -
additiveExpression
		:	multiplicativeExpression
			(options{greedy=true;/*caused by command*/}:
				(	PLUS^				(options{greedy=true;}:LINE_BREAK!)?
				|	MINUS^				(options{greedy=true;}:LINE_BREAK!)?
				)
				multiplicativeExpression
			)*
		;

//*  /  %
multiplicativeExpression
		:	powerExpression
			(options{greedy=true;/*caused by command*/}:
				(	STAR^			(options{greedy=true;}:LINE_BREAK!)?
				|	DIV^			(options{greedy=true;}:LINE_BREAK!)?
				|	MOD^			(options{greedy=true;}:LINE_BREAK!)?
				)
				powerExpression
			)*
		;


//**
powerExpression
		:	bnotExpression
			(options{greedy=true;/*caused by command*/}:
				POWER^			(options{greedy=true;}:LINE_BREAK!)?
				bnotExpression
			)*
		;

//!  ~
bnotExpression
		:	(	BNOT			(options{greedy=true;}:LINE_BREAK!)?
			|	NOT			(options{greedy=true;}:LINE_BREAK!)?
			)*
			command
		;

command
{
	boolean seen_arguments = false;
	boolean seen_block = false;
	int can_be_command = 0;
}
		:	dotColonOrArrayAccess	{can_be_command = can_be_command_;}
			(
				{can_be_command > 0}?
				methodInvocationArgumentWithoutParen[false]	{seen_arguments = true;}
			)?
			(
				{can_be_command > 1}?
				codeBlock	{seen_block = true;}
			)?
			{
				if (seen_arguments || seen_block || ((2 == can_be_command) && isNotAssign(LA(1))))
				{
					#command = #(#[CALL, "CALL"], #command);
				}
			}
		|	("return"^|"break"^|"next"^)	(options{greedy=true;/*caused by command*/}:methodInvocationArgumentWithoutParen[false])?
		;


dotColonOrArrayAccess
		:	unaryExpression
			(options{greedy=true;/*caused by command*/}:
				(DOT^		methodCall	{can_be_command_ = 3;}
				|COLON2^	methodCall	{can_be_command_ = 1;}
				|LBRACK_ARRAY_ACCESS^
					(arrayReferenceArgument)?
					RBRACK!				{can_be_command_ = 1;}
				|EMPTY_ARRAY_ACCESS	{can_be_command_ = 1;}
				)
			)*
		;

//-(unary)  +(unary)
unaryExpression
		:	(	UNARY_PLUS^	(options{greedy=true;}:LINE_BREAK!)?
			|	UNARY_MINUS^	(options{greedy=true;}:LINE_BREAK!)?
			)*
			(methodCall
			|primaryExpressionThatCanNotBeMethodName	{can_be_command_ = 0;}
			)
		;

methodCall
{
	can_be_command_ = 1;
	boolean seen_arguments = false;
	boolean seen_block = false;
	boolean is_function = false;
}
		:	name:primaryExpressionThatCanBeMethodName
			{is_function = (2 == can_be_command_);}
			(options{greedy=true;/*caused by command*/}:arg:methodInvocationArgumentWithParen	{seen_arguments = true;})?
			(options{greedy=true;/*caused by command*/}:codeBlock	{seen_block = true;})?
			{
				if (seen_arguments || seen_block || (is_function && (LA(1) == LBRACK_ARRAY_ACCESS)/*this is for a[1]*/))
				{
					#methodCall = #(#[CALL, "CALL"], #methodCall);
					can_be_command_ = 0;
				}
			}
		;

//cruby can accept LPAREN in some situation but gives a warning:
//"don't put space before argument parentheses"
methodInvocationArgumentWithParen
{
//super and super() are different, the former takes implicit parameters from the calling method
boolean has_arg = false;
}
		:	LPAREN_WITH_NO_LEADING_SPACE!
				(methodInvocationArgumentWithoutParen[true]	{has_arg = true;})?
				(LINE_BREAK!)?
			RPAREN!
			{if (!has_arg) {#methodInvocationArgumentWithParen = #(#[ARG, "ARG"], #methodInvocationArgumentWithParen);}}
		;

methodInvocationArgumentWithoutParen[boolean should_ignore_line_break]
{
boolean seen_star_or_band = false;
}
		:	(normalMethodInvocationArgument[should_ignore_line_break]
			(options{greedy=true;/*caused by command*/}:
				COMMA!	{if (REST_ARG_PREFIX == LA(1) || BLOCK_ARG_PREFIX == LA(1))	{seen_star_or_band = true;break;}}
				normalMethodInvocationArgument[should_ignore_line_break]
			)*
			({seen_star_or_band}?	restMethodInvocationArgument
			|{seen_star_or_band}?	blockMethodInvocationArgument
			)?
			|	restMethodInvocationArgument
			|	blockMethodInvocationArgument
			)
			{#methodInvocationArgumentWithoutParen = #(#[ARG, "ARG"], #methodInvocationArgumentWithoutParen);}
		;

normalMethodInvocationArgument[boolean should_ignore_line_break]
		:	expression	(options{greedy=true;/*caused by command*/}:ASSOC^	expression)?	({should_ignore_line_break}?	LINE_BREAK!)?
		;

restMethodInvocationArgument
		:	REST_ARG_PREFIX	expression	(options{greedy=true;/*caused by command*/}:COMMA!	blockMethodInvocationArgument)?
		;

blockMethodInvocationArgument
		:	BLOCK_ARG_PREFIX	expression
		;

predefinedValue
		:	"nil"
		|	"true"
		|	"false"
		|	"__FILE__"
		|	"__LINE__"
		;

string
		:	DOUBLE_QUOTE_STRING
		|	SINGLE_QUOTE_STRING
		|	STRING_BEFORE_EXPRESSION_SUBSTITUTION^
			expression_substituation	{tellLexerWeHaveFinishedParsingStringExpressionSubstituation();}
			(STRING_BETWEEN_EXPRESSION_SUBSTITUTION	expression_substituation	{tellLexerWeHaveFinishedParsingStringExpressionSubstituation();})*
			STRING_AFTER_EXPRESSION_SUBSTITUTION
		;

protected
expression_substituation
		:	(	LCURLY_BLOCK!	(compoundStatement)?	RCURLY!
			|	GLOBAL_VARIABLE
			|	INSTANCE_VARIABLE
			|	CLASS_VARIABLE
			)
		;

regex
		:	REGEX
		|	REGEX_BEFORE_EXPRESSION_SUBSTITUTION^
			expression_substituation	{tellLexerWeHaveFinishedParsingRegexExpressionSubstituation();}
			(STRING_BETWEEN_EXPRESSION_SUBSTITUTION	expression_substituation	{tellLexerWeHaveFinishedParsingRegexExpressionSubstituation();})*
			STRING_AFTER_EXPRESSION_SUBSTITUTION
		;

command_output
		:	COMMAND_OUTPUT
		|	COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION^
			expression_substituation	{tellLexerWeHaveFinishedParsingStringExpressionSubstituation();}
			(STRING_BETWEEN_EXPRESSION_SUBSTITUTION	expression_substituation	{tellLexerWeHaveFinishedParsingStringExpressionSubstituation();})*
			STRING_AFTER_EXPRESSION_SUBSTITUTION
		;

heredoc
		:	HERE_DOC_BEGIN!
			(HERE_DOC_CONTENT
			|HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION^
			expression_substituation	{tellLexerWeHaveFinishedParsingHeredocExpressionSubstituation();}
			(HERE_DOC_BETWEEN_EXPRESSION_SUBSTITUTION	expression_substituation	{tellLexerWeHaveFinishedParsingHeredocExpressionSubstituation();})*
			HERE_DOC_AFTER_EXPRESSION_SUBSTITUTION
			)
		;

literal
		:	regex
		|	(options{greedy=true;/*caused by command*/}:string)+{ #literal = #([STRING, "STRING"], #literal); }
		|	heredoc
		|	command_output
		|	symbol
		|	W_ARRAY
		;

numeric
		:	INTEGER
		|	HEX
		|	BINARY
		|	OCTAL
		|	FLOAT
		|	ASCII_VALUE
		;

primaryExpressionThatCanBeMethodName
		:	FUNCTION	{can_be_command_ = 2;}
		|	IDENTIFIER		//this is OK: p = 1; p 1
		|	"self"
		|	"super"		{can_be_command_ = 2;}
		|	CONSTANT
		|	LEADING_COLON2	CONSTANT
		|	"retry"
		|	"yield"		{can_be_command_ = 2;}
		|	keyword_defined
		|	"redo"
		|	EMPTY_ARRAY
		|	EMPTY_ARRAY_ACCESS
		|	UNARY_PLUS_MINUS_METHOD_NAME
		;

primaryExpressionThatCanNotBeMethodName
		:	INSTANCE_VARIABLE
		|	GLOBAL_VARIABLE
		|	CLASS_VARIABLE
		|	predefinedValue
		|	numeric
		|	literal
		|	arrayExpression
		|	hashExpression
		|	LPAREN^	compoundStatement RPAREN!
		|	LPAREN_WITH_NO_LEADING_SPACE^ compoundStatement RPAREN!
		|	ifExpression
		|	unlessExpression
		|	whileExpression
		|	untilExpression
		|	caseExpression
		|	forExpression
		|	exceptionHandlingExpression
		|	moduleDefinition
		|	classDefinition
		|	methodDefinition
		;

primaryExpression
		:	primaryExpressionThatCanNotBeMethodName
		|	primaryExpressionThatCanBeMethodName
		;

arrayReferenceArgument
		:	keyValuePair
			(	COMMA! {if (ASSIGN == LA(1)||RBRACK == LA(1)) break;}
				(seen_star:REST_ARG_PREFIX)?	keyValuePair	{if (null != seen_star) break;}
			)*
		|	REST_ARG_PREFIX	expression	(LINE_BREAK!)?
		;

arrayExpression
		:	LBRACK^
				(arrayReferenceArgument)?
			RBRACK!
		;

keyValuePair
		:	expression	(ASSOC	expression)?	(LINE_BREAK!)?
		;

hashExpression
		:	LCURLY_HASH^
				(
					keyValuePair
					(options{greedy=true;}:{RCURLY != LA(2)}?	COMMA!	keyValuePair)*
					(COMMA!)?
				)?
			RCURLY!
		;

bodyStatement
			:	(compoundStatement
				("rescue" exceptionList thenOrTermialOrColon (compoundStatement)?)*
				("else" (compoundStatement)?)?
				("ensure" (compoundStatement)?)?
			|	("rescue" exceptionList thenOrTermialOrColon (compoundStatement)?)+
				("else" (compoundStatement)?)?
				("ensure" (compoundStatement)?)?
			|	("ensure" (compoundStatement)?)
			)
			{#bodyStatement = #(#[BODY, "BODY"], #bodyStatement);}
		;

exceptionHandlingExpression
		:	"begin"^
			(bodyStatement)?
			"end"!
		;

exceptionList
		:	((className|INSTANCE_VARIABLE|IDENTIFIER)	(COMMA!	(className|INSTANCE_VARIABLE|IDENTIFIER))*)?	(ASSOC	variable)?
		|	GLOBAL_VARIABLE
		;

ifExpression
		:	"if"^		(LINE_BREAK!)?
			expression thenOrTermialOrColon (compoundStatement)?
			("elsif" (expression)? thenOrTermialOrColon (compoundStatement)?)*
			("else" (compoundStatement)?)?
			"end"!
		;

unlessExpression
		:	"unless"^		(LINE_BREAK!)?
			expression thenOrTermialOrColon (compoundStatement)?
			("else" (compoundStatement)?)?
			"end"!
		;

caseExpression
		:	"case"^ (expression)? thenOrTermialOrColon		//(compoundStatement)?
			(keyword_when mrhs	thenOrTermialOrColon (compoundStatement)?)+
			("else" (compoundStatement)?)?
			"end"!
		;

forExpression
		:	"for"^	(LINE_BREAK!)?
			block_vars
			"in"		(LINE_BREAK!)?
			expression doOrTermialOrColon
			(compoundStatement)?
			"end"!
		;

whileExpression
		:	"while"^	(LINE_BREAK!)?
			expression doOrTermialOrColon
			(compoundStatement)?
			"end"!
		;

untilExpression
		:	"until"^	(LINE_BREAK!)?
			expression doOrTermialOrColon
			(compoundStatement)?
			"end"!
		;

moduleDefinition
		:	"module"^	(LINE_BREAK!)?
			moduleName (options {greedy=true;}:terminal)?	{enterScope();}
			(bodyStatement)?
			"end"!			{leaveScope();}
		;

moduleName
		:	CONSTANT	(COLON2^ CONSTANT)*
		| 	(LEADING_COLON2	CONSTANT)	(COLON2^ CONSTANT)*
		;

classDefinition
		:	"class"^	(LINE_BREAK!)?
			(	className	(LESS_THAN expression)?
			|	LEFT_SHIFT	expression
			)
			terminal			{enterScope();}
			(bodyStatement)?
			"end"!			{leaveScope();}
		;

/*
FIXME
cname	: tIDENTIFIER
		| tCONSTANT
		;

cpath	: tCOLON3 cname
		| cname
		| primary_value tCOLON2 cname
		;
*/
className
		:	(CONSTANT|FUNCTION)	(COLON2^	CONSTANT)*
		|	(LEADING_COLON2	CONSTANT)	(COLON2^	CONSTANT)*
		;

methodDefinition
		:	"def"^		{++is_in_method_definition;}
			(options{greedy=true;}:LINE_BREAK!)?
			methodName	{enterScope();}
			methodDefinitionArgument
			(bodyStatement)?
			"end"!		{leaveScope();--is_in_method_definition;}
		;

protected
methodNameSupplement
		:	(DOT|COLON2)
			(IDENTIFIER	(ASSIGN_WITH_NO_LEADING_SPACE)?
			|FUNCTION	(ASSIGN_WITH_NO_LEADING_SPACE)?
			|CONSTANT	(ASSIGN_WITH_NO_LEADING_SPACE)?
			|operatorAsMethodname
			|keyword
			)
		;

methodName
{boolean is_singleton_method = false;}
		:	(	operatorAsMethodname
			|	keywordAsMethodName
			|	(id:IDENTIFIER
				|function:FUNCTION
				|constant:CONSTANT
				)
				(methodNameSupplement{is_singleton_method = true;}|ASSIGN_WITH_NO_LEADING_SPACE)?
			|	(LPAREN!|LPAREN_WITH_NO_LEADING_SPACE!)
				expression	(LINE_BREAK!)?
				RPAREN!
				methodNameSupplement{is_singleton_method = true;}
			|	(INSTANCE_VARIABLE|CLASS_VARIABLE|GLOBAL_VARIABLE )	methodNameSupplement	{is_singleton_method = true;}
			|	("nil"|"self"|"true"|"false"|"__FILE__"|"__LINE__")	methodNameSupplement	{is_singleton_method = true;}
			|	UNARY_PLUS_MINUS_METHOD_NAME
			)
			{
				if (is_singleton_method) {
					#methodName = #(#[SINGLETON_METHOD, "SINGLETON_METHOD"], #methodName);
				}
			}
		;

operatorAsMethodname
		:	LEFT_SHIFT
		|	RIGHT_SHIFT
		|	EQUAL
		|	CASE_EQUAL
		|	GREATER_THAN
		|	GREATER_OR_EQUAL
		|	LESS_THAN
		|	LESS_OR_EQUAL
		|	PLUS
		|	MINUS
		|	STAR
		|	DIV
		|	MOD
		|	POWER
		|	BAND
		|	BOR
		|	BXOR
		|	(EMPTY_ARRAY	|EMPTY_ARRAY_ACCESS)	(options{greedy=true;}:ASSIGN_WITH_NO_LEADING_SPACE)?
		|	MATCH
		|	COMPARE
		|	BNOT
		|	SINGLE_QUOTE
		;

keyword
		:	keywordAsMethodName
		|	"nil"
		|	"self"
		|	"true"
		|	"false"
		|	"__FILE__"
		|	"__LINE__"
		;

keywordAsMethodName
		:	"and"
		|	"def"
		|	"end"
		|	"in"
		|	"or"
		|	"unless"
		|	"begin"
		|	"defined?"
		|	"ensure"
		|	"module"
		|	"redo"
		|	"super"
		|	"until"
		|	"BEGIN"
		|	"break"
		|	"do"
		|	"next"
		|	"rescue"
		|	"then"
		|	"when"
		|	"END"
		|	"case"
		|	"else"
		|	"for"
		|	"retry"
		|	"while"
		|	"alias"
		|	"class"
		|	"elsif"
		|	"if"
		|	"not"
		|	"return"
		|	"undef"
		|	"yield"
		;

methodDefinitionArgument
		:	lparen
				(methodDefinitionArgumentWithoutParen)?
			RPAREN!	{tellLexerWeHaveFinishedParsingMethodparameters();}
			(options {greedy=true;}:terminal)?
		|	(methodDefinitionArgumentWithoutParen)?	terminal
		;

methodDefinitionArgumentWithoutParen
{boolean seen_star_or_band = false;}
		:	normalMethodDefinitionArgument
				(COMMA!	{if (REST_ARG_PREFIX == LA(1) || BLOCK_ARG_PREFIX == LA(1))	{seen_star_or_band = true;break;}}	normalMethodDefinitionArgument)*
				(	{seen_star_or_band}?	restMethodDefinitionArgument
					|{seen_star_or_band}?	blockMethodDefinitionArgument
				)?
		|	restMethodDefinitionArgument
		|	blockMethodDefinitionArgument
		;

normalMethodDefinitionArgument
		:	variable	((ASSIGN|ASSIGN_WITH_NO_LEADING_SPACE)	expression)?
		;

variable
		:	id1:IDENTIFIER	{addVariable(id1);}
		|	id2:FUNCTION		{addVariable(id2);}
		;

restMethodDefinitionArgument
		:	REST_ARG_PREFIX	(variable	(COMMA!	blockMethodDefinitionArgument)?)?
		;

blockMethodDefinitionArgument
		:	BLOCK_ARG_PREFIX	variable
		;


thenOrTermialOrColon
		:	"then"!
		|	terminal	("then"!)?
		|	COLON!
		;

doOrTermialOrColon
		:	DO_IN_CONDITION!
		|	terminal
		|	COLON!
		;

operator_ASSIGN				:	(ASSIGN|ASSIGN_WITH_NO_LEADING_SPACE)				(options{greedy=true;}:LINE_BREAK!)?;

//LINE_BREAK is preserved after keyword, we have to do that because keyword can be used as function name.
//for example:
//  def class
//        Registry
//      end
//So the following rules are created so that we do not hve to put (LINE_BREAK)? everywhere.
//
keyword_BEGIN	:	"BEGIN"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_END		:	"END"		(options{greedy=true;}:LINE_BREAK!)?;
keyword___FILE__	:	"__FILE__"	(options{greedy=true;}:LINE_BREAK!)?;
keyword___LINE__	:	"__LINE__"	(options{greedy=true;}:LINE_BREAK!)?;
//keyword_alias		:	"alias"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_and		:	"and"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_begin	:	"begin"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_break	:	"break"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_case		:	"case"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_class	:	"class"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_def		:	"def"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_defined	:	"defined?"	(options{greedy=true;}:LINE_BREAK!)?;
keyword_do		:	"do"			(options{greedy=true;}:LINE_BREAK!)?;
keyword_else		:	"else"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_elsif		:	"elsif"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_end		:	"end"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_ensure	:	"ensure"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_false		:	"false"		(options{greedy=true;}:LINE_BREAK!)?;
//keyword_for	:	"for"			(options{greedy=true;}:LINE_BREAK!)?;
//keyword_if		:	"if"			(options{greedy=true;}:LINE_BREAK!)?;
//keyword_in		:	"in"			(options{greedy=true;}:LINE_BREAK!)?;
keyword_module	:	"module"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_next		:	"next"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_nil		:	"nil"			(options{greedy=true;}:LINE_BREAK!)?;
keyword_not		:	"not"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_or		:	"or"			(options{greedy=true;}:LINE_BREAK!)?;
keyword_redo		:	"redo"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_rescue	:	"rescue"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_retry	:	"retry"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_return	:	"return"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_self		:	"self"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_super	:	"super"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_then		:	"then"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_true		:	"true"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_undef	:	"undef"		(options{greedy=true;}:LINE_BREAK!)?;
//keyword_unless	:	"unless"		(options{greedy=true;}:LINE_BREAK!)?;
//keyword_until	:	"until"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_when	:	"when"		(options{greedy=true;}:LINE_BREAK!)?;
//keyword_while	:	"while"		(options{greedy=true;}:LINE_BREAK!)?;
keyword_yield		:	"yield"		(options{greedy=true;}:LINE_BREAK!)?;


//----------------------------------------------------------------------------
// The Ruby scanner. Do not use this one, use RubyLexer instead!
//----------------------------------------------------------------------------
class RubyLexerBase extends Lexer;

options {
	exportVocab=Ruby;	// call the vocabulary "Ruby"
	testLiterals=false;		// don't automatically test for literals
	k=3;				// "<=>" etc need this
	charVocabulary='\u0000'..'\uFFFE';
	//codeGenMakeSwitchThreshold = 2;
	codeGenBitsetTestThreshold=20;
}

{
	//The following methods are to be implemented in the subclass.
	//In fact they should be "abstract", but antlr refuses to generate
	//abstract class. We can either insert "abstract" keyword manually
	//after the lexer is generated, or simply use assert() to prevent
	//these function to run (so you have to overide them). I choosed
	//the later approach.
	protected boolean expectOperator(int k) throws CharStreamException		{assert(false);return false;}
	protected boolean expectUnary()	 throws CharStreamException			{assert(false);return false;}
	protected boolean expectHash()					{assert(false);return false;}
	protected boolean expectHeredoc()				{assert(false);return false;}
	protected boolean expectLeadingColon2()		{assert(false);return false;}
	protected boolean expectArrayAccess()				{assert(false);return false;}
	protected boolean lastTokenIsDotOrColon2()		{assert(false);return false;}
	protected boolean lastTokenIsSemi()				{assert(false);return false;}
	protected boolean lastTokenIsKeywordDefOrColonWithNoFollowingSpace()			{assert(false);return false;}
	protected boolean lastTokenIsColonWithNoFollowingSpace()			{assert(false);return false;}
	protected boolean shouldIgnoreLinebreak()			{assert(false);return false;}
	protected int trackDelimiterCount(char next_char, char delimeter, int delimeter_count)	{assert(false);return 0;}
	protected boolean isDelimiter(String next_line, String delimiter)	{assert(false);return false;}
	protected boolean isAsciiValueTerminator(char value)	{assert(false);return false;}
	protected boolean justSeenWhitespace()	{assert(false);return false;}
	protected void setSeenWhitespace()			{assert(false);}
	protected boolean expressionSubstitutionIsNext()	throws CharStreamException	{assert(false);return false;}
	protected boolean spaceIsNext()	throws CharStreamException	{assert(false);return false;}
	protected void setCurrentSpecialStringDelimiter(char delimiter, int delimiter_count)	{assert(false);}
	protected void updateCurrentSpecialStringDelimiterCount(int delimiter_count)	{assert(false);}
}

//QUESTION			:	'?'		;
LPAREN				:	'('		{if (!justSeenWhitespace()) {$setType(LPAREN_WITH_NO_LEADING_SPACE);}};
RPAREN				:	')'		;
LBRACK				:	'['		{if (expectArrayAccess()) {$setType(LBRACK_ARRAY_ACCESS);}};
RBRACK				:	']'		;
EMPTY_ARRAY		:	"[]"		{if (expectArrayAccess()) {$setType(EMPTY_ARRAY_ACCESS);}};
LCURLY_HASH			:	'{'		{if (!expectHash()) {$setType(LCURLY_BLOCK);}};
RCURLY				:	'}'		;
COMMA				:	','		;
COLON				:	':'		{if (!spaceIsNext())	{$setType(COLON_WITH_NO_FOLLOWING_SPACE);}};
COLON2				:	"::"		{if (expectLeadingColon2())	{$setType(LEADING_COLON2);}};

NOT					:	'!'		;
BNOT				:	'~'		;
//DIV				:	'/'		;
PLUS				:	'+'		{if (expectUnary())	{$setType(UNARY_PLUS);}};
MINUS				:	'-'		{if (expectUnary())	{$setType(UNARY_MINUS);}};
//MOD				:	'%'		;
STAR				:	'*'		{if (!expectOperator(1)) {$setType(REST_ARG_PREFIX);}};	//'f * g' can parsed as 'f(*g)' or '(f) * (g)'
LESS_THAN			:	'<'		;
GREATER_THAN		:	'>'		;
BXOR				:	'^'		;
BOR					:	'|'		;
BAND				:	'&'		{if (!expectOperator(1)) {$setType(BLOCK_ARG_PREFIX);}};
POWER				:	"**"		;
COMPARE				:	"<=>"	;
GREATER_OR_EQUAL	:	">="		;
LESS_OR_EQUAL		:	"<="		;
EQUAL				:	"=="		;
CASE_EQUAL			:	"==="	;
NOT_EQUAL			:	"!="		;
MATCH				:	"=~"		;
NOT_MATCH			:	"!~"		;
//LEFT_SHIFT		:	"<<"		;
RIGHT_SHIFT			:	">>"		;

ASSOC				:	"=>"		;
LOGICAL_AND			:	"&&"		;
LOGICAL_OR			:	"||"		;

ASSIGN				:	'='		{if (!justSeenWhitespace()) {$setType(ASSIGN_WITH_NO_LEADING_SPACE);}};
PLUS_ASSIGN			:	"+="	;
MINUS_ASSIGN		:	"-="		;
STAR_ASSIGN			:	"*="		;
//DIV_ASSIGN		:	"/="		;
//MOD_ASSIGN		:	"%="	;
POWER_ASSIGN		:	"**="	;
BAND_ASSIGN			:	"&="		;
BXOR_ASSIGN			:	"^="		;
BOR_ASSIGN			:	"|="		;
//LEFT_SHIFT_ASSIGN	:	"<<="	;
RIGHT_SHIFT_ASSIGN	:	">>="	;
LOGICAL_AND_ASSIGN	:	"&&="	;
LOGICAL_OR_ASSIGN	:	"||="	;


//ANTLR's linear approximate lookahead will cause trouble if you list UNARY_PLUS and UNARY_MINUS as two separated rules.
UNARY_PLUS_MINUS_METHOD_NAME
		:	{lastTokenIsKeywordDefOrColonWithNoFollowingSpace() ||
			lastTokenIsDotOrColon2()}?
			("+@"|"-@")
		;

SEMI
{
	boolean seen_line_feed = false;
}
		:	';'	(options{greedy=true;}:(WHITE_SPACE_CHAR!	|	LINE_FEED!{seen_line_feed = true;}	|	';'!))*
			{
				if (lastTokenIsSemi())
				{
					$setType(Token.SKIP);
				} else if (seen_line_feed) {
					$setType(LINE_BREAK);
				}
			}
		;

//treat "\n\n\n\n;" as one LINE_BREAK
LINE_BREAK
		:	PURE_LINE_BREAK	(options{greedy=true;}:SEMI!)?
			{
				if ((LINE_BREAK == _ttype) && shouldIgnoreLinebreak())
				{
					$setType(Token.SKIP);
				}
			}
		;

//If we use "ignore=WHITE_SPACE_CHAR", can not shutdown the warnings.
protected
PURE_LINE_BREAK
		:	LINE_FEED	(options{greedy=true;}:(LINE_FEED!|WHITE_SPACE_CHAR!))*
		;

//'\r' is no longer used as line break since Mac OSX, but still in use in  C:\ruby\samples\hello.rb
protected
LINE_FEED
		:	('\n'
			|	'\r'!	(options{greedy=true;}:'\n')?
			)
			{newline();}
		;

protected
REGEX_MODIFIER
		:	('o'	|	'x'	|	'p'	|	'n'	|	'i'	|	'u'	|	'm'	|	's')*
		;

COMMAND_OUTPUT
		:	{!lastTokenIsKeywordDefOrColonWithNoFollowingSpace()}?
			delimiter:'`'!
			({LA(1) != delimiter && !expressionSubstitutionIsNext()}?	STRING_CHAR)*
			end:.!//skip delimiter
			{
				if (end != delimiter)
				{
					$setType(COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION);
					setCurrentSpecialStringDelimiter(delimiter, 1);
				}
			}
		|	'`'	{$setType(SINGLE_QUOTE);}
		;

SINGLE_QUOTE_STRING
		:	'\''!
				(ESC
				|LINE_FEED
				|~('\''|'\\'|'\r'|'\n')
				)*
			'\''!
		;

//DIVIDE and REGEX both starts with '/', here we use semantic predicate to disambiguate.
REGEX
		:	{!expectOperator(2)}?
			delimiter:'/'!
			({LA(1) != delimiter && !expressionSubstitutionIsNext()}?	STRING_CHAR)*
			end:.!	//skip delimiter
			{
				if (end != delimiter)
				{
					$setType(REGEX_BEFORE_EXPRESSION_SUBSTITUTION);
					setCurrentSpecialStringDelimiter(delimiter, 1);
				}
				else
				{
					text.append("/");
					mREGEX_MODIFIER(true);
				}
			}
		|	"/="	{$setType(DIV_ASSIGN);}
		|	'/'	{$setType(DIV);}
		;

DOUBLE_QUOTE_STRING
		:	delimiter:'\"'!
			({LA(1) != delimiter && !expressionSubstitutionIsNext()}?	STRING_CHAR)*
			end:.!//skip delimiter
			{
				if (end != delimiter)
				{
					$setType(STRING_BEFORE_EXPRESSION_SUBSTITUTION);
					setCurrentSpecialStringDelimiter(delimiter, 1);
				}
			}
		;

protected
STRING_BETWEEN_EXPRESSION_SUBSTITUTION[char delimiter, int delimiter_count]
		:	({(delimiter_count > 0) && (delimiter_count = trackDelimiterCount(LA(1), delimiter, delimiter_count)) != 0&& !expressionSubstitutionIsNext()}?	STRING_CHAR)*
			{
				//match and skip delimiter, there maybe no delimiter, e.g. ':#{cmd_name}'
				if (LA(1) != EOF_CHAR)
				{
					_saveIndex=text.length();
					matchNot(EOF_CHAR);
					text.setLength(_saveIndex);
				}

				if (0 == delimiter_count)
				{
					$setType(STRING_AFTER_EXPRESSION_SUBSTITUTION);
				}
				else
				{
					updateCurrentSpecialStringDelimiterCount(delimiter_count);
				}
			}
		;

SPECIAL_STRING
{
	int delimiter_count = 1;
}
		:	'%'!	'q'!	delimiter1:.!
			({(delimiter_count = trackDelimiterCount(LA(1), delimiter1, delimiter_count)) != 0}?	STRING_CHAR)*
			.!//skip delimiter
			{$setType(SINGLE_QUOTE_STRING);}
		|	'%'!	'Q'!	delimiter2:.!
			({(delimiter_count = trackDelimiterCount(LA(1), delimiter2, delimiter_count)) != 0 && !expressionSubstitutionIsNext()}?	STRING_CHAR)*
			{
				//match and skip delimiter
				_saveIndex=text.length();
				matchNot(EOF_CHAR);
				text.setLength(_saveIndex);

				if (0 == delimiter_count)
				{
					$setType(DOUBLE_QUOTE_STRING);
				}
				else
				{
					$setType(STRING_BEFORE_EXPRESSION_SUBSTITUTION);
					setCurrentSpecialStringDelimiter(delimiter2, delimiter_count);
				}
			}
		|	'%'!	'r'!	delimiter3:.!
			({(delimiter_count = trackDelimiterCount(LA(1), delimiter3, delimiter_count)) != 0 && !expressionSubstitutionIsNext()}?	STRING_CHAR)*
			{
				//match and skip delimiter
				_saveIndex=text.length();
				matchNot(EOF_CHAR);
				text.setLength(_saveIndex);

				if (0 == delimiter_count)
				{
					text.append("/");
					mREGEX_MODIFIER(true);
					$setType(REGEX);
				}
				else
				{
					$setType(REGEX_BEFORE_EXPRESSION_SUBSTITUTION);
					setCurrentSpecialStringDelimiter(delimiter3, delimiter_count);
				}
			}
		|	'%'!	'x'!	delimiter4:.!
			({(delimiter_count = trackDelimiterCount(LA(1), delimiter4, delimiter_count)) != 0 && !expressionSubstitutionIsNext()}?	STRING_CHAR)*
			{
				//match and skip delimiter
				_saveIndex=text.length();
				matchNot(EOF_CHAR);
				text.setLength(_saveIndex);

				if (0 == delimiter_count)
				{
					$setType(COMMAND_OUTPUT);
				}
				else
				{
					$setType(COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION);
					setCurrentSpecialStringDelimiter(delimiter4, delimiter_count);
				}
			}
		|	'%'! ('w'!|'W'!)	delimiter5:.!
			({(delimiter_count = trackDelimiterCount(LA(1), delimiter5, delimiter_count)) != 0}?	STRING_CHAR)*
			.!	//skip delimiter
			{$setType(W_ARRAY);}
		|	{!expectOperator(2)}?	'%'!
			{_saveIndex=text.length();}					//Ignore delimiter2 (for unknown reason, antlr does not do it for us, even if we specified !)
			delimiter6:~('=' | 'a'..'z' | 'A'..'Z' | '0'..'9')		//"%=" is always MOD_ASSIGN. English character is not allowed to avoid collison with %q %Q etc.
			{text.setLength(_saveIndex);}
			({(delimiter_count = trackDelimiterCount(LA(1), delimiter6, delimiter_count)) != 0 && !expressionSubstitutionIsNext()}?	STRING_CHAR)*
			{
				//match and skip delimiter
				_saveIndex=text.length();
				matchNot(EOF_CHAR);
				text.setLength(_saveIndex);

				if (0 == delimiter_count)
				{
					$setType(DOUBLE_QUOTE_STRING);
				}
				else
				{
					$setType(STRING_BEFORE_EXPRESSION_SUBSTITUTION);
					setCurrentSpecialStringDelimiter(delimiter6, delimiter_count);
				}
			}
		|	"%="	{$setType(MOD_ASSIGN);}
		|	"%"		{$setType(MOD);}
		;

protected
STRING_CHAR
		:	~('\r'|'\n'|'\\')
		|	LINE_FEED
		|	ESC
		;


//The first '-' after "<<" is alway interpreted as heredoc's special meaning, so be greedy
HERE_DOC_BEGIN
		:	{expectHeredoc()}?	"<<"!	HERE_DOC_DELIMITER
		|	"<<="	{$setType(LEFT_SHIFT_ASSIGN);}
		|	"<<"		{$setType(LEFT_SHIFT);}
		;

protected
HERE_DOC_CONTENT[String delimiter, int type1, int type2]
		:	(next_line:ANYTHING_OTHER_THAN_LINE_FEED_AND_POUND	{if (expressionSubstitutionIsNext()) break;}
			LINE_FEED	{if (isDelimiter(next_line.getText(), delimiter)) break;}
			)+
			{
				if (expressionSubstitutionIsNext())
				{
					//eat '#'
					_saveIndex=text.length();
					matchNot(EOF_CHAR);
					text.setLength(_saveIndex);
					$setType(type1);
				}
				else
				{
					//skip delimiter
					text.setLength(text.length() - next_line.getText().length() - 1);
					$setType(type2);
				}
			}
		;

protected
ANYTHING_OTHER_THAN_LINE_FEED_AND_POUND
		:	({!expressionSubstitutionIsNext()}?	(~('\r'|'\n'|'\\')|ESC))*
		;

protected
HERE_DOC_DELIMITER
		:	(options{greedy=true;}:'-')?
				((options{greedy=true;}:	~(' ' | '\n' | '\r' | '\'' | '"' | ',' | ')' | '=' | '.' | ';'))+
				|	'\''!	(~('\'' | '\r' | '\n'))+	'\''!
				|	'"'!	(~('"' | '\r' | '\n'))+	'"'!
				)
		;

RDOC
		:	{getColumn()==1}?	"=begin"
			(options {greedy=false;}:	LINE)*
			{getColumn()==1}?	"=end"
			{
				$setType(Token.SKIP);
			}
		;

protected
ANYTHING_OTHER_THAN_LINE_FEED
		:	(~('\r'|'\n'))*
		;

protected
LINE
		:	ANYTHING_OTHER_THAN_LINE_FEED	LINE_FEED
		;

//It's OK as long as the next char(whatever it is) is eaten
protected
ESC
		:	'\\' .
		;

// An identifier.  Note that testLiterals is set to true!  This means
// that after we match the rule, we look in the literals table to see
// if it's a literal or really an identifer
//Following this initial character, an identifier can be any combination
//of letters, digits, and underscores
IDENTIFIER
options{testLiterals=true;}
		:	{getColumn() == 1}?	"__END__"	((WHITE_SPACE_CHAR)*	LINE_BREAK	(.)*	{$setType(Token.EOF_TYPE);})?
		|	('a'..'z'|'_') ('a'..'z'|'A'..'Z'|'_'|'0'..'9')*		(	{if (lastTokenIsDotOrColon2()) {$setType(FUNCTION);}}
												|'?'	{$setType(FUNCTION);}	//PREDICATE_FUNCTION
												|'!'	{$setType(FUNCTION);}	//DESTRUCTIVE_FUNCTION
												)
		;

GLOBAL_VARIABLE
		:	'$'	'-'	(IDENTIFIER_CONSTANT_AND_KEYWORD|'#')?
		|	'$'	IDENTIFIER_CONSTANT_AND_KEYWORD
		|	'$'	(options{greedy=true;}:'0'..'9')+
		|	'$'	('!'|'@'|'&'|'`'|'\''|'+'|'~'|'='|'/'|'\\'|','|';'|'.'|'<'|'>'|'*'|'$'|'?'|':'|'\"')
		;

protected
IDENTIFIER_CONSTANT_AND_KEYWORD
		:	('a'..'z'|'A'..'Z'|'_')	(options{greedy=true;}:	'a'..'z'|'A'..'Z'|'_'|'0'..'9')*
		;

INSTANCE_VARIABLE
		:	'@'	IDENTIFIER_CONSTANT_AND_KEYWORD
		;

CLASS_VARIABLE
		:	'@'	INSTANCE_VARIABLE
		;

CONSTANT
options{testLiterals=true;}
		:	('A'..'Z')	('a'..'z'|'A'..'Z'|'_'|'0'..'9')*
		;

INTEGER
		:	'0'!	OCTAL_CONTENT			{$setType(OCTAL);}
		|	'0'!	('b'!|'B'!)	BINARY_CONTENT	{$setType(BINARY);}
		|	'0'!	('x'!|'X'!)	HEX_CONTENT	{$setType(HEX);}
		|	'0'	(
					//Use semantic prediction to avoid 0.times
					{(LA(2)>='0')&&(LA(2)<='9')}? FLOAT_WITH_LEADING_DOT {$setType(FLOAT);}
					|/*none*/
				)
		|	NON_ZERO_DECIMAL	(
									//Use semantic prediction to avoid things like "2..3", "2...3", "2.times"
									{(LA(2)>='0')&&(LA(2)<='9')}? FLOAT_WITH_LEADING_DOT {$setType(FLOAT);}
									|EXPONENT {$setType(FLOAT);}
									|/*none*/
								)
		|	FLOAT_WITH_LEADING_DOT	{$setType(FLOAT);}
		|	'.'	{$setType(DOT);}
		|	".."	{$setType(INCLUSIVE_RANGE);}
		|	"..."	{$setType(EXCLUSIVE_RANGE);}
		|	'?'	(
					{isAsciiValueTerminator(LA(2))}?	(~('\\'|' '|'\n'|'\r'))	{$setType(ASCII_VALUE);}
					|'\\'		~('C' | 'M' | 'x')	{$setType(ASCII_VALUE);}
					|('\\'	('C'|'M') '-')+	('a'..'z' | '?')	{$setType(ASCII_VALUE);}
					|'\\'	'x'	HEX_CONTENT	{$setType(ASCII_VALUE);}
					|{$setType(QUESTION);}	//If it does not "look like"(not depend on context!) integer, then it is QUESTION operator.
				)
		;

protected
UNDER_SCORE
		:	'_'
		;

protected
FLOAT_WITH_LEADING_DOT
		:	'.'	('0'..'9')+	(EXPONENT)?
		;

protected
NON_ZERO_DECIMAL
options{ignore=UNDER_SCORE; }
		:	('1'..'9'	('0'..'9')*)
		;

protected
OCTAL_CONTENT
options{ignore=UNDER_SCORE;}
		:	('0'..'7')+
		;

protected
HEX_CONTENT
options{ignore=UNDER_SCORE;}
		:	('0'..'9'|'A'..'F'|'a'..'f')+
		;

protected
BINARY_CONTENT
options{ignore=UNDER_SCORE;}
		:	('0'|'1')+
		;

protected
EXPONENT
		:	('e'|'E')	('+'|'-')?	('0'..'9')+
		;

COMMENT
		:	{!lastTokenIsColonWithNoFollowingSpace()}?	'#'	ANYTHING_OTHER_THAN_LINE_FEED
			{
				$setType(Token.SKIP);
			}
		|	'#'!
			{
				$setType(STRING_BEFORE_EXPRESSION_SUBSTITUTION);
				setCurrentSpecialStringDelimiter('#'/*useless*/, 0);
			}
		;

protected
WHITE_SPACE_CHAR
		:	' '
		|	'\t'
		|	'\f'
		|	'\13'
		;

WHITE_SPACE
		:	(WHITE_SPACE_CHAR)+
			{
				setSeenWhitespace();
				$setType(Token.SKIP);
			}
		;

LINE_CONTINUATION
		:	'\\'	LINE_FEED	{$setType(Token.SKIP);}
		;

//Except the pysical end of file(antlr will detect it for us),
//the following special characters are treated as end of file as well.
END_OF_FILE
		:	('\0'		// NULL
      			|'\004'	// ^D
      			|'\032'	// ^Z
      			)
      			{$setType(Token.EOF_TYPE);}
		;

