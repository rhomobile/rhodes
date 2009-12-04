/**
 * Copyright 2006-2007 Yu Su, Yu Zhang
 * Distributed under the BSD License
 */

package com.xruby.runtime.javasupport;

import com.xruby.compiler.RubyCompiler;
import com.xruby.compiler.codegen.CompilationResults;
import com.xruby.runtime.lang.*;

/**
 *
 */
public class RequireExample {
    public static void main(String[] args) throws Exception {

        String program_text = "" +
                "import 'java.util.*'\n" +
                "r = Random.new" + "\n" +
                //call overloaded methods
                "puts r.nextInt\n" +
                "puts r.nextInt(3)\n" +
                //call methods inherited superclass(for example:Object)
                "puts r.toString\n" +


                //call static methods and fields
                "require_java 'java.lang.Math'\n"+
                "puts JMath.PI\n"+
                "puts JMath.cos(0)\n"+


                //access Java constant variables
                "require_java 'java.lang.System'\n"+
                "out = System::out\n"+
                "out.println('ok!')\n"+


                //escape from name collision
                "require_java 'java.lang.Object'\n"+
                "o = JObject.new\n" +
                "puts o.toString\n" +


                //implement a Java interface
                "require_java 'java.lang.Runnable'\n"+
                "require_java 'java.lang.Thread'\n"+
                "class MyRunnable < Runnable\n"+
                "   def run\n"+
                "       puts 'Thread is running!'\n"+
                "   end\n"+
                "end\n"+
                "r = MyRunnable.new\n"+
                "thread = JThread.new(r)\n"+
                "thread.start()\n"+

                //Creating Java Arrays and accessing Java Arrays
                "require_java 'com.xruby.runtime.javasupport.AssistantTester'\n" +
                "require_java 'java.lang.reflect.Array'\n"+
                "require_java 'java.lang.String'\n"+
                "s = JArray.newInstance(JString, 2)\n"+
                "s[0]='Hello '\n"+
                "s[1]='World!'\n"+
                "AssistantTester.echo(s)\n"+

                //Handling Java exceptions
                "require_java 'java.io.FileInputStream'\n"+
                "require_java 'java.io.FileNotFoundException'\n"+
                "require_java 'java.io.IOException'\n"+

                "begin\n"+
                "   f = FileInputStream.new('myfile')\n"+
                "rescue FileNotFoundException=>fnfe\n"+
                "   puts fnfe.getMessage\n"+
                "rescue IOException=>ioe\n"+
                "   puts ioe\n"+
                "end\n"+

                "array = ArrayList.new\n" +
                "array.add 1\n" +
                "array.add 2\n" +
                "array.add 3\n" +
                "for i in 1 ... 3\n" +
                "    puts array.get(i)\n" +
                "end\n"+

                //JavaBean support
                "require_java 'javax.swing.JFrame'\n"+
                "f = JFrame.new('hello')\n"+
                //calls setVisible
                "f.visible= true\n"+
                //calls getTitle
                "puts f.title\n"+
                "\n";

        RubyCompiler compiler = new RubyCompiler();
        CompilationResults codes = compiler.compileString(program_text);
        RubyProgram p = codes.getRubyProgram();

        RubyRuntime.init(args);
        p.invoke();
        RubyRuntime.fini();

    }
}

class AssistantTester{
    public static void echo(String[] str){
        for(String s : str){
            System.out.print(s);
        }
        System.out.println();
    }
}


