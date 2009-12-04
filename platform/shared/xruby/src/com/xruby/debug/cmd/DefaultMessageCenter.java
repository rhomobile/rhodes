/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug.cmd;

import com.xruby.debug.MessageCenter;

import static java.lang.System.out;

/**
 * Output all the message to the stdout/err
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class DefaultMessageCenter implements MessageCenter {

    public void emitMessage(String msg) {
        out.println(msg);
    }

    public void emitMessage(String format, Object... args) {
        out.println(String.format(format, args));
    }
}
