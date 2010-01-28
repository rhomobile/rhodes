package j2me.lang;

public class CharacterMe {
	public static final int MIN_CODE_POINT = 0x000000;
	private static final int FAST_PATH_MAX = 255;
	
    public static final int MIN_SUPPLEMENTARY_CODE_POINT = 0x010000;
    public static final char MAX_HIGH_SURROGATE = '\uDBFF';
    public static final char MIN_LOW_SURROGATE  = '\uDC00';
    public static final char MAX_LOW_SURROGATE  = '\uDFFF';
    public static final char MIN_HIGH_SURROGATE = '\uD800';
    
    public static final byte  DASH_PUNCTUATION            = 20;
    public static final byte  START_PUNCTUATION           = 21;
    public static final byte  END_PUNCTUATION             = 22;
    public static final byte CONNECTOR_PUNCTUATION       = 23;
    public static final byte  OTHER_PUNCTUATION           = 24;
    public static final byte   MATH_SYMBOL                 = 25;
    public static final byte   CURRENCY_SYMBOL             = 26;
    public static final byte   MODIFIER_SYMBOL             = 27;
    public static final byte   UNASSIGNED                  = 0;
    
    public static int charCount(int codePoint) {
        return codePoint >= MIN_SUPPLEMENTARY_CODE_POINT? 2 : 1;
    }
    public static boolean isHighSurrogate(char ch) {
        return ch >= MIN_HIGH_SURROGATE && ch <= MAX_HIGH_SURROGATE;
    }
    public static boolean isLowSurrogate(char ch) {
        return ch >= MIN_LOW_SURROGATE && ch <= MAX_LOW_SURROGATE;
    }
    public static int toCodePoint(char high, char low) {
        return ((high - MIN_HIGH_SURROGATE) << 10)
            + (low - MIN_LOW_SURROGATE) + MIN_SUPPLEMENTARY_CODE_POINT;
    }
	
    public static boolean isWhitespace(char ch) {
        return isWhitespace((int)ch);
    }
    public static boolean isWhitespace(int codePoint) {
    	boolean bWhiteSpace = false;
    	
    	if ( codePoint == Character.MAX_VALUE )
    		return false;
    	
        if (codePoint >= MIN_CODE_POINT && codePoint <= FAST_PATH_MAX) {
            bWhiteSpace =  CharacterDataLatin1.isWhitespace((char)codePoint);
        } else {
        	//TODO: Character_isWhitespace non Latin
        	throw new RuntimeException("Not Implemented - CharacterMe::boolean isWhitespace(int codePoint)");
        }
        
        return bWhiteSpace;
    }
	
    static int codePointAtImpl(char[] a, int index, int limit) {
        char c1 = a[index++];
        if (isHighSurrogate(c1)) {
            if (index < limit) {
                char c2 = a[index];
                if (isLowSurrogate(c2)) {
                    return toCodePoint(c1, c2);
                }
            }
        }
        return c1;
    }
    
    public static int codePointAt(char[] a, int index) {
    	return codePointAtImpl(a, index, a.length);
        }
    
    public static boolean isLetterOrDigit(char ch) {
        return isLetterOrDigit((int)ch);
    }
    public static boolean isLetterOrDigit(int codePoint) {
        boolean bLetterOrDigit = false;

    	if ( codePoint == Character.MAX_VALUE )
    		return false;
        
        if (codePoint >= MIN_CODE_POINT && codePoint <= FAST_PATH_MAX) {
            bLetterOrDigit = CharacterDataLatin1.isLetterOrDigit((char)codePoint);
        } else {
        	//TODO: //isLetterOrDigit non Latin
        	throw new RuntimeException("Not Implemented - CharacterMe::boolean isLetterOrDigit(int codePoint)");
        }
        return bLetterOrDigit;
    }
    
    public static boolean isLetter(char ch) {
        return isLetter((int)ch);
    }
    public static boolean isLetter(int codePoint) {
        boolean bLetter = false;

    	if ( codePoint == Character.MAX_VALUE )
    		return false;
        
        if (codePoint >= MIN_CODE_POINT && codePoint <= FAST_PATH_MAX) {
            bLetter = CharacterDataLatin1.isLetter((char)codePoint);
        } else {
        	//TODO: isLetter non-Latin
        	throw new RuntimeException("Not Implemented - CharacterMe::boolean isLetter(int codePoint)");
        }
        return bLetter;
    }
    public static boolean isSpaceChar(char ch) {
        return isSpaceChar((int)ch);
    }
    public static boolean isSpaceChar(int codePoint) {
        boolean bSpaceChar = false;

    	if ( codePoint == Character.MAX_VALUE )
    		return false;
        
        if (codePoint >= MIN_CODE_POINT && codePoint <=  FAST_PATH_MAX) {
            bSpaceChar =  CharacterDataLatin1.isSpaceChar((char)codePoint);
        } else {
        	//TODO: isSpaceChar non Latin
        	throw new RuntimeException("Not Implemented - CharacterMe::boolean isSpaceChar(int codePoint)");
        }
        return bSpaceChar;
    }
    public static boolean isISOControl(char ch) {
        return isISOControl((int)ch);
    }
    public static boolean isISOControl(int codePoint) {
        return (codePoint >= 0x0000 && codePoint <= 0x001F) || 
            (codePoint >= 0x007F && codePoint <= 0x009F);
    }
    public static int getType(char ch) {
        return getType((int)ch);
    }

    public static int getType(int codePoint) {
        int type = CharacterMe.UNASSIGNED;

        if (codePoint >= MIN_CODE_POINT && codePoint <= FAST_PATH_MAX) {
            type = CharacterDataLatin1.getType((char)codePoint);
        } else {
        	//TODO: getType non Latin
        	throw new RuntimeException("Not Implemented - CharacterMe::int getType(int codePoint)");
        }
        return type;
    }
    
    public static boolean isDigit(int c) {
        return c >= '0' && c <= '9';
    }
    
    public static boolean isJavaIdentifierStart(char ch) {
        return CharacterDataLatin1.isJavaIdentifierStart(ch);
    }
    
    public static boolean isJavaIdentifierPart(char ch) {
        return CharacterDataLatin1.isJavaIdentifierStart(ch);
    }
    
}
