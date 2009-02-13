package com.xruby.runtime.lang.annotation;
/*
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
*/

//@Retention(RetentionPolicy.RUNTIME)
//@Target(ElementType.TYPE)
public abstract class RubyLevelObject {
	public String name(){return "";}// default "";
	public abstract String objectClass();
}
