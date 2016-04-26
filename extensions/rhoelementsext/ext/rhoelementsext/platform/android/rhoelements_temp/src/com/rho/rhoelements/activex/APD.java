package com.rho.rhoelements.activex;

import com.rho.rhoelements.apd.ApdEngine;

public class APD extends ActiveX
{
	static private APD myInstance = new APD();
	static private ApdEngine myApdEngine = new ApdEngine();

	private APD()
	{
	};

	static public synchronized APD getInstance()
	{
		// decide here if this class should be a singleton
		return myInstance;
	}

	@Override
	protected Object clone() throws CloneNotSupportedException
	{
		throw new CloneNotSupportedException("Clone is not allowed.");
	}

	//args[0 ] - int command, args[1] - String params
	public String[] psexternal(String[] args) throws Exception
	{
		if (args.length != 2)
		{
			// TODO: change to return/error code since this has to go through NPAPI back to user
			throw new Exception("Invalid number of parameters to APD.psexternal. Expecting 2,  recieved " + args.length);
		}
		myApdEngine.PSExternal(Integer.parseInt(args[0]), args[1]);
		return null;
	}

	//args[0 ] - int command, args[1] - String params
	public String[] psexternalex(String[] args) throws Exception
	{
		if (args.length != 2)
		{
			// TODO: change to return/error code since this has to go through NPAPI back to user
			throw new Exception("Invalid number of parameters to APD.psexternal. Expecting 2,  recieved " + args.length);
		}
		Integer temp = new Integer(myApdEngine.PSExternalEx(Integer.parseInt(args[0]), args[1]));

		String[] result = new String[1];
		result[0] = temp.toString();

		return result;
	}

	//no args
	public String[] psgetlastmessage(String[] args) throws Exception
	{
		if (args.length != 0)
		{
			// TODO: change to return/error code since this has to go through NPAPI back to user
			throw new Exception("Invalid number of parameters to APD.psexternal. Expecting 0,  recieved " + args.length);
		}

		String[] result = new String[1];
		result[0] = myApdEngine.PSGetLastMessage();

		return result;
	}
}
