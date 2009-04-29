/* 
 * $Id: OpCode.java 124053 2005-01-04 01:24:35Z dfs $
 *
 * Copyright 2000-2005 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


package org.apache.oro.text.regex;

import j2me.lang.CharacterMe;

/**
 * The OpCode class should not be instantiated.  It is a holder of various
 * constants and static methods pertaining to the manipulation of the 
 * op-codes used in a compiled regular expression.
 *
 * @version @version@
 * @since 1.0
 */
final class OpCode {

  private OpCode() { }

  // Names, values, and descriptions of operators correspond to those of
  // Perl regex bytecodes and for compatibility purposes are drawn from
  // regcomp.h in the Perl source tree by Larry Wall.
  static final char  // Has Operand   Meaning
     _END     = 0,   // no       End of program.
     _BOL     = 1,   // no       Match "" at beginning of line.
     _MBOL    = 2,   // no       Same, assuming multiline.
     _SBOL    = 3,   // no       Same, assuming singleline.
     _EOL     = 4,   // no       Match "" at end of line.
     _MEOL    = 5,   // no       Same, assuming multiline.
     _SEOL    = 6,   // no       Same, assuming singleline.
     _ANY     = 7,   // no       Match any one character (except newline).
     _SANY    = 8,   // no       Match any one character.
     _ANYOF   = 9,   // yes      Match character in (or not in) this class.
     _CURLY   = 10,  // yes      Match this simple thing {n,m} times.
     _CURLYX  = 11,  // yes      Match this complex thing {n,m} times.
     _BRANCH  = 12,  // yes      Match this alternative, or the next...
     _BACK    = 13,  // no       Match "", "next" ptr points backward.
     _EXACTLY = 14,  // yes      Match this string (preceded by length).
     _NOTHING = 15,  // no       Match empty string.
     _STAR    = 16,  // yes      Match this (simple) thing 0 or more times.
     _PLUS    = 17,  // yes      Match this (simple) thing 1 or more times.
     _ALNUM   = 18,   // no       Match any word character
     _NALNUM  = 19,   // no       Match any non-word character
     _BOUND   = 20,  // no       Match "" at any word boundary
     _NBOUND  = 21,  // no       Match "" at any word non-boundary
     _SPACE   = 22,  // no       Match any whitespace character
     _NSPACE  = 23,  // no       Match any non-whitespace character
     _DIGIT   = 24,  // no       Match any numeric character
     _NDIGIT  = 25,  // no       Match any non-numeric character
     _REF     = 26,  // yes      Match some already matched string
     _OPEN    = 27,  // yes      Mark this point in input as start of #n.
     _CLOSE   = 28,  // yes      Analogous to OPEN.
     _MINMOD  = 29,  // no       Next operator is not greedy.
     _GBOL    = 30,  // no       Matches where last m//g left off.
     _IFMATCH = 31,  // no       Succeeds if the following matches.
     _UNLESSM = 32,  // no       Fails if the following matches.
     _SUCCEED = 33,  // no       Return from a subroutine, basically.
     _WHILEM  = 34,  // no       Do curly processing and see if rest matches.
     _ANYOFUN = 35,  // yes      Match unicode character in this class.
     _NANYOFUN= 36,  // yes      Match unicode character not in this class.
     _RANGE   = 37,  // yes      Range flag in 
    // Change the names of these constants later to make it clear they
    // are POSIX classes.
     _ALPHA   = 38,
     _BLANK   = 39,
     _CNTRL   = 40,
     _GRAPH   = 41,
     _LOWER   = 42,
     _PRINT   = 43,
     _PUNCT   = 44,
     _UPPER   = 45,
     _XDIGIT  = 46,
     _OPCODE  = 47,
     _NOPCODE = 48,
     _ONECHAR = 49,
     _ALNUMC  = 50,
     _ASCII   = 51;

  // Lengths of the various operands.
  static final int _operandLength[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // OpCode 0-9
    2, 2, 0, 0, 0, 0, 0, 0, 0, 0, // OpCode 10-19
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, // OpCode 20-29
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // OpCode 30-39
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // OpCode 40-49 
    0, 0                          // OpCode 50-51 
  };

  static final char _opType[] = {
	_END, _BOL, _BOL, _BOL, _EOL, _EOL, _EOL, _ANY, _ANY, _ANYOF, _CURLY,
	_CURLY, _BRANCH, _BACK, _EXACTLY, _NOTHING, _STAR, _PLUS, _ALNUM,
	_NALNUM, _BOUND, _NBOUND, _SPACE, _NSPACE, _DIGIT, _NDIGIT, _REF,
	_OPEN, _CLOSE, _MINMOD,	_BOL, _BRANCH, _BRANCH, _END, _WHILEM,
	_ANYOFUN, _NANYOFUN, _RANGE, _ALPHA, _BLANK, _CNTRL, _GRAPH,
	_LOWER, _PRINT, _PUNCT, _UPPER, _XDIGIT, _OPCODE, _NOPCODE,
	_ONECHAR, _ALNUMC, _ASCII
  };

  static final char _opLengthVaries[] = {
    _BRANCH, _BACK, _STAR, _PLUS, _CURLY, _CURLYX, _REF, _WHILEM
  };

  static final char _opLengthOne[] = {
    _ANY, _SANY, _ANYOF, _ALNUM, _NALNUM, _SPACE, _NSPACE, _DIGIT, _NDIGIT, 
    _ANYOFUN, _NANYOFUN, _ALPHA, _BLANK, _CNTRL, _GRAPH, _LOWER, _PRINT,
    _PUNCT, _UPPER, _XDIGIT, _OPCODE, _NOPCODE, _ONECHAR, _ALNUMC,
    _ASCII
  };

  static final int  _NULL_OFFSET  = -1;
  static final char _NULL_POINTER =  0;

  static final int _getNextOffset(char[] program, int offset) {
    return ((int)program[offset + 1]); 
  }

  static final char _getArg1(char[] program, int offset) {
    return program[offset + 2]; 
  }

  static final char _getArg2(char[] program, int offset) {
    return program[offset + 3]; 
  }

  static final int _getOperand(int offset) {
    return (offset + 2);
  }

  static final boolean _isInArray(char ch, char[] array, int start) {
    while(start < array.length)
      if(ch == array[start++])
	return true;
    return false;
  }

  static final int _getNextOperator(int offset) { return (offset + 2); }
  static final int _getPrevOperator(int offset) { return (offset - 2); }

  static final int _getNext(char[] program, int offset) {
    int offs;

    if(program == null)
      return _NULL_OFFSET;


    offs = _getNextOffset(program, offset);
    if(offs == _NULL_POINTER)
      return _NULL_OFFSET;

    if(program[offset] == OpCode._BACK)
      return (offset - offs);

    return (offset + offs);
  }

  // doesn't really belong in this class, but we want Perl5Matcher not to
  // depend on Perl5Compiler
  // Matches Perl's definition of \w, which is different from [:alnum:]
  static final boolean _isWordCharacter(char token) {
    return (CharacterMe.isLetterOrDigit(token) || token == '_');
  }
}


