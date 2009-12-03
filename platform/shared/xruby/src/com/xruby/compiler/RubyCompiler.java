/**
 * Copyright 2005-2007 Xue Yong Zhi, Yu Su, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.compiler;

import antlr.RecognitionException;
import antlr.TokenStreamException;
import com.xruby.compiler.codegen.CompilationResults;
import com.xruby.compiler.codegen.RubyCompilerImpl;
import com.xruby.compiler.parser.RubyParser;
import com.xruby.runtime.lang.RubyBinding;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;

public class RubyCompiler {

    public static final String VERSION = "0.3.3";

    private RubyBinding binding_;
    private boolean strip_;
    private boolean enableDebug = true;
    private boolean verbose_ = true;
    
    public RubyCompiler() {
    	this.binding_ = null;
    	this.strip_ = false;
    }
    
    public RubyCompiler(boolean strip) {
    	this.binding_ = null;
    	this.strip_ = strip;
    }
    
    public RubyCompiler(RubyBinding binding, boolean strip) {
        binding_ = binding;
        strip_ = strip;
    }
    
    public CompilationResults compileString(String text)
    	throws RecognitionException, TokenStreamException {
    	return compile(null, null, new StringReader(text));
    }
    
    public CompilationResults compileString(String filename, String text)
    	throws RecognitionException, TokenStreamException {
    	return compile(null, filename, new StringReader(text));
    }
    
    public CompilationResults compileStdin() 
    	throws FileNotFoundException, RecognitionException, TokenStreamException {
    	Reader reader = new InputStreamReader(System.in);
        return compile(null, null, new BufferedReader(reader));
    }

    public CompilationResults compileFile(String filename)
    	throws FileNotFoundException, RecognitionException, TokenStreamException {
    	File f = new File(filename);
    	if (f.isDirectory()) {
    		return this.compileDir(filename, f);
    	} else {
    		return this.compileSingleFile(null, f);
    	}
    }

	private CompilationResults compileDir(final String extra, File dir) 
		throws FileNotFoundException, RecognitionException, TokenStreamException {
		CompilationResults dirResults = new CompilationResults();
		for (String filename : dir.list()) {
			if (filename.charAt(0) == '.') {
		        continue;
		    }
			
		    File file = new File(dir, filename);
		    if (file.isDirectory()) {
		    	CompilationResults results = this.compileDir(extra, file);
		    	dirResults.add(results);
		    } else if (filename.endsWith(".rb")) {
		    	CompilationResults results = this.compileSingleFile(extra, file);
		    	dirResults.add(results);
		    }
		}
		
		return dirResults;
	}

	private CompilationResults compileSingleFile(final String extra, File file) 
		throws FileNotFoundException, RecognitionException, TokenStreamException {
		if (verbose_) { 
		    System.out.println("Compiling " + file.getPath() + "...");
		}
		try{
			Reader reader = new InputStreamReader(new FileInputStream(file), "UTF-8");
			
			return compile(extra, file.getPath(), new BufferedReader(reader));
		}catch(UnsupportedEncodingException exc)
		{
			throw new RecognitionException();
		}
	}

    private CompilationResults compile(String extra, String filename, Reader reader)
        throws RecognitionException, TokenStreamException {
    	RubyParser parser = createParser(reader);
        RubyCompilerImpl compiler = new RubyCompilerImpl(extra, filename);

        // Enable debug or not
        if (enableDebug) {
            compiler.enableDebug();
        }

        return compiler.compile(parser.parse(filename), binding_);
    }

	private RubyParser createParser(Reader reader) {
    	if (null == binding_) {
    		return new RubyParser(reader, strip_);
    	} else {
    		return new RubyParser(reader, binding_.getVariableNames(), strip_);
    	}
	}

    public void enableDebug() {
        enableDebug = true;
    }

    public void setVerbose() {
        verbose_= true;
    }
}
