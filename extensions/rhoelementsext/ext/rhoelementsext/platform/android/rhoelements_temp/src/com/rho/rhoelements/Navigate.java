package com.rho.rhoelements;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;


/**
 * @author Geoff Day (XFH386, Initial Creation)
 *
 */
public class Navigate
{
	/*******************************************************************************/
	// Embed values into format string.
	// Each %s in format is replaced with the next value in turn.
	// %n is replaced by the nth value (n is 1 to 9)
	public static String embed(String format, String[] names, String[] values) throws NavigateException
	{
		String result = "";
		int index = 0, state = 0, new_index = 0;
		boolean char_done, json_done = false;

		// Return empty string format is null
		if (format == null)
			return "";

		// Treat null values as empty array
		if (values == null)
			values = new String[] {};

		for (int n = 0; n < format.length(); n++)
		{
			char c = format.charAt(n);
			char_done = false;

			while (!char_done)
			{
				switch (state)
				{
				case 0:
					// Normal processing, waiting for %
					if (c == '%')
						state = 1;
					else
						result += c;
					
					char_done = true;
					break;

				case 1:
					// Got % last time
					if (c == '%')
					{
						result += '%';
						state = 0;
					}
					else if (c == 's' || c == 'S')
					{
						if (index < values.length)
							result += values[index++];
						else
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Out of values processing %s"));

						state = 0;
					}
					else if ((c == 'j' || c == 'J') && (!json_done) &&
							((format.length() >= n+3) && (format.substring(n, n+4).compareToIgnoreCase("json") == 0)))
					{
						// Treat null names as empty array
						if (names == null)
							names = new String[] {};

						int iNvpLength = names.length;

						if (iNvpLength == 0)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "JSON names array is zero length - no JSON output"));
						}
						else if ( iNvpLength != values.length )
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "JSON names and values arrays different lengths"));
							iNvpLength = 0;
						}
						JSONObject j = new JSONObject();
						for (int idx = 0; idx < iNvpLength; idx++)
						{
							if (names[idx] != null)
							{
								try
								{
									if ((values == null) || (values[idx] == null))
									{
										j.put(names[idx], JSONObject.NULL);
									}
									else if ((values[idx].startsWith("[")) && (values[idx].endsWith("]")))
									{										
										j.put(names[idx], new JSONArray(values[idx]));
									}
									else
									{
										j.put(names[idx], values[idx]);
									}
								}
								catch (JSONException e)
								{									
									throw new NavigateException(values[idx].length(), "JSONException: " + e.toString());									
								}
							}
							else
							{
								Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "null Name Specified - skipping"));
							}
						}
						result += j.toString();
						state = 0;
						n += 3;
						json_done = true;
						char_done = true;
					}
					else if (Character.isDigit(c))
					{
						// Start building up index
						new_index = Character.digit(c, 10);
						state = 2;
					}
					else
					{
						result += '%';
						result += c;
						state = 0;
					}

					char_done = true;
					break;

				case 2:
					// Building up new index value
					if (Character.isDigit(c))
					{
						// Add digit to index
						new_index = (new_index * 10) + Character.digit(c, 10);
						char_done = true;
					}
					else
					{
						if (new_index <= values.length)
						{
							result += values[new_index - 1];
							index = new_index;
						}
						else
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value out of range %" + new_index));

						// Process this non-digit character through the state machine
						char_done = false;

						state = 0;
					}
					break;
				}
			}
		}

		// Handle end of string according to what state we're in
		switch (state)
		{
		case 0:
			// Nothing further to do
			break;

		case 1:
			// Got a % last time so just add it to the result
			result += '%';
			break;

		case 2:
			// We were building a new index, so assume that's done now and add value
			if (new_index <= values.length)
				result += values[new_index - 1];
			else
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value out of range %" + new_index));
			break;
		}

		if (result.length() <= Common.MAX_URL)
		{
			return result;
		}
		throw new NavigateException(result.length(), "MAX_URL Exceeded");
	}
}
