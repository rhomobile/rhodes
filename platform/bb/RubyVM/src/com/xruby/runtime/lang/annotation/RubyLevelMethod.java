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
//@Target({ElementType.METHOD, ElementType.TYPE})
public abstract class RubyLevelMethod {
	public abstract String name();
	public String[] alias(){ return null; }// default {};
	public boolean singleton(){ return false; }// default false;
	public boolean module() {return false;}//default false;
	public boolean privateMethod() {return false;}//default false;
}
