/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
