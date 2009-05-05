/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang.annotation;
/*
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
*/
//@Retention(RetentionPolicy.RUNTIME)
//@Target(ElementType.TYPE)
public abstract class RubyLevelClass {
	public abstract String name();
	public String superclass() { return "Object"; }
	public String[] modules() { return null; }//default {};
	public UndefMethod[] undef(){ return null; }// default {};
	public DummyMethod[] dummy(){ return null; }//default {};
}