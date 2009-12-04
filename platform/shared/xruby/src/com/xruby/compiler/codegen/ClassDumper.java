package com.xruby.compiler.codegen;


public class ClassDumper {
	private static boolean dump;
	private static String dumpPath;
	private static ClassFileWriter cfw;
	private static String separator;
	
	static {
		dump = Boolean.getBoolean("xruby.method.dump");
		if (dump) {
			dumpPath = System.getProperty("xruby.method.dump_path");
			cfw = new ClassFileWriter();
			separator = System.getProperty("file.separator");
		}
	}
	
	public void dump(String classname, byte[] contents) {
		if (dump) {
			try {
				if (!dumpPath.endsWith(separator)) {
					dumpPath += separator;
				}
				String filename = dumpPath + classname + ".class";
				cfw.write(filename, contents);
			} catch (Exception e) {
			}
		}
	}
}
