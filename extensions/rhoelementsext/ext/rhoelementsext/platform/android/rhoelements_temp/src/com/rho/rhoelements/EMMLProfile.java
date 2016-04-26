package com.rho.rhoelements;

import java.util.ArrayList;

/**
 * A container class to hold information on a single EMML profile.
 * Contains:
 * the name of the profile page that contained the profile,
 * the profile's name,
 * and the settings that it includes.
 * @author Ben Kennedy
 */
public class EMMLProfile
{
	private String profileName;
	private String pageName;
	private ArrayList<PluginSetting> settings;

	/**
	 * Default Constructor
	 */
	public EMMLProfile()
	{
		settings = new ArrayList<PluginSetting>();
	}

	/**
	 * @param pageName the name of the profile page
	 * @param profileName the name of the profile
	 */
	public EMMLProfile(String pageName, String profileName)
	{
		this();
		this.pageName = pageName;
		this.profileName = profileName;
	}

	/**
	 * Sets the profiles name
	 * @param name the name to give the profile
	 */
	public void setProfileName(String profileName)
	{
		this.profileName = profileName;
	}

	/**
	 * @param setting adds the PluginSetting to the Profile
	 */
	public void addSetting(PluginSetting setting)
	{
		settings.add(setting);
	}

	/**
	 * @param setting adds the PluginSetting to the Profile
	 */
	public void addSettings(ArrayList<PluginSetting> setting)
	{
		settings.addAll(setting);
	}

	/**
	 * @return the ArrayList of settings
	 */
	public ArrayList<PluginSetting> getSettings()
	{
		return settings;
	}

	@Override
	public boolean equals(Object o)
	{
		return super.equals(o);
	}

	@Override
	public int hashCode()
	{
		return super.hashCode();
	}

	@Override
	public String toString()
	{
		return super.toString();
	}

	/**
	 * @return the name of the profile to which this profile belongs
	 */
	public String getPageName()
	{
		return pageName;
	}

	/**
	 * @param pageName the pageName to set
	 */
	public void setPageName(String pageName)
	{
		this.pageName = pageName;
	}

	/**
	 * @return the name of this profile
	 */
	public String getProfileName()
	{
		return profileName;
	}
}
