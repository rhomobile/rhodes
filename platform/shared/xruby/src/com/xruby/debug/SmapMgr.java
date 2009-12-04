/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Yu Su (beanworms@gmail.com)
 */
public class SmapMgr {
    private static final String RB_SUFFIX = ".rb";

    private URLClassLoader loader;

    // Key: file name; Value: Map
    private Map<String, String> map = new HashMap<String, String>();

    /**
     * Classpath Management
     * 
     * @param loader URLClassLoader
     */
    SmapMgr(URLClassLoader loader) {
        this.loader = loader;
    }

    /**
     * Return the content of smap file, e.g.
     * debug2_test.rb => debug2_test/debug2_test.rb.smap
     *
     * @param fileName script file's name
     * @return smap's content, otherwise null
     */
    String getSmap(String fileName) {
        String smap = map.get(fileName);
        String path = null;
        if(smap == null) {
            if(fileName.endsWith(RB_SUFFIX)) {
                int pos = fileName.lastIndexOf(RB_SUFFIX);
                String packageName = fileName.substring(0, pos);
                path = String.format("xruby/%s/%s.smap", packageName, fileName);
            }
            else {
                // TODO: handle special condition
            }

            if(path != null) {
                try {
                    URL tmp = loader.findResource(path);  // TODO: Handle null return
                    BufferedReader reader = new BufferedReader(new InputStreamReader(tmp.openStream()));

                    StringBuffer content = new StringBuffer();
                    String line = reader.readLine();
                    while (line != null) {
                        content.append(line).append("\n");
                        line = reader.readLine();
                    }
                    smap = content.toString();

                    map.put(fileName, smap);
                } catch (IOException e) {
                    // TODO: Handle the IO error
                    return null;
                }
            }
            else {
                return null;
            }
        }

        return smap;
    }
}
