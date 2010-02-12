/**
 * Copyright 2005-2007 Xue Yong Zhi, Yu Su
 * Distributed under the BSD License
 */

package com.xruby;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

class CommandLineOptions {
	private boolean compileOnly_ = false;
	private boolean help_ = false;
	private boolean eval_one_line_ = false;
	private boolean verbose_ = false;
	private boolean switch_ = false;
	private boolean strip_ = false;
	private boolean is_pe_ = false;
    private boolean enableDebug = false;
    private String eval_script_ = "";
	private String file_ = null;
	String backupExtension_ = null;
	private List<String> vars_ = new ArrayList<String>();
	private List<String> args_ = new ArrayList<String>();

	public boolean isCompileOnly() {
		return compileOnly_;
	}

	public boolean isHelp() {
		return help_;
	}

	public boolean isEvalOneLine() {
		return eval_one_line_;
	}

	public boolean isVerbose() {
		return verbose_;
	}

	public boolean isStrip() {
		return strip_;
	}

	public boolean isPe() {
		return is_pe_;
	}

    public boolean isEnableDebug() {
        return enableDebug;
    }
    
    public void enableDebug() {
    	this.enableDebug = true;
    }

    public String getEvalScript() {
		if (is_pe_) {
			return "while gets();" + eval_script_ + ";end";
		}
		return eval_script_;
	}

	public String getFilename() {
		return file_;
	}

	public String getBackupExtension() {
		return backupExtension_;
	}

	public String[] getVars() {
		return vars_.toArray(new String[0]);
	}

	public String[] getArgs() {
		return args_.toArray(new String[0]);
	}

	private String getRidOfQuote(String s) {
		if (s.charAt(0) == '\"' || s.charAt(0) == '\'') {
			return s.substring(1, s.length() - 1);
		} else {
			return s;
		}
	}

	//ruby supports single quote in the command line, while common-cli can not do that.
	//e.g. ruby -e 'print "hello"'
	private String[] preProcessSingleQuote(String[] args) {
		ArrayList<String> a = new ArrayList<String>();

		boolean find_single_quote = false;
		String temp = null;
		for (String s : args) {
			if (find_single_quote) {
				if (s.charAt(s.length() - 1) == '\'') {
					a.add(getRidOfQuote(temp + " " + s));
					find_single_quote = false;
				} else {
					temp = temp + " " + s;
				}
			} else if (s.charAt(0) == '\'') {
				if (s.charAt(s.length() - 1) == '\'') {
					a.add(getRidOfQuote(s));
				} else {
					temp = s;
					find_single_quote = true;
				}
			} else {
				a.add(s);
			}
		}

		return a.toArray(new String[] {});
	}

	public void parseOptionsFromFile(String filename) {
		String s = readOptionsFromFile(filename);
		if (null == s) {
			return;
		}

		if (s.equals("-s")) {
			moveArgsToVars();
		}
	}

	private void moveArgsToVars() {
		for (int i = 0; i < args_.size();) {
			String s = args_.get(i);
			if (s.charAt(0) == '-') {
				vars_.add(s.substring(1));
				args_.remove(i);
			} else {
				++i;
			}
		}
	}

	private String readOptionsFromFile(String filename) {
		//e.g. #! /usr/local/bin/ruby -s\n
		try {
			BufferedReader reader = new BufferedReader(new FileReader(filename));

			do {
				String s = reader.readLine();
				if (s != null && s.length() > "#! ruby".length()) {
					if (s.charAt(0) == '#' && s.charAt(1) == '!') {
						int i = s.indexOf("ruby");
						if (i > 0) {
							return s.substring(i + 4).trim();
						}
					}
				}
			} while (strip_);

			reader.close();
			return null;
		} catch (IOException e) {
			return null;
		}
	}

	private String[] preProcess_pe_i(String[] args) {
		List<String> a = new ArrayList<String>();

		for (String s : args) {
			if (s.equals("-pe")) {
				is_pe_ = true;
				a.add("-e");
			} else if (s.startsWith("-i")) {
				if (!s.equals("-i")) {
					backupExtension_ = s.substring(2);
				}
			} else {
				a.add(s);
			}
		}

		return a.toArray(new String[0]);
	}

	@SuppressWarnings("unchecked")
	public CommandLineOptions(String[] args) {
		if (args.length == 0) {
			return;
		}

		args = preProcessSingleQuote(args);
		args = preProcess_pe_i(args);

		CommandLineParser parser = new PosixParser();
		Options options = new Options();
		options.addOption("h", false, "display help");
		options.addOption("c", false, "compiler only");
		options.addOption("e", true, "eval one line");
		options.addOption("v", false, "print version number, then turn on verbose mode");
		options.addOption("s", false, "enable some switch parsing for switches after script name");
		options.addOption("x", false, "strip off text before #!ruby line");
        options.addOption("g", false, "enable debug");
        options.addOption("n", true, "provide class prefix name");

        CommandLine line;
		try {
			line = parser.parse(options, args, true);
		} catch (ParseException e) {
			throw new Error(e.toString());
		}

		if (line.hasOption("c")) {
			compileOnly_ = true;

            if (line.hasOption("g")) {
                enableDebug = true;
            }
            if (line.hasOption("v")) {
    			verbose_ = true;
    		}
    		if (line.hasOption("n")) {
    			String str = line.getOptionValue("n"); 
    			com.xruby.compiler.codegen.NameFactory.name_classprefix_ = str;
    		}
            
        } else if (line.hasOption("h")) {
			help_ = true;
		} else if (line.hasOption("v")) {
			verbose_ = true;
		} else if (line.hasOption("e")) {
			eval_one_line_ = true;
			eval_script_ = line.getOptionValue("e");
		} else if (line.hasOption("s")) {
			switch_ = true;
		} else if (line.hasOption("x")) {
			strip_ = true;
		}

		if (line.getArgList().size() > 0) {
			file_ = (String)line.getArgList().remove(0);
			args_.addAll(line.getArgList());
			if (switch_) {
				moveArgsToVars();
			}
		}
	}

}
