/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2007 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;
import javolution.lang.Configurable;

/**
 * <p> This class represents a high-level security context (low level 
 *     security being addressed by the system security manager).</p>
 *     
 * <p> Applications may extend this base class to address specific security
 *     requirements. For example:[code]
 *     // This class defines custom policy with regards to database access. 
 *     public abstract class DatabaseAccess extends SecurityContext  {
 *         public static boolean isReadAllowed(Table table) {
 *             SecurityContext policy = SecurityContext.current();
 *             return (policy instanceof DatabaseAccess.Permission) ?
 *                 ((DatabaseAccess.Permission)policy).isReadable(table) : false;
 *         }
 *         public interface Permission { 
 *             boolean isReadable(Table table);
 *             boolean isWritable(Table table);
 *         }
 *     }[/code]</p>
 *     
 * <p> The use of interfaces (such as <code>Permission</code> above) makes 
 *     it easy for custom policies to support any security actions.
 *     For example:[code]
 *     class Policy extends SecurityContext implements DatabaseAccess.Permission, FileAccess.Permission {
 *          public boolean isReadable(Table table) { 
 *              return !table.isPrivate();
 *          }
 *          public boolean isWritable(Table table) { 
 *              return Session.getSession().getUser().isAdministrator();
 *          }
 *          public boolean isReadable(File file) { 
 *              return true;
 *          }
 *          public boolean isWritable(File file) { 
 *              return false;
 *          }
 *     }
 *     ...
 *     Policy localPolicy = new Policy();
 *     SecurityContext.enter(localPolicy); // Current thread overrides default policy (configurable)  
 *     try {                               // (if allowed, ref. SecurityContext.isReplaceable())
 *         ...
 *         DatabaseAccess.isReadAllowed(table);   
 *         ...
 *         FileAccess.isWriteAllowed(file);
 *         ...
 *     } finally {
 *         SecurityContext.exit();
 *     }[/code]</p>    
 *     
 * <p> The default permissions managed by the {@link #DEFAULT} implementation
 *     are the permission to {@link #isReplaceable replace} the current security
 *     context by default) and the permission to {@link #isModifiable modify} 
 *     all the application {@link javolution.lang.Configurable.Logic 
 *     configuration} settings.</p>
 *
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.2, August 5, 2007
 */
public abstract class SecurityContext extends Context {

    /**
     * Holds the default security context.
     */
    private static volatile SecurityContext _Default = new Default();

    /**
     * Holds the default security context implementation (configurable).
     */
    public static final Configurable/*<Class<? extends SecurityContext>>*/DEFAULT 
        = new Configurable(_Default.getClass()) {
        protected void notifyChange() {
            _Default = (SecurityContext) ObjectFactory.getInstance((Class) get())
            .object();
        }
    };
    
    /**
     * Default constructor.
     */
    protected SecurityContext() {
    }

    /**
     * Returns the current security context. If the current thread has not 
     * entered any security context then  {@link #getDefault()} is returned.
     *
     * @return the current security context.
     */
    public static/*SecurityContext*/Context getCurrent() {
        for (Context ctx = Context.getCurrent(); ctx != null; ctx = ctx.getOuter()) {
            if (ctx instanceof SecurityContext)
                return (SecurityContext) ctx;
        }
        return SecurityContext._Default;
    }

    /**
     * Returns the default instance ({@link #DEFAULT} implementation).
     *
     * @return the default instance.
     */
    public static SecurityContext getDefault() {
        return SecurityContext._Default;
    }
    
    // Implements Context abstract method.
    protected final void enterAction() {
        // Checks if the previous security context is replaceable.
        SecurityContext previousPolicy = SecurityContext._Default;
        for (Context ctx = this.getOuter(); ctx != null; ctx = ctx.getOuter()) {
            if (ctx instanceof SecurityContext) {
                previousPolicy = (SecurityContext) ctx;
                break;
            }
        }
        if (!previousPolicy.isReplaceable()) 
            throw new SecurityException("Current Security Context not Replaceable");
    }

    // Implements Context abstract method.
    protected final void exitAction() {
        // Do nothing.
    }

    /**
     * Indicates if a new security context can be entered (default 
     * <code>true</code>). Applications may return <code>false</code> and 
     * prevent untrusted code to increase their privileges. Usually, 
     * such security setting should also prevent reconfiguring of the 
     * {@link #DEFAULT default} context by making {@link #DEFAULT} not 
     * {@link #isModifiable modifiable}.
     * 
     * @return <code>true</code> if a new security context can be entered;
     *         <code>false</code> otherwise.
     */
    public boolean isReplaceable () {
        return true;
    }    

    /**
     * Indicates if this security context allows modification of the 
     * {@link javolution.lang.Configurable.Logic configuration} settings 
     * (default <code>true</code>). Applications may override this method
     * to return <code>false</code> and prevent untrusted code to update the 
     * some/all configuration parameters. 
     * 
     * @param  cfg the configurable to check if modifiable.
     * @return <code>true</code> if the specified configurable can be modified;
     *         <code>false</code> otherwise.
     */
    public boolean isModifiable (Configurable cfg) {
        return true;
    }
        
    /**
     * Default implementation. 
     */
    private static class Default extends SecurityContext {
    }
    
    // Allows instances of private classes to be factory produced. 
    static {
        ObjectFactory.setInstance(new ObjectFactory() {
            protected Object create() {
                return new Default();
            } }, _Default.getClass());
    }
}

