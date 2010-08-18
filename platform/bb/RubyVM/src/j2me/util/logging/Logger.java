/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util.logging;


public class Logger {

    private static final Logger INSTANCE = new Logger();
    
    public static Logger getLogger(String name) {
        return INSTANCE; 
    }

    private Logger() {        
    }
    
    public boolean isLoggable(Level level) {
        return (level == Level.SEVERE) || (level == Level.WARNING);
    }

    public void log(LogRecord record) {
    }

    public void severe(String msg) {
        System.out.println("[error] " + msg);               
    }

    public void warning(String msg) {
        System.out.println("[warning] " + msg);
    }

    public void info(String msg) {
        System.out.println("[info] " + msg);
    }

    public void config(String msg) {
    }

    public void fine(String msg) {
    }

    public void finer(String msg) {
    }

    public void finest(String msg) {
    }

    public void throwing(String sourceClass, String sourceMethod, Throwable thrown) {
        
    }

    public void entering(String sourceClass, String sourceMethod) {
    }
    
    public void exiting(String sourceClass, String sourceMethod) {
    }
    
    public void log(Level level,
            String msg) {        
    }
    
    public void log(Level level,
            String msg,
            Throwable thrown) {        
    }
}
