////////////////////////////////////////////////////////////////////////////////
// RegExp.cpp
////////////////////////////////////////////////////////////////////////////////
//
//  Modifications for Browser, 2008
//  This file has been extensively modified to clarify the functionality of the
//  code but the functionality its self has remained largely untouched, the
//  only change being to make the matching case insensitive and memory leaks
//  removed.
//
////////////////////////////////////////////////////////////////////////////////

#include "regexp.h"

//  When the regular expression is parsed certain sequences of characters will
//  be converted into 'Operators'.  The table below shows the operators which 
//  can be formed, each operator is assigned a unique number between 0 and 39.
//  Operators may or may not have an operand, e.g. the ANYOF operator will have
//  a series of characters as its operand and it will match any of the sequence
//  of characters.  The operand can be either a string or a 'node', which is
//  just another operator, its 'next operator' pointer and its associated operand.
//  The re column contains the sequence of characters which
//  will map to the operator where applicable

// definition	number	operand?	re	meaning 
#define	END		0		// no			End of m_compiledExpression, the final node. 
#define	BOL		1		// no		^	Match beginning of line. 
#define	EOL		2		// no		$	Match end of line. 
#define	ANY		3		// no		.	Match any character. 
#define	ANYOF	4		// str		[	Match any of the characters in the operand. 
#define	ANYBUT	5		// str		[^	Match none of the characters in the operand. 
#define	BRANCH	6		// node		|	Branches are created either side of the |, Match this node or the branch's node e.t.c. 
#define	BACK	7		// no			"next" ptr points backward.  Used to implement loops, e.g. when matching a* to aaa will loop 3 times before matching. 
#define	EXACTLY	8		// str			Match the string in the operand exactly. 
#define	NOTHING	9		// no			Match empty string. Used to implement choice, e.g. a? will either match 'a' or an empty string.
#define	STAR	10		// node		*	Match the node 0 or more times.  A* will match <empty>, A, AA, AAA e.t.c.
#define	PLUS	11		// node		+	Match this 1 or more times. A+ will match A, AA, AAA e.t.c.
#define	OPEN	20		// no		(	Open bracket.  Brackets are used to evaluate 
						//				portions of the regular expression as a unit, just as in
						//				mathematics.  They are also used to remember portions
						//				of the expression for replacing later.
						//				Maximum 10 bracketed expressions, OPEN+1 is bracket 1 etc. 
#define	CLOSE	30		// no		)	Close an open bracket, CLOSE+1 is bracket 1 etc. 

// Utility definitions.

//  FAIL(m) is not used by the regexp class and can be ignored 
#define	FAIL(m)		{ regerror(m); return(NULL); }
//  Returns whether the given operator is a repitition operator (i.e. *, + or ?)
#define	IS_REPETITION_OP(c)	((c) == '*' || (c) == '+' || (c) == '?')
//  Sequence of characters which can not be present in a string being matched 
//  exactly (operand to the EXACTLY operator), e.g. Browser.  
#define	META		"^$.[()|?+*\\"

//  The following definitions are called when the parser experiences issues.
//  TRACE0 is more serious.  The output will be the reason why the regular 
//  expression did not compile.
//  ToDo - point these to the Browser SpbLog file
#define TRACE0(arg) OutputDebugString(TEXT(arg))
#define TRACE1 TRACE0

// Flags to be passed up and down.
//  The flags maintain attributes of the regular expression branch currently 
//  being processed.  Flags can be used to:
//  1: Process the regular expression more efficiently, e.g. checking the 
//     expression's branch contains a *, which could be greedy, and ensuring
//	   the longest string in the expression is matched before processing the *.
//     So matching aa to a*aaaa will fail before processing the * as 'aaaa' 
//	   is not in 'aa'
//  2: Checking for syntactic correctness, e.g. the character preceeding a '?'
//     can not be a null string, HASWIDTH.
//  3: Determining the complexity of the compiled output, e.g. it is simpler to
//     parse b* than it is to parse (blah)* as the latter contains a sub expression.
//     The former will be output as STAR EXACTLY (b) whereas the latter will need
//     separate branches (BRANCH) and looping (BACK) in the compiled expression
//  HASWIDTH is used to keep track of whether the branch or node can be empty and
//  is used to determine whether the operand of * or + is valid (as they cannot exist
//  on their own.  .* = valid; * = not valid.  The '.' has width.
#define	HASWIDTH	01	// Known never to match null string. 
//  SIMPLE is used to determine the complexity of the generated output, see 3: above
//  In b* the 'b' is simple but in (blah)* the '(blah)' is not simple.
#define	SIMPLE		02	// Simple enough to be STAR/PLUS operand. 
//  SPSTART is used to determine whether the branch in the compiled output starts
//  with a * or a +, note that the * or + will not appear first in the uncompiled 
//  regular expression.  It is used as described in 1: above to mitigate against
//  the inefficiencies of inefficient operators.
#define	SPSTART		04	// Starts with * or +. 
//  WORST flag indicates that none of the above apply, It is the worst case because
//  no efficiency savings can be made.
#define	WORST		0	// Worst case. 

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
CRegExp::CRegExp()
	:m_bCompiled(FALSE)				//  Whether the expression has been compiled.
	,m_compiledExpression(NULL)		//  The compiled form of the regular expression
	,m_szLastMatchedString(NULL)				//  The last string successfully matched to the regular expression.
{
	//  Browser Modification:
	//  these variables are now initialised in the constructor call
	/*m_bCompiled = FALSE;
	m_compiledExpression = NULL;
	m_szLastMatchedString = NULL;*/

	//  For each of the possible sub expressions (bracketed portions of regular expression)
	for( int i = 0; i < NSUBEXP; i++ )
	{
		//  Initialise the associated matched tag expression arrays, arrays
		//  which store the portions of the input string which match the 
		//  bracketed portions in the regular expression.
		m_szStartp[i] = NULL;
		m_szEndp[i] = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
CRegExp::~CRegExp()
{
	//  Free memory associated with compiled regular expression, modified for Browser,
	//  previous version was missing []
	delete[] m_compiledExpression;		
	//  Free memory associated with (ToDo - what is this?), modified for 
	// Browser, previous version was missing []
	delete[] m_szLastMatchedString;
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
CRegExp* CRegExp::CompileRegularExpression(const char *regularExpression)
{
	//  Pointer to compiled regular expression (m_compiledExpression), used to navigate over
	//  m_compiledExpression without altering it.
	char *pCompiledExpression;

	//  flags variable passed to subroutines to determine the nature of the 
	//  compiled expression
	int flags;

	//  Test if the regularExpression to compile is not NULL
	if (regularExpression == NULL)
		return NULL;
	
	// First pass: determine size, legality. 
	//  The regular expression is parsed twice, first to ascertain the compiled
	//  expression size and to determine syntactic correctness, secondly to
	//  actually output the compiled expression.  m_bOutputCompiledExpression is used to keep 
	//  track of which pasre is being performed, setting it to false lets other
	//  subroutines know not to actually modify the compiled expression.
	m_bOutputCompiledExpression = FALSE;

	//  Set the m_regularExpression member variable to the input regular expression, 
	//  m_regularExpression will point to the current character in the input expression
	//  being processed.
	m_regularExpression = (char *)regularExpression;
	
	//  Maximum number of parentheses is hard coded as 10, reset the number of
	//  brackets found in the regularExpression to 1.  This actually means the
	//  first '(' found will be bracket 1.
	m_countOfParentheses = 1;

	//  The size of the compiled regular expression in bytes, this variable is 
	//  set on the first pass over the expression by reg().
	m_compiledExpressionSize = 0L;

	//  Declare a dummy node containing no data.  This serves no purpose other
	//  than to provide an initialiser for m_lastByteInCompiledExpression.
	m_regdummy[0] = NOTHING;
	m_regdummy[1] = m_regdummy[2] = 0;
	//  m_lastByteInCompiledExpression is a pointer to keep track of the position of the last byte in  
	//  the compiled regular expression, it is set to NULL for the first pass
	//  over the regular expression though its value only accessed or modified
	//  on the second pass.  Either initialising is being done for good practise
	//  or it is legacy functionality which has been overtaken by the m_bOutputCompiledExpression
	//  variable (which keeps track of which pass is being performed)
	m_lastByteInCompiledExpression = m_regdummy;

	//  Parse the regular expression, because m_bOutputCompiledExpression is false
	//  no output will be generated.  We want to compile the entire expression
	//  and so the value of 'paren' is set to 0.
	//  m_compiledExpressionSize will be populated by this call as will the flags
	//  variable.  If m_regularExpression does not parse, i.e. it is not syntactically
	//  correct and failure is returned.
	if (CompileExpression(0, &flags) == NULL)
		return(NULL);

	// Allocate space. 
	//  Browser modification - improved memory management
	//  Delete any previous compiled expression
	delete[] m_compiledExpression;
	m_compiledExpression = NULL;	
	//  End Browser modifications

	//  Create memory to store the compiled expression in
	m_compiledExpression = new char[m_compiledExpressionSize];

	//  Test to see if the memory failed to be allocated and return if failure.
	if (m_compiledExpression == NULL)
		return NULL;
	
	//  Browser modification
	//   - Moved this line below testing if progam is NULL
	//  Set the contents of the compiled expression to NULL
	memset( m_compiledExpression, 0, m_compiledExpressionSize * sizeof(char) ); 

	//  Second pass over regular expression, produce compiled expression:
	//  regularExpression will compile and we have allocated an area of
	//  memory in which to store the compiled expression.  Actually output the
	//  compiled expression.
	m_bOutputCompiledExpression = TRUE;
	//  Reset the member variables which were modified during the first
	//  call to 'CompileExpression()'
	m_regularExpression = (char *)regularExpression;
	m_countOfParentheses = 1;
	m_lastByteInCompiledExpression = m_compiledExpression;
	
	//  Call CompileExpression again on the entire expression with 'paren' set
	//  to 0.  This will instantiate flags and populate m_compiledExpression.
	//  This call _should not_ fail as we have already checked the expression
	//  for syntactic correctness.
	if (CompileExpression(0, &flags) == NULL)
		return NULL;

	///////////////////////////////////////////////////////
	// 
	//  The expression is now compiled.  The remainder of this
	//  method is to provide efficiency savings during pattern
	//  matching
	//
	///////////////////////////////////////////////////////

	//  This object has attempted to compile an expression, MatchString will now not
	//  fail immediately - Todo: What happens if the expression fails to compile and we do a find?
	/*  Todo: CompileRegularExpression sets m_bCompiled to TRUE irrespective of whether or not the compile is successful. 
	    This can cause problems with MatchString trying to access memory that has not been 
		allocated or initialized if MatchString is run after a failed CompileRegularExpression. 
	*/
	m_bCompiled = TRUE;

	// Dig out information for optimizations. 

	//  The worst case defaults for no optimisation being performed are as follows:
	m_efficiencyStartChar = '\0';	//  \0 is special case meaning start character is unknown
	m_efficiencyStartsAnchor = 0;		//  False, regular expression does not start with '^'
	m_efficiencyLongestString = NULL;		//  Reg Expression does not contain a string literal
	m_efficiencyLongestStringlen = 0;		//  Reg Expression does not contain a string literal
	pCompiledExpression = m_compiledExpression;		// First BRANCH of compiled expression. 
	//  Efficiency savings are only applicable if there is only one branch in
	//  the expression, i.e. $4|5 will not have any savings applied.
	//  'pCompiledExpression' points to the first branch in the compiled expression (which is always
	//  the first node m_compiledExpression[0])
	//  The final node is always END
	//  If OP(NextNode(m_compiledExpression[0])) == END there are no '|' in the 
	//  regular expression
	if (OP(NextNode(pCompiledExpression)) == END) 
	{	
		// Only one top-level choice. 
		//  Set pCompiledExpression to the operand of the first (and only) BRANCH operator, this
		//  will be the first non-BRANCH node in the regular expression, e.g. for the 
		//  expression '.exp' this will be ANY, for the expression 'exp' this will
		//  be EXACTLY, for the expression '^exp' this will be BOL.
		pCompiledExpression = OPERAND(pCompiledExpression);

		// Starting-point info. 
		//  If the first node in the expression is EXACTLY then set the m_efficiencyStartChar
		//  variable to the operand of EXACTLY, which will be the string to
		//  match.
		if (OP(pCompiledExpression) == EXACTLY)
			m_efficiencyStartChar = *OPERAND(pCompiledExpression);
		//  If the first node in the expression is BOL then set the m_efficiencyStartsAnchor
		//  variable to true as the first character is a '^'
		else if (OP(pCompiledExpression) == BOL)
			m_efficiencyStartsAnchor = 1;

		//  Browser: The following comment which came shipped with the
		//  original code is confusing and is better explained beneath.
		// If there's something expensive in the r.e., find the
		// longest literal string that must appear and make it the
		// m_efficiencyLongestString.  Resolve ties in favor of later strings, since
		// the m_efficiencyStartChar check works with the beginning of the r.e.
		// and avoiding duplication strengthens checking.  Not a
		// strong reason, but sufficient in the absence of others.

		//  The final efficiency test is to set the m_efficiencyLongestString variable to the
		//  length of the longest string literal, e.g. the expression 
		//  string.*longerstring will set m_efficiencyLongestString to 'longerstring'.
		//  This only has an efficiency saving if the regular expression starts
		//  with a greedy operator, * or +, consider matching the input string
		//  aaaaaaa to a*aaa.  The first attempt to match will assign the entire
		//  input to the a* portion of the pattern then fail as there is no input 
		//  left to match the remaining 'aaa' portion of the pattern.  The matching
		//  routine will then backtrack and match the a* to 'aaaaaa' leaving only 
		//  a single 'a' in the input string, which will again fail.  This 
		//  back tracking continues until a match is found and can be computationally 
		//  expensive.

		//  The SPSTART flag keeps track of whether the expression starts with 
		//  a greedy operator or not.
		if (flags&SPSTART) 
		{
			//  Expression does contain a greedy operator, find the longest literal
			//  string in the expression.
			char *longest = NULL;
			size_t len = 0;

			//  For each node in the compiled regular expression
			for (; pCompiledExpression != NULL; pCompiledExpression = NextNode(pCompiledExpression))
				//  If the node is EXACTLY and the length of it's operand (the string to match)
				//  is longer or of equal length to the current longest string...
				if (OP(pCompiledExpression) == EXACTLY && strlen(OPERAND(pCompiledExpression)) >= len) 
				{
					//  Set the newly found string as the current longest string
					longest = OPERAND(pCompiledExpression);
					len = strlen(OPERAND(pCompiledExpression));
				}
			//  End For

			//  Note strings of equal length are handled so that the last string
			//  to appear in the regular expression is set to 'm_efficiencyLongestString'.  This is
			//  to avoid duplication of effort with the starting character tests
			//  above.  "Not a strong reason, but sufficient in the absence 
			//  of others."
			
			//  Set member variables
			m_efficiencyLongestString = longest;
			m_efficiencyLongestStringlen = (int)len;
		}
	}

	return this;
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
char *CRegExp::CompileExpression(int paren, int *pFlags)
{
	//  Pointer to the compiled expression (when paren is 0) or sub-expression
	//  when paren is > 0
	char *pCompiledExpressionToReturn;
	
	//  The current expression being parsed may contain multiple branches, 
	//  e.g. a|b|c contains three branches, the input can match either 'a',
	//  'b' or 'c'.
	char *pCurrentWorkingBranch;
	
	//  Variable to store the location of the node created to be the end of 
	//  the current working branch
	char *pEndBranchNode;

	//  Current bracketed expression being processed
	int currentBracket;
	//  Flags variable which will be instantiated when creating branches
	int flags;

	//  Most patterns to match have width (all except those followed by * or ?, 
	//  e.g. .? can match the empty string, as can .*).  Therefore return 
	//  HASWIDTH by default to the calling proceedure.
	*pFlags = HASWIDTH;	// Tentatively. 

	//  If this function is being called to add a parenthesised pattern 
	//  to the compiled expression process that here.
	if (paren) 
	{
		// Make an OPEN node. 
		//  Check the number of parentheses has not exceeded 9 and return failure
		//  if it does.
		if (m_countOfParentheses >= NSUBEXP)
		{
			TRACE1("Too many (). NSUBEXP is set to %d\n");
			return NULL;
		}
		//  m_countOfParentheses is initialised to 1, therefore we do not need
		//  to increment it until after we have created the node.
		currentBracket = m_countOfParentheses;
		m_countOfParentheses++;
		//  There are 10 node IDs associated with Brackets, 9 of which are 
		//  used.  OPEN+0 is never used.
		//  Create a new node with the ID being based on how many brackets have
		//  been created thus far, the node ID for open brackets must be unique
		//  to keep track of when they are closed.
		pCompiledExpressionToReturn = WriteNodeToExpression(OPEN+currentBracket);
	}

	// Pick up the branches, linking them together. 
	//  Create a branch.  This will process all the pattern up to the first '|' 
	//  found (or \0) and populate m_compiledExpression
	pCurrentWorkingBranch = CreateBranch(&flags);
	
	//  If there was some syntactic error in the regular expression cascade
	//  the NULL return.
	if (pCurrentWorkingBranch == NULL)
		return(NULL);
	
	//  pCurrentWorkingBranch now points to a compiled regular expression branch.
	//  If we are processing a bracketed expression we have already created an OPEN
	//  node, make the OPEN node's next pointer point to the created branch.
	//  If we are not processing a bracketed expression just assign our return 
	//  pointer to the created branch.
	if (paren)
		AppendNodeToNodeChain(pCompiledExpressionToReturn, pCurrentWorkingBranch);	// OPEN -> first. 
	else
		pCompiledExpressionToReturn = pCurrentWorkingBranch;
	//  End if(paren)

	//  We have already set pFlags (the returned flags) to HASWIDTH  by default, 
	//  however if when creating our first branch above it was determined that 
	//  it did not have width then we want to clear this flag.
	//  the variable flags is the Flags received from the call to CreateBranch
	//  pFlags is the Flags to be returned to the calling method
	//  ~ (bitwise NOT) has greater precedence than & (bitwise AND)
	//  ~(~flags&HASWIDTH) will return 0 if the lowest bit of flags 
	//  (HASWIDTH) is 0, else it will return 1.
	*pFlags &= ~(~flags&HASWIDTH);	// Clear bit if bit 0. 
	//  Set SPSTART to calling application if it was set in our call to CreateBranch
	*pFlags |= flags&SPSTART;

	//  Process all remaining branches in the regular expression
	while (*m_regularExpression == '|') {
		//  Skip over '|'
		m_regularExpression++;

		//  Create a new branch in the compiled output up to the next '|' or '\0'
		pCurrentWorkingBranch = CreateBranch(&flags);
		//  Syntax error in branch, cascade NULL return value.
		if (pCurrentWorkingBranch == NULL)
			return (NULL);

		//  The pCompiledExpressionToReturn pointer points to the previous created
		//  branch, point this previous branch's next pointer to the newly created 
		//  branch
		AppendNodeToNodeChain(pCompiledExpressionToReturn, pCurrentWorkingBranch);	// BRANCH -> BRANCH. 

		//  Again clear the HASWIDTH flag in the returned flags and set SPSTART
		//  depeneding on the flags received from the previous call to CreateBranch.
		*pFlags &= ~(~flags&HASWIDTH);
		*pFlags |= flags&SPSTART;
	}

	// Make a closing node, and hook it on the end. 
	//  A branch's next pointer will always point to either a CLOSE node if this
	//  branch is being created inside a bracket or an END node if it is the
	//  top level regular expression.  END in the compiled expression denotes the
	//  end of the expression.
	//  Create the END or CLOSE node as appropriate
	pEndBranchNode = WriteNodeToExpression((paren) ? CLOSE+currentBracket : END);
	//  pCompiledExpressionToReturn potentially points to a chain of branches, 
	//  a|b|c.  Point the next pointer of the _final branch in this chain_ to
	//  the newly created end branch node.
	//  e.g. a|b|c will create:
	//  00 BRANCH		(next = 6)
	//  03 EXACTLY a	(next = ?)
	//  06 BRANCH		(next = 12)
	//  09 EXACTLY b	(next = ?)
	//  12 BRANCH		(next = 18)
	//  15 EXACTLY c	(next = ?)
	//  18 END			
	AppendNodeToNodeChain(pCompiledExpressionToReturn, pEndBranchNode);

	// Hook the tails of the branches to the closing node. 
	//  Each branch now points to the next branch, however the next pointers
	//  of all those branch's _operands_ are not set, we want to set these 
	//  next pointers to the newly created END node, so once one of the OR'd
	//  conditions succeeds it knows where the branch conditions end.
	//  e.g. a|b|c will now create:
	//  00 BRANCH		(next = 6)
	//  03 EXACTLY a	(next = 18)  <-- Operand of 00
	//  06 BRANCH		(next = 12)
	//  09 EXACTLY b	(next = 18)  <-- Operand of 06
	//  12 BRANCH		(next = 18)
	//  15 EXACTLY c	(next = 18)  <-- Operand of 12
	//  18 END			
	for (pCurrentWorkingBranch = pCompiledExpressionToReturn; pCurrentWorkingBranch != NULL; pCurrentWorkingBranch = NextNode(pCurrentWorkingBranch))
		AppendNodeToBranch(pCurrentWorkingBranch, pEndBranchNode);

	// Check for proper termination. 
	//  If we are processing a bracketed expression we expect the next character
	//  to be ')', if it isn't then the expression is not syntactically correct
	if (paren && *m_regularExpression++ != ')') 
	{
		TRACE0("unterminated ()\n");
		return NULL;
	} 
	else if (!paren && *m_regularExpression != '\0') 
	{
		//  If we're not processing a bracketed expression we expect it to end
		//  with a \0 at the end of the pattern, if not then, again, there
		//  is a syntax error.
		if (*m_regularExpression == ')') 
		{
			TRACE0("unmatched ()\n");
			return NULL;
		} 
		else
		{
			TRACE0("internal error: junk on end\n");
			return NULL;
		}
		// NOTREACHED 
	}

	return (pCompiledExpressionToReturn);
}


////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
char *CRegExp::CreateBranch(int *pFlags)
{
	//  Pointer to the compiled branch, return value for this method.
	char *pCompiledBranchToReturn;
	
	//  A Branch is a sequence of Pieces, each being created by 'CreatePiece'.  
	//  This variable is used to assemble a sequence of pieces which make up
	//  the branch.
	char *pChainOfPieces;
	
	//  Variable to hold a pointer to the head node of the last piece to be
	//  added to the chain of pieces.
	char *pLastCreatedPiece;

	//  Flags variable which will be instantiated when creating pieces
	int flags;
	//  Only function is to decreae readibility of while loop condition.
	int c;

	//  Assume the worst case for the returned flags, there may be nothing
	//  in the branch (x|)
	*pFlags = WORST;				// Tentatively. 

	//  Create a new BRANCH node
	pCompiledBranchToReturn = WriteNodeToExpression(BRANCH);
	pChainOfPieces = NULL;

	//  Loop through the branch stopping when \0 (end of regular expression), 
	//  '|' (Another branch needs to process this or ')' (end of bracketed
	//  sub-expression)
	while ((c = *m_regularExpression) != '\0' && c != '|' && c != ')') {
		//  Compile the next 'piece' in the branch, a Piece is a sequence of
		//  characters followed by an optional *, ? or +
		//  e.g. .*ab+c? is composed of three pieces.  CreatePiece advances the 
		//  m_regularExpression pointer by n (where n is the characters contained
		//  in the processed piece)
		pLastCreatedPiece = CreatePiece(&flags);
		//  pLastCreatedPiece now points to the first node in the compiled piece
		//  if it is NULL then we have a syntax error.
		if (pLastCreatedPiece == NULL)
			return(NULL);

		//  There is at least one piece in the branch (therefore it has width), 
		//  instantiate the return flags with those returned from 
		//  CreatePiece()
		*pFlags |= flags&HASWIDTH;
		if (pChainOfPieces == NULL)		// First piece. 
			*pFlags |= flags&SPSTART;
		else
		{
			//  Add the last created piece to the chain of pieces
			//  (Set the next pointer of the final piece in the chain to
			//  the newly created piece)
			AppendNodeToNodeChain(pChainOfPieces, pLastCreatedPiece);
		}
		//  This step is not absolutely necessary, it advances the pChainOfPieces
		//  pointer so it always points to the last created piece, however AppendNodeToNodeChain
		//  follows the next pointers until it gets to the final node anyway, 
		//  the only advantage in this line is a minor efficiency saving when adding
		//  new pieces to the branch.
		pChainOfPieces = pLastCreatedPiece;
	}

	//  If the chain of pieces is NULL the loop did not execute, meaning 
	//  there was no data in the branch, e.g. (x|) contains two branches, the
	//  latter of which contains no data.
	//  Create a NOTHING Node.
	if (pChainOfPieces == NULL)			// Loop ran zero times. 
		(void) WriteNodeToExpression(NOTHING);

	return(pCompiledBranchToReturn);
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
char *CRegExp::CreatePiece(int *pFlags)
{
	//  Pointer to the compiled piece to be returned by this function
	char *pCompiledPieceToReturn;

	//  Local variable used to store the character following the
	//  atom in the currently processing piece
	char charFollowingAtom;

	//  Local variable used in the creation of new nodes to set next pointers
	char *next;

	//  Flags variable which will be instantiated when creating pieces
	int flags;

	//  The beginning of the piece will consist of a sequence of characters 
	//  (see piece definition), turn this sequence of ordinary characters
	//  into an atom (node).
	pCompiledPieceToReturn = CreateAtom(&flags);
	//  Cascade NULL return value caused by Syntactic errors in the regular 
	//  expression
	if (pCompiledPieceToReturn == NULL)
		return(NULL);

	//  CreateAtom advances the m_regularExpression pointer so that it now 
	//  points to the character following the atom.  The point of doing this
	//  is because a piece is defined as a sequence of characters followed by 
	//  an optional *, ? or +, here we are testing whether this optional 
	//  character is present.
	charFollowingAtom = *m_regularExpression;
	if (!IS_REPETITION_OP(charFollowingAtom)) {
		//  There is no repitition operator following the atom, the piece 
		//  currently being processed consists of no more than a single atom.
		//  The flags returned have been set by 'CreateAtom()'
		*pFlags = flags;
		return(pCompiledPieceToReturn);
	}

	//  If the code reaches here there is a repitition operator following the
	//  atom and the compiled output needs to be modified to include back
	//  pointers and NOTHING nodes to enable the pattern to be matched later.
	//  Examples are given below.

	//  Logical check, if the operator is * or + then it can not have
	//  something before it which can match the empty string.  This tests
	//  that the * or + operators have an appropriate operand.
	if (!(flags&HASWIDTH) && charFollowingAtom != '?')
	{
		//  Operand to * or + can be null string, return error condition.
		TRACE0("*+ charFollowingAtomerand could be empty\n");
		return NULL;
	}

	//  Set the flags to the calling function
	switch (charFollowingAtom) {
		//  If the character following the atom is a '*' then
		//  the SPSTART flag is true as the compiled expression starts
		//  with a special character.
	case '*':	*pFlags = WORST|SPSTART;				break;
		//  Similar to '*', a '+' character following the atom indicates 
		//  that the compiled expression will start with a special character
		//  (Note it doesn't at the moment, we put it there in the next 
		//   if / else block)
		//  Input text matching a '+' must have width as '+' matches _1_ or more
		//  instances, therefore it will not match the NULL string.
	case '+':	*pFlags = WORST|SPSTART|HASWIDTH;		break;
		//  No assumptions can be made for the '?' character
	case '?':	*pFlags = WORST;						break;
	}

	//  This is where repition operators are converted into compiled output
	//  in the compiled expression, there are 5 separate cases to consider.
	//  Note in the given examples the layout of the nodes are over simplified,
	//  they will not always be 3 bytes apart as string operands cause them to
	//  be spaced more, additionally the 'next' pointers are stored as relative 
	//  offsets, not absolute addresses.

	//  1: Expression in the form x*, where x is SIMPLE.  SIMPLE means it does
	//     not contain sub expressions, e.g. x*
	if (charFollowingAtom == '*' && (flags&SIMPLE))
		//  Before the Insert pCompiledPieceToReturn contains just the created 
		//  atom
		//  00 EXACTLY		(next = ?, Operand = 'x')	<-- pCompiledPieceToReturn
		InsertOperatorBeforeOperand(STAR, pCompiledPieceToReturn);
		//  A 'x' already output is actually the OPERAND for the STAR, therefore
		//  the star is inserted before the 'x'
		//  00 STAR			(next = ?)					<-- pCompiledPieceToReturn
		//  03 EXACTLY		(next = ?, Operand = 'x')
	else if (charFollowingAtom == '*') {
		// Emit x* as (x&|), where & means "self".  0 or more x's followed by nothing
		//  2: Expression in the form x*, where x is not SIMPLE.  Not SIMPLE 
		//     means it is composed of a sub expression, e.g. (blah)*
		//  Before modifications pCompiledPieceToReturn contains the  
		//  sub expression, (blah)
		//  00 OPEN		(next = 03)
		//  03 BRANCH	(next = 09)
		//  06 EXACTLY	(next = 09, Operand = 'blah')
		//  09 CLOSE	(next = ?)
		InsertOperatorBeforeOperand(BRANCH, pCompiledPieceToReturn);		// Either x 
		//  A branch is added to provide a place to loop back to,
		//  currently only the string 'blah' will be matched
		//  00 BRANCH	(next = ?)
		//  03 OPEN		(next = 06)
		//  06 BRANCH	(next = 09)
		//  09 EXACTLY	(next = 12, Operand = 'blah')
		//  12 CLOSE	(next = ?)
		AppendNodeToBranch(pCompiledPieceToReturn, WriteNodeToExpression(BACK));	// and loop 
		//  A BACK node is added to the end of the compiled expression, 
		//  WriteNodeToExpression(BACK) and set next pointer of the CLOSE node to the
		//  newly created BACK node
		//  00 BRANCH	(next = ?)
		//  03 OPEN		(next = 06)
		//  06 BRANCH	(next = 09)
		//  09 EXACTLY	(next = 12, Operand = 'blah')
		//  12 CLOSE	(next = 15)
		//  15 BACK		(prev = ?)
		AppendNodeToBranch(pCompiledPieceToReturn, pCompiledPieceToReturn);		// back 
		//  Set the next pointer of the BACK node, which actually points
		//  backwards so I've called it the 'prev' pointer.  The introduction
		//  of this loop means the pattern will match 'blah', 'blahblah', 
		//  'blahblahblah' e.t.c.
		//  00 BRANCH	(next = ?)
		//  03 OPEN		(next = 06)
		//  06 BRANCH	(next = 09)
		//  09 EXACTLY	(next = 12, Operand = 'blah')
		//  12 CLOSE	(next = 15)
		//  15 BACK		(prev = 00)
		AppendNodeToNodeChain(pCompiledPieceToReturn, WriteNodeToExpression(BRANCH));	// or 
		//  Add another BRANCH node to the end of the compiled expression, this
		//  will act as an alternative to the branch node above.  Link the newly
		//  added BRANCH node to the previous branch by updating the next 
		//  pointer of the previous BRANCH node.
		//  00 BRANCH	(next = 18)
		//  03 OPEN		(next = 06)
		//  06 BRANCH	(next = 09)
		//  09 EXACTLY	(next = 12, Operand = 'blah')
		//  12 CLOSE	(next = 15)
		//  15 BACK		(prev = 00)
		//  18 BRANCH	(next = ?)
		AppendNodeToNodeChain(pCompiledPieceToReturn, WriteNodeToExpression(NOTHING));	// null. 
		//  Add another node to match an empty string, now the pattern will 
		//  match either a looping 'blah' or nothing.  Once all the 'blah's 
		//  have been subsumed by the 'blah' loop the parser can follow the
		//  NOTHING branch to complete successfully, or fail if the patter does
		//  not match.  Set the nothing brahch's next pointer to point to the
		//  newly created NOTHING node.
		//  00 BRANCH	(next = ?)
		//  03 OPEN		(next = 06)
		//  06 BRANCH	(next = 09)
		//  09 EXACTLY	(next = 12, Operand = 'blah')
		//  12 CLOSE	(next = 15)
		//  15 BACK		(prev = 00)
		//  18 BRANCH	(next = 21)
		//  21 NOTHING	(next = ?)
	} else if (charFollowingAtom == '+' && (flags&SIMPLE))
		//  3: Expression in the form x+, where x is SIMPLE.  SIMPLE means it does
		//     not contain sub expressions, e.g. x+
		//  Before the Insert pCompiledPieceToReturn contains just the created 
		//  atom
		//  00 EXACTLY		(next = ?, Operand = 'x')	<-- pCompiledPieceToReturn
		InsertOperatorBeforeOperand(PLUS, pCompiledPieceToReturn);
		//  A 'x' already output is actually the OPERAND for the PLUS, therefore
		//  the plus is inserted before the 'x'
		//  00 PLUS			(next = ?)					<-- pCompiledPieceToReturn
		//  03 EXACTLY		(next = ?, Operand = 'x')
	else if (charFollowingAtom == '+') {
		// Emit x+ as x(&|), where & means "self".  x followed by looping self or nothing.
		//  4: Expression in the form x+, where x is not SIMPLE.  Not SIMPLE
		//     means it comprises of an expression, e.g. (blah)+
		//  Before modifications pCompiledPieceToReturn contains the  
		//  sub expression, (blah)
		//  00 OPEN		(next = 03)
		//  03 BRANCH	(next = 09)
		//  06 EXACTLY	(next = 09, Operand = 'blah')
		//  09 CLOSE	(next = ?)
		next = WriteNodeToExpression(BRANCH);		// Either 
		//  A branch node is added after the 'blah' EXACTLY node as we want to 
		//  match at least one 'blah' before branching and looping to match 
		//  more.
		//  00 OPEN		(next = 03)
		//  03 BRANCH	(next = 09)
		//  06 EXACTLY	(next = 09, Operand = 'blah')
		//  09 CLOSE	(next = ?)
		//  12 BRANCH	(next = ?)
		AppendNodeToNodeChain(pCompiledPieceToReturn, next);
		//  Link the newly created branch node to the existing 'blah' EXACTLY
		//  node by setting the EXACTLY node's next pointer.
		//  00 OPEN		(next = 03)
		//  03 BRANCH	(next = 09)
		//  06 EXACTLY	(next = 09, Operand = 'blah')
		//  09 CLOSE	(next = 12)
		//  12 BRANCH	(next = ?)
		AppendNodeToNodeChain(WriteNodeToExpression(BACK), pCompiledPieceToReturn);	// loop back 
		//  Create a BACK node and add it to the end of the compiled expression.
		//  Point this BACK node's 'next' pointer to the branch matching
		//  'blah'.  This means once the first 'blah' is matched it can then 
		//  match subsequent 'blah's
		//  00 OPEN		(next = 03)
		//  03 BRANCH	(next = 09)
		//  06 EXACTLY	(next = 09, Operand = 'blah')
		//  09 CLOSE	(next = 12)
		//  12 BRANCH	(next = ?)
		//  15 BACK		(prev = 00)
		AppendNodeToNodeChain(next, WriteNodeToExpression(BRANCH));	// or 
		//  Create an additional branch and the end of the compiled output 
		//  to provide an alternative to looping, repeatedly matching 'blah'.
		//  Set the next pointer of the BRANCH containing the loop to this
		//  newly created BRANCH node so the parser knows where to go if the 
		//  loop match doesn't succeed
		//  00 OPEN		(next = 03)
		//  03 BRANCH	(next = 09)
		//  06 EXACTLY	(next = 09, Operand = 'blah')
		//  09 CLOSE	(next = 12)
		//  12 BRANCH	(next = 18)
		//  15 BACK		(prev = 00)
		//  18 BRANCH	(next = ?)
		AppendNodeToNodeChain(pCompiledPieceToReturn, WriteNodeToExpression(NOTHING));	// null. 
		//  Another node is added to match an empty string, now the pattern will 
		//  match either a looping 'blah' or nothing.  Once all the 'blah's 
		//  have been subsumed by the 'blah' loop the parser can follow the
		//  NOTHING branch to complete successfully, or fail if the pattern
		//  does not match.  The nothing operator is linked to the previously
		//  created branch by setting that branch's next pointer.
		//  00 OPEN		(next = 03)
		//  03 BRANCH	(next = 09)
		//  06 EXACTLY	(next = 09, Operand = 'blah')
		//  09 CLOSE	(next = 12)
		//  12 BRANCH	(next = 18)
		//  15 BACK		(prev = 00)
		//  18 BRANCH	(next = 21)
		//  21 NOTHING	(next = ?)
	} else if (charFollowingAtom == '?') {
		// Emit x? as (x|)  (x or nothing)
		//  5: Expression in the form x?, e.g. x?
		//  Before modifications pCompiledPieceToReturn contains an EXACTLY
		//  node with the string parameter 'x'
		//  00 EXACTLY	(next = ?, Operand = 'x')
		InsertOperatorBeforeOperand(BRANCH, pCompiledPieceToReturn);		// Either x 
		//  A BRANCH node is inserted before the compiled piece, this will 
		//  provide the choice to match 'x'.
		//  00 BRANCH	(next = ?)
		//  03 EXACTLY	(next = ?, Operand = 'x')
		AppendNodeToNodeChain(pCompiledPieceToReturn, WriteNodeToExpression(BRANCH));	// or 
		//  A BRANCH node is added to the end of the compiled piece, this
		//  complements the previously added BRANCH and provides the choice to 
		//  not match 'x', i.e. match the empty string.  The 'next' pointer
		//  of the first BRANCH node is made to point to the newly created
		//  BRANCH node.
		//  00 BRANCH	(next = 06)
		//  03 EXACTLY	(next = ?, Operand = 'x')
		//  06 BRANCH	(next = ?)
		next = WriteNodeToExpression(NOTHING);		// null. 
		//  A NOTHING node is added to the end of the compiled piece to provide
		//  the alternative to matching 'x'
		//  00 BRANCH	(next = 06)
		//  03 EXACTLY	(next = ?, Operand = 'x')
		//  06 BRANCH	(next = ?)
		//  09 NOTHING	(next = ?)
		AppendNodeToNodeChain(pCompiledPieceToReturn, next);
		//  The BRANCH containing the NOTHING node is made to point to the 
		//  NOTHING node ('next' pointer updated).
		//  00 BRANCH	(next = 06)
		//  03 EXACTLY	(next = ?, Operand = 'x')
		//  06 BRANCH	(next = 09)
		//  09 NOTHING	(next = ?)
		AppendNodeToBranch(pCompiledPieceToReturn, next);
		//  The 'next' pointer of the EXACTLY node is made to point to the
		//  first item in the alternative branch, so if it fails to match 'x'
		//  it will attempt to match the empty string.  - ToDo... Is this the right reason???
		//  00 BRANCH	(next = 06)
		//  03 EXACTLY	(next = 09, Operand = 'x')
		//  06 BRANCH	(next = 09)
		//  09 NOTHING	(next = ?)
	}

	//  Advance to the next character in the regular expression
	m_regularExpression++;

	//  Check the next charcter in the regular expression is not a repetition
	//  operator
	if (IS_REPETITION_OP(*m_regularExpression))
	{
		//  Expression looks something like **, *?, ??, +* e.t.c. Syntax error.
		TRACE0("nested *?+\n");
		return NULL;
	}

	return(pCompiledPieceToReturn);
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
char *CRegExp::CreateAtom(int *pFlags)
{
	//  Pointer to the compiled atom to be returned by this function
	char *pCompiledAtomToReturn;

	//  Flags variable which will be instantiated when creating pieces
	int flags;

	//  The created node will not necessarily have any flags 
	//  (e.g. BOL has none) so assume none as the default to return.
	*pFlags = WORST;		// Tentatively. 

	//  Switch statement on the current character in the regular expression.
	//  Note that m_regularExpression is advanced and so will point to
	//  the _subsequent_ character in the regular expression within the 
	//  case statements.
	switch (*m_regularExpression++) {
	case '^':
		//  Found the carat character representing a BOL node, this has no
		//  operands.
		pCompiledAtomToReturn = WriteNodeToExpression(BOL);
		break;
	case '$':
		//  Found the dollar sign representing a EOL node, this has no operands.
		pCompiledAtomToReturn = WriteNodeToExpression(EOL);
		break;
	case '.':
		//  Found the dot character, representing an ANY node, this has no
		//  operands.
		pCompiledAtomToReturn = WriteNodeToExpression(ANY);
		//  Dot operators will always match a character so they have width
		//  and they are simple enough to act as a */+ operand
		*pFlags |= HASWIDTH|SIMPLE;
		break;
	case '[': {
		//  Found the start of a range
		//  Variables to store the first and last items in the range, stored
		//  as ints but these are actually ASCII character codes.
		int rangestart;
		int rangeend;
		//  Local variable to store a character in the range for comparison, 
		//  again this is actually an ASCII character code.
		int c;

		//  Work out whether we are dealing with [range] or [^range], the first
		//  will match any character in 'range', the second will match any
		//  character not in 'range.  The first is compiled to an ANY node, 
		//  the latter to an ANYBUT node, both of which take an operand
		//  which is the characters in the range
		if (*m_regularExpression == '^') {	// Complement of range. 
			pCompiledAtomToReturn = WriteNodeToExpression(ANYBUT);
			m_regularExpression++;
		} else
			pCompiledAtomToReturn = WriteNodeToExpression(ANYOF);

		//  Test for the first character in the range being ']' or '-'
		if ((c = *m_regularExpression) == ']' || c == '-') {
			//  If it is this is not a syntax error and the character is 
			//  output as part of the ANYOF / ANYBUT operand.
			WriteCharacterToExpression(c);
			m_regularExpression++;
		}

		//  While not at the end of the range
		while ((c = *m_regularExpression++) != '\0' && c != ']') {
			if (c != '-')
				WriteCharacterToExpression(tolower(c));
							//  Browser - Made Case Insensitive
							//  c is pointing to the second character in the range
							//  if this isn't '-' then the range looks something
							//  like [ABCD...] and each character should be output
							//  to the operand.
			else if ((c = *m_regularExpression) == ']' || c == '\0')
				WriteCharacterToExpression('-');	//  c is now pointing to the character after the '-'
							//  If the range looks like ...-] then output
							//  a '-' to the operand.
			else 
			{
				//  The remaining alternative is that the range is of the form
				//  [...A-Z...].  The first series of dots will be handled
				//  by the first condition in this if / else clause.  The 
				//  second series of dits will be handled on subsequent loops 
				//  round the while loop.
				//  m_regularExpression and c point to the same place, the 
				//  character after the '-', set the start and end pointers
				rangestart = (unsigned) (char)*(m_regularExpression-2);
				rangeend = (unsigned) (char)c;
				
				//  Test to check that the end pointer's character is greater
				//  than the start character lexlexigraphically.
				if (rangestart > rangeend)
				{
					//  Character codes are not ascending, syntax error
					TRACE0("invalid [] range\n");
					return NULL;
				}

				//  The range [A-D] will be output as a string operand ABCD.
				//  Loop through the character codes from start to end and
				//  output the string to the compiled expression.
				//  Browser made case insensitive
				for (rangestart++; rangestart <= rangeend; rangestart++)
					WriteCharacterToExpression(tolower(rangestart));
				m_regularExpression++;
			}
		}
		//  String operands always end with the NULL character
		WriteCharacterToExpression('\0');

		//  Test the range actually ends with a square bracket otherwise it's a
		//  syntax error.
		if (c != ']')
		{
			TRACE0("unmatched []\n");
			return NULL;
		}
		//  Ranges will always match a character so they have width
		//  and they are simple enough to act as a */+ operand
		*pFlags |= HASWIDTH|SIMPLE;
		break;
		}
	case '(':
		//  An open bracket in the regular expression begins the processing
		//  of a new sub-expression, this is accomplished by a call to the 
		//  same function which parses the overall expression, setting the
		//  'paren' parameter.
		pCompiledAtomToReturn = CompileExpression(1, &flags);
		if (pCompiledAtomToReturn == NULL)
			return(NULL);

		//  Pass up the flags returned by CompileExpression.
		*pFlags |= flags&(HASWIDTH|SPSTART);
		break;
	case '\0':
	case '|':
	case ')':
		// supposed to be caught earlier
		//  as part of branch or expression processing.  This is not a syntax
		//  error but is an error in the regular expression handler class.
		TRACE0("internal error: \\0|) unexpected\n");
		return NULL;
		break;
	case '?':
	case '+':
	case '*':
		//  The first character of a piece is being processed and pieces should
		//  not start with a repetition operator, it is like trying to match
		//  'a(*)'
		TRACE0("?+* follows nothing\n");
		return NULL;
		break;
	case '\\':
		//  Escaped character, create a new node in the compiled expression
		//  similar to a new string of length 1.
		//  If there is no character after the \ it is a syntax error.
		if (*m_regularExpression == '\0')
		{
			TRACE0("trailing \\\n");
			return NULL;
		}
		//  Create a new EXACTLY node with an operand equal to the character
		//  code of the escape character.
		pCompiledAtomToReturn = WriteNodeToExpression(EXACTLY);
		WriteCharacterToExpression(*m_regularExpression++);
		//  EXACTLY operands must always end with the string terminator
		WriteCharacterToExpression('\0');
		//  The operator will not match the empty string and can function as
		//  a * or + operand, set the appropriate flags.
		*pFlags |= HASWIDTH|SIMPLE;
		break;
	default: {
		//  The default case is the character is a string literal, therefore
		//  subsume this and all subsequent characters in the string literal
		//  into a single EXACTLY node.

		//  Local variables to hold the length of the string and the character
		//  following the string.
		size_t lengthOfStringLiteral;
		char charBeyondStringLiteral;

		//  make m_regularExpression point to the start of the string literal,
		//  it was incrmemeted in the switch().
		m_regularExpression--;

		//  Set the lengthOfStringLiteral variable, META contains the characters
		//  ^$.[()|?+*\ and so lengthOfStringLiteral will be set to the length of the
		//  string up to the first one of those characters.  Finding any one
		//  of those characters indicates the end of the atom.
		lengthOfStringLiteral = strcspn(m_regularExpression, META);
		if (lengthOfStringLiteral == 0)
		{
			//  The string should never be of 0 length, we have already
			//  tested for all of the META characters in the previous
			//  cases of the switch statement.  Getting here is an error in
			//  the Regular Expression class rather than a syntax error.
			TRACE0("internal error: strcspn 0\n");
			return NULL;
		}
		//  Set charBeyondStringLiteral to the Character immediately following the string,
		//  so if the regular expression is da+, charBeyondStringLiteral will
		//  point to '+'
		charBeyondStringLiteral = *(m_regularExpression + lengthOfStringLiteral);

		//  Consider the case of compiling the regular expression abc*, it is 
		//  composed of 3 nodes, two EXACTLY nodes and a STAR node, on the first 
		//  pass through this method lengthOfStringLiteral will be 3 and charBeyondStringLiteral 
		//  will be '*'
		//  If the string is followed by a repetition operator (*, ? or +) and
		//  the length of the string literal is more than 1 character we do not
		//  want to consider the * or the final character (*'s operand) so
		//  we take 1 off the length of the literal.
		//  With the example abc* on the first pass through the following if 
		//  statement evaluates to true and the lengthOfStringLiteral is set to
		//  2.  On the second pass through, when m_regularExpression is c*, the
		//  if statement evaluates to false.
		if (lengthOfStringLiteral > 1 && IS_REPETITION_OP(charBeyondStringLiteral))
			lengthOfStringLiteral--;		// Back off clear of ?+* operand. 

		//  Since we are matching a string we will not match the empty string,
		//  set the flag appropriately
		*pFlags |= HASWIDTH;

		//  ab cannot be an operand of *, only single characters can, set the flag
		//  as appropriate.  In the example of compiling abc* this will only
		//  evaluate to true on the second pass of CreateAtom (c*)
		if (lengthOfStringLiteral == 1)
			*pFlags |= SIMPLE;

		//  Create an EXACTLY node and set the operand of that node to the 
		//  string literal.  In the example of compiling abc* the first pass
		//  with produce an EXACTLY node with the operand 'ab' and the second
		//  pass with produce an EXACTLY node with the operand 'c'.  
		pCompiledAtomToReturn = WriteNodeToExpression(EXACTLY);
		for (; lengthOfStringLiteral > 0; lengthOfStringLiteral--)
			WriteCharacterToExpression(*m_regularExpression++);
		//  The operand of the exactly node is always terminated by the null
		//  string.
		WriteCharacterToExpression('\0');
		break;
		}
	}

	return(pCompiledAtomToReturn);
}



////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
void CRegExp::InsertOperatorBeforeOperand(char theOperator, char *theOperand)
{
	//  Local variable to keep track of where to insert the operator
	char *place;

	//  If we are on the initial pass over the Regular Expression just
	//  work out the theoretical size and return
	if (!m_bOutputCompiledExpression) {
		m_compiledExpressionSize += 3;
		return;
	}

	//  If we are on the second pass over the regular expression move the whole
	//  compiled expression 3 bytes down between the operand we want to insert
	//  before and the end of the expression
	(void) memmove(theOperand+3, theOperand, (size_t)((m_lastByteInCompiledExpression - theOperand)*sizeof(char)));
	m_lastByteInCompiledExpression += 3;

	//  Put the new operator where the operand used to be.  NULL the next 
	//  pointer
	place = theOperand;		// Op node, where operand used to be. 
	*place++ = theOperator;
	*place++ = '\0';
	*place++ = '\0';
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
void CRegExp::AppendNodeToNodeChain(char *nodeChain, char *orphanNode)
{
	//  Local variables used to iterate over the node chain
	char *scan;
	char *temp;
//	int offset;

	//  If this is the first pass over the regular expression we do not wish
	//  to change anything
	if (!m_bOutputCompiledExpression)
		return;

	// Find last node. 
	//  Follow the next pointers in the node chain until the next pointer
	//  is NULL, keeping 'scan' pointing at the final node in the chain.
	for (scan = nodeChain; (temp = NextNode(scan)) != NULL; scan = temp)
		continue;

	//  Modifications made by Browser:
	// Changed by Andreas Pohl, 01/07/2002
	// org: *((short *)(scan+1)) = (OP(scan) == BACK) ? scan - val : val - scan;
	// Andreas:
	/*short *sp=new short;
	char *cp=(char *)sp;
	*sp=(OP(scan)==BACK)? scan - val: val - scan;
	*(scan+1)=*cp;
	*(scan+2)=*(cp+1);*/

	//  Browser - Andreas Phol's change works on mobile devices, the original code
	//        works on the desktop, modified so the same code works on both devices
//#ifndef UNDER_CE
	//  Original Code
//	*((short *)(scan+1)) = (OP(scan) == BACK) ? scan - orphanNode : orphanNode - scan;
//#else
	short *sp=new short;
	char *cp=(char *)sp;
	//  Set the 'next' pointer of the end node (scan) to point at the orphan 
	//  node.  In memory the orphan node will be after the final node in the 
	//  node chain, the actual value written in the 'next' pointer is the 
	//  byte offset of the orphan node from the last node in the chain.
	//  Therefore last node in node chain + value of next pointer = orphan node.
	//  The exception to this is when the operator is of type BACK.  Under this
	//  situation the orphan node appears above the node chain and so the value
	//  of the next pointer is interpreted as 'go up this number of byes in 
	//  memory'.
	//  The 'next' pointer can only hold 127 in positive numbers (the blocks
	//  are signed bytes) and so if the next pointer needs to be more than this
	//  store the multiples of 128 in the third byte in the node.  This is
	//  new behaviour for Browser
	*sp=(OP(scan)==BACK)? scan - orphanNode: orphanNode - scan;
	//  Begin New code for Browser.
	if (*sp > 127)
	{
		*(scan+2)=(*sp / 128);
		*(scan+1)=(*sp - (*(scan+2)*128));
	}
	//  end new code for Browser
	else
	{
 		*(scan+1)=*cp;
		*(scan+2)=*(cp+1);
	}
	delete sp;  //  Browser modification to free allocated memory
//#endif
}


////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
void CRegExp::AppendNodeToBranch(char *branch, char *node)
{
	// "Operandless" and "op != BRANCH" are synonymous in practice.
	//  If this is our first pass over the regular expression do not modify
	//  the compiled expression data structure and reuturn.
	//  This method only applies to BRANCH nodes, if the given node is
	//  not a BRANCH then return.
	if (!m_bOutputCompiledExpression || OP(branch) != BRANCH)
		return;

	//  The operand of the branch is the first node in that branch's node chain.
	//  Add the node to the end of the branch's node chain.
	AppendNodeToNodeChain(OPERAND(branch), node);
}


////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
int CRegExp::MatchString(const char *szcStringToMatch)
{
	//  Take a copy of the user specified string to match
	char *szStringToMatch = (char *)szcStringToMatch;	// avert const poisoning 
	
	//  Temporary loop variable used to loop over the input string attempting
	//  to find potential match points.
	char *szTemp;

	// Be paranoid. 
	//  Check the user has not provided a NULL string to match
	if(szStringToMatch == NULL) 
	{
		TRACE0("NULL argument to regexec\n");
		return(-1);
	}

	// Check validity of regex
	//  m_bCompiled will only be true if we have previously received and compiled
	//  a legal regular expression.  If we have not previously compiled an 
	//  expression return 'no match'
	if (!m_bCompiled) 
	{
		TRACE0("No regular expression provided yet.\n");
		return(-1);
	}

	// If there is a "must appear" string, look for it. 
	//  m_efficiencyLongestString contains the longest literal string which
	//  must be present in the input string in order to match the regular
	//  expression, this is set when the regular expression is compiled.  Check
	//  the string we are trying to match contains this 
	//  m_efficiencyLongestString.  If the string is not present return 'no
	//  match'.
	if (m_efficiencyLongestString != NULL && 
		stristr(szStringToMatch, m_efficiencyLongestString) == NULL)  
		return(-1);

	// Mark beginning of line for ^
	//  Set the m_szStartOfStringToMatch to the user specified string to be
	//  matched, the member variable will be used during the matching routine
	//  'MatchStringToPattern' if a BOL (beginning of line) is found in the compiled 
	//  expression)
	m_szStartOfStringToMatch = szStringToMatch;

	//  Find a Match, there are 3 techniques for attempting to find a match 
	//  to the regular expression in the input string of increasing 
	//  computational complexity:
	
	// Simplest case:  anchored match need be tried only once. 
	//  1. If we know the input string must start with a certain character.
	//     We need only attempt to match the input string to the pattern
	//     from the start of the input.  This will be the case if the regular
	//     expression started with the ^ character.
	if (m_efficiencyStartsAnchor)
	{
		//  The regular expression must match the start of the input string,
		//  there is only one potential match, the start of the string.
		if( TestPotentialMatch(szStringToMatch) )
		{
			// Delete any previously stored found string
			//  Browser, modified to properly delete m_szLastMatchedString
			delete[] m_szLastMatchedString;  
			m_szLastMatchedString = NULL;
			
			//  Save the found substring so it can be used during the replace
			//  operation if required.
			m_szLastMatchedString = new char[LengthOfMatchedString()+1];
			m_szLastMatchedString[LengthOfMatchedString()] = '\0';
			strncpy(m_szLastMatchedString, szStringToMatch, LengthOfMatchedString() );

			//  Return 0 as we found a match at the start of the input string,
			//  i.e. index 0.
			return 0;
		}
		//String not found
	return -1;
	}

	// Messy cases:  unanchored match. 
	//  2. The input string does not need to start with a specific character
	//     but if a portion of the input string is going to match the pattern
	//     we know which character it needs to start with.  Using this 
	//     knowledge we can iterate over the input string and only attempt
	//     to match from the start character, e.g. if the pattern is "a+e" and
	//     the input string is abacadae we will attempt to match the pattern
	//     at input index 0 (abacadae), 2 (acadae), 4 (adae) and 6 (ae), 
	//     eventually succeeding at input 6.
	if (m_efficiencyStartChar != '\0') 
	{
		//  We know what character the input string must start with.  Iterate
		//  over each of these characters in the input string and attempt fo
		//  find a match starting at each of the characters.
		for (szTemp = szStringToMatch; szTemp != NULL; 
					szTemp = strichr(szTemp+1, m_efficiencyStartChar))
			//  Try and find a match on the specified substring
			if (TestPotentialMatch(szTemp))
			{
				//  Successfully matched with the string szTemp.  
				//  nPos is the position matched, szTemp
				//  and szcStringToMatch point to the same string, determine
				//  the length szTemp is along the string via pointer 
				//  arithmatic.
				int nPos = szTemp - szcStringToMatch;

				//  Delete any previously stored found string.
				//  Browser, modified to properly delete m_szLastMatchedString
				delete[] m_szLastMatchedString;
				m_szLastMatchedString = NULL;
				
				//  Save the found substring so it can be used during the replace
				//  operation if required.
				m_szLastMatchedString = new char[LengthOfMatchedString()+1];
				m_szLastMatchedString[LengthOfMatchedString()] = '\0';
				strncpy(m_szLastMatchedString, szTemp, LengthOfMatchedString() );

				//  Return the position of the found match in the input string.
				return nPos;
			}
			//  szTemp is NULL.  We have tested for matches at every potential
			//  place in the input string and not succeeded, return failure.
		return -1;
	} 
	else 
	{
		// We don't -- general case
		//  3. We have no preconceptions about which part of the input string 
		//     can potentially match the regular expression, use a brute force
		//     technique and attempt to find a match at every chacter in the
		//     input string.
		for (szTemp = szStringToMatch; !TestPotentialMatch(szTemp); szTemp++)
			//  TestPotentialMatch has failed, check to see if we are at the 
			//  end of the input string.
			if (*szTemp == '\0')
				//  If we have reached the end of the input string without 
				//  finding a match then return failure.
				return(-1);

		//  Successfully matched with the string szTemp.  
		//  nPos is the position matched, szTemp
		//  and szcStringToMatch point to the same string, determine
		//  the length szTemp is along the string via pointer 
		//  arithmatic.
		int nPos = szTemp - szcStringToMatch;

		//  Delete any previously stored found string
		//  Browser, modified to properly delete m_szLastMatchedString
		delete[] m_szLastMatchedString;  
		m_szLastMatchedString = NULL;
		
		//  Save the found substring so it can be used during the replace
		//  operation if required.
		m_szLastMatchedString = new char[LengthOfMatchedString()+1];
		m_szLastMatchedString[LengthOfMatchedString()] = '\0';
		strncpy(m_szLastMatchedString, szTemp, LengthOfMatchedString() );
			
		//  Return the position of the found match in the input string.
		return nPos;
	}
	// NOTREACHED, we have covered all possible forms of the input string.
}


////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
int	CRegExp::TestPotentialMatch(char *szStringToMatch)
{
	//  Variables declared for syntactic sugar, used to loop through and clear
	//  the subexpression arrays m_szStartp and m_szEndp.
	int i;
	char **pszStartp;
	char **pszEndp;

	//  m_szStringToMatch is used by MatchStringToPattern, use the member 
	//  variable to transfer the user specified input string to that function.
	m_szStringToMatch = szStringToMatch;

	//  Clear the arrays of matched subexpressions, portions of the previous 
	//  match attempt which matched bracketed sub expressions in the regular
	//  expression.
	pszStartp = m_szStartp;
	pszEndp = m_szEndp;
	for (i = NSUBEXP; i > 0; i--) 
	{
		*pszStartp++ = NULL;
		*pszEndp++ = NULL;
	}

	//  Attempt to match the specified user string against the previously
	//  compiled regular expression.
	if (MatchStringToPattern(m_compiledExpression)) 
	{
		//  The input string matched, and in doing so populated 
		//  m_szStartp[1 thru NSUBEXP] and m_szEndp[1 thru NSUBEXP].  Still need
		//  to populate those arrays with the start and end portion of the 
		//  matched string.
		//  Start position is string given to the function.
		m_szStartp[0] = szStringToMatch;
		//  m_szStringToMatch is advanced during MatchStringToPattern and 
		//  after the match is set to the end of the matched portion of the 
		//  input string.
		m_szEndp[0] = m_szStringToMatch;

		//  Return TRUE to indicate the match succeeded.
		return(1);
	} 
	//  The specified input string did not match, return FALSE.
	else
		return(0);
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
int	CRegExp::MatchStringToPattern(char *compiledExpression)
{
	//  Variables to hold the current and next node to be processed in the
	//  compiled pattern.
	char *currentProcessingNode;	// Current node. 
	char *nextProcessingNode;		// Next node. 

	//  Loop through each of the nodes in the current compiled regular 
	//  expression
	for (currentProcessingNode = compiledExpression; 
			currentProcessingNode != NULL; 
			currentProcessingNode = nextProcessingNode)
	{
		//  Advance the next at the beginning of the loop to act as a lookahead
		nextProcessingNode = NextNode(currentProcessingNode);

		//  Ensure the character in the input string pointed to by 
		//  m_szStringToMatch can be swallowed by the current node.  This
		//  will depened on the type of node.
		switch (OP(currentProcessingNode)) {
		case BOL:
			//  Beginning of Line node, ensure m_szStringToMatch has not
			//  yet been advanced passed the beginning of the input string
			//  (m_szStartOfStringToMatch)
			if (m_szStringToMatch != m_szStartOfStringToMatch)
				return(0);
			break;
		case EOL:
			//  End of Line node, ensure there are no characters left in the
			//  input string.
			if (*m_szStringToMatch != '\0')
				return(0);
			break;
		case ANY:
			//  Match '.', Just check the input at the current position isn't 
			//  NULL.
			if (*m_szStringToMatch == '\0')
				return(0);
			//  '.' swallows one character, advance the current position 
			//  pointer
			m_szStringToMatch++;
			break;
		case EXACTLY: 
			{
				//  Match a string of characters to the input string, 
				//  the character string to match 
				//  is stored in the current node's operand.
			size_t lengthOfNodeString;
			
			//  Extract the regular expression node's string from the compiled
			//  expression.
			char *const szNodeString = OPERAND(currentProcessingNode);

			// Inline the first character, for speed.  
			//  Firstly test the first character of the two strings is identical
			//  to avoid unnecessary calls to C++ string functions
			//  Browser - modified to perform case insenstive comparison
			if (tolower(*szNodeString) != tolower(*m_szStringToMatch))  
				return(0);

			//  Node's string is stored just as a C++ string, ends with \0 so
			//  can perform C++ string operations on it.
			lengthOfNodeString = strlen(szNodeString);

			//  Test all the characters in the two strings match.
			//  Browser - modified to perform case insensitive comparison.
			if (lengthOfNodeString > 1 && 
				_strnicmp(szNodeString, m_szStringToMatch, 
				lengthOfNodeString) != 0)  
				return(0);

			//  Advance the input string pointer by the number of characters matched in the string.
			m_szStringToMatch += lengthOfNodeString;
			break;
			}
		case ANYOF:
			//  e.g. [0-2]
			//  Stored in the Compiled Expression as:
			//  00  ANYOF
			//  01  pointer to next node
			//  02  pointer to next node
			//  03  '0'
			//  04  '1'
			//  05  '2'
			//  06  '\0'
			//  OPERAND(currentProcessingNode) = "012"
			//  Ensure the current character in the input string being processed
			//  is in the range of possible characters, '0', '1' or '2' in our 
			//  example.
			//  Browser - modified to perform case insensitive comparison.
			if (*m_szStringToMatch == '\0' ||
					strichr(OPERAND(currentProcessingNode), 
					*m_szStringToMatch) == NULL)  
				return(0);
			//  One character has been swallowed by this node, advance the 
			//  current input pointer.
			m_szStringToMatch++;
			break;
		case ANYBUT:
			//  Exactly the same as the ANYOF operator except we test that the
			//  current character in the input string is NOT in the range of 
			//  specified characters.
			//  Browser - modified to perform case insensitive comparison.
			if (*m_szStringToMatch == '\0' ||
					strichr(OPERAND(currentProcessingNode), 
					*m_szStringToMatch) != NULL)  
				return(0);
			//  One character has been swallowed by this node, advance the 
			//  current input pointer.
			m_szStringToMatch++;
			break;
		case NOTHING:
			//  Take no action for the NOTHING node, this is used to give choice
			//  to the parser, e.g. x? can match 'x' or NOTHING.
			break;
		case BACK:
			//  Take no action for the BACK node, this is used to implement
			//  looping in the parser, e.g. (ab)+ will match "ab", "abab", 
			//  "ababab" e.t.c.
			break;
		case OPEN+1: case OPEN+2: case OPEN+3:
		case OPEN+4: case OPEN+5: case OPEN+6:
		case OPEN+7: case OPEN+8: case OPEN+9:
			{
				//  An open bracket has been found in the compiled expression,
				//  there can be a maximum of 9 bracketed sub expressions.

				//  Calculate which bracket number we are dealing with, this is
				//  required for saving the result of parsing the sub 
				//  expression.  Sub expressions need to be saved so they can 
				//  be accessed during the replace operation.
				//  OP(currentProcessingNode) will be OPEN + x where x is the
				//  bracket number.  E.g. The second bracket will be:
				//  (OPEN + 2) - OPEN = 2
			const int bracketNumber = OP(currentProcessingNode) - OPEN;

			//  Take a copy of the current input pointer position, as this
			//  will be modified in the next call.
			char *const input = m_szStringToMatch;

			//  Recursively call the match routine with the nodes within the 
			//  bracket.
			if (MatchStringToPattern(nextProcessingNode))
			{
				// Don't set m_szStartp if some later
				// invocation of the same parentheses
				// already has.

				//  Set the start pointer for the string inside this bracket.
				//  These lines will only be called when all the recursion 
				//  unwinds so the first value to be put in this array is 
				//  the most up to date data, therefore only set if the value
				//  is NULL.
				if (m_szStartp[bracketNumber] == NULL)
					m_szStartp[bracketNumber] = input;

				//  Pattern match following this open bracket succeeded.
				return(1);
			} 
			else
			{	
				//  Pattern match following this open bracket failed.
				return(0);
			}
			break;
			}
		case CLOSE+1: case CLOSE+2: case CLOSE+3:
		case CLOSE+4: case CLOSE+5: case CLOSE+6:
		case CLOSE+7: case CLOSE+8: case CLOSE+9:
			{
				//  A close bracket has been found in the compiled expression, 
				//  similar to OPEN brackets.

				//  Calculate which bracket we are dealing with.
			const int bracketNumber = OP(currentProcessingNode) - CLOSE;

			//  Take a copy of the current input pointer position, as this
			//  will be modified in the next call.
			char *const input = m_szStringToMatch;

			//  Recursively call the match routine with the nodes following the 
			//  bracketed expression.
			if (MatchStringToPattern(nextProcessingNode))
			{
				// Don't set m_szEndp if some later
				// invocation of the same parentheses
				// already has.
				 
				//  Set the end pointer for the string inside this bracket.
				//  These lines will only be called when all the recorsion 
				//  unwinds so the first value to be put in this array is
				//  the most up to date data, therefore only set if the value
				//  is NULL.
				if (m_szEndp[bracketNumber] == NULL)
					m_szEndp[bracketNumber] = input;

				//  Pattern match following this close bracket succeeded.
				return(1);
			} 
			else
			{
				//  Pattern match following this close bracket failed.
				return(0);
			}
			break;
			}
		case BRANCH: 
			{
				//  Encountered a BRANCH node which implements choice in the
				//  regular expression, e.g. a|b will match 'a' or 'b'

				//  Save a copy of the current input pointer.  This is used
				//  if one of the branches failed to match so we can reset the
				//  pointer and try a differnet branch, e.g. in the example
				//  above matching a|b to 'b' will fail in the first branch
				//  but in doing so will advance the input pointer to '\0' 
				//  (beyond 'b').  The pointer can be reset to 'b' and the match
				//  will succeed for the second a|b branch.
			char *const startBranchInputPointer = m_szStringToMatch;

			if (OP(nextProcessingNode) != BRANCH)  // No choice. 
			{ 
				//  If the compiled expression consists of two consecutive 
				//  BRANCH nodes then jump straight to the second BRANCH and
				//  do not process the first.  This can be caused by (a?), which
				//  will compile to:
				//  BRANCH		(All REs start with a Branch)
				//  OPEN +1		(Open Bracket)
				//  BRANCH		(All brackets start with a branch)
				//  BRANCH		(pointer to next branch to skip matching 'a')
				//  EXACTLY a	(match 'a')
				//  BRANCH		
				//  NOTHING		(don't match 'a')
				//  CLOSE +1
				nextProcessingNode = OPERAND(currentProcessingNode);	// Avoid recursion. 
			}
			else 
			{
				//  Loop through all the possible branches 
				//  (NextNode(currentProcessingNode) for a BRANCH node will point
				//  to the next possible branch.
				while (OP(currentProcessingNode) == BRANCH) 
				{
					//  Check to see of the nodes in the current branch match
					//  the input pattern.  The current Branch's nodes are
					//  stored in the Operand of the Branch.
					if (MatchStringToPattern(OPERAND(currentProcessingNode)))
						//  The input string matches the current branch, return
						//  success
						return(1);

					//  The current branch has failed to match so try the 
					//  next branch, reset the input string pointer to the value
					//  before it started processing branches.
					m_szStringToMatch = startBranchInputPointer;

					//  Set the currentProcessingNode to the next branch.
					currentProcessingNode = NextNode(currentProcessingNode);
				}

				//  We have looped through all the possible branches and not 
				//  found a match.  Return failure.
				return(0);
				// NOTREACHED 
			}
			break;
			}
		//  The next node in the compiled expression is a STAR
		//  or PLUS, these are greedy operators which can match any number of
		//  input characters (0 or more for * and 1 or more for +).  The very simple
		//  case is a* to match 0 or more 'a' characters, but * may also be
		//  preceeded by [], [^], . (dot).  Similarly for +.  ()* / ()+ is 
		//  handled differently, by the OPEN / CLOSE cases.
		case STAR: 
		case PLUS: 
			{
			//  Determine the character associated with the operator for the
			//  very simple case of a* / a+.  For the more complicated cases set
			//  the next character to NULL.
			const char chAssociatedCharacter =
				(OP(nextProcessingNode) == EXACTLY) ? *OPERAND(nextProcessingNode) : '\0';
			
			//  Declare a variable to hold the number of characters in the input
			//  string being swallowed by the operator
			size_t swallowedCharacters;
			
			//  Save a copy of the position of the pointer to the input string
			//  where we are currently reading.  As we backtrack on our greedy
			//  operators this is used to reset the input pointer.
			char *const szSavedStringToMatch = m_szStringToMatch;
			
			//  Determine the minimum allowable number of characters which can
			//  be swallowed by the operator, this will be 0 for * and 1 for +.
			const size_t min = (OP(currentProcessingNode) == STAR) ? 0 : 1;

			//  Start by assuming the maximum possible characters are swallowed
			//  by the operator and attempt to continue matching to the compiled
			//  regular expression.  If the match fails at some stage the 
			//  recursion will unwind to this point where the number of 
			//  swallowed characters will be decremented and the match attempted
			//  again.  This implementation of regular expression uses greedy
			//  operators which can be computationally expensive.
			for (swallowedCharacters = MaxGreedyCharacters(OPERAND(currentProcessingNode)) + 1; 
				swallowedCharacters > min; swallowedCharacters--) 
			{
				//  m_szStringToMatch is the characters remaining in the input
				//  string after we have swallowed the specified number
				//  of characters for the * or + operator, recursively call
				//  MatchStringToPattern to process this string.
				m_szStringToMatch = szSavedStringToMatch + swallowedCharacters - 1;
				
				// If it could work, try it. 
				//  Browser - Implemented a case insensitive comparison here.
				//  For efficiency check if it is a very simple case of a* / a+ and 
				//  if it is check the next character is the input string is
				//  the same as the character associated with the operator.
				if (chAssociatedCharacter == '\0' || 
					tolower(*m_szStringToMatch) == tolower(chAssociatedCharacter)) 
					if (MatchStringToPattern(nextProcessingNode))
						return(1);
						//  TRUE is returned if the subsequent input string 
						//  matches the remaining pattern, if FALSE is returned
						//  the number of swallowed characters is decremented
						//  until it reaches the minimum number of allowable
						//  characters.
			}
			//  The input string fails to match for any number of swallowed
			//  characters, return failure.
			return(0);
			break;
			}
			//  Terminal operator indicates we have reached the end of the 
			//  compiled expression, return success.
		case END:
			return(1);	// Success! 
			break;
			//  Unexpected node, code log fault and return FALSE.
		default:
			TRACE0("regexp corruption\n");
			return(0);
			break;
		}
	}

	// We get here only if there's trouble -- normally "case END" is
	// the terminating point.  This should never happen during normal operation	 
	TRACE0("corrupted pointers\n");
	return(0);
}


////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
size_t CRegExp::MaxGreedyCharacters(char *operandToGreedy)
{
	//  Local variable to hold a count of characters, if needed.
	size_t count;
	//  Local variables to iterate over characters in m_szStringToMatch for the
	//  EXACTLY operator.
	char *szInputIter;
	char characterToMatch;

	//  Browser - new variables to enable case insensitive comparison for
	//  ANYOF and ANYBUT
	char* lcaseInputString;
	size_t retVal;
	UINT lcaseIter;
	//  End Browser modifications

	//  The number of characters which can be swallowed is dependant on the 
	//  operand of the PLUS or STAR operator.
	switch (OP(operandToGreedy)) 
	{
	case ANY:
		//  '.*' can swallow the entire remaining length of the input string, 
		//  return this length
		return(strlen(m_szStringToMatch));
		break;
	case EXACTLY:
		//  'a*' can swallow a's so return the number of consecutive characters
		//  in the input string, m_szStringToMatch
		//  The character to match is stored in the Operand of the EXACTLY 
		//  operator, though the operand is strictly a string it will only ever
		//  be one character, ab* will not match ababababab, it matches abbbbbbb.
		characterToMatch = *OPERAND(operandToGreedy);

		//  Loop over the input string counting the consecutive matching 
		//  characters.
		//  Browser - modified to make case insensitive
		count = 0;
		for (szInputIter = m_szStringToMatch; 
			tolower(*szInputIter) == tolower(characterToMatch); 
			szInputIter++)  
			count++;

		//  Return the consecutive characters.
		return(count);
		break;
	case ANYOF:
		{
		//  Browser - modification to enable case insensitive comparison.
		//  Create a lower case copy of the input string to match.
		//  The characters in the compiled expression for ANYOF are always stored
		//  as lowercase.
		UINT inputLength = strlen(m_szStringToMatch);
		lcaseInputString = new char[inputLength+1];
		memset(lcaseInputString, 0, inputLength+1);
		strcpy(lcaseInputString, m_szStringToMatch);
		for(lcaseIter = 0; lcaseIter < inputLength; lcaseIter++)
		{
			lcaseInputString[lcaseIter] = tolower(lcaseInputString[lcaseIter]);
		}

		//  The ANYOF operand holds a string of characters which the input string
		//  characters CAN be.
		//  Return the index of the first character in the lower case 
		//  m_szStringTomatch which is not in that string.
		retVal = (strspn(lcaseInputString, OPERAND(operandToGreedy)));

		//  Browser - free memory associated with lower case conversion
		delete[] lcaseInputString;
		lcaseInputString = NULL;
		//  End Browser modifications
		
		return retVal;
		//  Browser - Original case sensitive return
//		return(strspn(m_szStringToMatch, OPERAND(node)));  
		break;
		}
	case ANYBUT:
		{
		//  Browser - modification to enable case insensitive comparison.
		//  Create a lower case copy of the input string to match.
		//  The characters in the compiled expression for ANYBUT are always stored
		//  as lowercase.
		UINT inputLength = strlen(m_szStringToMatch);
		lcaseInputString = new char[inputLength+1];
		memset(lcaseInputString, 0, inputLength+1);
		strcpy(lcaseInputString, m_szStringToMatch);
		for(lcaseIter = 0; lcaseIter < inputLength; lcaseIter++)
		{
			lcaseInputString[lcaseIter] = tolower(lcaseInputString[lcaseIter]);
		}

		//  The ANYBUT operand holds a string of characters which the input string
		//  characters CAN NOT be.
		//  Return the index of the first character in the lower case 
		//  m_szStringTomatch which is not in that string.
		retVal = (strcspn(lcaseInputString, OPERAND(operandToGreedy)));

		//  Browser - free memory associated with lower case conversion
		delete[] lcaseInputString; 
		lcaseInputString = NULL;
		//  End Browser modifications

		return retVal;
		//  Browser - Original case sensitive return
//		return(strcspn(m_szStringToMatch, OPERAND(node))); 
		}
		break;
	default:		// Oh dear.  Called inappropriately. 

		//  Function called with an invalid operand for * or +, log an error
		//  and return failure.
		TRACE0("internal error: bad call of MaxGreedyCharacters\n");
		return(0);	// Best compromise. 
		break;
	}
	// NOTREACHED 
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
///					Modified to work on mobile devices by Andreas Phol
///					Fixed Problem of Pointer overflow for compiled expressions 
///					longer than 255 - DCC
////////////////////////////////////////////////////////////////////////////////
char *CRegExp::NextNode(char *pNode)
{
	// Changed by Andreas Pohl, 01/07/2002
	// org: const short &offset = *((short*)(p+1));
	// andreas: const short &offset = *((short*)(p+1));

	//  Browser - Andreas Phol's change works on mobile devices, 
	//					the original code works on the desktop, modified
	//					so the same code works on desktop and mobile devices.
	
	//  The next pointer is stored in the byte directly after the operator as
	//  a relative offset of the 'next' operator from the current operator.
	//  e.g. in the Bitmap (which equates to x?):
	//  Loc.	Val.	Interpretation:
	//  0x0000	6		BRANCH
	//  0x0001	17		Next Operator at location 0x0017	(END)
	//  0x0002	0		
	//  0x0003	6		BRANCH
	//  0x0004	8		Next Operator at location 0x0011	(BRANCH)
	//  0x0005	0		
	//  0x0006	8		EXACTLY
	//  0x0007	8		Next Operator at location 0x0014	(NOTHING)
	//  0x0008	0		
	//  0x0009	x		string value 'x'
	//  0x0010	0		string value '\0'
	//  0x0011	6		BRANCH
	//  0x0012	3		Next Operator at 0x0014				(NOTHING)
	//  0x0013	0		
	//  0x0014	9		NOTHING
	//  0x0015	3		Next Operator at 0x0017				(END)
	//  0x0016	0		
	//  0x0017	0		END

//#ifndef UNDER_CE
	//  Original Code
//	const short &offset = *((short*)(pNode+1));
//#else
	//  Browser Modification - 
	//  Andreas Phol's code change assumed pNode+2 (second pointer)
	//  contained multiples of 256, but this has been modified to contain
	//  multiples of 128 (as using signed bytes).  Old code commented
	//  out
	//	const short &offset = *(pNode+1) + *(pNode+2)*256;
	const short &offset = *(pNode+1) + *(pNode+2)*128;
//#endif
	if (offset == 0)
		return(NULL);

	if (offset < 0 || offset > 100)
		int i = 9;

	//  Return the value being pointed at.  If the operator is of type BACK then
	//  the pointer actually points backwards, not forwards, so the offset
	//  needs to be subtracted from the current operator location.
	return((OP(pNode) == BACK) ? pNode-offset : pNode+offset);
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Derived from original work by Henry Spencer
////////////////////////////////////////////////////////////////////////////////
char* CRegExp::ReplaceString( const char* szReplaceExp )
{
	//  Pointer to the replace expression which can be used as a working copy
	char *szReplaceExpWorking = (char *)szReplaceExp;

	//  Memory allocated to store the return string
	char *szReturnBuffer;

	//  Local variables for manipulating the replace expression.
	char currentChar;
	int bracketNumber;
	size_t lengthOfTaggedExpression;

	//  Check that we have been provided with a valid replace expression and
	//  a 'MatchStringToPattern' has succeeded.
	if( szReplaceExp == NULL || m_szLastMatchedString == NULL )
		return NULL;

	// First compute the length of the string which will be returned.
	int lengthOfReplaceString = 0;

	//  Iterate over each character in the replace string
	while ((currentChar = *szReplaceExpWorking++) != '\0') 
	{
		//  BracketNumber 0 indicates the entire matched string
		//  BracketNumber 1 - 9 indicates a tagged expression.
		//  BracketNumber -1 indicates no bracket.
		//  Calculate the BracketNumber
		if (currentChar == '&')
			bracketNumber = 0;
		else if (currentChar == '\\' && iswdigit(*szReplaceExpWorking))
			bracketNumber = *szReplaceExpWorking++ - '0';
		else
			bracketNumber = -1;

		//  Calculate the length in the output string required by this replace 
		//  expression
		if (bracketNumber < 0) 
		{	
			//  Ordinary character, not a tagged expression.  
			//  Also Check for \\ and \& which will be ordinary characters.
			if (currentChar == '\\' && (*szReplaceExpWorking == '\\' || *szReplaceExpWorking == '&'))
				currentChar = *szReplaceExpWorking++;

			//  As it's only an ordinary character increment the length of the 
			//  output string by 1.
			lengthOfReplaceString++;
		} 
		//  Check validity of the start and end pointers to the input string 
		//  which were populated during MatchStringToPattern.  This ignores
		//  user attempting to access invalid subexpression, e.g. \3 for
		//  a regular expression with only 2 sub expressions.
		else if (m_szStartp[bracketNumber] != NULL && m_szEndp[bracketNumber] != NULL &&
					m_szEndp[bracketNumber] > m_szStartp[bracketNumber]) 
		{
			//  Get tagged expression and calculate the length using pointer
			//  arithmatic.
			lengthOfTaggedExpression = 
					m_szEndp[bracketNumber] - m_szStartp[bracketNumber];
			lengthOfReplaceString += lengthOfTaggedExpression;
		}
	}

	//  Now allocate memory for the string which will be returned to the user
	//  containing the replaced expression.
	szReturnBuffer = new char[lengthOfReplaceString+1];
	if( szReturnBuffer == NULL )
		return NULL;
	//  Browser - modified to nullify the memory in the newly 
	//  allocated string
	memset(szReturnBuffer, 0, lengthOfReplaceString+1);

	//  Pointer to allocated memory, sole function is so we have a pointer
	//  to return to the user.
	char* szReturnBufferPointer = szReturnBuffer;

	// Add null termination
	//  Browser:
	//  This line is obsolete following the nullification of all the memory
	//  a couple of lines above.
	//szReturnBuffer[lengthOfReplaceString] = '\0';
	
	// Now we can create the string
	//  Reset the pointer to the replace expression
	szReplaceExpWorking = (char *)szReplaceExp;

	//  Iterate over each character in the replace expression.  Much of
	//  the functionality is similar to the calculation of the replace string
	//  length.
	while ((currentChar = *szReplaceExpWorking++) != '\0') 
	{
		if (currentChar == '&')
			bracketNumber = 0;
		else if (currentChar == '\\' && iswdigit(*szReplaceExpWorking))
			bracketNumber = *szReplaceExpWorking++ - '0';
		else
			bracketNumber = -1;

		if (bracketNumber < 0) 
		{	
			// Ordinary character or escaped character. 
			if (currentChar == '\\' && 
				(*szReplaceExpWorking == '\\' || *szReplaceExpWorking == '&'))
				//  For escaped characters advance the current Char pointer to copy
				//  the character, rather than the '\'
				currentChar = *szReplaceExpWorking++;

			//  Copy the character directly.
			*szReturnBuffer++ = currentChar;
		} 
		else if (m_szStartp[bracketNumber] != NULL && 
				m_szEndp[bracketNumber] != NULL &&
				m_szEndp[bracketNumber] > m_szStartp[bracketNumber]) 
		{
			//  Get tagged expression
			lengthOfTaggedExpression = m_szEndp[bracketNumber] - m_szStartp[bracketNumber];
			int tagOffset = m_szStartp[bracketNumber] - m_szStartp[0];
			
			//  Copy the tagged exression.  The tagged expression is the characters
			//  between m_szStartp[bracket] and m_szEndp[bracket], which are
			//  in turn just memory offsets from the start of 
			//  m_szLastMatchedString.
			strncpy(szReturnBuffer, m_szLastMatchedString + tagOffset, 
				lengthOfTaggedExpression);
			szReturnBuffer += lengthOfTaggedExpression;
		}
	}
	//  Return the pointer to the replaced expression.
	return szReturnBufferPointer;
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Initially Created
////////////////////////////////////////////////////////////////////////////////
char* CRegExp::stristr(char* szStringToBeSearched, const char* szSubstringToSearchFor)
{
	char* pPos = NULL;
	//  Copies of strings to avoid lowering the case of the actual strings
	char* szCopy1 = NULL;
	char* szCopy2 = NULL;

	// verify parameters
	if ( szStringToBeSearched == NULL ||
			szSubstringToSearchFor == NULL )
	{
	  return szStringToBeSearched;
	}

	// empty substring - return input (consistent with strstr)
	if (strlen(szSubstringToSearchFor) == 0 )
	{
		return szStringToBeSearched;
	}

	//  duplicate strings and convert to lower case
	szCopy1 = _strlwr(_strdup(szStringToBeSearched));
	szCopy2 = _strlwr(_strdup(szSubstringToSearchFor));

	if ( szCopy1 == NULL || szCopy2 == NULL  ) 
	{
		//  Problem converting strings to lower case, return
		free((void*)szCopy1);
		free((void*)szCopy2);
		return NULL;
	}

	//  use strstr on the lower case copies.
	pPos = strstr(szCopy1, szCopy2);

	if ( pPos != NULL ) {
		// map to the original string
		pPos = szStringToBeSearched + (pPos - szCopy1);
	}

	free((void*)szCopy1);
	free((void*)szCopy2);

	return pPos;
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Initially Created
////////////////////////////////////////////////////////////////////////////////
char* CRegExp::strichr(char* str, char c)
{
	//  If the lower case character is not found in the string return the result of looking for the 
	//  upper case character
	if (strchr(str, tolower(c)) == NULL)
	{
		return strchr(str, toupper(c));
	}
	else
		return strchr(str, tolower(c));

}