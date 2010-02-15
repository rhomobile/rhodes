/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.compiler.codedom;

import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.StringTokenizer;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

/**
 * Actually this is not only for blocks, but also for methods
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class BlockFarm {
    private static Map<String, Map<String, int[]>> farm =
            new HashMap<String, Map<String, int[]>>();

    /**
     * 3                               # Number of blocks/methods
     * test_debug2.BLOCK$2 19 20       # block/method name [start] [end]
     * test_debug2.BLOCK$0 3 5
     * test_debug2.test_method$1 14 16
     *
     * @param scriptName script file's name
     * @return block map, string form
     */
    public static String getMapByName(String scriptName) {
        Set<String> files = farm.keySet();
        if (!files.contains(scriptName)) {
            return null;
        } else {
            StringBuffer description = new StringBuffer();
            Map<String, int[]> map = farm.get(scriptName);
            description.append(map.size()).append("\n");  // Number of blocks

            // blocks/methods
            Set<String> blocks = map.keySet();
            for (String blockName : blocks) {
                int[] range = map.get(blockName);
                String line = String.format("%s %d %d\n", blockName, range[0], range[1]);
                description.append(line);
            }

            return description.toString();
        }
    }

    static void markBlock(Block block) {
        String blockName = block.getName();
        String name = extractBlockName(blockName);
        String scriptName = extractScriptName(blockName);

        Map<String, int[]> map = farm.get(scriptName);

        // First block in this file
        if(map == null) {
            map = new HashMap<String, int[]>();
            farm.put(scriptName, map);
        }

        int start = block.getStartPosition();
        int end = block.getEndPosition();
        int[] range = new int[] {start, end};
        map.put(name, range);
    }

    static void markMethod(String scriptName, String methodName, int[] range) {
        Map<String, int[]> map = farm.get(scriptName);

        if(map == null) {
            map = new HashMap<String, int[]>();
            farm.put(scriptName, map);
        }

        methodName = extractBlockName(methodName);
        map.put(methodName, range);
    }

    private static String extractBlockName(String name) {
        // TODO: We should put this method to the NameFactory,
        // TODO: and put NameFactory under the compiler package, for extractMethodName
        Pattern pattern = Pattern.compile("/");
        Matcher matcher = pattern.matcher(name);
        String blockName = matcher.replaceAll(".");

        return blockName;
    }

    private static String extractScriptName(String blockName) {
        if(blockName.startsWith(com.xruby.compiler.codegen.NameFactory.name_classprefix_ + "/")) {
            blockName = blockName.substring(6);
        }
        StringTokenizer st = new StringTokenizer(blockName, "/");

        // TODO: maybe we need create a global variable to keep current script name
        // TODO: Add a new global helper method getCurrentScriptName(). We need fix this in the feature.
        if(st.hasMoreTokens()) {
            return st.nextToken() + ".rb";
        } else {
            return null;
        }
    }

    public static Map<String, Map<String, int[]>> getBlockMap() {
        return farm;
    }
}
