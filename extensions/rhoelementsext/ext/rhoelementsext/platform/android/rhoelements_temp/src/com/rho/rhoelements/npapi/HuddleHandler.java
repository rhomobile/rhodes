package com.rho.rhoelements.npapi;

import java.io.StringWriter;
import java.text.DecimalFormat;
import java.text.Format;

import android.text.format.Formatter;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;

public class HuddleHandler
{
	private Huddle currentHuddle;
	private HuddleParser myParser;

	private String constructReply(String[] result) throws Exception
	{
		String reply = new String();

		StringWriter writer = new StringWriter();

		DecimalFormat typeFormatter = new DecimalFormat("00");
		DecimalFormat sizeFormatter = new DecimalFormat("000000");

		writer.write(typeFormatter.format(Huddle.SOT) + sizeFormatter.format(0));
		writer.write(typeFormatter.format(Huddle.HUDDLE_TYPE) + sizeFormatter.format(6) + "RETURN");

		for (int i = 0; i < result.length; i++)
		{
			writer.write(typeFormatter.format(Huddle.ARG) + sizeFormatter.format(result[i].length()) + result[i]);

		}
		writer.write(typeFormatter.format(Huddle.EOA) + sizeFormatter.format(0));

		reply = writer.toString();
		return reply;
	}

	private void getStart() throws Exception
	{
		myParser.getNextChunk();
		if (myParser.currentChunk.type != Huddle.SOT)
		{
			throw new Exception("received huddle does not contain EOT, rejecting message");
		}
	}

	private String getHuddleType() throws Exception
	{

		myParser.getNextChunk();
		if (myParser.currentChunk.type != Huddle.HUDDLE_TYPE)
		{
			throw new Exception("received huddle does not contain HUDDLE_TYPE chunk, rejecting message");

		}

		return myParser.currentChunk.payload;
	}

	public String handle(String message)
	{
		currentHuddle = null;
		String[] result = null;
		try
		{

			myParser = new HuddleParser(message);
			getStart();
			String HuddleType = getHuddleType();

			if (HuddleType.equalsIgnoreCase("JSOBJECT"))
			{
				currentHuddle = new JSObjectHuddle(myParser);
			}
			else if (HuddleType.equalsIgnoreCase("ACTIVEX"))
			{
				currentHuddle = new ActiveXHuddle(myParser);
			}
			else if (HuddleType.equalsIgnoreCase("NEW"))
			{
				currentHuddle = new NewHuddle(myParser);
			}
			else if (HuddleType.equalsIgnoreCase("DELETE"))
			{
				currentHuddle = new DeleteHuddle(myParser);
			}

			result = currentHuddle.execute();
			if (result != null)
			{
				String reply = constructReply(result);
				return reply;
			}
		}
		catch (Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
		}

		return null;
	}
}