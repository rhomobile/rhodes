package com.xruby.compiler.codegen;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class ClassFileWriter {
	public void write(String name, byte[] contents) throws FileNotFoundException, IOException {
		File file = new File(name);
        File parent = file.getParentFile();
        if (!parent.exists()) {
            parent.mkdirs();
        }
        OutputStream fos = null;
        try {
            fos = new FileOutputStream(file);
            fos.write(contents);
        } finally {
            if (fos != null) {
                fos.close();
            }
        }
	}
}
