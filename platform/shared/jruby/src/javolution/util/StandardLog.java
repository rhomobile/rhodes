/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.util;

import javolution.Javolution;
import javolution.context.LogContext;
import j2me.lang.CharSequence;
import j2me.util.logging.Level;
import j2me.util.logging.Logger;
import j2me.util.logging.LogRecord;

/**
 * <p> This class represents a specialized logging context forwarding events 
 *     to a standard logger (<code>java.util.logging.Logger</code>).</p>
 *
 * <p> This class leverages the capabilities of the standard logging facility 
 *     and extends it to support specialized {@link LogContext logging} on a 
 *     thread or object basis. For example:[code]
 *     StandardLog remoteLog = new StandardLog(Logger.getLogger("remote"));
 *     StandardLog.enter(remoteLog); 
 *     try {
 *         StandardLog.fine("Current thread uses a remote logger");
 *         ...       
 *     } finally {
 *         StandardLog.exit(remoteLog); // Reverts to previous logging context.
 *     }[/code]</p>
 *     
 *     
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.1, August 1, 2007
 */
public class StandardLog extends LogContext {

    /**
     * The logger associated to this logging context.
     */
    private Logger _logger;

    /**
     * Creates a logging context forwarding events to the root logger 
     * (<code>Logger.getLogger("")</code>).
     */
    public StandardLog() {
        this(Logger.getLogger(""));
    }

    /**
     * Creates a standard log context forwarding events to the specified 
     * logger.
     * 
     * @param logger the logger to which log events are forwarded to.
     */
    public StandardLog(Logger logger) {
        _logger = logger;
    }

    /**
     * Returns the logger to which this context forwards the events to.
     * 
     * @return the logger for this standard logging context.
     */
    public final Logger getLogger() {
        return _logger;
    }

    /**
     * Checks if a message of the given level would actually be logged
     * by this logger.
     *
     * @param level the message logging level
     * @return <code>true</code> if a message of specified level would actually
     *        be logged;<code>false</code> otherwise.
     */
    public static boolean isLoggable(Level level) {
        LogContext log = (LogContext) LogContext.getCurrent();
        if (log instanceof StandardLog) {
            return ((StandardLog) log)._logger.isLoggable(level);
        } else if (level.intValue() >= Level.WARNING.intValue()) {
            return log.isWarningLogged();
        } else if (level.intValue() >= Level.INFO.intValue()) {
            return log.isInfoLogged();
        } else {
            return false;
        }
    }

    /**
     * Log a specific LogRecord. If the current logging context is not a 
     * {@link StandardLog}, an {@link LogContext#logError error}, 
     * {@link LogContext#logWarning warning} or
     * {@link LogContext#logInfo info} is possibly logged.
     *
     * @param record the LogRecord to be published.
     */
    public static void log(LogRecord record) {
        LogContext log = (LogContext) LogContext.getCurrent();
        if (log instanceof StandardLog) {
            ((StandardLog) log)._logger.log(record);
        } else {
            Throwable error = record.getThrown();
            if (error != null) {
                if (log.isErrorLogged()) { 
                    log.logError(error, Javolution.j2meToCharSeq(record.getMessage()));
                }
            } else if (record.getLevel().intValue() > Level.WARNING.intValue()) {
                if (log.isWarningLogged()) { 
                    log.logWarning(Javolution.j2meToCharSeq(record.getMessage()));
                }
            } else if (record.getLevel().intValue() > Level.INFO.intValue()) {
                if (log.isInfoLogged()) {
                    log.logInfo(Javolution.j2meToCharSeq(record.getMessage()));
                }
            }
        }
    }

    /**
     * Logs a {@link Level#SEVERE SEVERE} message. If the current logging 
     * context is not a {@link StandardLog} a {@link LogContext#warning 
     * warning} message is logged.
     * 
     * @param msg the severe message.
     */
    public static void severe(String msg) {
        LogContext log = (LogContext) LogContext.getCurrent();
        if (log instanceof StandardLog) {
            ((StandardLog) log)._logger.severe(msg);
        } else if (log.isWarningLogged()) {
            log.logWarning(Javolution.j2meToCharSeq(msg));
        }
    }

    /**
     * Logs a {@link Level#CONFIG CONFIG} message. If the current logging 
     * context is not a {@link StandardLog} no message is logged.
     * 
     * @param msg the config message.
     */
    public static void config(String msg) {
        LogContext log = (LogContext) LogContext.getCurrent();
        if (log instanceof StandardLog) {
            ((StandardLog) log)._logger.config(msg);
        }
    }

    /**
     * Logs a {@link Level#FINE FINE} message. If the current logging 
     * context is not a {@link StandardLog} no message is logged.
     * 
     * @param msg the fine message.
     */
    public static void fine(String msg) {
        LogContext log = (LogContext) LogContext.getCurrent();
        if (log instanceof StandardLog) {
            ((StandardLog) log)._logger.fine(msg);
        }
    }

    /**
     * Logs a {@link Level#FINER FINER} message. If the current logging 
     * context is not a {@link StandardLog} no message is logged.
     * 
     * @param msg the finer message.
     */
    public static void finer(String msg) {
        LogContext log = (LogContext) LogContext.getCurrent();
        if (log instanceof StandardLog) {
            ((StandardLog) log)._logger.finer(msg);
        }
    }

    /**
     * Logs a {@link Level#FINEST FINEST} message. If the current logging 
     * context is not a {@link StandardLog} no message is logged.
     * 
     * @param msg the finest message.
     */
    public static void finest(String msg) {
        LogContext log = (LogContext) LogContext.getCurrent();
        if (log instanceof StandardLog) {
            ((StandardLog) log)._logger.finest(msg);
        }
    }

    /**
     * Logs throwing an exception. If the current logging context is not a 
     * {@link StandardLog} an {@link LogContext#logError error} is logged.
     * 
     * @param sourceClass name of class that issued the logging request.
     * @param sourceMethod name of the method.
     * @param thrown the error that is being thrown.
     */
    public static void throwing(String sourceClass, String sourceMethod,
            Throwable thrown) {
        LogContext log = (LogContext) LogContext.getCurrent();
        if (log instanceof StandardLog) {
            ((StandardLog) log)._logger.throwing(sourceClass, sourceMethod,
                    thrown);
        } else if (log.isErrorLogged()) {
            log.logError(thrown, (CharSequence) null);
        }
    }

    /**
     * Log a method entry. If the current logging context is not a 
     * {@link StandardLog} no entry is logged.
     * 
     * @param sourceClass name of class that issued the logging request.
     * @param sourceMethod name of method that is being entered.
     */
    public static void entering(String sourceClass, String sourceMethod) {
        LogContext log = (LogContext) LogContext.getCurrent();
        if (log instanceof StandardLog) {
            ((StandardLog) log)._logger.entering(sourceClass, sourceMethod);
        }
    }

    /**
     * Log a method return. If the current logging context is not a 
     * {@link StandardLog} no return is logged.
     * 
     * @param sourceClass name of class that issued the logging request.
     * @param sourceMethod name of method that is being returned.
     */
    public static void exiting(String sourceClass, String sourceMethod) {
        LogContext log = (LogContext) LogContext.getCurrent();
        if (log instanceof StandardLog) {
            ((StandardLog) log)._logger.exiting(sourceClass, sourceMethod);
        }
    }

    // Implements LogContext abstract method.
    public boolean isInfoLogged() {
        return _logger.isLoggable(Level.INFO);
    }

    // Implements LogContext abstract method.
    public boolean isWarningLogged() {
        return _logger.isLoggable(Level.WARNING);
    }

    // Implements LogContext abstract method.
    public boolean isErrorLogged() {
        return _logger.isLoggable(Level.SEVERE);
    }

    // Implements LogContext abstract method.
    public void logInfo(CharSequence message) {
        _logger.info(message.toString());
    }

    // Implements LogContext abstract method.
    public void logWarning(CharSequence message) {
        _logger.warning(message.toString());
    }

    // Implements LogContext abstract method.
    public void logError(Throwable error, CharSequence message) {
        String description = (message != null) ? message.toString() : "";
        if (error != null) {
            _logger.log(Level.SEVERE, description, error);
        } else {
            _logger.log(Level.SEVERE, description);            
        }
    }

}