/** 
* \file RegExp.h
* \brief Implementation code for Regular Expressions.
*
* This code has been derived from work by Henry Spencer. 
* The main changes are
* 1. All char variables and functions have been changed to char
*    counterparts
* 2. Added GetFindLen() & GetReplaceString() to enable search
*    and replace operations.
* 3. And of course, added the C++ Wrapper
*
* The original copyright notice follows:
*
* Copyright (c) 1986, 1993, 1995 by University of Toronto.
* Written by Henry Spencer.  Not derived from licensed software.
*
* Permission is granted to anyone to use this software for any
* purpose on any computer system, and to redistribute it in any way,
* subject to the following restrictions:
*
* 1. The author is not responsible for the consequences of use of
* this software, no matter how awful, even if they arise
* from defects in it.
*
* 2. The origin of this software must not be misrepresented, either
* by explicit claim or by omission.
*
* 3. Altered versions must be plainly marked as such, and must not
* be misrepresented (by explicit claim or omission) as being
* the original software.
*
* 4. This notice must not be removed or altered.
*
* Ported to compile with eMbedded Visual C++ 3 by Andreas Pohl in 2002.
* Changes:
*  - TCHAR to char
*  - generic text routines to SBCS ones (for example _tcslen to strlen)
*  - CRegExp::NextNode
*  - CRegExp::regtail
*
* Modified for use in Symbol Browser by Darryn Campbell in 2008
* Changes:
*  - Made case insensitive
*    - Added functions:
*       char *stristr(char* szStringToBeSearched, const char* szSubstringToSearchFor);
*       char* strichr(char* str, char c);
*  - Eliminated memory leaks
*  - Commented code to aid legibility
*/

#include <windows.h>

/**  The number of bracketed expressions {}
*  These are remembered for purposes of replacing
*  Changing this value above 10 will not increase
*  the capacity of the class to handle >10 bracketed
*  expressions as the OPEN and CLOSE operators are
*  defined as being 10 apart in the operator table.
*/
#define NSUBEXP  10		

/**
* Class to implement regular expression functionality, see \ref CRegEx for a more user friendly interface.
<PRE>
1. Compile your specified Regular Expression using \ref CompileRegularExpression
2a. Attempt to match a string against your regular expression using \ref MatchString
2b. Replace characters in your specified string using \ref ReplaceString
</PRE>
*/
class CRegExp
{
public:
	/**
	*  Constructor for CRegExp class, initialises member variables where appropriate
	*/
	CRegExp();

	/**
	*  Destructor for CRegExp class, free memory which is not freed elsewhere
	*/
	~CRegExp();

	/**
	*  Compile the regular expression.  
	*  The format of the compiled expression is more fully explained in 
	*  CompiledPattern.xls.
	*  For example, compiling ".*da" will yield:
\verbatim 
BRANCH
STAR
ANY
EXACTLY
"da"
END
\endverbatim
	* \param szcRegularExpression String to be compiled into a regular expression.
	*/
	CRegExp *CompileRegularExpression( const char *szcRegularExpression );

	/**
	*  Method to accept a string to be matched against a previously compiled
	*  regular expression.  
	*  If the input string matches the expression then the 
	*  position of the match in the string is returned, that is the position of the
	*  first character in the string which is part of the pattern.  If the string
	*  does not match the compiled expression then -1 is returned.
	*  e.g. Consider the compiled expression: b+c[1-4] and the 
	*  input string: BBC1.  MatchString will return 0 as the first character
	*  in the input string (index 0) is part of the pattern.  For
	*  the input string: watchBBC1 MatchString will return 5 as the portion of the
	*  input string which matches the pattern starts at character 6 (index 5).
	*  This method uses some information from compilation of the regular expression
	*  to increase efficiency.
	* \param str String to Match
	* \return Position of the Match, or -1
	*/
	int MatchString(const char *str);

	/**
	*  Function responsible for producing the 'Replace' string from the
	*  provided replace expression.  
	*  Prerequsites to calling this function are the class must have been provided
	*  with a regular expression which successfully compiled and matched a 'find' 
	*  string to that compiled pattern.  
	*  \param sReplaceExp 
	*  The replace expression can access matched
	*  tagged expressions using \1 for tagged expression 1 up to \9 for tagged
	*  expression 9.  The entire matched expression can be accessed via '&'.  '\'
	*  or '&' can both be escaped: '\\' or '\&'.
	*  \return 
	*  This function returns a string with all \1 thru \9 replaced with their 
	*  relevent tagged expressions and '&' replaced with the entire matched 
	*  expression.  It is the responsibility of the caller to deallocate the
	*  memory associated with this 'Replace' string.
	*  e.g. Regular Expression (a|b)+([cd])*(e) matched to string:
	*  "abadce" with replace expression: "\3--\2--\1--&" will procuce string:
	*  e--dc--aba--abadce.  The replace expression can also contain ordinary
	*  characters which will be copied directly into the output, '-' in the 
	*  example.
	*/
	char* ReplaceString( const char* sReplaceExp );
	
	/**
	*  Inline function to find the length of the string matched to the 
	*  given regular expression during a Find.  If the regular expression is
	*  b+c[1-4] and the string to match is "watchBBC1" then the length of
	*  the matching string will be 4 as only BBC1 is matched by the pattern.
	*  \return The length of the matched string
	*/
	int LengthOfMatchedString()
	{
		//  m_szStartp[0] holds a pointer to the start of the entire matched string.
		//  m_szEndp[0] holds a pointer to the end of the entire matched string.
		//  Check the string to match is not NULL.
		if( m_szStartp[0] == NULL || m_szEndp[0] == NULL )
			return 0;

		//  Remember m_szStartp[0] and m_szEndp[0] point to positions in the string being 
		//  matched, the difference of them will return the number of characters
		//  in the matched string via pointer arithmatic.
		return m_szEndp[0] - m_szStartp[0];
	};


private:
	/////////////////////////////////////////////
	//
	//   Find Methods
	//
	/////////////////////////////////////////////

	/**
	*  Function to test whether the specified string matches the previously
	*  compiled regular expression.  
	*  \param szStringToMatch The string to Match
	*  \return 1 if the match succeeds and 0 if the
	*  string does not match the pattern.  If the match succeeds the arrays of
	*  sub expression matches (\ref m_szStartp / \ref m_szEndp) are populated but if the 
	*  match fails their contents should be considered invalid.
	*/
	int TestPotentialMatch(char *szStringToMatch);

	/**
	*  Function for matching \ref m_szStringToMatch to the previously compiled 
	*  regular expression.  Iterate over the nodes in the
	*  compiled expression and perform actions (dependant on the type of node) to
	*  determine whether m_szStringToMatch is matching the expression.
	*  This function is called recursively for the following nodes: OPEN, CLOSE, 
	*  BRANCH, STAR, PLUS.  Though computationally expensive this is required to
	*  implement back tracking and processing of the tree branches.  
	*  \return
	*  Once
	*  all the nodes in the regular expression have been swallowed successfully
	*  by m_szStringToMatch the match has been successful and the function Returns 
	*  TRUE.  The function will return FALSE at any stage if the input fails to 
	*  match the node being processed which will either return FALSE to the user
	*  or try to match an alternative branch / backtrack when the function has
	*  been called recursively.
	*  \param compiledExpression previously compiled regular expression
	*/
	int MatchStringToPattern(char *compiledExpression);

	/**
	*  Determine the maximum number of characters which can be swallowed by either
	*  the * or + operators for the current position of m_szStringToMatch.  
	*  \param node
	*  The parameter is the operand to either the * or + operator, which can logically 
	*  be either ANY (e.g. .*); EXACTLY (e.g. a*); ANYOF (e.g. [a-d]*) or ANYBUT
	*  (e.g. [^a-d]*).
	*  \return Maximum number of characters which can be swallowed
	*/
	size_t MaxGreedyCharacters(char *node);

	////////////////////////////////////////////
	//
	//   Compilation Methods
	//
	////////////////////////////////////////////

	/**
	*  CompileExpression will take the m_regularExpression input and compile it
	*  to \ref m_compiledExpression.  Bracketed sub-expressions are handled as separate
	*  expressions and CompileExpression is called on each sub-expression with
	*  the appropriate parenthesis number:
	*  a([a-c])d([e-f])g will be compiled as:
\verbatim 
CompileExpression(0)		//  Compile a()d()g
CompileExpression(1)		//  Compile ([a-c])
CompileExpression(2)		//  Compile ([e-f])
\endverbatim 
	*  \param paren Bracketed sub-expression being compiled.
	*  \param pFlags pFlags is instantiated with the flags being returned.  See regexp.cpp
	*  for flag definitions.
	*  \return Returns a pointer to the compiled expression or NULL on failure
	*/
	char *CompileExpression(int paren, int *pFlags);

	/**
	*  Creates a new branch in the compiled output.  A branch forms one side
	*  of the OR operator '|' and this method is responsible for compiling
	*  the pattern in the current branch to \ref m_compiledExpression.
	*  m_regularExpression iterates over the input string and on entry to 
	*  this method will be at the start of the current branch.
	*  e.g. The regular expression a|b|c will result in 3 calls to CreateBranch
	*  \pFlags pFlags is instantiated with the flags being returned.  See regexp.cpp
	*  for flag definitions.
	*  \return The newly created branch.
	*/
	char *CreateBranch(int *pFlags);

	/**
	*  Creates a new 'piece' in the compiled output.  A piece is defined as 
	*  an 'atom' followed by an optional repitition operator, *, + or ?.  An atom is 
	*  essentially a single node and is created by CreateAtom().
	*  The main purpose of CreatePiece is to create the nodes and branches 
	*  required to format the compiled expression to cope with *, + and ?.
	*  For example (x)* is interpreted as two branches, one which can match '(x)' 
	*  and one which can match NOTHING, the branch matching x being followed by
	*  an optional BACK pointer, implementing a loop, to enable the expression to 
	*  match xx, xxx e.t.c.
	*  \pFlags pFlags is instantiated with the flags being returned.  See regexp.cpp
	*  for flag definitions.
	*  \return The newly created piece.
	*/
	char *CreatePiece(int *pFlags);

	/**
	*  Creates a new 'atom' in the compiled output.  An atom is the simplest 
	*  component of a regular expression consisting of a single operator and
	*  its associated operand.  All operators (except END, BRANCH, BACK, NOTHING,
	*  STAR, PLUS, OPEN, CLOSE) and their associated operands can be subsumed
	*  in a single atom.
	*  \pFlags pFlags is instantiated with the flags being returned.  See regexp.cpp
	*  for flag definitions.
	*  \return The newly created atom.
	*/
	char *CreateAtom(int *pFlags);

	//////////////////////////////////////////////////////////
	//
	//   Node Chain Modification Methods
	//
	//////////////////////////////////////////////////////////

	/**
	*  Insert the specified Operator before the specified operand in the 
	*  compiled expression.  This is used when the operand is written to the 
	*  compiled expression before the operator and the operator 
	*  is required to come before the operand.  See the \ref OPERAND method
	*  comments for the structure of a node.
	*  E.g. a* will output the EXACTLY 'a' operator (The Operand of the STAR
	*  operator) before the STAR operator is output, however the compiled
	*  expression needs to be in the format:
\verbatim
00 STAR      (next = ?, operand = 03)
03 EXACTLY   (next = ?, operand = 'a')
\endverbatim
	*  \param theOperator The operator to insert before the operand.
	*  \param theOperand The operand before which to insert the operator.
	*/
	void InsertOperatorBeforeOperand(char theOperator, char *theOperand);

	/**
	*  Nodes are linked together by their next pointers, this method 
	*  will set the next pointer of the node at the end of the specified 
	*  node chain to point to the specified 'orphan' node.
	*  e.g. 
\verbatim
00 Node1	(next = 03)	<-- nodeChain
03 Node1	(next = 06)
06 Node1	(next = ?)
09 Node1	(next = ?)	<-- orphanNode
becomes
00 Node1	(next = 03)	<-- nodeChain
03 Node1	(next = 06)
06 Node1	(next = 09)
09 Node1	(next = ?)	<-- orphanNode
after calling this method.
\endverbatim
	*  \param nodeChain Chain of notes onto which to attach the orphanNode
	*  \param orphanNode Node to append to the specified nodeChain
	*/
	void AppendNodeToNodeChain(char *nodeChain, char *orphanNode);

	/**
	*  Any BRANCH node comprises of a series of zero or more child nodes, the
	*  first child node being stored as the operand to the BRANCH.  The child 
	*  nodes can be linked together to form a node chain.  This function will 
	*  set the next pointer of the final node in the branch's node chain to
	*  point to the node being appended.
	*  e.g. 
\verbatim
00 BRANCH	(next = ?, operand = 03)
03 Node 1	(next = 06)
06 Node 2	(next = ?)
09 Node 3	(next = ?)
becomes
00 BRANCH	(next = ?, operand = 03)
03 Node 1	(next = 06)
06 Node 2	(next = 09)
09 Node 3	(next = ?)
after calling this method.
\endverbatim
	*  \param branch to have its node chain appended to.
	*  \param node Node to append to the specified branch's node chain.
	*/
	void AppendNodeToBranch(char *branch, char *node);

	/**
	*  Given a node this function will return a pointer to the byte 
	*  in the compiled expression pointed at by the node's 'next' pointer.
	*  The return value will either be another node or NULL if the given 
	*  node has no children.  The type of Node can be obtained using the OP() 
	*  function.
\verbatim
e.g. given:
00 Node1 (next = 03)
03 Node2 (next = 06)
06 Node3 (next = 09)
09 Node4 (next = NULL)
NextNode(Node2) will return *Node3
OP(NextNode(Node2)) will return Node3
NextNode(Node4) will return NULL
\endverbatim
	*  \param pNode Node you want to find the 'Next Node' of
	*  \return pointer to a byte (see description)
	*/
	char *NextNode(char *pNode);

	// Inline functions
private:
	/**
	*  Function to return the operator at the position specified by p, this 
	*  relies on p actually pointing to an operator as all it does is 
	*  dereference the pointer.
	*  \param p Position of the operator.
	*  \return Operator of p.
	*/
	char OP(char *p) {return *p;};

	/**
	*  Funtion to return the operand associated with the operator at position
	*  p.  Again this relies on p actually pointing to an operator with the 
	*  additional restriction that the operator must actually have an operand,
	*  see the Operator table in regexp.cpp.
	*  The structor of the operator and operand nodes is as follows:
\verbatim
+----------------------+
| Operator             |	<-- p
+----------------------+
| pointer to next node |
+----------------------|
| pointer overflow     |
+----------------------+
| Operand              |	<-- (p+1)+1
+----------------------+
| Operand              |
+----------------------|
| Operand              |
+----------------------+
\endverbatim
	*  The Operand will be either a Node or a String, so is actually variable
	*  length but is shown above as a Node occupying 3 bytes.
	*  As can be seen in the diagram OPERAND returns the operand (p+2) of the
	*  operator p.
	*  \param p operator which you want the operand of.
	*  \return operand associated with the operator at position p.
	*/
	char *OPERAND( char *p) {return (char*)((short *)(p+1)+1); };

	/**
	*  Write a character to the compiled expression, used to write literal 
	*  strings, ranges of characters and escaped characters.
	*  Writes the ASCII character code to the compiled expression.
	*  Will only write a character if \ref m_bOutputCompiledExpression is true.
	*  \param b Character to write
	*/
	void WriteCharacterToExpression(char b)
	{
		if (m_bOutputCompiledExpression)
			*m_lastByteInCompiledExpression++ = b;
		else
			m_compiledExpressionSize++;
	};

	/**
	*  Write a node to the compiled expression, used to create all types of
	*  node defined in the Operator table in regexp.cpp.
	*  Will only write a node if the \ref m_bOutputCompiledExpression is true.
	*  \param op Node to write to the compiled expression
	*  \return \ref m_lastByteInCompiledExpression
	*/
	char *	WriteNodeToExpression(char op)
	{
		if (!m_bOutputCompiledExpression) {
			m_compiledExpressionSize += 3;
			return m_lastByteInCompiledExpression;
		}

		//  Add the operator node to the end of the m_compiledExpression byte
		//  array and advance the end pointer to the last byte in said array.
		//  Next pointer is always initialised as \0, therefore it is not
		//  sufficient to just call WriteNodeToExpression(), it must be linked to the other nodes
		//  using the functions regtail, regoptail and InsertOperatorBeforeOperand.
		*m_lastByteInCompiledExpression++ = op;
		*m_lastByteInCompiledExpression++ = '\0';		/* Null next pointer. */
		*m_lastByteInCompiledExpression++ = '\0';

		return m_lastByteInCompiledExpression-3;
	};


private:
	/*
	*  The regular expression is parsed twice, first to ascertain the compiled
	*  expression size and to determine syntactic correctness, secondly to
	*  actually output the compiled expression.  \ref m_bOutputCompiledExpression is used to keep 
	*  track of which parse is being performed.
	*/
	BOOL m_bOutputCompiledExpression;

	/**
	*  bCompiled is used to keep track of whether a regular expression has
	*  been compiled for the current object.  If an expression has not been
	*  compiled this variable is set to False and \ref MatchString will fail immediately.
	*/
	BOOL m_bCompiled;

	/**
	*  m_szLastMatchedString is populated with the last string to match the regular 
	*  expression, populated by \ref MatchString and used by GetReplaceString.
	*  If only a substring of the user specified input string matches the 
	*  pattern then m_szLastMatchedString will be set to this substring.
	*/
	char *m_szLastMatchedString;

	/**
	*  m_szStartp is an array of pointers to reginput which define the 
	*  positions in reginput which start matching each of the bracketed
	*  expressions in the m_regularExpression.  m_szStartp[0]
	*  is a special case of this which points to the start of the the entire 
	*  matched string.
	*/
	char *m_szStartp[NSUBEXP];

	/**
	*  m_szEndp is an array of pointers to reginput which define the 
	*  positions in reginput which end matching each of the bracketed
	*  expressions in the m_regularExpression.  m_szEndp[0]
	*  is a special case of this which points to the end of the entire 
	*  matched string.
	*/
	char *m_szEndp[NSUBEXP];

	//  Efficiency:
	//  The following variables are used to increase the efficiency of pattern
	//  matching
	
	/**
	*  m_efficiencyStartChar stores the character which the matching string must start with
	*  (if known), if unknown m_efficiencyStartChar is set to \0.
	*/
	char m_efficiencyStartChar;		// Internal use only. 
	/**  m_efficiencyStartsAnchor stores whether the first character is '^' or not, possible values 
	*  are '0' and '1'.
	*/
	char m_efficiencyStartsAnchor;		// Internal use only. 
	/**
	*  m_efficiencyLongestString is the longest string in the regular expression which must be matched
	*  exactly, e.g. the expression string.*longerstring will set m_efficiencyLongestString to 
	*  'longerstring'
	*/
	char *m_efficiencyLongestString;		// Internal use only. 
	/**
	*  strlen(\ref m_efficiencyLongestString), This will be 12 for the expression 
	*  string.*longerstring
	*/
	int m_efficiencyLongestStringlen;		// Internal use only. 
	//  End Efficiency
	
	/**
	*  m_compiledExpression is the compiled regular expression, this variable is set as a 
	*  result of calling CompileRegularExpression
	*/
	char *m_compiledExpression;		// Unwarranted chumminess with compiler. 

	/**
	*  m_regularExpression is used to iterate over the given regular expression.  It points
	*  to the character in the regular expression given to \ref CompileRegularExpression which is
	*  currently being processed.
	*/
	char *m_regularExpression;	// Input-scan pointer. 
	
	/**
	*  Count of the number of bracketed expressions in the regular expression,
	*  this is limited to 10.  Note that even though \ref NSUBEXP is used and defined
	*  as 10 you can not simply increase the value of the NSUBEXP define because
	*  the close operator is defined as 30 (and the open operator is defined
	*  as 20), see the operator definition table in regexp.cpp.
	*/
	int m_countOfParentheses;		// () count. 

	/**
	*  m_lastByteInCompiledExpression points to the last byte (character) 
	*  in the compiled regular expression.
	*  As bytes are added to the compiled expression the value is incrememted
	*/
	char *m_lastByteInCompiledExpression;		// Code-emit pointer; ®dummy = don't. 
	
	/**
	*  m_regdummy serves no real purpose other than as an initialiser for
	*  \ref m_lastByteInCompiledExpression
	*/
	char m_regdummy[3];	// NOTHING, 0 next ptr 
	
	/**
	*  The size of the compiled regular expression in bytes.  This variable
	*  is set on the first pass over the expression and is used to determine 
	*  the size of m_m_compiledExpression
	*/
	long m_compiledExpressionSize;		// Code size. 

	/**
	*  Variable to hold the string which is being matched to the compiled 
	*  pattern.  This string will either be the entire user specified
	*  string to match or a substring thereof if only a portion of
	*  the string is being matched to the pattern.  Used and modified 
	*  by RegMatch.  As portions of the input string are matched this pointer
	*  is advanced so it always points to the remaining characters in the input
	*  string still to be matched.
	*/
	char *m_szStringToMatch;			 

	/**
	*  Pointer to the start of the user specified string to match.  This
	*  is used to match to expressions containing the BOL, beginning of line
	*  (^) character.
	*/
	char *m_szStartOfStringToMatch;		// Beginning of input, for ^ check. 

	/////////////////////////////////////
	//
	//  Browser added Methods
	//
	/////////////////////////////////////
	
	/**
	*  Returns a pointer to the first occurance of a search string in a string, 
	*  ignoring case
	*/
	char *stristr(char* szStringToBeSearched, 
					const char* szSubstringToSearchFor);
	
	/**
	*  perform case insensitive comparison for finding the first instance 
	*  of a character in a string
	*/
	char* strichr(char* str, char c);
};

