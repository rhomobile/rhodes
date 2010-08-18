/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;

import javolution.Javolution;
import javolution.lang.Configurable;
import javolution.util.StandardLog;
import j2me.lang.CharSequence;

/**
 * <p> This class represents a context for object-based/thread-based logging
 *     capabilities.</p>
 *
 * <p> The {@link #DEFAULT default} logging context is {@link 
 *     StandardLog StandardLog} to leverage <code>java.util.logging</code> 
 *     capabilities.</p>
 *     
 * <p> Logging can be temporarily modified on a thread or object basis.
 *     For example:[code]
 *     public static main(String[] args) {
 *         LogContext.enter(LogContext.NULL); // Temporarily disables logging.
 *         try { 
 *             ClassInitializer.initializeAll();  // Initializes bootstrap, extensions and classpath classes.
 *         } finally {
 *             LogContext.exit(LogContext.NULL); // Goes back to default logging.
 *         }
 *         ...
 *     }[/code]</p>
 *     
 * <p> Applications may extend this base class to address specific logging
 *     requirements. For example:[code]
 *     // This class allows for custom logging of session events. 
 *     public abstract class SessionLog extends LogContext  {
 *         public static void start(Session session) {
 *             LogContext log = LogContext.current();
 *             if (log instanceof SessionLog.Loggable) { 
 *                 ((SessionLog.Loggable)log).logStart(session);
 *             } else if (log.isInfoLogged()){
 *                 log.logInfo("Session " + session.id() + " started");
 *             }
 *         }
 *         public static void end(Session session) { ... }
 *         public interface Loggable { 
 *             void logStart(Session session);
 *             void logEnd(Session session);
 *         }
 *     }[/code]</p>
 *     
 * <p> The use of interfaces (such as <code>Loggable</code> above) makes it easy
 *     for any context to support customs logging events.
 *     For example:[code]
 *     class MyLog extends StandardLog implements SessionLog.Loggable, DatabaseLog.Loggable { 
 *         ...   // Specialized logging for session and database events. 
 *     }
 *     MyLog myLog = new MyLog();
 *     LogContext.enter(myLog);
 *     try {
 *         ...
 *         LogContext.info("Informative message"); // Standard logging.   
 *         ...
 *         DatabaseLog.fail(transaction); // Database custom logging.
 *         ... 
 *         SessionLog.start(session); // Session custom logging.
 *         ...
 *     } finally {
 *         LogContext.exit(myLog);
 *     }[/code]</p>
 *     
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.2, August 5, 2007
 */
public abstract class LogContext extends Context {

    /**
     * Holds the default logging context instance.
     */
    private static volatile LogContext _Default = new StandardLog();

    /**
     * Holds the logging context implementation forwarding log events to the 
     * root <code>java.util.logging.Logger</code> (default logging context).
     * The info/warning/error events are mapped to the info/warning/severe 
     * log levels respectively.
     */
    public static final Class/*<? extends LogContext>*/STANDARD = _Default.getClass();

    /**
     * Holds a logging context implementation ignoring logging events.
     */
    public static final Class/*<? extends LogContext>*/NULL = new NullLog().getClass();

    /**
     * Holds a context logging messages to <code>System.out</code>.
     */
    public static final Class/*<? extends LogContext>*/SYSTEM_OUT = new SystemOut()
            .getClass();

    /**
     * Holds a context logging informative/warnings/errors events to 
     * the system console.
     */
    public static final Class/*<? extends LogContext>*/CONSOLE = new ConsoleLog()
            .getClass();

    /**
     * Holds the logging context default implementation (configurable, 
     * default value {@link #STANDARD}).
     */
    public static final Configurable/*<Class<? extends LogContext>>*/
    DEFAULT = new Configurable(STANDARD) {
        protected void notifyChange() {
            _Default = (LogContext) ObjectFactory.getInstance((Class) get())
                    .object();
        }
    };

    /**
     * Default constructor.
     */
    protected LogContext() {
    }

    /**
     * Returns the current logging context. If the current thread has not 
     * entered any logging context the {@link #getDefault()} is returned.
     *
     * @return the current logging context.
     */
    public static/*LogContext*/Context getCurrent() {
        for (Context ctx = Context.getCurrent(); ctx != null; ctx = ctx.getOuter()) {
            if (ctx instanceof LogContext)
                return (LogContext) ctx;
        }
        return LogContext._Default;
    }

    /**
     * Returns the default instance ({@link #DEFAULT} implementation).
     *
     * @return the default instance.
     */
    public static LogContext getDefault() {
        return LogContext._Default;
    }

    /**
     * Logs the specified informative message.
     * 
     * @param message the informative message being logged.
     * @see #logInfo(CharSequence)
     */
    public static void info(CharSequence message) {
        LogContext logContext = (LogContext) LogContext.getCurrent();
        logContext.logInfo(message);
    }

    /**
     * Equivalent to {@link #info(CharSequence)} (for J2ME compatibility).
     */
    public static void info(String message) {
        LogContext logContext = (LogContext) LogContext.getCurrent();
        logContext.logInfo(message);
    }

    /**
     * Logs the specified warning message to the current logging context.
     * 
     * @param message the warning message being logged.
     * @see #logWarning(CharSequence)
     */
    public static void warning(CharSequence message) {
        LogContext logContext = (LogContext) LogContext.getCurrent();
        logContext.logWarning(message);
    }

    /**
     * Equivalent to {@link #warning(CharSequence)} (for J2ME compatibility).
     */
    public static void warning(String message) {
        LogContext logContext = (LogContext) LogContext.getCurrent();
        logContext.logWarning(message);
    }

    /**
     * Logs the specified error to the current logging context.
     * 
     * @param error the error being logged.
     */
    public static void error(Throwable error) {
        LogContext logContext = (LogContext) LogContext.getCurrent();
        logContext.logError(error, (CharSequence) null);
    }

    /**
     * Logs the specified error and error message to the current logging
     * context. 
     * 
     * @param error the error being logged.
     * @param message the supplementary message.
     */
    public static void error(Throwable error, CharSequence message) {
        LogContext logContext = (LogContext) LogContext.getCurrent();
        logContext.logError(error, message);
    }

    /**
     * Equivalent to {@link #error(Throwable, CharSequence)}
     * (for J2ME compatibility).
     */
    public static void error(Throwable error, String message) {
        LogContext logContext = (LogContext) LogContext.getCurrent();
        logContext.logError(error, message);
    }

    /**
     * Logs the specified error message to the current logging
     * context. 
     * 
     * @param message the error message being logged.
     */
    public static void error(CharSequence message) {
        LogContext logContext = (LogContext) LogContext.getCurrent();
        logContext.logError(null, message);
    }

    /**
     * Equivalent to {@link #error(CharSequence)} (for J2ME compatibility).
     */
    public static final void error(String message) {
        LogContext logContext = (LogContext) LogContext.getCurrent();
        logContext.logError(null, message);
    }

    /**
     * Indicates if informative messages are logged.
     * 
     * @return <code>true</code> if informative messages are logged;
     *         <code>false</code> otherwise.
     */
    public abstract boolean isInfoLogged();

    /**
     * Logs the specified informative message.
     * 
     * @param message the informative message being logged.
     */
    public abstract void logInfo(CharSequence message);

    /**
     * Equivalent to {@link #logInfo(CharSequence)} (for J2ME compatibility).
     */
    public final void logInfo(String message) {
        logInfo(Javolution.j2meToCharSeq(message));
    }

    /**
     * Indicates if warning messages are logged.
     * 
     * @return <code>true</code> if warnings message are logged;
     *         <code>false</code> otherwise.
     */
    public abstract boolean isWarningLogged();

    /**
     * Logs the specified warning message.
     * 
     * @param message the warning message being logged.
     */
    public abstract void logWarning(CharSequence message);

    /**
     * Equivalent to {@link #logWarning(CharSequence)} (for J2ME compatibility).
     */
    public final void logWarning(String message) {
        logWarning(Javolution.j2meToCharSeq(message));
    }
    /**
     * Indicates if errors are logged.
     * 
     * @return <code>true</code> if errors are logged;
     *         <code>false</code> otherwise.
     */
    public abstract boolean isErrorLogged();

    /**
     * Logs the specified error.
     * 
     * @param error the error being logged or <code>null</code> if none.
     * @param message the associated message or <code>null</code> if none.
     */
    public abstract void logError(Throwable error, CharSequence message);

    /**
     * Equivalent to {@link #logError(Throwable, CharSequence)} 
     * (for J2ME compatibility).
     */
    public final void logError(Throwable error, String message) {
        logError(error, Javolution.j2meToCharSeq(message));
    }

    // Implements Context abstract method.
    protected void enterAction() {
        // Do nothing.
    }

    // Implements Context abstract method.
    protected void exitAction() {
        // Do nothing.
    }

    /**
     * This class represents the system logging context.
     */
    private static class SystemOut extends LogContext {

        public boolean isInfoLogged() {
            return true;
        }

        public void logInfo(CharSequence message) {
            System.out.print("[info] ");
            System.out.println(message);
        }

        public boolean isWarningLogged() {
            return true;
        }

        public void logWarning(CharSequence message) {
            System.out.print("[warning] ");
            System.out.println(message);
        }

        public boolean isErrorLogged() {
            return true;
        }

        public void logError(Throwable error, CharSequence message) {
            System.out.print("[error] ");
            if (error != null) {
                System.out.print(error.getClass().getName());
                System.out.print(" - ");
            }
            String description = (message != null) ? message.toString()
                    : (error != null) ? error.getMessage() : "";
            System.out.println(description);
            if (error != null) {
                error.printStackTrace();
            }
        }
    }

    /**
     * This class represents a non-logging context.
     */
    private static final class NullLog extends SystemOut {

        public boolean isInfoLogged() {
            return false;
        }

        public void logInfo(CharSequence message) {
            // Do nothing.
        }

        public boolean isWarningLogged() {
            return false;
        }

        public void logWarning(CharSequence message) {
            // Do nothing.
        }

        public boolean isErrorLogged() {
            return false;
        }

        public void logError(Throwable error, CharSequence message) {
            // Do nothing.
        }
    }

    /**
     * This class represents the console logging context.
     */
    private static class ConsoleLog extends SystemOut {
        /*@JVM-1.6+@
         final java.io.PrintWriter writer;
         ConsoleLog() {
         java.io.Console console = System.console();
         writer = console != null ? console.writer() : null;
         }

         public void logInfo(CharSequence message) {
         if (writer == null) {
         super.logInfo(message);
         } else {
         writer.print("[info] ");
         writer.println(message);
         }
         }

         public void logWarning(CharSequence message) {
         if (writer == null) {
         super.logWarning(message);
         } else {             
         writer.print("[warning] ");
         writer.println(message);
         }                
         }

         public void logError(Throwable error, CharSequence message) {
         if (writer == null) {
         super.logError(error, message);
         } else {             
         writer.print("[error] ");
         if (error != null) {
         writer.print(error.getClass().getName());
         writer.print(" - ");
         }
         String description = (message != null) ? message.toString()
         : (error != null) ? error.getMessage() : "";
         writer.println(description);
         writer.println();
         }
         }                          
         /**/
    }

    // Allows instances of private classes to be factory produced. 
    static {
        ObjectFactory.setInstance(new ObjectFactory() {
            protected Object create() {
                return new ConsoleLog();
            } }, CONSOLE);
        ObjectFactory.setInstance(new ObjectFactory() {
            protected Object create() {
                return new NullLog();
            } }, NULL);
        ObjectFactory.setInstance(new ObjectFactory() {
            protected Object create() {
                return new SystemOut();
            } }, SYSTEM_OUT);
    }
}