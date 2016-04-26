package com.rho.rhoelements;

public class NavigateException extends Exception
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 2181979200188072197L;
	private int length;

	public NavigateException(int len, String str)
	{
		super(str);
		length = len;
	}

	public int GetLength()
	{
		return length;
	}
}
