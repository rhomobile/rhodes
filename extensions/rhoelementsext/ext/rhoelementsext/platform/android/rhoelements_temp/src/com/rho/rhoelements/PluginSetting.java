package com.rho.rhoelements;

/**
 * A container object 
 * @author Geoff Day
 * @author Ben Kennedy
 */
public class PluginSetting
{
	private String name;
	private String value;
	private String module;

	public PluginSetting (String module, String name, String value)
	{
		this.name = name;
		this.value = value;
		this.module = module;
	}

	public String getName ()
	{
		return (name == null ? "" : name);
	}

	public String getValue ()
	{
		return (value == null ? "" : value);
	}

	public String getModule ()
	{
		return (module == null ? "" : module);
	}

	@Override
	public boolean equals(Object subject)
	{
		if(subject instanceof PluginSetting)
		{
			PluginSetting subjectA = (PluginSetting) subject;
			if(subjectA.getName().equals(name) && subjectA.getValue().equals(value) && subjectA.getModule().equals(module))
			{
				return true;
			}
		}
		return false;
	}

	@Override
	public int hashCode()
	{
		return (name.hashCode() / 3) + (value.hashCode() / 3) + (module.hashCode() / 3);
	}

	@Override
	public String toString()
	{
		return "{"+ module + "," + name + "," + value + "}";
	}
}
