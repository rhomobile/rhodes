/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.lang;

import javolution.context.SecurityContext;
//import javolution.text.TextFormat;

/**
 *  <p> This class facilitates separation of concerns between the configuration
 *      logic and the application code.</p>
 *  
 *  <p> Does your class need to know or has to assume that the configuration is
 *      coming from system properties ??</p>
 *      
 *  <p> The response is obviously NO!</p>
 *  
 *  <p> Let's compare the following examples:[code]
 *      class Document {
 *          private static final Font DEFAULT_FONT
 *              = Font.decode(System.getProperty("DEFAULT_FONT") != null ? System.getProperty("DEFAULT_FONT") : "Arial-BOLD-18");
 *          ...
 *      }[/code]
 *      With the following (using this class):[code]
 *      class Document {
 *          public static final Configurable<Font> DEFAULT_FONT = new Configurable<Font>(new Font("Arial", Font.BOLD, 18));
 *          ...
 *      }[/code]
 *      Not only the second example is cleaner, but the actual configuration 
 *      data can come from anywhere (even remotely). Low level code does not 
 *      need to know.</p>
 * 
 *  <p> Furthermore, with the second example the configurable data is 
 *      automatically documented in the JavaDoc (public). Still only instances 
 *      of {@link Logic} may set this data. There is no chance
 *      for the user to modify the configuration by accident.</p>
 *  
 * <p>  Configurable instances have the same textual representation as their 
 *      current values. For example:[code]
 *       public static final Configurable<String> AIRPORT_TABLE
 *            = new Configurable<String>("Airports");
 *       ...
 *       String sql = "SELECT * FROM " + AIRPORT_TABLE // AIRPORT_TABLE.get() is superfluous 
 *           + " WHERE State = '" + state  + "'";[/code]
 *      </p>
 *  
 *  <p> Unlike system properties (or any static mapping), configuration 
 *      parameters may not be known until run-time or may change dynamically.
 *      They may depend upon the current run-time platform, 
 *      the number of cpus, etc. Configuration parameters may also be retrieved
 *      from external resources such as databases, XML files, 
 *      external servers, system properties, etc.[code]
 *      public abstract class FastComparator<T> implements Comparator<T>, Serializable  {     
 *          public static final Configurable<Boolean> REHASH_SYSTEM_HASHCODE 
 *              = new Configurable<Boolean>(isPoorSystemHash()); // Test system hashcode. 
 *      ...
 *      public abstract class ConcurrentContext extends Context {
 *          public static final Configurable<Integer> MAXIMUM_CONCURRENCY 
 *              = new Configurable<Integer>(Runtime.getRuntime().availableProcessors() - 1);
 *                  // No algorithm parallelization on single-processor machines.
 *     ...
 *     public abstract class XMLInputFactory {    
 *          public static final Configurable<Class<? extends XMLInputFactory>> DEFAULT 
 *              = new Configurable<Class<? extends XMLInputFactory>>(XMLInputFactory.Default.class);
 *                  // Default class implementation is a private class. 
 *     ...
 *     [/code]</p>
 *      
 *  <p> Reconfiguration is allowed at run-time as configurable can be 
 *      {@link Configurable#notifyChange() notified} of changes in their
 *      configuration values. Unlike system properties, configurable can be 
 *      used in applets or unsigned webstart applications.</p>
 *      
 *  <p> Here is an example of configuration of a web application from 
 *      a property file:[code]
 *      public class Configuration extends Configurable.Logic implements ServletContextListener {
 *          public void contextInitialized(ServletContextEvent sce) {
 *              try {
 *                  ServletContext ctx = sce.getServletContext();
 *               
 *                  // Loads properties.
 *                  Properties properties = new Properties();
 *                  properties.load(ctx.getResourceAsStream("WEB-INF/config/configuration.properties"));
 *               
 *                  // Reads properties superceeding default values.
 *                  Configurable.read(properties);
 *                  
 *              } catch (Exception ex) {
 *                  LogContext.error(ex);
 *              }
 *          }
 *      }[/code]
 *      This listener is registered in the <code>web.xml</code> file:[code]
 *      <web-app>
 *          <listener>
 *              <listener-class>mypackage.Configuration</listener-class>
 *           </listener>
 *      </web-app>[/code]
 *      The property file contains the full names of the configurables static
 *      fields and the textual representation of their new values:[code]
 *      # File configuration.properties
 *      javolution.util.FastComparator#REHASH_SYSTEM_HASHCODE = true
 *      javolution.context.ConcurrentContext#MAXIMUM_CONCURRENCY = 0
 *      javolution.xml.stream.XMLInputFactory#DEFAULT = com.foo.bar.XMLInputFactoryImpl
 *      [/code]</p>
 *      
 * <p> Configuration settings are global (affect all threads). For thread-local
 *     environment settings {@link javolution.context.LocalContext.Reference 
 *     LocalContext.Reference} instances are recommended.</p>   
 *     
 * <p> <i>Note:</i> Any type for which a text format is 
 *    {@link TextFormat#getInstance(Class) known} can be configured from 
 *    <code>String</code> properties.</p>
 *       
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.1, July 4, 2007
 */
public class Configurable/*<T>*/{

    /**
     * Holds the current value. 
     */
    private Object/*{T}*/_value;

    /**
     * Default constructor. 
     */
    public Configurable(Object/*{T}*/defaultValue) {
        _value = defaultValue;
    }

    /**
     * Returns the current value for this configurable.
     * 
     *  @return the current value.
     */
    public final Object/*{T}*/get() {
        return _value;
    }

    /**
     * Returns the string representation of the value of this configurable.
     * 
     * @return <code>String.valueOf(this.get())</code>
     */
    public String toString() {
        return String.valueOf(_value);
    }

    /**
     * Convenience method to read the specified properties (key/value mapping) 
     * and reconfigures accordingly. The configurables are identified by their 
     * field names (e.g. <code>
     * "javolution.context.ConcurrentContext#MAXIMUM_CONCURRENCY"</code>).
     * Conversion of <code>String</code> values is performed   
     * using {@link javolution.text.TextFormat#getInstance(Class)}.   
     *@JVM-1.1+@
     public static void read(j2me.util.Map properties) {
     j2me.util.Iterator i = properties.entrySet().iterator();
     while (i.hasNext()) {
     j2me.util.Map.Entry entry = (j2me.util.Map.Entry) i.next();
     String key = String.valueOf(entry.getKey());
     Object value = entry.getValue();
     try {
     int sep = key.indexOf('#');
     if (sep < 0) // Not a configurable property.
     continue; 
     
     // Found a configurable property being superseded.
     javolution.context.LogContext.info("Configure " + key + " to " + value);      
     String className = key.substring(0, sep);
     String fieldName = key.substring(sep + 1);

     Class cls = Reflection.getClass(className);     
     Configurable cfg = (Configurable) cls.getDeclaredField(
     fieldName).get(null);
     Object previous = cfg.get();
     if ((previous == null) || !(value instanceof String)) {
     // No automatic conversion, use value directly.
     LOGIC.configure(cfg, value);
     continue;
     }
     String str = (String) value;
     if (previous instanceof String) {
     LOGIC.configure(cfg, value);
     continue;
     }

     javolution.text.TextFormat format = javolution.text.TextFormat.getInstance(previous.getClass());
     if (format != null) {
     LOGIC.configure(cfg, format.parse(javolution.Javolution.j2meToCharSeq(str)));
     continue;
     }     
     
     javolution.context.LogContext.warning(javolution.text.Text.valueOf(
     "No text format found for type "
     + previous.getClass() + " (" + key + "), please register the text format" + 
       " using TextFormat.setInstance(Class, TextFormat) static method"));

     } catch (Exception ex) {
     javolution.context.LogContext.warning(javolution.text.Text.valueOf("Cannot set property " + key
     + "(" + ex.toString() + ")"));
     }
     }
     }
     /**/

    static final Logic LOGIC = new Logic() {
    }; // To access configuration setting. 

    
    /**
     * Notifies this configurable that its runtime value has been changed.
     * The default implementation does nothing. 
     */
    protected void notifyChange() {
        // Does nothing.
    }

    /**
     * This class represents a configuration logic capable of setting  
     * {@link Configurable} values. For example:[code]
     * class MyApplication {
     *     private static final Configuration CONFIGURATION = new Configuration();
     *     public static void main(String[] args) {
     *         CONFIGURATION.run();
     *         ...       
     *     }
     *     static class Configuration extends Configurable.Logic implements Runnable {
     *         public void run() {
     *             Properties properties = System.getProperties();
     *             // Properties could be loaded automatically if the property names 
     *             // were the full names of the configurable fields (ref. Configurable.read(Map)).
     *             String concurrency = properties.get("MAXIMUM_CONCURRENCY");
     *             if (concurrency != null) { 
     *                 configure(ConcurrentContext.MAXIMUM_CONCURRENCY, TypeFormat.parseInt(concurrency));
     *             }                
     *             ...
     *         }
     *    }
     * }[/code]
     * Applications can prevent configuration modifications through 
     * {@link SecurityContext}. For example:[code]
     *  public class MyPolicy extends SecurityContext {
     *       public boolean isModifiable (Configurable cfg) {
     *           return false;
     *       }
     *  }
     *  ...
     *  configure(SecurityContext.DEFAULT, MyPolicy.class); // Global setting.
     *  [/code]
     */
    public static abstract class Logic {

        /**
         * Sets the run-time value of the specified configurable. If 
         * configurable value is different from the previous one, then  
         * {@link Configurable#notifyChange()} is called. This method 
         * raises <code>SecurityException</code> if the specified 
         * configurable is not {@link SecurityContext#isModifiable(Configurable)
         * modifiable}.
         * 
         * @param  cfg the configurable being configurated.
         * @param  value the new run-time value.
         * @throws SecurityException if the specified configurable cannot 
         *         be modified.
         */
        protected final/*<T>*/void configure(Configurable/*<T>*/cfg,
                Object/*{T}*/value) {
            SecurityContext policy = (SecurityContext) SecurityContext
                    .getCurrent();
            if (!policy.isModifiable(cfg))
                throw new SecurityException(
                        "Configurable modification disallowed by SecurityContext");
            Object previous = cfg._value;
            cfg._value = value;
            boolean change = (value == null) ? previous != null : !value
                    .equals(previous);
            if (change) {
                cfg.notifyChange();
            }
        }
    }

}