package com.rho.rhoelements.activex;

public class ODAX extends ActiveX
{
	private ODAX()
	{
	};

	static public ODAX getInstance()
	{
		// decide here if this class should be a singleton
		ODAX temp = new ODAX();
		return temp;
	}
}
