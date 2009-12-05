/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby;

import com.xruby.compiler.RubyCompiler;
import com.xruby.compiler.codegen.CompilationResults;
import com.xruby.runtime.lang.GlobalVariables;
import com.xruby.runtime.lang.RubyProgram;
import com.xruby.runtime.lang.RubyRuntime;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.nio.channels.FileChannel;

public class Main {
	private CommandLineOptions options;
	
	public Main(CommandLineOptions options) {
		this.options = options;
	}
	
	public void run() throws Exception {
		if (options.isHelp()) {
			help();
			return;
		}

		if (options.isEvalOneLine()) {
			//eval string
			if (options.getBackupExtension() != null && options.getFilename() != null) {
				copyFile(options.getFilename(), options.getFilename() + options.getBackupExtension());
			}
			RubyCompiler compiler = new RubyCompiler(options.isStrip());
			CompilationResults results = compiler.compileString(options.getEvalScript());
			if (options.isPe()) {
				redirectStdinout(options.getFilename());
			}

			run(results, null);
		} else if (options.getFilename() == null) {
			//eval STDIN
			CompilationResults results = compileStdin();
			run(results, null);
		} else if (options.isCompileOnly()){
			//compile and just save the result
			String filename = options.getFilename();
			options.enableDebug();
			CompilationResults results = compile(filename);
			results.save(filename);
		} else {
			//eval file
			String filename = options.getFilename();
			options.enableDebug();
			CompilationResults results = compile(filename);
			options.parseOptionsFromFile(filename);
			GlobalVariables.setProgramName(filename);
			GlobalVariables.importValuesFromCommandLine(options.getVars());
			run(results, options.getArgs());
		}
	}
	
    private void redirectStdinout(String filename) throws IOException {
        //count line number
        BufferedReader r = new BufferedReader(new FileReader(filename));
        int line = 0;
        StringBuilder sb = new StringBuilder();
        while (r.readLine()!= null) {
            ++line;
            sb.append(line);
            sb.append("\n");
        }
        r.close();

        //redirect stdin
        InputStream s = new ByteArrayInputStream(sb.toString().getBytes());
        System.setIn(s);

        //redirect stdout
        //TODO how does ruby do it? only with "-i" option?
        OutputStream output = new FileOutputStream(filename);
        System.setOut(new PrintStream(output));
    }

    private void help() {
        System.out.println("Usage: xruby [-c] filename1, filename2, ...");
    }
    
    private CompilationResults compileStdin() throws Exception {
    	RubyCompiler compiler = createCompiler();
    	return compiler.compileStdin();
    }
    
    private CompilationResults compile(String filename) throws Exception {
    	RubyCompiler compiler = createCompiler();
    	return compiler.compileFile(options.getFilename());
    }
    
    private RubyCompiler createCompiler() {
    	RubyCompiler compiler = new RubyCompiler(options.isStrip());
        if (options.isVerbose()) {
            compiler.setVerbose();
        }
        if (options.isEnableDebug()) {
            compiler.enableDebug();
        }
		return compiler;
    }

    private void run(CompilationResults results, String[] args) throws Exception {
        RubyProgram p = (RubyProgram)results.getRubyProgram();
        RubyRuntime.init(args);
        p.invoke();
        RubyRuntime.fini();
    }

    //TODO move to a separated class
    private void copyFile(String from, String to) {
    	FileChannel srcChannel = null;
    	FileChannel dstChannel = null;

    	try {
    		srcChannel = new FileInputStream(from).getChannel();
    		dstChannel = new FileOutputStream(to).getChannel();

    		dstChannel.transferFrom(srcChannel, 0, srcChannel.size());
    	} catch (IOException e) {
    		throw new Error(e);
    	} finally {
    		try {
    			if (srcChannel != null) {
    				srcChannel.close();
    			}

    			if (dstChannel != null) {
    				dstChannel.close();
    			}
    		} catch (IOException ioe) {
    			// do nothing
    		}
    	}
    }
    
    public static void main(String[] args) throws Exception {
    	try{
	    	CommandLineOptions options = new CommandLineOptions(args);
	    	Main xruby = new Main(options);
	    	xruby.run();
    	}catch(antlr.RecognitionException exc)
    	{
    		System.out.println("XRuby compilation failed. Line: " + exc.line + "; Column: " + exc.column + "; Description: " + exc.getErrorMessage());
    		//System.out.println("XRuby compilation failed: " + exc);
    		System.exit(1);
    	}catch(Exception exc1)
	    {
    		System.out.println("XRuby compilation failed: " + exc1);
    		System.exit(1);
	    }
    }
}
