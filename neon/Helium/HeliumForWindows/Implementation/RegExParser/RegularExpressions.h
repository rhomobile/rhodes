/**
 * \file RegularExpressions.h
 * \brief Class to wrap regular expression functionality. 
 */

#include "regexp.h"

/**
 * Class to provide a user friendly way of using Regular Expressions.
 */
class CRegEx
{
public:
	/**
	 * Function to replace insertion specifiers with passed strings. 
	 * Regular expression constructor, performs no action.
	*/
	CRegEx();
	/**
	 * Function to replace insertion specifiers with passed strings. 
	 * Regular expression destructor, performs no action.
	*/
	~CRegEx();

	/**
	 * Note: All parameters are case insensitive.
	 * Given a regular expression returns the position of 
 	 * the first match found in the input.  Returns -1 if no 
	 * match was found.
 	 * Only a subset of the full functionality of regular expressions 
 	 * is available, most notable absentees are {}, the ability to specify lazy repition
 	 * operators and back references, e.g. matching using <([a-z])>.*</(\1)> to match the
 	 * the contents of simple HTML tags.
\verbatim
+------+-----------------------------------------------------+
|char  | Description                                         |
|      |                                                     |
| ^    | Matches operator at beginning of string, e.g. ^h    |
|      | will match hello.                                   |
| ^    | Caret immediately following left bracket ([) will   |
|      | exclude remaining characters within brackets,       |
|      | e.g. n[^0-9] will not match n1.                     |
| $    | Matches end of the string, e.g. o$ will match       |    
|      | hello but not howdy.                                |
| |    | Allows expression either side to match target       |
|      | string, e.g. a|b will match a or b.                 |
| .    | Will match any character.                           |
| *    | Character to the left of the * should match 0 or    |
|      | more times.                                         |
| +    | Similar to * but will match 1 or more times.        |
| ?    | Match character to the left 0 or 1 times.           |
| ()   | Affects order of pattern evaluation and serves to   |
|      | tag expressions during find &                       |
|      | replace operation.                                  |
| []   | Brackets, [], enclosing a set of characters         |
|      | indicates that any of the enclosed characters       |
|      | may match the target character.                     |
| -    | Allows specification of a range within [] brackets, | 
|      | e.g. [a-z0-9] will match any alphanumeric digit.    |
+------------------------------------------------------------+
\endverbatim
	 * Examples :
\code
Find(L"hello ABC", L"abc")
\endcode
 will return 6
\code
Find(L"This is <bold>Bold</bold> Text", L"<[^<>]+>")
\endcode
will return 8
		\param inputString String to be match
		\param patternToMatch The pattern against which to match the string
	 */
	int Find(
		LPCWSTR inputString,
		LPCWSTR patternToMatch);

	/**
	 * Matches an input string against a specified regular expression and 
	 * creates a new string based on the replace expression.<P>
	 * Note: All parameters are case insensitive<P>

	 * If the input successfully parses the 'result' parameter will be populated
	 * with the replaceExpression modified such that token specifiers are replaced with
	 * the actual tokens found in the inputString.  Tokens are specified in the regular
	 * expression with () parentheses.  Tokens are accessed in the replaceExpression
	 * using \1 for the first bracketed expression, \2 for the second e.t.c.
	 * '&' can be used to access the entire matched expression, which \b may be only
	 * a substring of the inputString.  '\\\\' and '\\&' can be used to escape '\\' or '&'.  
	 * <P>Example:
\code
FindAndReplace(
L"rgb:ff,00,cd",
L".*:([a-f0-9]+),([a-f0-9]+),([a-f0-9]+)",
L"color:#\\1\\2\\3",
outputString);
\endcode
	 * After which outputString will contain the text "color:#ff00cd"<P>
	 * Usage:
\code
CRegEx* RegExObj = new CRegEx();
int lengthOfReplace = 0;
BOOL parsed = FALSE;
//  Firstly Find / Replace to get the length of the returned string
parsed = RegExObj->FindAndReplace(INPUT_STRING, PATTERN_TO_MATCH, REPLACE_EXPRESSION, &lengthOfReplace, NULL);
//  We now know the length of the substituted String
TCHAR *substitutedString = new TCHAR[lengthOfReplace];
memset(substitutedString, 0, lengthOfReplace * sizeof(TCHAR));
//  Secondly Find / Replace to actually do the Finding / Replacing
parsed = RegExObj->FindAndReplace(INPUT_STRING, PATTERN_TO_MATCH, REPLACE_EXPRESSION, &lengthOfReplace, substitutedString);
//  Substituted String now contains the result of the Replace, do your processing
delete substitutedString;
\endcode
	 * \param inputString The string to match against the regular expression
	 * \param patternToMatch The regular expression to perform the find.  See \find for the syntax of this parameter
	 * \param replaceExpression The replace expression template
	 * \param lengthOfResult The length of the result parameter including the terminating NULL character.  Populated regardless
	 * of whether the result parameter is NULL, this can be used to determine the size of string
	 * buffer to assign to result.  Note that if the expression does not parse then
	 * lengthOfResult will contain the length of the input string.
	 * \param result The result of substituting the tokens in the replaceExpression with the matches found in the inputString.  
	 *  If the expression fails to parse this parameter is populated with the inputString.
	 * \return The position of the first match found in the input string or -1 
	 *  if no match was found.
	 */
	int FindAndReplace(
		LPCWSTR inputString, 
		LPCWSTR patternToMatch, 
		LPCWSTR replaceExpression, 
		int* lengthOfResult, 
		TCHAR* result);

};
