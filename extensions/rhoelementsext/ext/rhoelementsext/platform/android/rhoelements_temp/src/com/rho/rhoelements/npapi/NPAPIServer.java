package com.rho.rhoelements.npapi;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;

import android.os.Handler;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;

public class NPAPIServer
{
	private ServerSocket Server;
	private Socket Connection;
	private Thread serverThread;
	private Handler threadHandler;

	private HuddleHandler myHuddleHandler;

	// This script adds the NPAPI <embed> tag to a page
	public final static String jsAddNPAPI = "var body = document.getElementsByTagName('body')[0];" + "var embed = document.createElement('embed');" + "embed.setAttribute('type', 'application/x-wtg-jsobjects');" + "embed.setAttribute('hidden', 'true');" + "body.appendChild(embed);";

	public NPAPIServer() throws IOException
	{
		// Create Handler to sync everything back to main thread
		threadHandler = new Handler();

		// create huddle handler
		myHuddleHandler = new HuddleHandler();

		// Create local socket server bound to any available port
		Server = new ServerSocket();
		Server.bind(null);
		// GD TESTING Server = new ServerSocket(4042, 50, InetAddress.getByName("192.168.1.10"));

		// Start server thread listening for connections
		serverThread = new Thread(new ServerThread());
		serverThread.start();

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Bound to port " + getPort()));
	}

	public int getPort()
	{
		try
		{
			// Get bound port from server socket
			return Server.getLocalPort();
		}
		catch (Exception e)
		{
			return -1;
		}
	}

	class ServerThread implements Runnable
	{
		public void run()
		{
			BufferedReader input = null;
			OutputStreamWriter output = null;

			// Loop waiting for connection
			while (true)
			{
				try
				{
					Connection = Server.accept();
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Connection accepted"));

					// Get input stream from connection
					input = new BufferedReader(new InputStreamReader(Connection.getInputStream()));
					output = new OutputStreamWriter(Connection.getOutputStream());

					// Read lines until the connection is broken
					while (true)
					{
						String message = input.readLine();
						if (message == null)
							throw new Exception("Connection broken");

						// Processing of JS object calls is thread safe because it's done through MetaReceiver.setMeta()
						String Reply = myHuddleHandler.handle(message);
						if (Reply != null)
						{
							output.write(Reply);
							output.flush();
						}
					}
				}

				catch (Exception e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));

					// Close the current reader, if any
					try
					{
						input.close();
					}
					catch (Exception x)
					{
					}

					// Clean up the current connection, if any
					try
					{
						Connection.close();
					}
					catch (Exception x)
					{
					}

					// Quit if the server socket is now closed, otherwise loop around for another connection
					if (Server.isClosed())
						break;
				}
			}

			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Thread stopping"));
		}
	}

	public void close()
	{
		// Close the listening socket 
		try
		{
			Server.close();
		}
		catch (Exception e)
		{
		}

		// Close the current connection, if any
		try
		{
			Connection.close();
		}
		catch (Exception e)
		{
		}
	}
}
