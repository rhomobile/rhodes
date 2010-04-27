package com.rho;

import java.util.Vector;
//import java.util.Enumeration;

public class Properties {
    /** An appropriate initial size for storage vectors (10). */
    private static int INITIAL_SIZE = 10;

    private String[] initProps;
    private Vector keys;
    private Vector vals;
    
    public Properties() {
        keys = new Vector(INITIAL_SIZE);
        vals = new Vector(INITIAL_SIZE);
    }
    public Properties(String[] propertyStrings) {
        initProps = propertyStrings;
    }
    
    public void clear()
    {
    	keys.removeAllElements();
    	vals.removeAllElements();
    	
    	initProps = null;
    }
    
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

    public synchronized void addProperty(String key, 
                                         String value) {
        if (keys == null) {
            convertInitPropsToVectors();
        }

    	keys.addElement(key);
        vals.addElement(value);

        return;
    }
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

    public synchronized String setPropertyAt(int index, String value) {
        String rv;

        if (keys == null) {
            convertInitPropsToVectors();
        }

        rv = (String)vals.elementAt(index);

        vals.setElementAt(value, index);

        return rv;
    }
    
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
    
    
    public String getValueAt(int index) {
        if (keys == null) {
            return initProps[(index * 2) + 1];
        }

        return (String)vals.elementAt(index);
    }

    public String getKeyAt(int index) {
        if (keys == null) {
            return initProps[index * 2];
        }

        return (String)keys.elementAt(index);
    }

    public int size() {
        if (keys == null) {
            return initProps.length / 2;
        }

        return keys.size();
    }

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
