package com.rho.rhoelements.npapi;

import com.rho.rhoelements.activex.ActiveXFactory;

public class DeleteHuddle extends Huddle
{
	private String ObjectID;
	private String ObjectType;
	private HuddleParser myParser;

	DeleteHuddle(HuddleParser parser) throws Exception
	{
		// get ActiveX Type
		myParser = parser;

		myParser.getNextChunk();
		if (myParser.currentChunk.type != ACTIVEX_TYPE)
		{
			throw new Exception("Expecting ACTIVEX_TYPE message type, found " + myParser.currentChunk.type);
		}
		ObjectType = myParser.currentChunk.payload;

		myParser.getNextChunk();
		if (myParser.currentChunk.type != ACTIVEX_ID)
		{
			throw new Exception("Expecting ACTIVEX_ID message type, found " + myParser.currentChunk.type);
		}
		ObjectID = myParser.currentChunk.payload;
	}

	@Override
	public String[] execute()
	{
		ActiveXFactory.deleteObjectByID(ObjectID);
		return null;
	}
}
