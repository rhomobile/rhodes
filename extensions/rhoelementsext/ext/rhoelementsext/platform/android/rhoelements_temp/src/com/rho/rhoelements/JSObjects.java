package com.rho.rhoelements;

import java.io.BufferedReader;
import java.io.FileReader;

import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.TextView;

public class JSObjects
{
	private Handler handler;

	public JSObjects()
	{
		// Construct Handler in this thread context
		handler = new JSObjectsHandler();
	}

	/**************************************************************************/
	public String getOEMInfo()
	{
		return Build.PRODUCT;
	}

	/**************************************************************************/
	public String getUUID()
	{
		// Get serial number from UUID file built into image
		// TODO Check UUID is unique
		try
		{
			BufferedReader reader = new BufferedReader(new FileReader("/sys/hardware_id/uuid"));
			return reader.readLine();
		}
		catch (Exception e)
		{
			return "Unknown";
		}
	}

	/**************************************************************************/
	public void doEMML(String equiv, String content)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "'" + equiv + "', '" + content + "'"));

		// Use the same method to process as is used for actual <meta> tags.
		// It will also synchronise to the main thread.
		Common.metaReceiver.setMeta(equiv, content);
	}

	/**************************************************************************/
	public Boolean doLog(String comment, int severity)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "'" + comment + "', '" + severity + "'"));

		// Handle request in the main thread so we can get the current URL
		handler.sendMessage(handler.obtainMessage(0, new LogRequest(severity, comment)));
		
		return true;
	}

	/**************************************************************************/
	class JSObjectsHandler extends Handler
	{
		@Override
		public void handleMessage(Message message)
		{
			// Message is JSObjectsRequest object
			JSObjectsRequest request = (JSObjectsRequest) message.obj;
			request.execute();
		}
	}

	/**
	 * Base class for all JSObjects requests
	 */
	abstract class JSObjectsRequest
	{
		/**
		 * Overridden by specific request handlers
		 */
		abstract void execute();
	}

	/**************************************************************************/
	class LogRequest extends JSObjectsRequest
	{
		int severity;
		String comment;

		public LogRequest(int severity, String comment)
		{
			this.severity = severity;
			this.comment = comment;
		}

		@Override
		void execute()
		{
			// Translate severity figure to text and use as 'caller' field
			String caller;
			switch (severity)
			{
			case 1:
				caller = "Low";
				break;
			case 2:
				caller = "Medium";
				break;
			case 3:
				caller = "High";
				break;
			default:
				caller = "Unknown";
				break;
			}

			Common.logger.add(new LogEntry(LogEntry.PB_LOG_USER, caller, Common.elementsCore.getCurrentUrl(), comment, 0));
		}
	}
}
