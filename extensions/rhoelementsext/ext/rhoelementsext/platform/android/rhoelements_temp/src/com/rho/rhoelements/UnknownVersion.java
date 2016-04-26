package com.rho.rhoelements;

public class UnknownVersion extends Version
{
	public UnknownVersion(String module)
	{
		super(module);

		// Set the version numbers to zero
		major = 0;
		minor = 0;
		maintenanceRelease = 0;
		releaseCandidate = 0;
		hotfix = 0;

		buildTag = "Unknown";
	}
}
