package j2me.lang;

import java.io.PrintStream;
import com.rho.Sprintf;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.RubyArray;

public class PrintStreamMe {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("APP");
	
    public static void printf( PrintStream out, String strFormat, RubyArray args)
    {
    	String strRes = Sprintf.sprintf(strFormat, args);
    	if ( out == System.out )
    		LOG.INFO(strRes);
    	else if ( out == System.err )
    		LOG.ERROR(strRes);
    	else
    		out.print(strRes);
    }

    public static String sprintf( String strFormat, RubyArray args )
    {
    	return Sprintf.sprintf(strFormat, args);
    }
    
}
