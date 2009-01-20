package j2me.lang;

import java.io.PrintStream;
import org.jruby.util.Sprintf;

import com.xruby.runtime.builtin.RubyArray;

public class PrintStreamMe {
    public static void printf( PrintStream out, String strFormat, RubyArray args)
    {
    	String strRes = Sprintf.sprintf(strFormat, args);
    	out.print(strRes);
    }

    public static String sprintf( String strFormat, RubyArray args )
    {
    	return Sprintf.sprintf(strFormat, args);
    }
    
}
