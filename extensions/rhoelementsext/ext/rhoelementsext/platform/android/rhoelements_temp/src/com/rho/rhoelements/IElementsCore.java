package com.rho.rhoelements;

/**
 * @author Geoff Day (XFH386, Initial Creation)
 *
 */
public interface IElementsCore
{
	public void navigate (String url);
	public String getCurrentUrl();
	public Version getVersion();
}
