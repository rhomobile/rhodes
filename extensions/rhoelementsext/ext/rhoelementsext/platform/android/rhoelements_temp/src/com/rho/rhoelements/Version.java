package com.rho.rhoelements;

public class Version
{
	protected String moduleName;
	protected int major;
	protected int minor;
	protected int maintenanceRelease;
	protected int releaseCandidate;
	protected int hotfix;
	protected String buildTag;

	public String getModuleName()
	{
		return moduleName;
	}
	
	public int getHotfix()
	{
		return hotfix;
	}

	public String getBuildTag()
	{
		return buildTag;
	}

	/**
	 * Formats version string as major.minor.maintenance.release
	 * 
	 * @return Formatted version string
	 */
	public String getString()
	{
		return String.format("%d.%d.%d.%d", major, minor, maintenanceRelease, releaseCandidate);
	}

	/**
	 * Formats version string as major.minor.maintenance.release.hotfix
	 * 
	 * @return Formatted version string
	 */
	public String getStringWithHotfix()
	{
		return String.format("%d.%d.%d.%d.%d", major, minor, maintenanceRelease, releaseCandidate, hotfix);
	}

	/**
	 * Constructor taking just the module name; version numbers and build tag are set to defaults.
	 * 
	 * @param module Module name
	 */
	public Version(String module)
	{
		moduleName = module;
		major = BuildInfo.getMajor();
		minor = BuildInfo.getMinor();
		maintenanceRelease = BuildInfo.getMaintenanceRelease();
		releaseCandidate = BuildInfo.getReleaseCandidate();
		hotfix = BuildInfo.getHotfix();
		buildTag = BuildInfo.getBuildTime();
	}

	/**
	 * Constructor taking module name and build time; version numbers are set to defaults.
	 * 
	 * @param module Module name
	 * @param time Build time (as string)
	 */
	public Version(String module, String tag)
	{
		this(module);
		buildTag = tag;
	}
	
	@Override
	public String toString()
	{
		return getString();
	}
}
