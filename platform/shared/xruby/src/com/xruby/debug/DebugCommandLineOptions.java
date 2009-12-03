/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.GnuParser;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

/**
 * Wrap the options for commandline debuuger
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class DebugCommandLineOptions {
    private static final String SEPARATOR = ";"; //semicolon

    // _S means short name e.g. --sourcepath is abbreviated to -s
    private static final String SOURCE_PATH = "sourcepath"; // long name
    private static final String SOURCE_PATH_S = "s"; // short name
    private static final String CLASS_PATH = "classpath"; // long name
    private static final String CLASS_PATH_S = "cp"; // short name

    private List<String> pathList;
    private String classPath;
    private StringBuffer entrance;

    /**
     * Constructor
     *
     * @param args Arguments
     */
    public DebugCommandLineOptions(String[] args) {
        pathList = new ArrayList<String>();

        GnuParser parser = new GnuParser();

        Options options = new Options();
        options.addOption(SOURCE_PATH_S, SOURCE_PATH, true, "path for the source code, seperated by semicolon");
        options.addOption(CLASS_PATH_S, CLASS_PATH, true, "classpath for debugee");

        CommandLine line;
        try {
            line = parser.parse(options, args, true);
        } catch (ParseException e) {
            throw new Error(e.toString());
        }

        if (line.hasOption(SOURCE_PATH_S)) {
            String paths = line.getOptionValue(SOURCE_PATH_S);
            StringTokenizer st = new StringTokenizer(paths, SEPARATOR);

            while (st.hasMoreTokens()) {
                String path = st.nextToken();
                pathList.add(path);
            }
        }

        if (line.hasOption(CLASS_PATH_S)) {
            this.classPath = line.getOptionValue(CLASS_PATH_S);
        }

        String[] tmp = line.getArgs();
        entrance = new StringBuffer();
        for (String str : tmp) {
            entrance.append(str).append(" ");
        }
    }

    // -------------------------
    //    Setters and Getters
    // -------------------------
    public List<String> getPathList() {
        return pathList;
    }


    public String getClassPath() {
        return classPath;
    }

    public List<URL> getClassPathList() {
        StringTokenizer st = new StringTokenizer(classPath, ";");
        List<URL> list = new ArrayList<URL>();

        while(st.hasMoreTokens()) {
            File file = new File(st.nextToken());
            if(file.exists()) {
                try {
                    list.add(file.toURI().toURL());
                } catch (MalformedURLException e) {
                    // TODO: Handle this exception
                }
            }
        }

        return list;
    }


    public String getEntrance() {
        return entrance.toString();
    }
}
