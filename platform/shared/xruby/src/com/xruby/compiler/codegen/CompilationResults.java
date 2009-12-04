/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import java.io.*;
import java.util.*;
import java.util.jar.*;

import com.xruby.compiler.*;
import com.xruby.compiler.codedom.BlockFarm;
import com.xruby.runtime.lang.RubyProgram;

class CompilationResult {

    private final String name_;
    private final byte[] code_;

    public CompilationResult(String name, byte[] code) {
        name_ = name;
        code_ = code;
    }

    public void save(JarOutputStream jarstream) throws FileNotFoundException, IOException {
        String filename_to_save = NameFactory.createClassFileName(name_);
        jarstream.putNextEntry(new JarEntry(filename_to_save));
        jarstream.write(code_);
    }

    public Class load(XRubyClassLoader loader) {
        return loader.load(name_, code_);
    }
}

public class CompilationResults {

    private final List<CompilationResult> results_ = new ArrayList<CompilationResult>();

    public void add(CompilationResult result) {
        results_.add(result);
    }
    
    public void add(CompilationResults results) {
    	for (CompilationResult result : results.results_) {
    		this.results_.add(result);
    	}
    }

    public int size() {
        return results_.size();
    }

    public void save(String script_name) throws FileNotFoundException, IOException {
        String targetFileName = NameFactory.createJarFileName(script_name);
		File targetFile = new File(targetFileName);
        FileOutputStream fstream = new FileOutputStream(targetFile);
        JarOutputStream jarstream = new JarOutputStream(fstream, createManifest(script_name));

        for (CompilationResult result : results_) {
            result.save(jarstream);
        }

        writeBlockInfo(script_name, jarstream);

        jarstream.close();
        System.out.println("Generated " + targetFileName);
    }

    private void writeBlockInfo(String script_name, JarOutputStream jarstream) throws IOException {
        // TODO: Add debug check here
        // TODO: We need a loop statement to support multiple files
        // if(is_debug?) {...}
        String smap = BlockFarm.getMapByName(script_name);
        if(smap == null) {
            smap = "0\n";
        }

        jarstream.putNextEntry(new JarEntry(NameFactory.createClassNameForSmap(script_name)));
        jarstream.write(smap.getBytes());
    }

    private Manifest createManifest(String script_name) {
        Manifest manifest = new Manifest();
        Attributes attrs = manifest.getMainAttributes();
        attrs.putValue("Manifest-Version", "1.0");
        attrs.putValue("Created-By", RubyCompiler.VERSION + " (XRuby)");
        attrs.putValue("Main-Class", NameFactory.createMainClass(script_name));
        //If use -jar on the java.exe command line, java.exe will quietly ignore
        //the set environment classpath and any -classpath or -cp command line options.
        //So have to mention them in the Class-Path manifest entry
        attrs.putValue("Class-Path", "xruby-"+ RubyCompiler.VERSION + ".jar");
        return manifest;
    }

    /*
     * @return an instance of RubyProgram.
     */
    public RubyProgram getRubyProgram() throws InstantiationException, IllegalAccessException {
        XRubyClassLoader loader = new XRubyClassLoader();
        Class classToRun = null;
        for (CompilationResult result : results_) {
            classToRun = result.load(loader);
        }

        //The "main program" is always the last one.
        return (RubyProgram)classToRun.newInstance();
    }
}