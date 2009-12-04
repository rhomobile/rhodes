// $ANTLR 2.7.6 (2005-12-22): "src/com/xruby/compiler/parser/ruby.g" -> "RubyLexerBase.java"$

package com.xruby.compiler.parser;

import java.io.InputStream;
import antlr.TokenStreamException;
import antlr.TokenStreamIOException;
import antlr.TokenStreamRecognitionException;
import antlr.CharStreamException;
import antlr.CharStreamIOException;
import antlr.ANTLRException;
import java.io.Reader;
import java.util.Hashtable;
import antlr.CharScanner;
import antlr.InputBuffer;
import antlr.ByteBuffer;
import antlr.CharBuffer;
import antlr.Token;
import antlr.CommonToken;
import antlr.RecognitionException;
import antlr.NoViableAltForCharException;
import antlr.MismatchedCharException;
import antlr.TokenStream;
import antlr.ANTLRHashString;
import antlr.LexerSharedInputState;
import antlr.collections.impl.BitSet;
import antlr.SemanticException;

public class RubyLexerBase extends antlr.CharScanner implements RubyTokenTypes, TokenStream
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
public RubyLexerBase(InputStream in) {
	this(new ByteBuffer(in));
}
public RubyLexerBase(Reader in) {
	this(new CharBuffer(in));
}
public RubyLexerBase(InputBuffer ib) {
	this(new LexerSharedInputState(ib));
}
public RubyLexerBase(LexerSharedInputState state) {
	super(state);
	caseSensitiveLiterals = true;
	setCaseSensitive(true);
	literals = new Hashtable();
	literals.put(new ANTLRHashString("END", this), new Integer(159));
	literals.put(new ANTLRHashString("case", this), new Integer(146));
	literals.put(new ANTLRHashString("next", this), new Integer(94));
	literals.put(new ANTLRHashString("while", this), new Integer(149));
	literals.put(new ANTLRHashString("break", this), new Integer(93));
	literals.put(new ANTLRHashString("end", this), new Integer(42));
	literals.put(new ANTLRHashString("then", this), new Integer(157));
	literals.put(new ANTLRHashString("until", this), new Integer(150));
	literals.put(new ANTLRHashString("and", this), new Integer(49));
	literals.put(new ANTLRHashString("module", this), new Integer(151));
	literals.put(new ANTLRHashString("not", this), new Integer(51));
	literals.put(new ANTLRHashString("return", this), new Integer(92));
	literals.put(new ANTLRHashString("undef", this), new Integer(38));
	literals.put(new ANTLRHashString("def", this), new Integer(153));
	literals.put(new ANTLRHashString("retry", this), new Integer(133));
	literals.put(new ANTLRHashString("when", this), new Integer(158));
	literals.put(new ANTLRHashString("class", this), new Integer(152));
	literals.put(new ANTLRHashString("do", this), new Integer(41));
	literals.put(new ANTLRHashString("unless", this), new Integer(145));
	literals.put(new ANTLRHashString("super", this), new Integer(131));
	literals.put(new ANTLRHashString("yield", this), new Integer(134));
	literals.put(new ANTLRHashString("or", this), new Integer(50));
	literals.put(new ANTLRHashString("redo", this), new Integer(135));
	literals.put(new ANTLRHashString("if", this), new Integer(143));
	literals.put(new ANTLRHashString("__FILE__", this), new Integer(107));
	literals.put(new ANTLRHashString("BEGIN", this), new Integer(156));
	literals.put(new ANTLRHashString("for", this), new Integer(147));
	literals.put(new ANTLRHashString("alias", this), new Integer(39));
	literals.put(new ANTLRHashString("false", this), new Integer(106));
	literals.put(new ANTLRHashString("__LINE__", this), new Integer(108));
	literals.put(new ANTLRHashString("defined?", this), new Integer(155));
	literals.put(new ANTLRHashString("rescue", this), new Integer(139));
	literals.put(new ANTLRHashString("begin", this), new Integer(142));
	literals.put(new ANTLRHashString("else", this), new Integer(140));
	literals.put(new ANTLRHashString("in", this), new Integer(148));
	literals.put(new ANTLRHashString("self", this), new Integer(130));
	literals.put(new ANTLRHashString("elsif", this), new Integer(144));
	literals.put(new ANTLRHashString("ensure", this), new Integer(141));
	literals.put(new ANTLRHashString("true", this), new Integer(105));
	literals.put(new ANTLRHashString("nil", this), new Integer(104));
}

public Token nextToken() throws TokenStreamException {
	Token theRetToken=null;
tryAgain:
	for (;;) {
		Token _token = null;
		int _ttype = Token.INVALID_TYPE;
		resetText();
		try {   // for char stream error handling
			try {   // for lexical error handling
				switch ( LA(1)) {
				case '(':
				{
					mLPAREN(true);
					theRetToken=_returnToken;
					break;
				}
				case ')':
				{
					mRPAREN(true);
					theRetToken=_returnToken;
					break;
				}
				case ']':
				{
					mRBRACK(true);
					theRetToken=_returnToken;
					break;
				}
				case '{':
				{
					mLCURLY_HASH(true);
					theRetToken=_returnToken;
					break;
				}
				case '}':
				{
					mRCURLY(true);
					theRetToken=_returnToken;
					break;
				}
				case ',':
				{
					mCOMMA(true);
					theRetToken=_returnToken;
					break;
				}
				case '~':
				{
					mBNOT(true);
					theRetToken=_returnToken;
					break;
				}
				case ';':
				{
					mSEMI(true);
					theRetToken=_returnToken;
					break;
				}
				case '\n':  case '\r':
				{
					mLINE_BREAK(true);
					theRetToken=_returnToken;
					break;
				}
				case '`':
				{
					mCOMMAND_OUTPUT(true);
					theRetToken=_returnToken;
					break;
				}
				case '\'':
				{
					mSINGLE_QUOTE_STRING(true);
					theRetToken=_returnToken;
					break;
				}
				case '/':
				{
					mREGEX(true);
					theRetToken=_returnToken;
					break;
				}
				case '"':
				{
					mDOUBLE_QUOTE_STRING(true);
					theRetToken=_returnToken;
					break;
				}
				case '%':
				{
					mSPECIAL_STRING(true);
					theRetToken=_returnToken;
					break;
				}
				case '_':  case 'a':  case 'b':  case 'c':
				case 'd':  case 'e':  case 'f':  case 'g':
				case 'h':  case 'i':  case 'j':  case 'k':
				case 'l':  case 'm':  case 'n':  case 'o':
				case 'p':  case 'q':  case 'r':  case 's':
				case 't':  case 'u':  case 'v':  case 'w':
				case 'x':  case 'y':  case 'z':
				{
					mIDENTIFIER(true);
					theRetToken=_returnToken;
					break;
				}
				case '$':
				{
					mGLOBAL_VARIABLE(true);
					theRetToken=_returnToken;
					break;
				}
				case 'A':  case 'B':  case 'C':  case 'D':
				case 'E':  case 'F':  case 'G':  case 'H':
				case 'I':  case 'J':  case 'K':  case 'L':
				case 'M':  case 'N':  case 'O':  case 'P':
				case 'Q':  case 'R':  case 'S':  case 'T':
				case 'U':  case 'V':  case 'W':  case 'X':
				case 'Y':  case 'Z':
				{
					mCONSTANT(true);
					theRetToken=_returnToken;
					break;
				}
				case '.':  case '0':  case '1':  case '2':
				case '3':  case '4':  case '5':  case '6':
				case '7':  case '8':  case '9':  case '?':
				{
					mINTEGER(true);
					theRetToken=_returnToken;
					break;
				}
				case '#':
				{
					mCOMMENT(true);
					theRetToken=_returnToken;
					break;
				}
				case '\t':  case '\u000b':  case '\u000c':  case ' ':
				{
					mWHITE_SPACE(true);
					theRetToken=_returnToken;
					break;
				}
				case '\\':
				{
					mLINE_CONTINUATION(true);
					theRetToken=_returnToken;
					break;
				}
				case '\u0000':  case '\u0004':  case '\u001a':
				{
					mEND_OF_FILE(true);
					theRetToken=_returnToken;
					break;
				}
				default:
					if ((LA(1)=='<') && (LA(2)=='=') && (LA(3)=='>')) {
						mCOMPARE(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='=') && (LA(2)=='=') && (LA(3)=='=')) {
						mCASE_EQUAL(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='*') && (LA(2)=='*') && (LA(3)=='=')) {
						mPOWER_ASSIGN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='>') && (LA(2)=='>') && (LA(3)=='=')) {
						mRIGHT_SHIFT_ASSIGN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='&') && (LA(2)=='&') && (LA(3)=='=')) {
						mLOGICAL_AND_ASSIGN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='|') && (LA(2)=='|') && (LA(3)=='=')) {
						mLOGICAL_OR_ASSIGN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='[') && (LA(2)==']')) {
						mEMPTY_ARRAY(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)==':') && (LA(2)==':')) {
						mCOLON2(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='*') && (LA(2)=='*') && (true)) {
						mPOWER(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='>') && (LA(2)=='=')) {
						mGREATER_OR_EQUAL(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='<') && (LA(2)=='=') && (true)) {
						mLESS_OR_EQUAL(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='=') && (LA(2)=='=') && (true)) {
						mEQUAL(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='!') && (LA(2)=='=')) {
						mNOT_EQUAL(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='=') && (LA(2)=='~')) {
						mMATCH(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='!') && (LA(2)=='~')) {
						mNOT_MATCH(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='>') && (LA(2)=='>') && (true)) {
						mRIGHT_SHIFT(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='=') && (LA(2)=='>')) {
						mASSOC(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='&') && (LA(2)=='&') && (true)) {
						mLOGICAL_AND(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='|') && (LA(2)=='|') && (true)) {
						mLOGICAL_OR(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='+') && (LA(2)=='=')) {
						mPLUS_ASSIGN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='-') && (LA(2)=='=')) {
						mMINUS_ASSIGN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='*') && (LA(2)=='=')) {
						mSTAR_ASSIGN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='&') && (LA(2)=='=')) {
						mBAND_ASSIGN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='^') && (LA(2)=='=')) {
						mBXOR_ASSIGN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='|') && (LA(2)=='=')) {
						mBOR_ASSIGN(true);
						theRetToken=_returnToken;
					}
					else if (((LA(1)=='+'||LA(1)=='-') && (LA(2)=='@'))&&(lastTokenIsKeywordDefOrColonWithNoFollowingSpace() ||
			lastTokenIsDotOrColon2())) {
						mUNARY_PLUS_MINUS_METHOD_NAME(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='<') && (LA(2)=='<')) {
						mHERE_DOC_BEGIN(true);
						theRetToken=_returnToken;
					}
					else if (((LA(1)=='=') && (LA(2)=='b'))&&(getColumn()==1)) {
						mRDOC(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='@') && (_tokenSet_0.member(LA(2)))) {
						mINSTANCE_VARIABLE(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='@') && (LA(2)=='@')) {
						mCLASS_VARIABLE(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='[') && (true)) {
						mLBRACK(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)==':') && (true)) {
						mCOLON(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='!') && (true)) {
						mNOT(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='+') && (true)) {
						mPLUS(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='-') && (true)) {
						mMINUS(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='*') && (true)) {
						mSTAR(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='<') && (true)) {
						mLESS_THAN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='>') && (true)) {
						mGREATER_THAN(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='^') && (true)) {
						mBXOR(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='|') && (true)) {
						mBOR(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='&') && (true)) {
						mBAND(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)=='=') && (true)) {
						mASSIGN(true);
						theRetToken=_returnToken;
					}
				else {
					if (LA(1)==EOF_CHAR) {uponEOF(); _returnToken = makeToken(Token.EOF_TYPE);}
				else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
				}
				}
				if ( _returnToken==null ) continue tryAgain; // found SKIP token
				_ttype = _returnToken.getType();
				_returnToken.setType(_ttype);
				return _returnToken;
			}
			catch (RecognitionException e) {
				throw new TokenStreamRecognitionException(e);
			}
		}
		catch (CharStreamException cse) {
			if ( cse instanceof CharStreamIOException ) {
				throw new TokenStreamIOException(((CharStreamIOException)cse).io);
			}
			else {
				throw new TokenStreamException(cse.getMessage());
			}
		}
	}
}

	public final void mLPAREN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LPAREN;
		int _saveIndex;
		
		match('(');
		if (!justSeenWhitespace()) {_ttype = LPAREN_WITH_NO_LEADING_SPACE;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mRPAREN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = RPAREN;
		int _saveIndex;
		
		match(')');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mLBRACK(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LBRACK;
		int _saveIndex;
		
		match('[');
		if (expectArrayAccess()) {_ttype = LBRACK_ARRAY_ACCESS;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mRBRACK(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = RBRACK;
		int _saveIndex;
		
		match(']');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mEMPTY_ARRAY(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = EMPTY_ARRAY;
		int _saveIndex;
		
		match("[]");
		if (expectArrayAccess()) {_ttype = EMPTY_ARRAY_ACCESS;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mLCURLY_HASH(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LCURLY_HASH;
		int _saveIndex;
		
		match('{');
		if (!expectHash()) {_ttype = LCURLY_BLOCK;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mRCURLY(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = RCURLY;
		int _saveIndex;
		
		match('}');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mCOMMA(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = COMMA;
		int _saveIndex;
		
		match(',');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mCOLON(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = COLON;
		int _saveIndex;
		
		match(':');
		if (!spaceIsNext())	{_ttype = COLON_WITH_NO_FOLLOWING_SPACE;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mCOLON2(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = COLON2;
		int _saveIndex;
		
		match("::");
		if (expectLeadingColon2())	{_ttype = LEADING_COLON2;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mNOT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = NOT;
		int _saveIndex;
		
		match('!');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mBNOT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = BNOT;
		int _saveIndex;
		
		match('~');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mPLUS(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = PLUS;
		int _saveIndex;
		
		match('+');
		if (expectUnary())	{_ttype = UNARY_PLUS;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mMINUS(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = MINUS;
		int _saveIndex;
		
		match('-');
		if (expectUnary())	{_ttype = UNARY_MINUS;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mSTAR(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = STAR;
		int _saveIndex;
		
		match('*');
		if (!expectOperator(1)) {_ttype = REST_ARG_PREFIX;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mLESS_THAN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LESS_THAN;
		int _saveIndex;
		
		match('<');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mGREATER_THAN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = GREATER_THAN;
		int _saveIndex;
		
		match('>');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mBXOR(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = BXOR;
		int _saveIndex;
		
		match('^');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mBOR(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = BOR;
		int _saveIndex;
		
		match('|');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mBAND(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = BAND;
		int _saveIndex;
		
		match('&');
		if (!expectOperator(1)) {_ttype = BLOCK_ARG_PREFIX;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mPOWER(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = POWER;
		int _saveIndex;
		
		match("**");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mCOMPARE(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = COMPARE;
		int _saveIndex;
		
		match("<=>");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mGREATER_OR_EQUAL(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = GREATER_OR_EQUAL;
		int _saveIndex;
		
		match(">=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mLESS_OR_EQUAL(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LESS_OR_EQUAL;
		int _saveIndex;
		
		match("<=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mEQUAL(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = EQUAL;
		int _saveIndex;
		
		match("==");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mCASE_EQUAL(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = CASE_EQUAL;
		int _saveIndex;
		
		match("===");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mNOT_EQUAL(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = NOT_EQUAL;
		int _saveIndex;
		
		match("!=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mMATCH(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = MATCH;
		int _saveIndex;
		
		match("=~");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mNOT_MATCH(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = NOT_MATCH;
		int _saveIndex;
		
		match("!~");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mRIGHT_SHIFT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = RIGHT_SHIFT;
		int _saveIndex;
		
		match(">>");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mASSOC(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = ASSOC;
		int _saveIndex;
		
		match("=>");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mLOGICAL_AND(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LOGICAL_AND;
		int _saveIndex;
		
		match("&&");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mLOGICAL_OR(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LOGICAL_OR;
		int _saveIndex;
		
		match("||");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = ASSIGN;
		int _saveIndex;
		
		match('=');
		if (!justSeenWhitespace()) {_ttype = ASSIGN_WITH_NO_LEADING_SPACE;}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mPLUS_ASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = PLUS_ASSIGN;
		int _saveIndex;
		
		match("+=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mMINUS_ASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = MINUS_ASSIGN;
		int _saveIndex;
		
		match("-=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mSTAR_ASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = STAR_ASSIGN;
		int _saveIndex;
		
		match("*=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mPOWER_ASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = POWER_ASSIGN;
		int _saveIndex;
		
		match("**=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mBAND_ASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = BAND_ASSIGN;
		int _saveIndex;
		
		match("&=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mBXOR_ASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = BXOR_ASSIGN;
		int _saveIndex;
		
		match("^=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mBOR_ASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = BOR_ASSIGN;
		int _saveIndex;
		
		match("|=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mRIGHT_SHIFT_ASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = RIGHT_SHIFT_ASSIGN;
		int _saveIndex;
		
		match(">>=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mLOGICAL_AND_ASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LOGICAL_AND_ASSIGN;
		int _saveIndex;
		
		match("&&=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mLOGICAL_OR_ASSIGN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LOGICAL_OR_ASSIGN;
		int _saveIndex;
		
		match("||=");
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mUNARY_PLUS_MINUS_METHOD_NAME(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = UNARY_PLUS_MINUS_METHOD_NAME;
		int _saveIndex;
		
		if (!(lastTokenIsKeywordDefOrColonWithNoFollowingSpace() ||
			lastTokenIsDotOrColon2()))
		  throw new SemanticException("lastTokenIsKeywordDefOrColonWithNoFollowingSpace() ||\r\n\t\t\tlastTokenIsDotOrColon2()");
		{
		switch ( LA(1)) {
		case '+':
		{
			match("+@");
			break;
		}
		case '-':
		{
			match("-@");
			break;
		}
		default:
		{
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		}
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mSEMI(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = SEMI;
		int _saveIndex;
		
			boolean seen_line_feed = false;
		
		
		match(';');
		{
		_loop484:
		do {
			if ((LA(1)=='\t'||LA(1)=='\n'||LA(1)=='\u000b'||LA(1)=='\u000c'||LA(1)=='\r'||LA(1)==' '||LA(1)==';') && (true) && (true)) {
				{
				switch ( LA(1)) {
				case '\t':  case '\u000b':  case '\u000c':  case ' ':
				{
					_saveIndex=text.length();
					mWHITE_SPACE_CHAR(false);
					text.setLength(_saveIndex);
					break;
				}
				case '\n':  case '\r':
				{
					_saveIndex=text.length();
					mLINE_FEED(false);
					text.setLength(_saveIndex);
					seen_line_feed = true;
					break;
				}
				case ';':
				{
					_saveIndex=text.length();
					match(';');
					text.setLength(_saveIndex);
					break;
				}
				default:
				{
					throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
				}
				}
				}
			}
			else {
				break _loop484;
			}
			
		} while (true);
		}
		
						if (lastTokenIsSemi())
						{
							_ttype = Token.SKIP;
						} else if (seen_line_feed) {
							_ttype = LINE_BREAK;
						}
					
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mWHITE_SPACE_CHAR(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = WHITE_SPACE_CHAR;
		int _saveIndex;
		
		switch ( LA(1)) {
		case ' ':
		{
			match(' ');
			break;
		}
		case '\t':
		{
			match('\t');
			break;
		}
		case '\u000c':
		{
			match('\f');
			break;
		}
		case '\u000b':
		{
			match('\13');
			break;
		}
		default:
		{
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mLINE_FEED(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LINE_FEED;
		int _saveIndex;
		
		{
		switch ( LA(1)) {
		case '\n':
		{
			match('\n');
			break;
		}
		case '\r':
		{
			_saveIndex=text.length();
			match('\r');
			text.setLength(_saveIndex);
			{
			if ((LA(1)=='\n') && (true) && (true)) {
				match('\n');
			}
			else {
			}
			
			}
			break;
		}
		default:
		{
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		}
		}
		newline();
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mLINE_BREAK(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LINE_BREAK;
		int _saveIndex;
		
		mPURE_LINE_BREAK(false);
		{
		if ((LA(1)==';') && (true) && (true)) {
			_saveIndex=text.length();
			mSEMI(false);
			text.setLength(_saveIndex);
		}
		else {
		}
		
		}
		
						if ((LINE_BREAK == _ttype) && shouldIgnoreLinebreak())
						{
							_ttype = Token.SKIP;
						}
					
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mPURE_LINE_BREAK(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = PURE_LINE_BREAK;
		int _saveIndex;
		
		mLINE_FEED(false);
		{
		_loop490:
		do {
			if ((LA(1)=='\t'||LA(1)=='\n'||LA(1)=='\u000b'||LA(1)=='\u000c'||LA(1)=='\r'||LA(1)==' ') && (true) && (true)) {
				{
				switch ( LA(1)) {
				case '\n':  case '\r':
				{
					_saveIndex=text.length();
					mLINE_FEED(false);
					text.setLength(_saveIndex);
					break;
				}
				case '\t':  case '\u000b':  case '\u000c':  case ' ':
				{
					_saveIndex=text.length();
					mWHITE_SPACE_CHAR(false);
					text.setLength(_saveIndex);
					break;
				}
				default:
				{
					throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
				}
				}
				}
			}
			else {
				break _loop490;
			}
			
		} while (true);
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mREGEX_MODIFIER(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = REGEX_MODIFIER;
		int _saveIndex;
		
		{
		_loop496:
		do {
			switch ( LA(1)) {
			case 'o':
			{
				match('o');
				break;
			}
			case 'x':
			{
				match('x');
				break;
			}
			case 'p':
			{
				match('p');
				break;
			}
			case 'n':
			{
				match('n');
				break;
			}
			case 'i':
			{
				match('i');
				break;
			}
			case 'u':
			{
				match('u');
				break;
			}
			case 'm':
			{
				match('m');
				break;
			}
			case 's':
			{
				match('s');
				break;
			}
			default:
			{
				break _loop496;
			}
			}
		} while (true);
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mCOMMAND_OUTPUT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = COMMAND_OUTPUT;
		int _saveIndex;
		char  delimiter = '\0';
		char  end = '\0';
		
		if (((LA(1)=='`') && ((LA(2) >= '\u0000' && LA(2) <= '\ufffe')))&&(!lastTokenIsKeywordDefOrColonWithNoFollowingSpace())) {
			delimiter = LA(1);
			_saveIndex=text.length();
			match('`');
			text.setLength(_saveIndex);
			{
			_loop499:
			do {
				if ((((LA(1) >= '\u0000' && LA(1) <= '\ufffe')) && ((LA(2) >= '\u0000' && LA(2) <= '\ufffe')))&&(LA(1) != delimiter && !expressionSubstitutionIsNext())) {
					mSTRING_CHAR(false);
				}
				else {
					break _loop499;
				}
				
			} while (true);
			}
			end = LA(1);
			_saveIndex=text.length();
			matchNot(EOF_CHAR);
			text.setLength(_saveIndex);
			
							if (end != delimiter)
							{
								_ttype = COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION;
								setCurrentSpecialStringDelimiter(delimiter, 1);
							}
						
		}
		else if ((LA(1)=='`') && (true)) {
			match('`');
			_ttype = SINGLE_QUOTE;
		}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mSTRING_CHAR(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = STRING_CHAR;
		int _saveIndex;
		
		switch ( LA(1)) {
		case '\n':  case '\r':
		{
			mLINE_FEED(false);
			break;
		}
		case '\\':
		{
			mESC(false);
			break;
		}
		default:
			if ((_tokenSet_1.member(LA(1)))) {
				{
				match(_tokenSet_1);
				}
			}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mSINGLE_QUOTE_STRING(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = SINGLE_QUOTE_STRING;
		int _saveIndex;
		
		_saveIndex=text.length();
		match('\'');
		text.setLength(_saveIndex);
		{
		_loop503:
		do {
			switch ( LA(1)) {
			case '\\':
			{
				mESC(false);
				break;
			}
			case '\n':  case '\r':
			{
				mLINE_FEED(false);
				break;
			}
			default:
				if ((_tokenSet_2.member(LA(1)))) {
					{
					match(_tokenSet_2);
					}
				}
			else {
				break _loop503;
			}
			}
		} while (true);
		}
		_saveIndex=text.length();
		match('\'');
		text.setLength(_saveIndex);
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mESC(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = ESC;
		int _saveIndex;
		
		match('\\');
		matchNot(EOF_CHAR);
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mREGEX(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = REGEX;
		int _saveIndex;
		char  delimiter = '\0';
		char  end = '\0';
		
		if (((LA(1)=='/') && ((LA(2) >= '\u0000' && LA(2) <= '\ufffe')) && (true))&&(!expectOperator(2))) {
			delimiter = LA(1);
			_saveIndex=text.length();
			match('/');
			text.setLength(_saveIndex);
			{
			_loop506:
			do {
				if ((((LA(1) >= '\u0000' && LA(1) <= '\ufffe')) && ((LA(2) >= '\u0000' && LA(2) <= '\ufffe')))&&(LA(1) != delimiter && !expressionSubstitutionIsNext())) {
					mSTRING_CHAR(false);
				}
				else {
					break _loop506;
				}
				
			} while (true);
			}
			end = LA(1);
			_saveIndex=text.length();
			matchNot(EOF_CHAR);
			text.setLength(_saveIndex);
			
							if (end != delimiter)
							{
								_ttype = REGEX_BEFORE_EXPRESSION_SUBSTITUTION;
								setCurrentSpecialStringDelimiter(delimiter, 1);
							}
							else
							{
								text.append("/");
								mREGEX_MODIFIER(true);
							}
						
		}
		else if ((LA(1)=='/') && (LA(2)=='=') && (true)) {
			match("/=");
			_ttype = DIV_ASSIGN;
		}
		else if ((LA(1)=='/') && (true)) {
			match('/');
			_ttype = DIV;
		}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mDOUBLE_QUOTE_STRING(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = DOUBLE_QUOTE_STRING;
		int _saveIndex;
		char  delimiter = '\0';
		char  end = '\0';
		
		delimiter = LA(1);
		_saveIndex=text.length();
		match('\"');
		text.setLength(_saveIndex);
		{
		_loop509:
		do {
			if ((((LA(1) >= '\u0000' && LA(1) <= '\ufffe')) && ((LA(2) >= '\u0000' && LA(2) <= '\ufffe')))&&(LA(1) != delimiter && !expressionSubstitutionIsNext())) {
				mSTRING_CHAR(false);
			}
			else {
				break _loop509;
			}
			
		} while (true);
		}
		end = LA(1);
		_saveIndex=text.length();
		matchNot(EOF_CHAR);
		text.setLength(_saveIndex);
		
						if (end != delimiter)
						{
							_ttype = STRING_BEFORE_EXPRESSION_SUBSTITUTION;
							setCurrentSpecialStringDelimiter(delimiter, 1);
						}
					
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mSTRING_BETWEEN_EXPRESSION_SUBSTITUTION(boolean _createToken,
		char delimiter, int delimiter_count
	) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = STRING_BETWEEN_EXPRESSION_SUBSTITUTION;
		int _saveIndex;
		
		{
		_loop512:
		do {
			if ((((LA(1) >= '\u0000' && LA(1) <= '\ufffe')))&&((delimiter_count > 0) && (delimiter_count = trackDelimiterCount(LA(1), delimiter, delimiter_count)) != 0&& !expressionSubstitutionIsNext())) {
				mSTRING_CHAR(false);
			}
			else {
				break _loop512;
			}
			
		} while (true);
		}
		
						//match and skip delimiter, there maybe no delimiter, e.g. ':#{cmd_name}'
						if (LA(1) != EOF_CHAR)
						{
							_saveIndex=text.length();
							matchNot(EOF_CHAR);
							text.setLength(_saveIndex);
						}
		
						if (0 == delimiter_count)
						{
							_ttype = STRING_AFTER_EXPRESSION_SUBSTITUTION;
						}
						else
						{
							updateCurrentSpecialStringDelimiterCount(delimiter_count);
						}
					
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mSPECIAL_STRING(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = SPECIAL_STRING;
		int _saveIndex;
		char  delimiter1 = '\0';
		char  delimiter2 = '\0';
		char  delimiter3 = '\0';
		char  delimiter4 = '\0';
		char  delimiter5 = '\0';
		char  delimiter6 = '\0';
		
			int delimiter_count = 1;
		
		
		if ((LA(1)=='%') && (LA(2)=='q')) {
			_saveIndex=text.length();
			match('%');
			text.setLength(_saveIndex);
			_saveIndex=text.length();
			match('q');
			text.setLength(_saveIndex);
			delimiter1 = LA(1);
			_saveIndex=text.length();
			matchNot(EOF_CHAR);
			text.setLength(_saveIndex);
			{
			_loop515:
			do {
				if ((((LA(1) >= '\u0000' && LA(1) <= '\ufffe')) && ((LA(2) >= '\u0000' && LA(2) <= '\ufffe')))&&((delimiter_count = trackDelimiterCount(LA(1), delimiter1, delimiter_count)) != 0)) {
					mSTRING_CHAR(false);
				}
				else {
					break _loop515;
				}
				
			} while (true);
			}
			_saveIndex=text.length();
			matchNot(EOF_CHAR);
			text.setLength(_saveIndex);
			_ttype = SINGLE_QUOTE_STRING;
		}
		else if ((LA(1)=='%') && (LA(2)=='Q')) {
			_saveIndex=text.length();
			match('%');
			text.setLength(_saveIndex);
			_saveIndex=text.length();
			match('Q');
			text.setLength(_saveIndex);
			delimiter2 = LA(1);
			_saveIndex=text.length();
			matchNot(EOF_CHAR);
			text.setLength(_saveIndex);
			{
			_loop517:
			do {
				if ((((LA(1) >= '\u0000' && LA(1) <= '\ufffe')))&&((delimiter_count = trackDelimiterCount(LA(1), delimiter2, delimiter_count)) != 0 && !expressionSubstitutionIsNext())) {
					mSTRING_CHAR(false);
				}
				else {
					break _loop517;
				}
				
			} while (true);
			}
			
							//match and skip delimiter
							_saveIndex=text.length();
							matchNot(EOF_CHAR);
							text.setLength(_saveIndex);
			
							if (0 == delimiter_count)
							{
								_ttype = DOUBLE_QUOTE_STRING;
							}
							else
							{
								_ttype = STRING_BEFORE_EXPRESSION_SUBSTITUTION;
								setCurrentSpecialStringDelimiter(delimiter2, delimiter_count);
							}
						
		}
		else if ((LA(1)=='%') && (LA(2)=='r')) {
			_saveIndex=text.length();
			match('%');
			text.setLength(_saveIndex);
			_saveIndex=text.length();
			match('r');
			text.setLength(_saveIndex);
			delimiter3 = LA(1);
			_saveIndex=text.length();
			matchNot(EOF_CHAR);
			text.setLength(_saveIndex);
			{
			_loop519:
			do {
				if ((((LA(1) >= '\u0000' && LA(1) <= '\ufffe')))&&((delimiter_count = trackDelimiterCount(LA(1), delimiter3, delimiter_count)) != 0 && !expressionSubstitutionIsNext())) {
					mSTRING_CHAR(false);
				}
				else {
					break _loop519;
				}
				
			} while (true);
			}
			
							//match and skip delimiter
							_saveIndex=text.length();
							matchNot(EOF_CHAR);
							text.setLength(_saveIndex);
			
							if (0 == delimiter_count)
							{
								text.append("/");
								mREGEX_MODIFIER(true);
								_ttype = REGEX;
							}
							else
							{
								_ttype = REGEX_BEFORE_EXPRESSION_SUBSTITUTION;
								setCurrentSpecialStringDelimiter(delimiter3, delimiter_count);
							}
						
		}
		else if ((LA(1)=='%') && (LA(2)=='x')) {
			_saveIndex=text.length();
			match('%');
			text.setLength(_saveIndex);
			_saveIndex=text.length();
			match('x');
			text.setLength(_saveIndex);
			delimiter4 = LA(1);
			_saveIndex=text.length();
			matchNot(EOF_CHAR);
			text.setLength(_saveIndex);
			{
			_loop521:
			do {
				if ((((LA(1) >= '\u0000' && LA(1) <= '\ufffe')))&&((delimiter_count = trackDelimiterCount(LA(1), delimiter4, delimiter_count)) != 0 && !expressionSubstitutionIsNext())) {
					mSTRING_CHAR(false);
				}
				else {
					break _loop521;
				}
				
			} while (true);
			}
			
							//match and skip delimiter
							_saveIndex=text.length();
							matchNot(EOF_CHAR);
							text.setLength(_saveIndex);
			
							if (0 == delimiter_count)
							{
								_ttype = COMMAND_OUTPUT;
							}
							else
							{
								_ttype = COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION;
								setCurrentSpecialStringDelimiter(delimiter4, delimiter_count);
							}
						
		}
		else if ((LA(1)=='%') && (LA(2)=='W'||LA(2)=='w')) {
			_saveIndex=text.length();
			match('%');
			text.setLength(_saveIndex);
			{
			switch ( LA(1)) {
			case 'w':
			{
				_saveIndex=text.length();
				match('w');
				text.setLength(_saveIndex);
				break;
			}
			case 'W':
			{
				_saveIndex=text.length();
				match('W');
				text.setLength(_saveIndex);
				break;
			}
			default:
			{
				throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
			}
			}
			}
			delimiter5 = LA(1);
			_saveIndex=text.length();
			matchNot(EOF_CHAR);
			text.setLength(_saveIndex);
			{
			_loop524:
			do {
				if ((((LA(1) >= '\u0000' && LA(1) <= '\ufffe')) && ((LA(2) >= '\u0000' && LA(2) <= '\ufffe')))&&((delimiter_count = trackDelimiterCount(LA(1), delimiter5, delimiter_count)) != 0)) {
					mSTRING_CHAR(false);
				}
				else {
					break _loop524;
				}
				
			} while (true);
			}
			_saveIndex=text.length();
			matchNot(EOF_CHAR);
			text.setLength(_saveIndex);
			_ttype = W_ARRAY;
		}
		else if (((LA(1)=='%') && (_tokenSet_3.member(LA(2))))&&(!expectOperator(2))) {
			_saveIndex=text.length();
			match('%');
			text.setLength(_saveIndex);
			_saveIndex=text.length();
			{
			delimiter6 = LA(1);
			match(_tokenSet_3);
			}
			text.setLength(_saveIndex);
			{
			_loop527:
			do {
				if ((((LA(1) >= '\u0000' && LA(1) <= '\ufffe')))&&((delimiter_count = trackDelimiterCount(LA(1), delimiter6, delimiter_count)) != 0 && !expressionSubstitutionIsNext())) {
					mSTRING_CHAR(false);
				}
				else {
					break _loop527;
				}
				
			} while (true);
			}
			
							//match and skip delimiter
							_saveIndex=text.length();
							matchNot(EOF_CHAR);
							text.setLength(_saveIndex);
			
							if (0 == delimiter_count)
							{
								_ttype = DOUBLE_QUOTE_STRING;
							}
							else
							{
								_ttype = STRING_BEFORE_EXPRESSION_SUBSTITUTION;
								setCurrentSpecialStringDelimiter(delimiter6, delimiter_count);
							}
						
		}
		else if ((LA(1)=='%') && (LA(2)=='=')) {
			match("%=");
			_ttype = MOD_ASSIGN;
		}
		else if ((LA(1)=='%') && (true)) {
			match("%");
			_ttype = MOD;
		}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mHERE_DOC_BEGIN(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = HERE_DOC_BEGIN;
		int _saveIndex;
		
		if (((LA(1)=='<') && (LA(2)=='<') && (_tokenSet_4.member(LA(3))))&&(expectHeredoc())) {
			_saveIndex=text.length();
			match("<<");
			text.setLength(_saveIndex);
			mHERE_DOC_DELIMITER(false);
		}
		else if ((LA(1)=='<') && (LA(2)=='<') && (LA(3)=='=')) {
			match("<<=");
			_ttype = LEFT_SHIFT_ASSIGN;
		}
		else if ((LA(1)=='<') && (LA(2)=='<') && (true)) {
			match("<<");
			_ttype = LEFT_SHIFT;
		}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mHERE_DOC_DELIMITER(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = HERE_DOC_DELIMITER;
		int _saveIndex;
		
		{
		if ((LA(1)=='-') && (_tokenSet_4.member(LA(2))) && (true)) {
			match('-');
		}
		else if ((_tokenSet_4.member(LA(1))) && (true) && (true)) {
		}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		
		}
		{
		switch ( LA(1)) {
		case '\'':
		{
			_saveIndex=text.length();
			match('\'');
			text.setLength(_saveIndex);
			{
			int _cnt547=0;
			_loop547:
			do {
				if ((_tokenSet_5.member(LA(1)))) {
					{
					match(_tokenSet_5);
					}
				}
				else {
					if ( _cnt547>=1 ) { break _loop547; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
				}
				
				_cnt547++;
			} while (true);
			}
			_saveIndex=text.length();
			match('\'');
			text.setLength(_saveIndex);
			break;
		}
		case '"':
		{
			_saveIndex=text.length();
			match('"');
			text.setLength(_saveIndex);
			{
			int _cnt550=0;
			_loop550:
			do {
				if ((_tokenSet_6.member(LA(1)))) {
					{
					match(_tokenSet_6);
					}
				}
				else {
					if ( _cnt550>=1 ) { break _loop550; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
				}
				
				_cnt550++;
			} while (true);
			}
			_saveIndex=text.length();
			match('"');
			text.setLength(_saveIndex);
			break;
		}
		default:
			if ((_tokenSet_7.member(LA(1)))) {
				{
				int _cnt544=0;
				_loop544:
				do {
					if ((_tokenSet_7.member(LA(1)))) {
						{
						match(_tokenSet_7);
						}
					}
					else {
						if ( _cnt544>=1 ) { break _loop544; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
					}
					
					_cnt544++;
				} while (true);
				}
			}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		}
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mHERE_DOC_CONTENT(boolean _createToken,
		String delimiter, int type1, int type2
	) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = HERE_DOC_CONTENT;
		int _saveIndex;
		Token next_line=null;
		
		{
		int _cnt533=0;
		_loop533:
		do {
			if (((LA(1) >= '\u0000' && LA(1) <= '\ufffe'))) {
				mANYTHING_OTHER_THAN_LINE_FEED_AND_POUND(true);
				next_line=_returnToken;
				if (expressionSubstitutionIsNext()) break;
				mLINE_FEED(false);
				if (isDelimiter(next_line.getText(), delimiter)) break;
			}
			else {
				if ( _cnt533>=1 ) { break _loop533; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
			}
			
			_cnt533++;
		} while (true);
		}
		
						if (expressionSubstitutionIsNext())
						{
							//eat '#'
							_saveIndex=text.length();
							matchNot(EOF_CHAR);
							text.setLength(_saveIndex);
							_ttype = type1;
						}
						else
						{
							//skip delimiter
							text.setLength(text.length() - next_line.getText().length() - 1);
							_ttype = type2;
						}
					
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mANYTHING_OTHER_THAN_LINE_FEED_AND_POUND(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = ANYTHING_OTHER_THAN_LINE_FEED_AND_POUND;
		int _saveIndex;
		
		{
		_loop538:
		do {
			if (((_tokenSet_8.member(LA(1))))&&(!expressionSubstitutionIsNext())) {
				{
				if ((_tokenSet_1.member(LA(1)))) {
					{
					match(_tokenSet_1);
					}
				}
				else if ((LA(1)=='\\')) {
					mESC(false);
				}
				else {
					throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
				}
				
				}
			}
			else {
				break _loop538;
			}
			
		} while (true);
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mRDOC(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = RDOC;
		int _saveIndex;
		
		if (!(getColumn()==1))
		  throw new SemanticException("getColumn()==1");
		match("=begin");
		{
		_loop553:
		do {
			// nongreedy exit test
			if ((LA(1)=='=') && (LA(2)=='e') && (LA(3)=='n')) break _loop553;
			if (((LA(1) >= '\u0000' && LA(1) <= '\ufffe')) && ((LA(2) >= '\u0000' && LA(2) <= '\ufffe')) && ((LA(3) >= '\u0000' && LA(3) <= '\ufffe'))) {
				mLINE(false);
			}
			else {
				break _loop553;
			}
			
		} while (true);
		}
		if (!(getColumn()==1))
		  throw new SemanticException("getColumn()==1");
		match("=end");
		
						_ttype = Token.SKIP;
					
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mLINE(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LINE;
		int _saveIndex;
		
		mANYTHING_OTHER_THAN_LINE_FEED(false);
		mLINE_FEED(false);
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mANYTHING_OTHER_THAN_LINE_FEED(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = ANYTHING_OTHER_THAN_LINE_FEED;
		int _saveIndex;
		
		{
		_loop557:
		do {
			if ((_tokenSet_8.member(LA(1)))) {
				{
				match(_tokenSet_8);
				}
			}
			else {
				break _loop557;
			}
			
		} while (true);
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mIDENTIFIER(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = IDENTIFIER;
		int _saveIndex;
		
		if (((LA(1)=='_') && (LA(2)=='_') && (LA(3)=='E'))&&(getColumn() == 1)) {
			match("__END__");
			{
			if ((LA(1)=='\t'||LA(1)=='\n'||LA(1)=='\u000b'||LA(1)=='\u000c'||LA(1)=='\r'||LA(1)==' ')) {
				{
				_loop563:
				do {
					if ((LA(1)=='\t'||LA(1)=='\u000b'||LA(1)=='\u000c'||LA(1)==' ')) {
						mWHITE_SPACE_CHAR(false);
					}
					else {
						break _loop563;
					}
					
				} while (true);
				}
				mLINE_BREAK(false);
				{
				_loop565:
				do {
					if (((LA(1) >= '\u0000' && LA(1) <= '\ufffe'))) {
						matchNot(EOF_CHAR);
					}
					else {
						break _loop565;
					}
					
				} while (true);
				}
				_ttype = Token.EOF_TYPE;
			}
			else {
			}
			
			}
		}
		else if ((_tokenSet_9.member(LA(1))) && (true) && (true)) {
			{
			switch ( LA(1)) {
			case 'a':  case 'b':  case 'c':  case 'd':
			case 'e':  case 'f':  case 'g':  case 'h':
			case 'i':  case 'j':  case 'k':  case 'l':
			case 'm':  case 'n':  case 'o':  case 'p':
			case 'q':  case 'r':  case 's':  case 't':
			case 'u':  case 'v':  case 'w':  case 'x':
			case 'y':  case 'z':
			{
				matchRange('a','z');
				break;
			}
			case '_':
			{
				match('_');
				break;
			}
			default:
			{
				throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
			}
			}
			}
			{
			_loop568:
			do {
				switch ( LA(1)) {
				case 'a':  case 'b':  case 'c':  case 'd':
				case 'e':  case 'f':  case 'g':  case 'h':
				case 'i':  case 'j':  case 'k':  case 'l':
				case 'm':  case 'n':  case 'o':  case 'p':
				case 'q':  case 'r':  case 's':  case 't':
				case 'u':  case 'v':  case 'w':  case 'x':
				case 'y':  case 'z':
				{
					matchRange('a','z');
					break;
				}
				case 'A':  case 'B':  case 'C':  case 'D':
				case 'E':  case 'F':  case 'G':  case 'H':
				case 'I':  case 'J':  case 'K':  case 'L':
				case 'M':  case 'N':  case 'O':  case 'P':
				case 'Q':  case 'R':  case 'S':  case 'T':
				case 'U':  case 'V':  case 'W':  case 'X':
				case 'Y':  case 'Z':
				{
					matchRange('A','Z');
					break;
				}
				case '_':
				{
					match('_');
					break;
				}
				case '0':  case '1':  case '2':  case '3':
				case '4':  case '5':  case '6':  case '7':
				case '8':  case '9':
				{
					matchRange('0','9');
					break;
				}
				default:
				{
					break _loop568;
				}
				}
			} while (true);
			}
			{
			switch ( LA(1)) {
			case '?':
			{
				match('?');
				_ttype = FUNCTION;
				break;
			}
			case '!':
			{
				match('!');
				_ttype = FUNCTION;
				break;
			}
			default:
				{
					if (lastTokenIsDotOrColon2()) {_ttype = FUNCTION;}
				}
			}
			}
		}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		
		_ttype = testLiteralsTable(_ttype);
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mGLOBAL_VARIABLE(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = GLOBAL_VARIABLE;
		int _saveIndex;
		
		if ((LA(1)=='$') && (LA(2)=='-')) {
			match('$');
			match('-');
			{
			switch ( LA(1)) {
			case 'A':  case 'B':  case 'C':  case 'D':
			case 'E':  case 'F':  case 'G':  case 'H':
			case 'I':  case 'J':  case 'K':  case 'L':
			case 'M':  case 'N':  case 'O':  case 'P':
			case 'Q':  case 'R':  case 'S':  case 'T':
			case 'U':  case 'V':  case 'W':  case 'X':
			case 'Y':  case 'Z':  case '_':  case 'a':
			case 'b':  case 'c':  case 'd':  case 'e':
			case 'f':  case 'g':  case 'h':  case 'i':
			case 'j':  case 'k':  case 'l':  case 'm':
			case 'n':  case 'o':  case 'p':  case 'q':
			case 'r':  case 's':  case 't':  case 'u':
			case 'v':  case 'w':  case 'x':  case 'y':
			case 'z':
			{
				mIDENTIFIER_CONSTANT_AND_KEYWORD(false);
				break;
			}
			case '#':
			{
				match('#');
				break;
			}
			default:
				{
				}
			}
			}
		}
		else if ((LA(1)=='$') && (_tokenSet_0.member(LA(2)))) {
			match('$');
			mIDENTIFIER_CONSTANT_AND_KEYWORD(false);
		}
		else if ((LA(1)=='$') && ((LA(2) >= '0' && LA(2) <= '9'))) {
			match('$');
			{
			int _cnt573=0;
			_loop573:
			do {
				if (((LA(1) >= '0' && LA(1) <= '9'))) {
					matchRange('0','9');
				}
				else {
					if ( _cnt573>=1 ) { break _loop573; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
				}
				
				_cnt573++;
			} while (true);
			}
		}
		else if ((LA(1)=='$') && (_tokenSet_10.member(LA(2)))) {
			match('$');
			{
			switch ( LA(1)) {
			case '!':
			{
				match('!');
				break;
			}
			case '@':
			{
				match('@');
				break;
			}
			case '&':
			{
				match('&');
				break;
			}
			case '`':
			{
				match('`');
				break;
			}
			case '\'':
			{
				match('\'');
				break;
			}
			case '+':
			{
				match('+');
				break;
			}
			case '~':
			{
				match('~');
				break;
			}
			case '=':
			{
				match('=');
				break;
			}
			case '/':
			{
				match('/');
				break;
			}
			case '\\':
			{
				match('\\');
				break;
			}
			case ',':
			{
				match(',');
				break;
			}
			case ';':
			{
				match(';');
				break;
			}
			case '.':
			{
				match('.');
				break;
			}
			case '<':
			{
				match('<');
				break;
			}
			case '>':
			{
				match('>');
				break;
			}
			case '*':
			{
				match('*');
				break;
			}
			case '$':
			{
				match('$');
				break;
			}
			case '?':
			{
				match('?');
				break;
			}
			case ':':
			{
				match(':');
				break;
			}
			case '"':
			{
				match('\"');
				break;
			}
			default:
			{
				throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
			}
			}
			}
		}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mIDENTIFIER_CONSTANT_AND_KEYWORD(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = IDENTIFIER_CONSTANT_AND_KEYWORD;
		int _saveIndex;
		
		{
		switch ( LA(1)) {
		case 'a':  case 'b':  case 'c':  case 'd':
		case 'e':  case 'f':  case 'g':  case 'h':
		case 'i':  case 'j':  case 'k':  case 'l':
		case 'm':  case 'n':  case 'o':  case 'p':
		case 'q':  case 'r':  case 's':  case 't':
		case 'u':  case 'v':  case 'w':  case 'x':
		case 'y':  case 'z':
		{
			matchRange('a','z');
			break;
		}
		case 'A':  case 'B':  case 'C':  case 'D':
		case 'E':  case 'F':  case 'G':  case 'H':
		case 'I':  case 'J':  case 'K':  case 'L':
		case 'M':  case 'N':  case 'O':  case 'P':
		case 'Q':  case 'R':  case 'S':  case 'T':
		case 'U':  case 'V':  case 'W':  case 'X':
		case 'Y':  case 'Z':
		{
			matchRange('A','Z');
			break;
		}
		case '_':
		{
			match('_');
			break;
		}
		default:
		{
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		}
		}
		{
		_loop578:
		do {
			switch ( LA(1)) {
			case 'a':  case 'b':  case 'c':  case 'd':
			case 'e':  case 'f':  case 'g':  case 'h':
			case 'i':  case 'j':  case 'k':  case 'l':
			case 'm':  case 'n':  case 'o':  case 'p':
			case 'q':  case 'r':  case 's':  case 't':
			case 'u':  case 'v':  case 'w':  case 'x':
			case 'y':  case 'z':
			{
				matchRange('a','z');
				break;
			}
			case 'A':  case 'B':  case 'C':  case 'D':
			case 'E':  case 'F':  case 'G':  case 'H':
			case 'I':  case 'J':  case 'K':  case 'L':
			case 'M':  case 'N':  case 'O':  case 'P':
			case 'Q':  case 'R':  case 'S':  case 'T':
			case 'U':  case 'V':  case 'W':  case 'X':
			case 'Y':  case 'Z':
			{
				matchRange('A','Z');
				break;
			}
			case '_':
			{
				match('_');
				break;
			}
			case '0':  case '1':  case '2':  case '3':
			case '4':  case '5':  case '6':  case '7':
			case '8':  case '9':
			{
				matchRange('0','9');
				break;
			}
			default:
			{
				break _loop578;
			}
			}
		} while (true);
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mINSTANCE_VARIABLE(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = INSTANCE_VARIABLE;
		int _saveIndex;
		
		match('@');
		mIDENTIFIER_CONSTANT_AND_KEYWORD(false);
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mCLASS_VARIABLE(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = CLASS_VARIABLE;
		int _saveIndex;
		
		match('@');
		mINSTANCE_VARIABLE(false);
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mCONSTANT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = CONSTANT;
		int _saveIndex;
		
		{
		matchRange('A','Z');
		}
		{
		_loop584:
		do {
			switch ( LA(1)) {
			case 'a':  case 'b':  case 'c':  case 'd':
			case 'e':  case 'f':  case 'g':  case 'h':
			case 'i':  case 'j':  case 'k':  case 'l':
			case 'm':  case 'n':  case 'o':  case 'p':
			case 'q':  case 'r':  case 's':  case 't':
			case 'u':  case 'v':  case 'w':  case 'x':
			case 'y':  case 'z':
			{
				matchRange('a','z');
				break;
			}
			case 'A':  case 'B':  case 'C':  case 'D':
			case 'E':  case 'F':  case 'G':  case 'H':
			case 'I':  case 'J':  case 'K':  case 'L':
			case 'M':  case 'N':  case 'O':  case 'P':
			case 'Q':  case 'R':  case 'S':  case 'T':
			case 'U':  case 'V':  case 'W':  case 'X':
			case 'Y':  case 'Z':
			{
				matchRange('A','Z');
				break;
			}
			case '_':
			{
				match('_');
				break;
			}
			case '0':  case '1':  case '2':  case '3':
			case '4':  case '5':  case '6':  case '7':
			case '8':  case '9':
			{
				matchRange('0','9');
				break;
			}
			default:
			{
				break _loop584;
			}
			}
		} while (true);
		}
		_ttype = testLiteralsTable(_ttype);
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mINTEGER(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = INTEGER;
		int _saveIndex;
		
		switch ( LA(1)) {
		case '1':  case '2':  case '3':  case '4':
		case '5':  case '6':  case '7':  case '8':
		case '9':
		{
			mNON_ZERO_DECIMAL(false);
			{
			if (((LA(1)=='.'))&&((LA(2)>='0')&&(LA(2)<='9'))) {
				mFLOAT_WITH_LEADING_DOT(false);
				_ttype = FLOAT;
			}
			else if ((LA(1)=='E'||LA(1)=='e')) {
				mEXPONENT(false);
				_ttype = FLOAT;
			}
			else {
			}
			
			}
			break;
		}
		case '?':
		{
			match('?');
			{
			if ((LA(1)=='\\') && (_tokenSet_11.member(LA(2)))) {
				match('\\');
				{
				match(_tokenSet_11);
				}
				_ttype = ASCII_VALUE;
			}
			else if ((LA(1)=='\\') && (LA(2)=='C'||LA(2)=='M')) {
				{
				int _cnt596=0;
				_loop596:
				do {
					if ((LA(1)=='\\')) {
						match('\\');
						{
						switch ( LA(1)) {
						case 'C':
						{
							match('C');
							break;
						}
						case 'M':
						{
							match('M');
							break;
						}
						default:
						{
							throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
						}
						}
						}
						match('-');
					}
					else {
						if ( _cnt596>=1 ) { break _loop596; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
					}
					
					_cnt596++;
				} while (true);
				}
				{
				switch ( LA(1)) {
				case 'a':  case 'b':  case 'c':  case 'd':
				case 'e':  case 'f':  case 'g':  case 'h':
				case 'i':  case 'j':  case 'k':  case 'l':
				case 'm':  case 'n':  case 'o':  case 'p':
				case 'q':  case 'r':  case 's':  case 't':
				case 'u':  case 'v':  case 'w':  case 'x':
				case 'y':  case 'z':
				{
					matchRange('a','z');
					break;
				}
				case '?':
				{
					match('?');
					break;
				}
				default:
				{
					throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
				}
				}
				}
				_ttype = ASCII_VALUE;
			}
			else if ((LA(1)=='\\') && (LA(2)=='x')) {
				match('\\');
				match('x');
				mHEX_CONTENT(false);
				_ttype = ASCII_VALUE;
			}
			else if (((_tokenSet_12.member(LA(1))))&&(isAsciiValueTerminator(LA(2)))) {
				{
				{
				match(_tokenSet_12);
				}
				}
				_ttype = ASCII_VALUE;
			}
			else {
				_ttype = QUESTION;
			}
			
			}
			break;
		}
		default:
			if ((LA(1)=='.') && (LA(2)=='.') && (LA(3)=='.')) {
				match("...");
				_ttype = EXCLUSIVE_RANGE;
			}
			else if ((LA(1)=='0') && ((LA(2) >= '0' && LA(2) <= '7'))) {
				_saveIndex=text.length();
				match('0');
				text.setLength(_saveIndex);
				mOCTAL_CONTENT(false);
				_ttype = OCTAL;
			}
			else if ((LA(1)=='0') && (LA(2)=='B'||LA(2)=='b')) {
				_saveIndex=text.length();
				match('0');
				text.setLength(_saveIndex);
				{
				switch ( LA(1)) {
				case 'b':
				{
					_saveIndex=text.length();
					match('b');
					text.setLength(_saveIndex);
					break;
				}
				case 'B':
				{
					_saveIndex=text.length();
					match('B');
					text.setLength(_saveIndex);
					break;
				}
				default:
				{
					throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
				}
				}
				}
				mBINARY_CONTENT(false);
				_ttype = BINARY;
			}
			else if ((LA(1)=='0') && (LA(2)=='X'||LA(2)=='x')) {
				_saveIndex=text.length();
				match('0');
				text.setLength(_saveIndex);
				{
				switch ( LA(1)) {
				case 'x':
				{
					_saveIndex=text.length();
					match('x');
					text.setLength(_saveIndex);
					break;
				}
				case 'X':
				{
					_saveIndex=text.length();
					match('X');
					text.setLength(_saveIndex);
					break;
				}
				default:
				{
					throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
				}
				}
				}
				mHEX_CONTENT(false);
				_ttype = HEX;
			}
			else if ((LA(1)=='.') && ((LA(2) >= '0' && LA(2) <= '9'))) {
				mFLOAT_WITH_LEADING_DOT(false);
				_ttype = FLOAT;
			}
			else if ((LA(1)=='.') && (LA(2)=='.') && (true)) {
				match("..");
				_ttype = INCLUSIVE_RANGE;
			}
			else if ((LA(1)=='0') && (true)) {
				match('0');
				{
				if (((LA(1)=='.'))&&((LA(2)>='0')&&(LA(2)<='9'))) {
					mFLOAT_WITH_LEADING_DOT(false);
					_ttype = FLOAT;
				}
				else {
				}
				
				}
			}
			else if ((LA(1)=='.') && (true)) {
				match('.');
				_ttype = DOT;
			}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mOCTAL_CONTENT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = OCTAL_CONTENT;
		int _saveIndex;
		
		{
		int _cnt612=0;
		_loop612:
		do {
			if (((LA(1) >= '0' && LA(1) <= '7'))) {
				matchRange('0','7');
				{
				if ((LA(1)=='_')) {
					_saveIndex=text.length();
					mUNDER_SCORE(false);
					text.setLength(_saveIndex);
				}
				else {
				}
				
				}
			}
			else {
				if ( _cnt612>=1 ) { break _loop612; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
			}
			
			_cnt612++;
		} while (true);
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mBINARY_CONTENT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = BINARY_CONTENT;
		int _saveIndex;
		
		{
		int _cnt623=0;
		_loop623:
		do {
			switch ( LA(1)) {
			case '0':
			{
				match('0');
				{
				if ((LA(1)=='_')) {
					_saveIndex=text.length();
					mUNDER_SCORE(false);
					text.setLength(_saveIndex);
				}
				else {
				}
				
				}
				break;
			}
			case '1':
			{
				match('1');
				{
				if ((LA(1)=='_')) {
					_saveIndex=text.length();
					mUNDER_SCORE(false);
					text.setLength(_saveIndex);
				}
				else {
				}
				
				}
				break;
			}
			default:
			{
				if ( _cnt623>=1 ) { break _loop623; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
			}
			}
			_cnt623++;
		} while (true);
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mHEX_CONTENT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = HEX_CONTENT;
		int _saveIndex;
		
		{
		int _cnt618=0;
		_loop618:
		do {
			switch ( LA(1)) {
			case '0':  case '1':  case '2':  case '3':
			case '4':  case '5':  case '6':  case '7':
			case '8':  case '9':
			{
				matchRange('0','9');
				{
				if ((LA(1)=='_')) {
					_saveIndex=text.length();
					mUNDER_SCORE(false);
					text.setLength(_saveIndex);
				}
				else {
				}
				
				}
				break;
			}
			case 'A':  case 'B':  case 'C':  case 'D':
			case 'E':  case 'F':
			{
				matchRange('A','F');
				{
				if ((LA(1)=='_')) {
					_saveIndex=text.length();
					mUNDER_SCORE(false);
					text.setLength(_saveIndex);
				}
				else {
				}
				
				}
				break;
			}
			case 'a':  case 'b':  case 'c':  case 'd':
			case 'e':  case 'f':
			{
				matchRange('a','f');
				{
				if ((LA(1)=='_')) {
					_saveIndex=text.length();
					mUNDER_SCORE(false);
					text.setLength(_saveIndex);
				}
				else {
				}
				
				}
				break;
			}
			default:
			{
				if ( _cnt618>=1 ) { break _loop618; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
			}
			}
			_cnt618++;
		} while (true);
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mFLOAT_WITH_LEADING_DOT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = FLOAT_WITH_LEADING_DOT;
		int _saveIndex;
		
		match('.');
		{
		int _cnt601=0;
		_loop601:
		do {
			if (((LA(1) >= '0' && LA(1) <= '9'))) {
				matchRange('0','9');
			}
			else {
				if ( _cnt601>=1 ) { break _loop601; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
			}
			
			_cnt601++;
		} while (true);
		}
		{
		if ((LA(1)=='E'||LA(1)=='e')) {
			mEXPONENT(false);
		}
		else {
		}
		
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mNON_ZERO_DECIMAL(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = NON_ZERO_DECIMAL;
		int _saveIndex;
		
		{
		matchRange('1','9');
		{
		if ((LA(1)=='_')) {
			_saveIndex=text.length();
			mUNDER_SCORE(false);
			text.setLength(_saveIndex);
		}
		else {
		}
		
		}
		{
		_loop608:
		do {
			if (((LA(1) >= '0' && LA(1) <= '9'))) {
				matchRange('0','9');
				{
				if ((LA(1)=='_')) {
					_saveIndex=text.length();
					mUNDER_SCORE(false);
					text.setLength(_saveIndex);
				}
				else {
				}
				
				}
			}
			else {
				break _loop608;
			}
			
		} while (true);
		}
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mEXPONENT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = EXPONENT;
		int _saveIndex;
		
		{
		switch ( LA(1)) {
		case 'e':
		{
			match('e');
			break;
		}
		case 'E':
		{
			match('E');
			break;
		}
		default:
		{
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		}
		}
		{
		switch ( LA(1)) {
		case '+':
		{
			match('+');
			break;
		}
		case '-':
		{
			match('-');
			break;
		}
		case '0':  case '1':  case '2':  case '3':
		case '4':  case '5':  case '6':  case '7':
		case '8':  case '9':
		{
			break;
		}
		default:
		{
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		}
		}
		{
		int _cnt628=0;
		_loop628:
		do {
			if (((LA(1) >= '0' && LA(1) <= '9'))) {
				matchRange('0','9');
			}
			else {
				if ( _cnt628>=1 ) { break _loop628; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
			}
			
			_cnt628++;
		} while (true);
		}
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	protected final void mUNDER_SCORE(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = UNDER_SCORE;
		int _saveIndex;
		
		match('_');
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mCOMMENT(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = COMMENT;
		int _saveIndex;
		
		if (((LA(1)=='#') && (true) && (true))&&(!lastTokenIsColonWithNoFollowingSpace())) {
			match('#');
			mANYTHING_OTHER_THAN_LINE_FEED(false);
			
							_ttype = Token.SKIP;
						
		}
		else if ((LA(1)=='#') && (true) && (true)) {
			_saveIndex=text.length();
			match('#');
			text.setLength(_saveIndex);
			
							_ttype = STRING_BEFORE_EXPRESSION_SUBSTITUTION;
							setCurrentSpecialStringDelimiter('#'/*useless*/, 0);
						
		}
		else {
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mWHITE_SPACE(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = WHITE_SPACE;
		int _saveIndex;
		
		{
		int _cnt633=0;
		_loop633:
		do {
			if ((LA(1)=='\t'||LA(1)=='\u000b'||LA(1)=='\u000c'||LA(1)==' ')) {
				mWHITE_SPACE_CHAR(false);
			}
			else {
				if ( _cnt633>=1 ) { break _loop633; } else {throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());}
			}
			
			_cnt633++;
		} while (true);
		}
		
						setSeenWhitespace();
						_ttype = Token.SKIP;
					
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mLINE_CONTINUATION(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = LINE_CONTINUATION;
		int _saveIndex;
		
		match('\\');
		mLINE_FEED(false);
		_ttype = Token.SKIP;
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	public final void mEND_OF_FILE(boolean _createToken) throws RecognitionException, CharStreamException, TokenStreamException {
		int _ttype; Token _token=null; int _begin=text.length();
		_ttype = END_OF_FILE;
		int _saveIndex;
		
		{
		switch ( LA(1)) {
		case '\u0000':
		{
			match('\0');
			break;
		}
		case '\u0004':
		{
			match('\004');
			break;
		}
		case '\u001a':
		{
			match('\032');
			break;
		}
		default:
		{
			throw new NoViableAltForCharException((char)LA(1), getFilename(), getLine(), getColumn());
		}
		}
		}
		_ttype = Token.EOF_TYPE;
		if ( _createToken && _token==null && _ttype!=Token.SKIP ) {
			_token = makeToken(_ttype);
			_token.setText(new String(text.getBuffer(), _begin, text.length()-_begin));
		}
		_returnToken = _token;
	}
	
	
	private static final long[] mk_tokenSet_0() {
		long[] data = new long[1025];
		data[1]=576460745995190270L;
		return data;
	}
	public static final BitSet _tokenSet_0 = new BitSet(mk_tokenSet_0());
	private static final long[] mk_tokenSet_1() {
		long[] data = new long[2048];
		data[0]=-9217L;
		data[1]=-268435457L;
		for (int i = 2; i<=1022; i++) { data[i]=-1L; }
		data[1023]=9223372036854775807L;
		return data;
	}
	public static final BitSet _tokenSet_1 = new BitSet(mk_tokenSet_1());
	private static final long[] mk_tokenSet_2() {
		long[] data = new long[2048];
		data[0]=-549755823105L;
		data[1]=-268435457L;
		for (int i = 2; i<=1022; i++) { data[i]=-1L; }
		data[1023]=9223372036854775807L;
		return data;
	}
	public static final BitSet _tokenSet_2 = new BitSet(mk_tokenSet_2());
	private static final long[] mk_tokenSet_3() {
		long[] data = new long[2048];
		data[0]=-2593791910388695041L;
		data[1]=-576460743847706623L;
		for (int i = 2; i<=1022; i++) { data[i]=-1L; }
		data[1023]=9223372036854775807L;
		return data;
	}
	public static final BitSet _tokenSet_3 = new BitSet(mk_tokenSet_3());
	private static final long[] mk_tokenSet_4() {
		long[] data = new long[2048];
		data[0]=-2882393925765571585L;
		for (int i = 1; i<=1022; i++) { data[i]=-1L; }
		data[1023]=9223372036854775807L;
		return data;
	}
	public static final BitSet _tokenSet_4 = new BitSet(mk_tokenSet_4());
	private static final long[] mk_tokenSet_5() {
		long[] data = new long[2048];
		data[0]=-549755823105L;
		for (int i = 1; i<=1022; i++) { data[i]=-1L; }
		data[1023]=9223372036854775807L;
		return data;
	}
	public static final BitSet _tokenSet_5 = new BitSet(mk_tokenSet_5());
	private static final long[] mk_tokenSet_6() {
		long[] data = new long[2048];
		data[0]=-17179878401L;
		for (int i = 1; i<=1022; i++) { data[i]=-1L; }
		data[1023]=9223372036854775807L;
		return data;
	}
	public static final BitSet _tokenSet_6 = new BitSet(mk_tokenSet_6());
	private static final long[] mk_tokenSet_7() {
		long[] data = new long[2048];
		data[0]=-2882394492701254657L;
		for (int i = 1; i<=1022; i++) { data[i]=-1L; }
		data[1023]=9223372036854775807L;
		return data;
	}
	public static final BitSet _tokenSet_7 = new BitSet(mk_tokenSet_7());
	private static final long[] mk_tokenSet_8() {
		long[] data = new long[2048];
		data[0]=-9217L;
		for (int i = 1; i<=1022; i++) { data[i]=-1L; }
		data[1023]=9223372036854775807L;
		return data;
	}
	public static final BitSet _tokenSet_8 = new BitSet(mk_tokenSet_8());
	private static final long[] mk_tokenSet_9() {
		long[] data = new long[1025];
		data[1]=576460745860972544L;
		return data;
	}
	public static final BitSet _tokenSet_9 = new BitSet(mk_tokenSet_9());
	private static final long[] mk_tokenSet_10() {
		long[] data = new long[1025];
		data[0]=-287987564470599680L;
		data[1]=4611686022990790657L;
		return data;
	}
	public static final BitSet _tokenSet_10 = new BitSet(mk_tokenSet_10());
	private static final long[] mk_tokenSet_11() {
		long[] data = new long[2048];
		data[0]=-1L;
		data[1]=-72057594037936137L;
		for (int i = 2; i<=1022; i++) { data[i]=-1L; }
		data[1023]=9223372036854775807L;
		return data;
	}
	public static final BitSet _tokenSet_11 = new BitSet(mk_tokenSet_11());
	private static final long[] mk_tokenSet_12() {
		long[] data = new long[2048];
		data[0]=-4294976513L;
		data[1]=-268435457L;
		for (int i = 2; i<=1022; i++) { data[i]=-1L; }
		data[1023]=9223372036854775807L;
		return data;
	}
	public static final BitSet _tokenSet_12 = new BitSet(mk_tokenSet_12());
	
	}
