package com.rho.rhoelements;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.BufferOverflowException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.nio.charset.CharsetEncoder;
import java.security.InvalidParameterException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;
import org.apache.http.HttpHost;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.params.ConnRoutePNames;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

/**
 * Maintains a queue of log entries and processes them in a thread.
 * BlockingQueue interface guarantees that put() and take() are atomic
 * so no additional locking is needed.
 * @author xfh386
 *
 */
public class Logger
{
	LinkedBlockingQueue<LogEntry> queue = new LinkedBlockingQueue<LogEntry>();
	Thread thread;
	URL logUrl;
	Protocol logProtocol;
	String mProxyHost = null;
	String mProxyPort = null;
	HttpHost mProxy = null;
	int logSize;
	
	Object writeLock = new Object();
	LoggerThread loggerRunnable;
	boolean errorEnabled, warningEnabled, infoEnabled, userEnabled, debugEnabled;
	boolean isTerminating;
	
	boolean getErrorEnabled() {return errorEnabled;}
	void setErrorEnabled(boolean value) {errorEnabled = value;}
	
	boolean getWarningEnabled() {return warningEnabled;}
	void setWarningEnabled(boolean value) {warningEnabled = value;}
	
	boolean getInfoEnabled() {return infoEnabled;}
	void setInfoEnabled(boolean value) {infoEnabled = value;}

	boolean getUserEnabled() {return userEnabled;}
	void setUserEnabled(boolean value) {userEnabled = value;}
	
	boolean getDebugEnabled() {return debugEnabled;}
	void setDebugEnabled(boolean value) {debugEnabled = value;}
	
	private Class rhodesLoggerClass = null;
	private Method rhodesLoggerMethod[] = null;
	private enum RHODES_LOG_CATEGORIES  {ERROR, WARNING, INFO, USER, DEBUG};

	/**
	 * Constructs Logger object and loads configuration
	 * @author Geoff Day (xfh386)
	 */
	public Logger()
	{
		try
		{
		    loadConfig();
		    if (Common.isRhodes)
		    {
			try
			{
        			rhodesLoggerMethod = new Method[5]; //We have 5 different log categories in RHODES_LOG_CATEGORIES
        			rhodesLoggerClass = Class.forName("com.rhomobile.rhodes.extmanager.RhoExtManager");
        			rhodesLoggerMethod[RHODES_LOG_CATEGORIES.ERROR.ordinal()] = rhodesLoggerClass.getMethod("logE", String.class, String.class);
        			rhodesLoggerMethod[RHODES_LOG_CATEGORIES.WARNING.ordinal()] = rhodesLoggerClass.getMethod("logW", String.class, String.class);
        			rhodesLoggerMethod[RHODES_LOG_CATEGORIES.INFO.ordinal()] = rhodesLoggerClass.getMethod("logI", String.class, String.class);
        			rhodesLoggerMethod[RHODES_LOG_CATEGORIES.USER.ordinal()] = rhodesLoggerClass.getMethod("logU", String.class, String.class);
        			rhodesLoggerMethod[RHODES_LOG_CATEGORIES.DEBUG.ordinal()] = rhodesLoggerClass.getMethod("logT", String.class, String.class);
			}
			catch (NullPointerException e)
			{
			    e.printStackTrace();
			} 
			catch (ClassNotFoundException e)
			{
			    e.printStackTrace();
			}
			catch (SecurityException e)
			{
			    e.printStackTrace();
			} 
			catch (NoSuchMethodException e)
			{
			    e.printStackTrace();
			}
			
		    }
		}
		catch(InvalidParameterException e)
		{
		    Common.log(e.getMessage());
		}
	}
	
	/**
	 * Loads configuration from common config object
	 * @author Ben Kennedy (ncvt73), Geoff Day (xfh386)
	 */
	public void loadConfig() throws InvalidParameterException
	{
		try
		{
			URI tempUri = new URI(Common.config.getSetting(Config.SETTING_LOG_URI));
			logProtocol = parseProtocol(Common.config.getSetting(Config.SETTING_LOG_PROTOCOL));
			
			if(logProtocol == null)
			{
				String scheme = tempUri.getScheme();
				if(scheme == null || scheme.equalsIgnoreCase(Protocol.FILE.name()))
				{
					logProtocol = Protocol.FILE;
				}
				else if(scheme.equalsIgnoreCase(Protocol.HTTP.name()))
				{
					logProtocol = Protocol.HTTP;
				}
				else
				{
					throw new InvalidParameterException("Unsupported log protocol: " + scheme + ". Log file will not be made.");
				}
			}
			
			tempUri = new URI(logProtocol.name().toLowerCase(), tempUri.getSchemeSpecificPart(), tempUri.getFragment());
		
			URI logUri = Common.parseAndroidURI(tempUri.toString());
			logUrl = logUri.toURL();
			
			//Get the LogMaxSize
			try
			{
				logSize = Integer.parseInt(Common.config.getSetting(Config.SETTING_LOG_SIZE)) * 1024;
			}
			catch(NumberFormatException e)
			{
				logSize = 0;
				Common.log("Could not parse Config value for LogMaxSize. LogMaxSize has been set to unlimited.");
			}
			
			setErrorEnabled(Common.config.getBooleanSetting(Config.SETTING_LOG_ERROR));
			setWarningEnabled(Common.config.getBooleanSetting(Config.SETTING_LOG_WARNING));
			setInfoEnabled(Common.config.getBooleanSetting(Config.SETTING_LOG_INFO));
			setUserEnabled(Common.config.getBooleanSetting(Config.SETTING_LOG_USER));
			setDebugEnabled(Common.config.getBooleanSetting(Config.SETTING_LOG_DEBUG));
			
			//Check whether the log recipient is a http server
			if (logProtocol == Protocol.HTTP)
			{
				// Parse address to get URI
				//Check if the user has set the log server port
		    	if (Common.config.getSetting(Config.SETTING_LOG_PORT) != null)
				{
		    		try
		    		{
		    			int port = Integer.parseInt(Common.config.getSetting(Config.SETTING_LOG_PORT));
		    			logUrl = new URL(logProtocol.name().toLowerCase(), logUrl.getHost(), port, logUrl.getFile());
		    		}
		    		catch (NumberFormatException e)
		    		{
		    			//Do nothing
		    		}
				}
			
				//Check if we need to tunnel through a proxy
				if (isUrlDirect(logUrl.getHost().toString()) == false)
				{
					if (Common.config.getSetting(Config.SETTING_HTTP_PROXY) == null || Common.config.getSetting(Config.SETTING_HTTP_PROXY).length() < 1)
						return;
					URL proxy = new URL(Common.config.getSetting(Config.SETTING_HTTP_PROXY));
					mProxyHost = proxy.getHost();
					mProxyPort = String.valueOf(proxy.getPort());
					if ( (mProxyHost != null) && (mProxyPort != null) )
						mProxy = new HttpHost(mProxyHost, Integer.parseInt(mProxyPort));
						
				}
			}
		}
		catch(URISyntaxException e)
		{
			Common.log(e.getMessage());
		}
		catch(InvalidParameterException e)
		{
			Common.log(e.getMessage());
		}
		catch (NumberFormatException e)
		{
			e.printStackTrace();
		}
		catch (MalformedURLException e)
		{
			e.printStackTrace();
		}
		catch (NullPointerException e)
		{
			e.printStackTrace();
		}
	}
	
	private Protocol parseProtocol(String setting)
	{
		if(setting != null)
		{
			if(setting.equalsIgnoreCase(Protocol.FILE.name()))
			{
				return Protocol.FILE;
			}
			else if(setting.equalsIgnoreCase(Protocol.HTTP.name()))
			{
				return Protocol.HTTP;
			}
		}
		return null;
	}
	public void start()
	{
        if (!Common.isRhodes) {
            isTerminating = false;
            loggerRunnable = new LoggerThread();
            thread = new Thread(loggerRunnable);
            thread.start();
        }

		Common.log("Logger started");
	}
	
	public void stop()
	{
		isTerminating = true;
		// Interrupt thread and give it time to terminate
		// Wait for any current log processing to finish
		synchronized(writeLock)
		{
			try
			{
				thread.interrupt();
				thread.join(3000);
				loggerRunnable.cleanUp();
			}
			catch (Exception e) {}
		}

		Common.log("Logger stopped");
	}
	
	public void add(LogEntry entry)
	{
		if (Common.isRhodes)
		    addRhodesLog(entry);
		else
		    addRhoElementsLog(entry);
	}
	
	private void addRhoElementsLog(LogEntry entry)
	{
		switch(entry.getSeverity())
		{
			case LogEntry.PB_LOG_ERROR:
				if (!errorEnabled)
					return;
				break;

			case LogEntry.PB_LOG_WARNING:
				if (!warningEnabled)
					return;
				break;

			case LogEntry.PB_LOG_INFO:
				if (!infoEnabled)
					return;
				break;
	
			case LogEntry.PB_LOG_USER:
				if (!userEnabled)
					return;
				break;
	
			case LogEntry.PB_LOG_DEBUG:
				if (!debugEnabled)
					return;
				break;
				
			default:
				// Allow anything else (shouldn't ever happen)
				break;
		}
		
		try {
			if (!isTerminating)
				queue.put(entry);
		}
		catch (Exception e) {}
	}
	
	private void addRhodesLog(LogEntry entry)
	{
	    try
	    {
		switch(entry.getSeverity())
		{
			case LogEntry.PB_LOG_ERROR:
			    rhodesLoggerMethod[RHODES_LOG_CATEGORIES.ERROR.ordinal()].invoke(null, entry.caller + " " + entry.function, entry.comment);
			    break;
			case LogEntry.PB_LOG_WARNING:
			    rhodesLoggerMethod[RHODES_LOG_CATEGORIES.WARNING.ordinal()].invoke(null, entry.caller + " " + entry.function, entry.comment);
			    break;
			case LogEntry.PB_LOG_INFO:
			    rhodesLoggerMethod[RHODES_LOG_CATEGORIES.INFO.ordinal()].invoke(null, entry.caller + " " + entry.function, entry.comment);
			    break;
			case LogEntry.PB_LOG_USER:
			    rhodesLoggerMethod[RHODES_LOG_CATEGORIES.USER.ordinal()].invoke(null, entry.caller + " " + entry.function, entry.comment);
			    break;
			case LogEntry.PB_LOG_DEBUG:
			    rhodesLoggerMethod[RHODES_LOG_CATEGORIES.DEBUG.ordinal()].invoke(null, entry.caller + " " + entry.function, entry.comment);
			    break;
			default:
			    // Allow anything else (shouldn't ever happen)
			    break;
		}
	    }
	    catch (NullPointerException e)
	    {
		e.printStackTrace();
	    } 
	    catch (IllegalArgumentException e)
	    {
		e.printStackTrace();
	    }
	    catch (IllegalAccessException e)
	    {	
		e.printStackTrace();
	    }
	    catch (InvocationTargetException e)
	    {
		e.printStackTrace();
	    }
	}
	
	class LoggerThread implements Runnable
	{
		private MappedByteBuffer buffer;
		private Charset charset;
		private CharsetEncoder encoder;
		private FileChannel fileChannel;
		
		public LoggerThread()
		{
	    		    	
		}
		
		public void cleanUp()
		{
			// Quit on InterruptedException
			LogEntry entry = null;
			while (!queue.isEmpty())
			{
				entry = queue.poll();
				loggerRunnable.writeLog(entry);
			}
			try
			{
				fileChannel.truncate(buffer.position());
				fileChannel.close();
			}
			catch (IOException e)
			{
				//Ignore
			}
		}
		
		@Override
		public void run()
		{
			charset = Charset.forName("UTF-8");
			encoder = charset.newEncoder();
			
			//Set up the MappedByteBuffer if the LogMaxSize has been set and the URI is a file.
			if ( (!Common.isRhodes) &&
				(logProtocol == Protocol.FILE) && 
				(logSize > 0) ) //$NON-NLS-1$
			{
				try
				{
					int logFileLength;
					File logFile = new File(logUrl.toURI());
					long longLogFileLength = 0;
					boolean isLogFileExist = logFile.exists();
					if (isLogFileExist)
					{
						longLogFileLength = logFile.length();
						if(longLogFileLength > Integer.MAX_VALUE) //This should never happen, due to restrictions on 32bit machines
						{
							logFileLength = Integer.MAX_VALUE;
						}
						else
						{
							logFileLength = (int) longLogFileLength;
						}
						
					}
					else
					{
						logFileLength = 0;
						logFile.createNewFile();
					}
					RandomAccessFile logFileStream = new RandomAccessFile(logFile, "rw");
					fileChannel = logFileStream.getChannel();
					
					//if the file is bigger than the new maxsize...
					if(isLogFileExist && logFileLength > logSize)
					{
						//truncate
						MappedByteBuffer tempBuffer = fileChannel.map(FileChannel.MapMode.READ_WRITE, 0, longLogFileLength);
						tempBuffer.position((int) longLogFileLength - logSize);
						tempBuffer.compact();
						fileChannel.truncate(logSize);
						logFileLength = logSize;
					}
					
					buffer = fileChannel.map(FileChannel.MapMode.READ_WRITE, 0, logSize);
					buffer.load();
					buffer.position(logFileLength); //Set to the end of the file.
				}
				catch(FileNotFoundException e)
				{
					Common.log("Could not create local log file at specified URI.");
				}
				catch(IOException e)
				{
					Common.log("Log file could not be created.");
				}
				catch (URISyntaxException e)
				{
					//Wont happen
				}
			}
			while (true)
			{
				try
				{
					// Wait for something to be available on the queue
					LogEntry entry = queue.take();
					// writeLog() is synchronized so stop() won't interrupt it
					synchronized(writeLock)
					{
						writeLog(entry);
					}
				}
				catch(InterruptedException e)
				{
					break;
				}
				catch(Exception e)
				{
					// Log any other exception and carry on
					Common.log("Error in log thread: " + e.getMessage());
				}
			}
		}
		
		void writeLog(LogEntry entry)
		{
			if (Common.logLog)
				Common.log(entry.getCaller() + " " + entry.getFunction() + " " + entry.getComment());
			
			if (logUrl == null)
				return;
			
			if (logProtocol == Protocol.FILE)
			{
				String text = date_format.format(entry.getDate()) + ", " +
					entry.getSeverityName() + ", " +
					entry.getCaller() + ", " +
					entry.getFunction() + ", " +
					entry.getComment() + ", " +
					String.valueOf(entry.getLine()) + "\r\n";
				
				if(buffer != null)
				{
					//Write to rotary log file with log entry details
					writeLogToBuffer(text);
				}
				else
				{
					// Write to unlimited log file with log entry details
					sendFile(text);
				}
			}
			else if (logProtocol == Protocol.HTTP)
			{
				// Send HTTP request with log entry details
				try {sendHTTP(entry);}
				catch(Exception e){Common.log("Error sending http log: " + e.getMessage());}
			}
		}
		
		private void writeLogToBuffer(String text)
		{
			try
			{
				ByteBuffer textBuffer = encoder.encode(CharBuffer.wrap(text));
				byte[] textBufferArray = textBuffer.array();
				//If length > total file size
				if(buffer.remaining() < text.length())
				{
					int remaining = buffer.remaining();
					buffer.flip();
					//find first /r/n
					boolean foundEndLine = false;
					try
					{					
						while(!foundEndLine)
						{
							char first = (char) buffer.get();
							//if we've found a line break and after removing this line there is enough space to append...
							if(first == '\r' && buffer.get() == '\n' && (buffer.position() + remaining) >= textBuffer.limit())
							{
								foundEndLine = true;
							}
						}
						buffer.compact();
					}
					catch(BufferUnderflowException e)
					{
						//Flush the whole file
						buffer.position(buffer.limit());
						buffer.compact();
					}
				}
				
				try
				{
					buffer.put(textBuffer);
				}
				catch(BufferOverflowException e)
				{
					Common.log("Log file size is smaller than one log entry. Increase the log file size"); //$NON-NLS-1$
				}
				
				//Forces the changes to the file
				buffer.force();
			}
			catch (CharacterCodingException e)
			{
				Common.log("Unexpected character in log stream");
			}
		}

		SimpleDateFormat date_format = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
		
		void sendFile(String text)
		{
			// Quit if empty filename
			try
			{
				File file = new File(logUrl.toURI());
				if (file.exists() == false)
					file.createNewFile();
				// Write to file - open and close each time in case someone wants to read it 
				FileWriter fWriter = new FileWriter(file, true);
				fWriter.write(text);
				fWriter.flush();
				fWriter.close();
			}
			catch (InvalidParameterException e)
			{
				Common.log(e.getMessage());
			}
			catch (IOException e)
			{
				Common.log("File error in writing to log file.");
			}
			catch (URISyntaxException e)
			{
				//Wont happen
			}
		}
		
		void sendHTTP(LogEntry entry) throws Exception
		{
	    	// Quit if empty host address
	    	if (logUrl == null)
	    		return;

			HttpClient client = new DefaultHttpClient();
			if (mProxy != null)
				client.getParams().setParameter(ConnRoutePNames.DEFAULT_PROXY, mProxy);
						
	    	// Disable sending of "Expect: 100-Continue" header
	    	client.getParams().setParameter("http.protocol.expect-continue", false);
	    	
	    	HttpPost request = new HttpPost(logUrl.toString());
	    	
	    	List<NameValuePair> params = new ArrayList<NameValuePair>();
	    	params.add(new BasicNameValuePair("LogSeverity", entry.getSeverityName()));
	    	params.add(new BasicNameValuePair("LogComment", entry.getComment()));
	    	params.add(new BasicNameValuePair("FunctionName", entry.getFunction()));
	    	params.add(new BasicNameValuePair("Caller", entry.getCaller()));
	    	UrlEncodedFormEntity entity = new UrlEncodedFormEntity(params);
	    	request.setEntity(entity);
	    	client.execute(request);
		}
	}
	
	private boolean isUrlDirect(String url)
	{
		String exSetting = Common.config.getSetting(Config.SETTING_PROXY_EXCEPTIONS);
		if (exSetting == null)
			return false;
		String[] proxyExceptions = exSetting.split(" ");
		for (int i=0; i<proxyExceptions.length; i++)
	    {
	    	if (proxyExceptions[i].length() == 0)
	    		continue;
	    	try
	    	{
				Pattern exceptionPattern = Pattern.compile(proxyExceptions[i]);
		    	Matcher exceptionMatcher = exceptionPattern.matcher(url);
		    	if (exceptionMatcher.find())
		    		return true;
	    	}
	    	catch (PatternSyntaxException e)
	    	{
	    		//Do nothing
	    	}
	    }
		return false;
	}
	
	public enum Protocol
	{
		FILE, HTTP
	}
}
