package com.rho.rhoelements.npapi;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.activex.ActiveX;
import com.rho.rhoelements.activex.ActiveXFactory;

public class ActiveXHuddle extends Huddle
{

	private String ObjectID;
	private String MethodName;
	private ArrayList<String> Args;
	private HuddleParser myParser;

	ActiveXHuddle(HuddleParser parser) throws Exception
	{
		// get ActiveX ID
		myParser = parser;

		myParser.getNextChunk();

		if (myParser.currentChunk.type != ACTIVEX_ID)
		{
			throw new Exception("Expecting ACTIVEXID message type, found " + myParser.currentChunk.type);
		}

		ObjectID = myParser.currentChunk.payload;

		myParser.getNextChunk();

		if (myParser.currentChunk.type != METHOD)
		{
			throw new Exception("Expecting METHOD message type, found " + myParser.currentChunk.type);
		}

		MethodName = myParser.currentChunk.payload;

		// get arguments
		Args = new ArrayList<String>();

		// repeat read until you get to EOA message
		while (true)
		{
			myParser.getNextChunk();
			if (myParser.currentChunk.type == EOA)
			{
				break;
			}
			else if (myParser.currentChunk.type != ARG)
			{
				throw new Exception("Expecting EOA or ARG message type, found " + myParser.currentChunk.type);
			}

			Args.add(myParser.currentChunk.payload);

		}
	}

	@Override
	public String[] execute()
	{
		String[] result = null;
		try
		{
			ActiveX temp = ActiveXFactory.getObjectByID(ObjectID);
			result = temp.Execute(MethodName, (String[]) Args.toArray(new String[0]));
		}
		catch (Exception E)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "error :  " + E.getMessage()));
		}

		return result;
	}
}
