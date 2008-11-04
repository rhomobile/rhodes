package j2me.lang;

public class StringMe {

    static int codePointAtImpl( String a, int index, int limit)
    {
        char c1 = a.charAt(index++);
        if (CharacterMe.isHighSurrogate(c1)) {
            if (index < limit) {
                char c2 = a.charAt(index);
                if (CharacterMe.isLowSurrogate(c2)) {
                    return CharacterMe.toCodePoint(c1, c2);
                }
            }
        }
        return c1;
    }
    
    public static int codePointAt( String str, int index) {
        if ((index < 0) || (index >= str.length())) {
            throw new StringIndexOutOfBoundsException(index);
        }
        return codePointAtImpl(str, index, str.length() );//, offset + index, offset + count);
    }
	
    public static String format(String format, Object arg1) {
    	//TODO: String.format
    	throw new RuntimeException("Not Implemented");
    	//return format;
    }
    public static String format(String format, int arg1) {
    	//TODO: String.format
    	throw new RuntimeException("Not Implemented");
    	//return format;
    }

    public static String replaceAll(String strThis, String regex, String replacement) {
    	//TODO: String.replaceAll - find by indexOf and use StringBuffer replace
    	throw new RuntimeException("Not Implemented");
    	//return strThis;
    }

    public static boolean matches(String strThis, String regex) {
    	//TODO: matches
    	throw new RuntimeException("Not Implemented");
    	//return false;
    }
}
