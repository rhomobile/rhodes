package com.rho.rhoelements.npapi;

public class HuddleParser
{
	public String message;

	public class Chunk
	{
		int type;
		String payload;
	}

	public Chunk currentChunk;

	HuddleParser(String inputMsg)
	{
		message = inputMsg;
	}

	public Chunk getNextChunk()
	{
		currentChunk = new Chunk();

		String typeString = message.substring(0, Huddle.TYPE_SIZE);

		currentChunk.type = Integer.parseInt(typeString);

		message = message.substring(Huddle.TYPE_SIZE);

		String lengthString = message.substring(0, Huddle.LENGTH_SIZE);
		int payloadLength = Integer.parseInt(lengthString);
		message = message.substring(Huddle.LENGTH_SIZE);

		currentChunk.payload = message.substring(0, payloadLength);
		message = message.substring(payloadLength);

		return currentChunk;
	}
}
