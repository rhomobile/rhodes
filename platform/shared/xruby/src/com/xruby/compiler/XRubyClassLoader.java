package com.xruby.compiler;

import com.xruby.compiler.codegen.NameFactory;

public class XRubyClassLoader extends ClassLoader {
	public Class load(String name) throws ClassNotFoundException {
		return super.loadClass(NameFactory.convertSlashToDot(name));
	}
	public Class load(String name, byte[] b) {
		return defineClass(NameFactory.convertSlashToDot(name), b, 0, b.length);
	}
}
