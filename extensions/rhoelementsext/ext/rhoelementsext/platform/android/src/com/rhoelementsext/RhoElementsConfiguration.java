package com.rhoelementsext;

import java.lang.Exception;

import com.rho.rhoelements.Config;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoConfig;

/**
 * 
 * @author FPC843
 * This class wraps the RhoElements Config class around the IRhoConfig interface in order to be used
 * by the RhoElements extension
 */
public class RhoElementsConfiguration implements IRhoConfig
{
    private static final String TAG = RhoElementsConfiguration.class.getSimpleName();
    private Config mConfig;
    private static RhoElementsConfiguration instance = null;
    
    private void RhoElementsConfiguration() {}
    
    /**
     * Returns the instance to this singleton class
     * @param config is the RhoElements configuration
     * @return the instance of this singleton class
     */
    public static RhoElementsConfiguration getInstance()
    {
	Logger.D(TAG, "RhoElementsConfiguration -- getInstance()");
	if (instance == null)
	    instance = new RhoElementsConfiguration();
	return instance;
    }
    
    /**
     * Initialises the object
     * @param config is the Config instance we want to wrap around IRhoConfig
     */
    public void setConfig(Config config) {mConfig = config;}
    
    /**
     * @see com.rhomobile.rhodes.extmanager.IRhoConfig
     */
    @Override
    public String getString(String name) {
        if (mConfig != null)
            return mConfig.getSetting(name);
        return null;
    }

    @Override
    public boolean isExist(String name) {
        return mConfig.getSetting(name) != null;
    }

    @Override
    public boolean getBool(String name) throws ValueNotFoundException {
        String str = getString(name);
        if (str == null) {
            throw new ValueNotFoundException(name);
        }
        return str.equals("1"); 
    }

    @Override
    public int getInt(String name) throws ValueNotFoundException {
        String str = getString(name);
        if (str == null) {
            throw new ValueNotFoundException(name);
        }
        try {
            return Integer.parseInt(str);
        }
        catch (Throwable e) {
            throw new ValueNotFoundException(name, e);
        }
    }

    @Override
    public double getDouble(String name) throws ValueNotFoundException {
        String str = getString(name);
        if (str == null) {
            throw new ValueNotFoundException(name);
        }
        try {
            return Double.parseDouble(str);
        }
        catch (Throwable e) {
            throw new ValueNotFoundException(name, e);
        }
    }

}