/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.StringTokenizer;

/**
 * @author Yu Su (beanworms@gmail.com)
 */
public class SourceCodeMgr {

    private static Map<String, SourceCode> cache ;
    private static Map<String, Map<String, int[]>> blockMap;

    static {
        cache = new HashMap<String, SourceCode>();
        blockMap = new HashMap<String, Map<String, int[]>>();
    }

    // TODO: handle exception
    // TODO: handle possible naming conflict
    // TODO: This method should take a location instance as its parameter.
    /**
     * Return the source code in specified position
     * @param sourceFile source file's name
     * @param lineNumber position
     * @return source code, return null if the position is illegal
     */
    public static String sourceLine(String sourceFile, int lineNumber) {
        if(cache.containsKey(sourceFile)) {
            return cache.get(sourceFile).sourceLine(lineNumber);
        }

        List<File> paths = DebugContext.getSourcePath();
        for (File path : paths) {
            if (path.isDirectory()) {
                String tmp = path.getAbsolutePath() + File.separatorChar + sourceFile;
                File file = new File(tmp);
                if(file.exists()) {
                    try {
                        SourceCode sc = new SourceCode(file);
                        cache.put(sourceFile, sc);
                        return sc.sourceLine(lineNumber);
                    } catch (XRubyDebugException e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        return null;
    }

    /**
     * Return the code snippet of given range
     * e.g. lineNumber = 12, range = 5, it returns the code between [7, 17] 
     *
     * @param sourceFile source file's name
     * @param lineNumber position
     * @param range range
     * @return sourece snippet
     */
    public static String getSourceSnippet(String sourceFile, int lineNumber, int range) {
        StringBuffer snippet = new StringBuffer();
        if(cache.containsKey(sourceFile)) {
            SourceCode code = cache.get(sourceFile);
            int start = lineNumber - range;
            int end = lineNumber + range - 1;

            if(start <= 0) {
                start = 1;
            }

            if(end > code.size()) {
                end = code.size();
            }

            // First, align the line number
            int width = String.valueOf(end).length();
            String format = "%" + width + "d";
            for(int i = start; i <= end; i ++) {
                String tmp = code.sourceLine(i);
                if(i != lineNumber) {
                    snippet.append(String.format(format + "   %s\n", i, tmp));
                } else {
                    snippet.append(String.format(format + "=> %s\n", i, tmp));
                }
            }
        }

        return snippet.toString();
    }

    /**
     * Return the real class's name, for methods and blocks
     * e.g. test_debug2.main -> test_debug2.BLOCK$0
     * 
     * @param classId class id
     * @param lineNumber position
     * @return class name
     * 
     * @throws XRubyDebugException illegal classId
     */
    public static String getRealClass(String classId, int lineNumber) throws XRubyDebugException {

        StringTokenizer st = new StringTokenizer(classId, ".");
        String scriptName;
        String packageName;

        if(st.hasMoreTokens()) {
            packageName = st.nextToken();
            scriptName =  packageName + ".rb";
        } else {
            throw new XRubyDebugException("IllegalArgumentExcpeiont, class");
        }
        
        // Retrieve block/method's name
        String blockName = retrieveBlockName(scriptName, lineNumber);
        if (blockName != null) {
            return blockName;
        } else {
            return "xruby." + packageName + ".main";
        }
    }


    private static String retrieveBlockName(String fileName, int lineNumber) throws XRubyDebugException {
        Map<String, int[]> map = blockMap.get(fileName);
        String realName = null;

        if(map == null) {  // The first time reading this file's map
            map = new HashMap<String, int[]>();
            blockMap.put(fileName, map);
            BufferedReader reader;
            try {
                String smap = DebugContext.getSmapMgr().getSmap(fileName);
                StringReader sr = new StringReader(smap);
                reader = new BufferedReader(sr);

                // Start parsing block info
                int number = Integer.parseInt(reader.readLine());
                int lastStartIdx = -1;
                for (int i = 0; i < number; i ++) {
                    String line = reader.readLine();
                    if (line.trim().equals("")) {
                        continue;
                    }

                    StringTokenizer seq = new StringTokenizer(line, " ");

                    // We assume the format is correct: [Block's Name] [start] [end]
                    String blockName = seq.nextToken();
                    String start = seq.nextToken();
                    String end = seq.nextToken();
                    int startIdx = Integer.parseInt(start);
                    int endIdx = Integer.parseInt(end);

                    if(lineNumber >= startIdx && lineNumber <= endIdx && 
                       (lastStartIdx == -1 || lastStartIdx < startIdx)) {
                        realName = blockName;
                        lastStartIdx = startIdx;
                    }

                    map.put(blockName, new int[]{startIdx, endIdx});
                }
            } catch (IOException e) {
                throw new XRubyDebugException(
                        String.format("Couldn't read this file or wrong format, %s:\n%s", fileName, e.getMessage()));
            }
        } else {  // already existed
            Set<String> blocks = map.keySet();
            for(String block: blocks) {
                int[] range = map.get(block);
                if(lineNumber > range[0] && lineNumber <= range[1]) {
                    return block;
                }
            }
        }

        return realName;
    }
}

class SourceCode {
    private List<String> lines;

    SourceCode(File file) throws XRubyDebugException {
        lines = new ArrayList<String>();
        if (!file.exists()) {
            throw new XRubyDebugException(String.format("%s doesn't exist", file.getName()));
        }

        try {
            BufferedReader reader = new BufferedReader(new FileReader(file));
            String line = reader.readLine();
            while (line != null) {
                lines.add(line);
                line = reader.readLine();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public String sourceLine(int lineNumber) {
        return lines.get(lineNumber - 1);
    }

    public int size() {
        return lines.size();
    }
}
