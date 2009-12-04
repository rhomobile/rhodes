/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

/**
 * Proxy of message
 *  
 * @author Yu Su (beanworms@gmail.com)
 */
public interface MessageCenter {
    public void emitMessage(String msg);
    public void emitMessage(String format, Object ... args);
}
