package com.rho.rhoelements.npapi;

import com.rho.rhoelements.Common;

public class JSObjectHuddle extends Huddle
{

	private String EmmlString;
	private String JSObject;
	private HuddleParser myParser;

	JSObjectHuddle(HuddleParser parser) throws Exception
	{
		myParser = parser;
		myParser.getNextChunk();

		if (myParser.currentChunk.type != JS_OBJECT_NAME)
		{
			throw new Exception("not a valid huddle, missing JS_OBJECT_NAME. Found : " + myParser.currentChunk.type);
		}

		JSObject = myParser.currentChunk.payload;
		myParser.getNextChunk();

		if (myParser.currentChunk.type != EMML)
		{
			throw new Exception("not a valid huddle, missing EMML. Found : " + myParser.currentChunk.type);
		}

		EmmlString = myParser.currentChunk.payload;
	}

	@Override
	public String[] execute()
	{
		Common.metaReceiver.setMeta(JSObject, EmmlString);
		return null;
	}
}
