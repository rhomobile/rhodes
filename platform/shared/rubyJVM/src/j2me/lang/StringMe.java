package j2me.lang;

import com.rho.Sprintf;
import com.xruby.runtime.builtin.RubyArray;

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
	
    public static String format(String format, RubyArray args) {
    	return Sprintf.sprintf(format, args);
    }

    public static String replaceAll(String strThis, String regex, String replacement) {
    	//TODO: String.replaceAll - implement regex replace
    	StringBuffer strRes = new StringBuffer();
    	
        int nEnd = strThis.indexOf(regex);
        if ( nEnd < 0 )
        	return strThis;

        int nStart = 0;
        for( ; nEnd >=0 ; nEnd = strThis.indexOf(regex,nStart) )
        {
        	strRes.append(strThis.substring(nStart, nEnd));
        	strRes.append(replacement);
        	
        	nStart = nEnd + regex.length();
        }
        
    	strRes.append(strThis.substring(nStart, strThis.length()));
        return strRes.toString();
    }

    public static String replaceFirst(String strThis, String regex, String replacement) {
    	StringBuffer strRes = new StringBuffer();
    	
        int nEnd = strThis.indexOf(regex);
        if ( nEnd < 0 )
        	return strThis;

        int nStart = 0;
        for( ; nEnd >=0 ; nEnd = -1 )
        {
        	strRes.append(strThis.substring(nStart, nEnd));
        	strRes.append(replacement);
        	
        	nStart = nEnd + regex.length();
        }
        
    	strRes.append(strThis.substring(nStart, strThis.length()));
        return strRes.toString();
    }
    
    public static boolean matches(String strThis, String regex) {
    	//TODO: matches
    	throw new RuntimeException("Not Implemented - StringMe::boolean matches(String strThis, String regex)");
    	//return false;
    }
}
