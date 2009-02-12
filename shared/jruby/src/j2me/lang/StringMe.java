package j2me.lang;

import org.jruby.util.Sprintf;
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
    
    public static String format(String format, int arg1) {
    	//TODO: String.format
    	throw new RuntimeException("Not Implemented");
    	//return format;
    }

    public static String replaceAll(String strThis, String regex, String replacement) {
    	//TODO: String.replaceAll - implement regex replace
    	
        int i = strThis.indexOf(regex);
        return (i < 0) ? strThis : // No target sequence found.
        	strThis.substring(0, i).concat(replacement).concat(
        			replaceAll( strThis.substring(i+replacement.length()), regex, 
        					replacement ) );  					
    }

    public static String replaceFirst(String strThis, String regex, String replacement) {
    	//TODO: String.replaceFirst - implement regex replace
    	
        int i = strThis.indexOf(regex);
        return (i < 0) ? strThis : // No target sequence found.
        	strThis.substring(0, i).concat(replacement).concat( 
        			strThis.substring(i+replacement.length()));
    }
    
    public static boolean matches(String strThis, String regex) {
    	//TODO: matches
    	throw new RuntimeException("Not Implemented");
    	//return false;
    }
}
