package com.rho.rhoelements;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.nio.CharBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.util.ArrayList;
import java.util.NoSuchElementException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * A pure Java Implementation of the EMML 1.1 parser utilising Regular Expressions for backward compatibility to EMML 1.0.
 * To be implemented by the RhoElements core and used to parse meta tags and the result of which to be passed on to
 * the plug-ins.
 * //TODO maybe change to hashmaps
 * //TODO test for escape characters in input
 * @author Ben Kennedy
 */
public class EMML
{
	private static EMML emml;
	/**
	 * Returns the singleton EMML object. Use instead of a constructor.
	 * @return the singleton EMML object
	 */
	public synchronized static EMML getEMML()
	{
		if(emml == null)
		{
			emml = new EMML();
		}
		return emml;
	}
	private Matcher	bracketMatcher;
	private Matcher commentMatcher;
	private ArrayList<Matcher> cTPatterns;
	private ArrayList<String> cTReplacers;
	private Pattern ctSplitter;
	private final String ESCAPE_CHAR = "\u0007\u0007"; //Used as a value that cannot come from inputs
	private ArrayList<Matcher> hEPatterns;
	private ArrayList<String> hEReplacers;
	private ArrayList<EMMLProfile> profileList;
	private Matcher profileSplitter;
	private Matcher urlMatcher;

	/**
	 * The constructor is private so noone can create an EMML object except the EMML class itself.
	 * This enforces singletonness
	 */
	private EMML()
	{
		init();
	}

	/**
	 * Adds a Regular Expression search and replace string 
	 * for the httpEquiv strings
	 * @param searchPattern the search pattern
	 * @param replacePattern the replace pattern
	 */
	public void addRegExPair_CT(String searchPattern, String replacePattern) 
	{
		cTPatterns.add(Pattern.compile(searchPattern, Pattern.CASE_INSENSITIVE).matcher(""));
		cTReplacers.add(replacePattern);
	}
	
	/**
	 * Adds a Regular Expression search and replace string 
	 * for the content strings
	 * @param searchPattern the search pattern
	 * @param replacePattern the replace pattern
	 */
	public void addRegExPair_HE(String searchPattern, String replacePattern) 
	{
		hEPatterns.add(Pattern.compile(searchPattern, Pattern.CASE_INSENSITIVE).matcher(""));
		hEReplacers.add(replacePattern);
	}

	/**
	 * Destroys all the profiling records
	 */
	public void clearProfiles()
	{
		profileList = new ArrayList<EMMLProfile>();
	}

	/**
	 * This is a singleton class so cloning is not wanted.
	 */
	public Object clone() throws CloneNotSupportedException
	{
		throw new CloneNotSupportedException(); 
	}

	/**
	 * Removes all of the profiles that are defined in the profile page that matches the input string 
	 * @param profileName the name of the profile page to be deleted.
	 * @throws NoSuchElementException if the profile name is not found
	 */
	public void deleteProfilePage(final String pageName) throws NoSuchElementException
	{
		if(pageName == null || profileList == null)
		{
			throw new NoSuchElementException(pageName);
		}
		boolean hasRemoved = false;
		for(int i = 0; i < profileList.size(); i++)
		{
			if(pageName.equalsIgnoreCase(profileList.get(i).getPageName()))
			{
				profileList.remove(i);
				hasRemoved = true;
				i--;
			}
		}
		if(!hasRemoved)
		{
			throw new NoSuchElementException(pageName);
		}
	}

	/**
	 * Returns the plugin settings list for the profile whose name is input
	 * @param profileName the name of the profile whose settings you want to retrieve
	 * @return the ArrayList of PluginSettings of the profile 
	 * @throws NoSuchElementException if the profile name is not found
	 */
	public ArrayList<PluginSetting> getProfile(String profileString) throws NoSuchElementException
	{
		if(profileString == null)
		{
			throw new NoSuchElementException();
		}
		String profileName = null;
		String pageName = "";
		String[] profileStrings = profileString.split("[.]");
		if(profileStrings.length == 1)
		{
			profileName = profileStrings[0];
		}
		else if(profileStrings.length == 2)
		{
			pageName = profileStrings[0];
			profileName = profileStrings[1];
		}
		else
		{
			throw new NoSuchElementException("Incorrect Syntax in EMML Tag: " + profileString);
		}
		for(EMMLProfile profile: profileList)
		{
			if(profileName.equalsIgnoreCase(profile.getProfileName()) && pageName.equalsIgnoreCase(profile.getPageName()))
			{
				return profile.getSettings();
			}
		}
		throw new NoSuchElementException("Profile Not Found: " + profileString); //If we don't have the profile with the name of the input variable
	}

	/**
	 * Opens, reads and parses profile information from an input File. Creates profiles
	 * from the profile page and stores them for later use.
	 * @param inputFile the file to be parsed
	 * @param name the name to give the profile. If empty, this Profile Page will replace the current temporary Profile Page
	 * @throws FileNotFoundException if the inputFile doesn't exist
	 * @throws IOException if an IO problem happens when using the file
	 */
	public void importProfilePage(FileInputStream inputFile, String name) throws IOException
	{
		/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "start"));
		if(inputFile == null)
		{
			return;
		}
		FileChannel fileChannel = inputFile.getChannel();
		Charset charset = Charset.forName("US-ASCII");
		CharsetDecoder decoder = charset.newDecoder();
		CharBuffer charBuffer = decoder.decode(fileChannel.map(FileChannel.MapMode.READ_ONLY, 0, fileChannel.size())); 
		if(name == null)
			name = "";

		try
		{
			deleteProfilePage(name);
		}
		catch(NoSuchElementException e)
		{
			//No worries, may not exist. Just carry on
		}

		//Remove all comments, including multiline comments
		commentMatcher.reset(charBuffer);
		charBuffer = CharBuffer.wrap(commentMatcher.replaceAll(""));

		//Remove all urls
		StringBuffer sb = new StringBuffer(charBuffer);
		
		ArrayList<String> urls = new ArrayList<String>();
		urlMatcher.reset(sb);
		while(urlMatcher.find())//When its found a "url('...');
		{
//			String url = urlMatcher.group(2);
			String url = urlMatcher.group(3); 
			int endPosition = urlMatcher.end();
			//While theres an open bracket
			for(String string = sb.substring(endPosition); sb.length() > (endPosition +1) && ((url.replaceAll("[^(]","").length() - url.replaceAll("[^)]","").length()) > 0); string = sb.substring(endPosition))
			{
				//Find a the next closing bracket
				bracketMatcher.reset(string);
				if(bracketMatcher.find())
				{
					url += bracketMatcher.group(2) + ")" + sb.substring(endPosition, endPosition + bracketMatcher.end(1)); //Add it to the URL
					endPosition += bracketMatcher.end(3);
				}
				else//Badly formed EMML
				{
					sb.setLength(0);
					/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad Syntax in Profile"));
					break;
				}
			}
			//Checks if it exited with an error (i.e. open bracket remains)
			if(sb.length() <= (endPosition +1) && ((url.replaceAll("[^(]","").length() - url.replaceAll("[^)]","").length()) > 0))
			{
				//incorrect input
				sb.setLength(0);
				/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad Syntax in Profile"));
			}
			else
			{
				//Adds the extracted url to the url List for injecting later
				//and replaces the extracted string with the ESCAPE character which is defined above
				urls.add(url); 
				sb.delete(urlMatcher.start(), endPosition);
				sb.insert(urlMatcher.start(), ESCAPE_CHAR);
			}
			urlMatcher.reset(sb);
		}
		
		//Remove all whitespace
		profileSplitter.reset(sb);
		
		while(profileSplitter.find())
		{
			EMMLProfile profile = new EMMLProfile(name.toLowerCase(), profileSplitter.group(1).trim());
			String profileContents = profileSplitter.group(2);
			String[] settingStrings = profileContents.split(";");
			for(String setting: settingStrings)
			{
				String[] settingComponents = setting.split(":");
				String setting1 = "";
				String setting2 = "";
				if(settingComponents.length >= 1)
				{
					setting1 = settingComponents[0];
					if(settingComponents.length >= 2)
					{
						for(int i = 1; i < settingComponents.length; i++)
						{
							setting2 += settingComponents[i];
						}
						
						//DO REPLACE
						while(setting2.matches(".*" + ESCAPE_CHAR + ".*"))
						{
							setting2 = setting2.replaceFirst(ESCAPE_CHAR, urls.remove(0));
						}
					}
				}
				profile.addSettings(parseEMMLTag(setting1, setting2));
			}
			profileList.add(profile);
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Added profile: " + profile.getPageName() + "." + profile.getProfileName()));
		}
		/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "end"));
	}

	/**
	 * Initialises and compiles the Regex Matchers and Patterns
	 */
	private void init()
	{
		//Set up profile array list
		profileList = new ArrayList<EMMLProfile>();

		//Setup the httpEquiv splitter
		ctSplitter = Pattern.compile("\\s*-\\s*");

		//Setup the url extractor
		//urlMatcher = Pattern.compile("\\s*(url[(]'(.*)'\\s*(.*)\\s*[)];)\\s*").matcher("");
//		urlMatcher = Pattern.compile("\\s*(url\\s*[(]\\s*'(.*?)('\\s*[)]))\\s*").matcher("");
		urlMatcher = Pattern.compile("\\s*(url\\s*[(]\\s*(['\"])(.*?)(\\2\\s*[)]))\\s*").matcher("");
		commentMatcher = Pattern.compile("[/][*].*?[*][/]", Pattern.DOTALL).matcher("");
		profileSplitter = Pattern.compile("\\.(.*?)\\{(.*?)\\}", Pattern.DOTALL).matcher("");
		bracketMatcher = Pattern.compile("(.*?)(['\"])\\s*([)])").matcher("");
		
		loadRegex(false);
	}

	/**
	 * 
	 */
	private void loadRegex(boolean useDefaults)
	{
		//Set up the Regex Patterns/Replacements ArrayLists
		cTPatterns = new ArrayList<Matcher>();
		cTReplacers = new ArrayList<String>();
		hEPatterns = new ArrayList<Matcher>();
		hEReplacers = new ArrayList<String>();
		try
		{
	        XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
	        XmlPullParser parser = factory.newPullParser();

	        if(useDefaults)
	        {
	        	parser.setInput(Common.mainActivity.getResources().openRawResource(RhoExtManager.getResourceId("raw","regex")), null);
	        }
	        else
	        {
	        	String regexFile = Common.config.getSetting(Config.SETTING_REGEX_FILE);
	        	if(regexFile == null)
	        	{
	        		Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, Config.SETTING_REGEX_FILE + " not defined in Config.xml. Using default regex patterns."));
	        		loadRegex(true);
	        		return;
	        	}
	        	parser.setInput(new FileReader(new File(Common.parseAndroidURI(regexFile))));
	        }
			String currentSection = "";
			for(int event = parser.getEventType(); event != XmlPullParser.END_DOCUMENT; event = parser.next())
			{
				switch(event)
				{
					case XmlPullParser.START_TAG:
						if(parser.getName().equalsIgnoreCase("Expression"))
						{
							String patternEx = null;
							String replaceEx = null;
							for(int i = 0; i < parser.getAttributeCount(); i++)
							{
								String name = parser.getAttributeName(i);
								if(name.equalsIgnoreCase("patternex"))
								{
									patternEx = parser.getAttributeValue(i);
								}
								if(name.equalsIgnoreCase("replaceex"))
								{
									replaceEx = parser.getAttributeValue(i).replaceAll("\\\\([0-9]+)", "\\$$1");
								}
							}
							
							if(patternEx == null || replaceEx == null)
							{
								/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Incorrect regex tag. Cannot find PatternEX/replaceEX attribute on line: " + parser.getLineNumber()));
								break;
							}
							if(currentSection.equals("Equivs"))
							{
								addRegExPair_HE(patternEx, replaceEx);
							}
							else if(currentSection.equals("Contents"))
							{
								addRegExPair_CT(patternEx, replaceEx);
							}
							else
							{
								/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Badly formed regex.xml. Regex pair outside of Equivs/Contents tags on line: " + parser.getLineNumber()));
							}
						}
						else if(parser.getName().equalsIgnoreCase("Equivs"))
						{
							currentSection = "Equivs";
						}
						else if(parser.getName().equalsIgnoreCase("Contents"))
						{
							currentSection = "Contents";
						}
						break;
					default: break;
				}
			}
		}
		catch (XmlPullParserException e)
		{
			if(!useDefaults)
			{
				/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "regex.xml error. Could not parse. Using default regex patterns."));
				loadRegex(true);
			}
			else
			{
				/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Could not parse default regex patterns."));
			}
		}
		catch (FileNotFoundException e)
		{
			if(!useDefaults)
			{
				/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "regex.xml file not found. Using default regex patterns."));
				loadRegex(true);
			}
			else
			{
				//Will never happen
				/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Could not parse default regex patterns."));
			}
		}
		catch (IOException e)
		{
			if(!useDefaults)
			{
				/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot read regex.xml. Using default regex patterns."));
				loadRegex(true);
			}
			else
			{
				//Should never happen
				/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Could not parse default regex patterns."));
			}
		}
		catch (URISyntaxException e)
		{
			if(!useDefaults)
			{
				/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "regex.xml URL is invalid in Config.xml. Using default regex patterns."));
				loadRegex(true);
			}
			else
			{
				//Will never happen
				/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Could not parse default regex patterns."));
			}
		}
	}
	
//	private void loadDefaultRegex()
//	{
//		
////		//Add all Regex Patterns
////		for(String[] array: EMMLRegexPatterns.regExPatternsHE)
////		{
////			addRegExPair_HE(array[0], array[1]);
////		}
////		for(String[] array: EMMLRegexPatterns.regExPatternsCT)
////		{
////			addRegExPair_CT(array[0], array[1]);
////		}
//	}

	/**
	 * Parses the http-equiv and contents/name parameters into the component parts;
	 * method, parameter and value.
	 * @param httpEquiv the http-equiv string to parse
	 * @param content the content or name string to parse
	 * @return an ArrayList of the PluginSettings generated by the input EMML tag 
	 */
	public synchronized ArrayList<PluginSetting> parseEMMLTag(String httpEquiv, String contents)
	{
		ArrayList<PluginSetting> returnArray = new ArrayList<PluginSetting>();

		//Do all HttpEquiv Regex Replacers to the HttpEquiv String
		for(int i = 0; i < hEPatterns.size(); i++)
		{
			Matcher current = hEPatterns.get(i);
			current.reset(httpEquiv);
			if(current.lookingAt())
			{
				httpEquiv = current.replaceAll(hEReplacers.get(i));
			}
		}
		
		//Do all Contents Regex Replacers to the Contents String
		for(int i = 0; i < cTPatterns.size(); i++)
		{
			Matcher current = cTPatterns.get(i);
			current.reset(contents);
			if(current.lookingAt())
			{
				contents = current.replaceAll(cTReplacers.get(i));
			}
		}
		
		//If the module name don't exist, then return empty arraylist
		if(httpEquiv.length() < 0) 
		{
			return returnArray;
		}

		//Split the module name
		String[] modNName = ctSplitter.split(httpEquiv.trim().toLowerCase());
		String module = modNName[0];
		String tempName = (modNName.length > 1 ? modNName[1] : "").trim();

		//Makes sure the last pair has a semicolon, doesn't matter if there are multiples.
		StringBuffer sb = new StringBuffer(contents + ";");
		
		//Extract "url('...');"s
		ArrayList<String> urls = new ArrayList<String>();
		urlMatcher.reset(sb);
		while(urlMatcher.find())//When its found a "url('...');
		{
//			String url = urlMatcher.group(2); 
			String url = urlMatcher.group(3);
			int endPosition = urlMatcher.end();
			//While theres an open bracket
			for(String string = sb.substring(endPosition); sb.length() > (endPosition +1) && ((url.replaceAll("[^(]","").length() - url.replaceAll("[^)]","").length()) > 0); string = sb.substring(endPosition))
			{
				//Find a the next closing bracket
				bracketMatcher.reset(string);
				if(bracketMatcher.find())
				{
					url += bracketMatcher.group(2) + ")" + sb.substring(endPosition, endPosition + bracketMatcher.end(1)); //Add it to the URL
					endPosition += bracketMatcher.end(3);
				}
				else//Badly formed EMML
				{
					sb.setLength(0);
					/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad Syntax: " + contents));
					break;
				}
			}
			//Checks if it exited with an error (i.e. open bracket remains)
			if(sb.length() <= (endPosition +1) && ((url.replaceAll("[^(]","").length() - url.replaceAll("[^)]","").length()) > 0))
			{
				//incorrect input
				sb.setLength(0);
				/*if(!Oem.testmode)*/Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad Syntax: " + contents));
			}
			else
			{
				//Adds the extracted url to the url List for injecting later
				//and replaces the extracted string with the ESCAPE character which is defined above
				urls.add(url); 
				sb.delete(urlMatcher.start(), endPosition);
				sb.insert(urlMatcher.start(), ESCAPE_CHAR);
			}
			urlMatcher.reset(sb);
		}

		//Now lets parse the contents string
		boolean nameUsed = false; //The name needs to get used somewhere
		String[] contentStrings = sb.toString().split("\\s*;\\s*", 0);//Split, removing trailing empty strings and removing whitespace around the ';
		final String invalidEvent = "^.*event:$";

		Pattern invalidEventPattern = Pattern.compile(invalidEvent, Pattern.CASE_INSENSITIVE);
		
		for(String content: contentStrings)
		{
			/*
			 * We need to check whether after an event property, there is a url.
			 * We basically don't want that syntax like "xyzevent:" produce any result
			 */
			Matcher matcher = invalidEventPattern.matcher(content);
			if (matcher.matches())
				continue;
			
			//set the method name from the HttpEquiv as the default method
			String name = tempName;
			String value = "";
			
			//Split, keeping trailing empty strings but removing whitespace around the ':'
			String[] pairs = content.split("\\s*:\\s*", -1);
			
			//An orphan is found
			if(pairs.length == 1) 
			{
				// if the name set by the HttpEquiv String has been used,
				if(nameUsed) 
				{
					//Then this value is an orphan and therefore a method.
					name = pairs[0];
				}
				//This is the first value not to have a method, so use the method from the HttpEquiv String
				else
				{
					value = pairs[0];
					nameUsed = true;
				}
			}
			//This is a name:value pair, treat accordingly
			else
			{
				name = pairs[0].trim();
				if(name.equalsIgnoreCase(tempName))
				{
					nameUsed = true;
				}
				value = pairs[1];
			}

			//If the value is a URL then add it in from the url List
			if(value.equals(ESCAPE_CHAR)) 
				value = "url:" + urls.remove(0);

			//If there is no method, then promote the value to the method
			if(name.length() == 0)
			{
				returnArray.add(new PluginSetting(module, value.trim(), ""));
				nameUsed = true;
			}
			else//Else treat like a regular plugin setting
			{
				returnArray.add(new PluginSetting(module, name.toLowerCase(), value.trim()));
			}
		}

		//If the method from the HttpEquiv string hasnt been used...
		if(!nameUsed && (tempName.length() > 0))
		{
			//...Use it
			returnArray.add(new PluginSetting(module, tempName, ""));
		}
		return returnArray;
	}    
}
