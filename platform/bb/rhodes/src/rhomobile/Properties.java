/*
 *   
 *
 * Copyright  1990-2008 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

package rhomobile;

import java.util.Vector;
import java.util.Enumeration;

/**
 * The <code>Properties</code> class represents a persistent set of
 * properties. Each key and its corresponding value in
 * the property list is a string.
 * <p>
 */
public class Properties {
    /** An appropriate initial size for storage vectors (10). */
    private static int INITIAL_SIZE = 10;

    /**
     * The initial properties as series of key-value pairs. This array will be
     * used until a property is added, set, or removed.
     * <p>
     * The Java installer adds properties, also when a system MIDlet is
     * run with arguments, the arguments are added to properties.
     * 99.99% of the time, when running a downloaded MIDlet properties are
     * not added the is no need to load the vectors.
     */
    private String[] initProps;

    /** A vector of property keys. */
    private Vector keys;
    /** A vector of property values. */
    private Vector vals;
    
    /**
     * Constructor - creates an empty property list.
     */
    public Properties() {
        keys = new Vector(INITIAL_SIZE);
        vals = new Vector(INITIAL_SIZE);
    }

    /**
     * Constructor - creates an populated property list.
     *
     * @param propertyStrings an array of Strings 2 per property, will be
     * filled in with a pair pattern of key and value, this exact array
     * will be used for property lookup (the array will not be changed).
     * No other allocation will be done until the array is changed and at
     * that time the initial property array will be released.
     */
    public Properties(String[] propertyStrings) {
        initProps = propertyStrings;
    }

    /**
     * Convert the init property String array to vectors for changing
     * properties.
     */
    private void convertInitPropsToVectors() {
        int numberOfProps;

        numberOfProps = initProps.length / 2;

        keys = new Vector(numberOfProps + INITIAL_SIZE);
        vals = new Vector(numberOfProps + INITIAL_SIZE);

        for (int i = 0; i < initProps.length; i += 2) {
            keys.addElement(initProps[i]);
            vals.addElement(initProps[i + 1]);
        }

        // The initial properties are no longer needed.
        initProps = null;
    }

    /**
     * Store multiple key:value pair.  Provided for parallelism with the 
     * <tt>getProperty</tt> method. Enforces use of strings for 
     * property keys and values. 
     *
     * @param key the key to be placed into this property list.
     * @param value the value corresponding to <tt>key</tt>.
     * @see #getProperty
     */
    public synchronized void addProperty(String key, 
                                         String value) {
        if (keys == null) {
            convertInitPropsToVectors();
        }

    	keys.addElement(key);
        vals.addElement(value);

        return;
    }

    /**
     * Store a single key:value pair.  Provided for parallelism with the 
     * <tt>getProperty</tt> method. Enforces use of strings for 
     * property keys and values.  If a key already exists in storage,
     * the value corresponding to that key will be replaced and returned.
     *
     * @param key the key to be placed into this property list.
     * @param value the value corresponding to <tt>key</tt>.
     * @return if the new property value replaces an existing one, the old 
     * value is returned.  otherwise, null is returned.
     * @see #getProperty
     * @see #removeProperty
     */
    public synchronized String setProperty(String key, String value) {
        int idx;
        String rv = null;

        if (keys == null) {
            convertInitPropsToVectors();
        }

        idx = keys.indexOf(key);

        if (idx == -1) {    // If I don't have this, add it and return null
            keys.addElement(key);
            vals.addElement(value);    
        } else {	    // Else replace it and return the old one.
            rv = (String)vals.elementAt(idx);
            vals.setElementAt(value, idx);
        }

        return rv;
    }

    /**
     * Store a single key:value pair. 
     * 
     * This method is very similar to setproperty() except with a difference 
     * that the keys are stored in case-insensitive manner. This is needed in 
     * certain situations like HTTP header names.
     *
     *
     * @param key the key to be placed into this property list.
     * @param value the value corresponding to <tt>key</tt>.
     * @return if the new property value replaces an existing one, the old 
     * value is returned.  otherwise, null is returned.
     * @see #getProperty
     * @see #removeProperty
     */
    public synchronized String setPropertyIgnoreCase(String key, String value) {
        int idx = -1;
        String rv = null;

        if (keys == null) {
            convertInitPropsToVectors();
        }

        for (int count = 0; count < keys.size(); count++) {
            if (((String)keys.elementAt(count)).equalsIgnoreCase(key)) {
                idx = count;
            }
        }

        if (idx == -1) {    // If I don't have this, add it and return null
            keys.addElement(key);
            vals.addElement(value);    
        } else {	    // Else replace it and return the old one.
            rv = (String)vals.elementAt(idx);
            vals.setElementAt(value, idx);
            /*
             * Also replace the key at idx in case there is change in case
             * for the key
             */
            keys.setElementAt(key, idx);
        }

        return rv;
    }

    /**
     * Replace the value of the property at the given index.
     *
     * @param index 0 based index of a property
     * @param value the new value for the property at <tt>index</tt>.
     *
     * @return previous value
     *
     * @exception IndexOutOfBoundsException if the index is out of bounds
     */
    public synchronized String setPropertyAt(int index, String value) {
        String rv;

        if (keys == null) {
            convertInitPropsToVectors();
        }

        rv = (String)vals.elementAt(index);

        vals.setElementAt(value, index);

        return rv;
    }

    /**
     * Searches for the property with the specified key in this property list.
     * The method returns <code>null</code> if the property is not found.
     *
     * @param   key   the property key.
     * @return  the value in this property list with the specified key value.
     * @exception NullPointerException is thrown if key is <code>null</code>.
     * @see     #setProperty
     * @see     #removeProperty
     */
    public String getProperty(String key) {
        String rv = null;

        if (key == null) {
            throw new NullPointerException();
        }

        if (keys == null) {
            for (int i = 0; i < initProps.length; i += 2) {
                if (initProps[i].equals(key)) {
                    rv = initProps[i + 1];
                    break;
                }
            }
        } else {
            int idx = keys.indexOf(key);
            if (idx > -1) {
                rv = (String)vals.elementAt(idx);
            }
        }

    	return rv;
    }

    /**
     * This method is very similar to getproperty() except with a difference 
     * that the keys are searched in case-insensitive manner. This is needed in 
     * certain situations like HTTP header names.
     *
     * @param   key   the property key.
     * @return  the value in this property list with the specified key value.
     * @exception NullPointerException is thrown if key is <code>null</code>.
     * @see     #setProperty
     * @see     #removeProperty
     */
    public String getPropertyIgnoreCase(String key) {
        String rv = null;

        if (key == null) {
            throw new NullPointerException();
        }

        if (keys == null) {
            for (int i = 0; i < initProps.length; i += 2) {
                if (initProps[i].equalsIgnoreCase(key)) {
                    rv = initProps[i + 1];
                    break;
                }
            }
        } else {
            int idx = -1;
            for (int count = 0; count < keys.size(); count++) {
                if (((String)keys.elementAt(count)).equalsIgnoreCase(key)) {
                    idx = count;
                }
            }
            if (idx > -1) {
                rv = (String)vals.elementAt(idx);
            }
        }

    	return rv;
    }
    
    
    /**
     * Gets a property value by index. Used by the JadWriter as part of the
     * JAD Tool.
     *
     * @param index 0 based index of a property
     * @return  the value of the property with the specified index.
     * @exception ArrayIndexOutOfBoundsException
     *     if an invalid index was given.
     */
    public String getValueAt(int index) {
        if (keys == null) {
            return initProps[(index * 2) + 1];
        }

        return (String)vals.elementAt(index);
    }

    /**
     * Gets a property key by index. Used by the JadWriter as part of the
     * JAD Tool.
     *
     * @param index 0 based index of a property
     * @return  the key of the property with the specified index.
     * @exception ArrayIndexOutOfBoundsException
     *     if an invalid index was given.
     */
    public String getKeyAt(int index) {
        if (keys == null) {
            return initProps[index * 2];
        }

        return (String)keys.elementAt(index);
    }

    /**
     * Gets the number of properties.
     *
     * @return  number of properties
     */
    public int size() {
        if (keys == null) {
            return initProps.length / 2;
        }

        return keys.size();
    }

    /**
     * Removes a property (key:value pair) from the property
     * list based on the key string.
     *
     * @param key the key to be removed from the property list. 
     * @return  the element associated with the key.
     * @see #setProperty
     * @see #getProperty
     */
    public synchronized String removeProperty(String key) {
	int idx;
	String rv = null;

        if (keys == null) {
            convertInitPropsToVectors();
        }

	idx = keys.indexOf(key);
	if (idx > -1) {
	    rv = (String)vals.elementAt(idx);
	    keys.removeElementAt(idx);
	    vals.removeElementAt(idx);
	}

	return rv;
    }

}
