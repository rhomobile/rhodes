package com.rhomobile.rhodes.http;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Enumeration;
import java.util.Hashtable;

import com.rho.RhoRuby;
import com.rhomobile.rhodes.RhoRubyImpl;
import com.rhomobile.rhodes.RhodesInstance;

import android.util.Log;

/**
 * Threaded socket server. This class listens at a socket and gives the
 * connections received to a pool of Threads
 */
@SuppressWarnings("unchecked")
abstract public class ThreadedServer implements Runnable {

	private static int threadId = 0;
	private InetAddrPort address = null;
	ServerSocket listen = null;

	private Hashtable threadSet;
	private int maxThreads = 1;
	private int minThreads = 1;
	private int maxIdleTimeMs = 0;
	private String name = this.getClass().getSimpleName();
	private int accepting = 0;
	private boolean running = false;
	private Object syncObject = new Object();

	public ThreadedServer() {
	}

	/* ------------------------------------------------------------------- */
	/*
	 * Construct
	 */
	public ThreadedServer(String name) {
		this();
		this.name = name;
	}

	/**
	 * Construct for specific port
	 */
	public ThreadedServer(int port) throws java.io.IOException {
		this();
		setAddress(new InetAddrPort(null, port));
	}

	/**
	 * Construct for specific address and port
	 */
	public ThreadedServer(InetAddress address, int port)
			throws java.io.IOException {
		this();
		setAddress(new InetAddrPort(address, port));
	}

	/**
	 * Construct for specific address and port
	 */
	public ThreadedServer(InetAddrPort address) throws java.io.IOException {
		this();
		setAddress(address);
	}

	/**
	 * Constructor.
	 * 
	 * @param address
	 *            The address to listen on
	 * @param minThreads
	 *            Minimum number of handler threads.
	 * @param maxThreads
	 *            Maximum number of handler threads.
	 * @param maxIdleTime
	 *            Idle time in milliseconds before a handler thread dies.
	 * @exception java.io.IOException
	 *                Problem listening to the socket.
	 */
	public ThreadedServer(InetAddrPort address, int minThreads, int maxThreads,
			int maxIdleTime) throws java.io.IOException {
		this();
		this.minThreads = minThreads == 0 ? 1 : minThreads;
		this.maxThreads = maxThreads == 0 ? this.maxThreads : maxThreads;
		this.maxIdleTimeMs = maxIdleTime;

		setAddress(address);
	}

	/**
	 * Handle new connection This method should be overridden by the derived
	 * class
	 */
	protected void handleConnection(InputStream in, OutputStream out) {
		throw new Error("handlerConnection must be overridden");
	}

	/**
	 * Handle new connection If access is required to the actual socket,
	 * override this method
	 */
	protected void handleConnection(Socket connection) {
		try {
			InputStream in = connection.getInputStream();
			OutputStream out = connection.getOutputStream();

			handleConnection(in, out);
			out.flush();

			in = null;
			out = null;
		} catch (Exception e) {
			Log.e(this.name, "Connection problem: " + e.getMessage());
		} finally {
			try {
				connection.setSoLinger(false, 0);
			} catch (Exception e) {
				Log.e(this.name, e.getMessage());
			}

			try {
				connection.close();
			} catch (Exception e) {
				Log.e(this.name, "Connection problem" + e.getMessage());
			}
			connection = null;
		}
	}

	public InetAddrPort getInetAddrPort() {
		return address;
	}

	public InetAddress getInetAddress() {
		return address.getInetAddress();
	}

	public int getPort() {
		return address.getPort();
	}

	public int getSize() {
		return this.threadSet.size();
	}

	public int getMinSize() {
		return this.minThreads;
	}

	public int getMaxSize() {
		return this.maxThreads;
	}

	public int getMaxIdleTimeMs() {
		return this.maxIdleTimeMs;
	}

	public synchronized void setAddress(InetAddress address, int port)
			throws java.io.IOException {
		setAddress(new InetAddrPort(address, port));
	}

	public synchronized void setAddress(InetAddrPort address)
			throws java.io.IOException {
		this.address = address;
		if (this.threadSet != null && this.running) {
			Log.d(this.getClass().getSimpleName(), "Restart for " + address);
			stop();
			start();
		}
	}

	/**
	 * Accept socket connection.
	 * 
	 * @param serverSocket
	 * @return Accepted Socket
	 * @exception java.io.IOException
	 */
	protected Socket accept(ServerSocket serverSocket)
			throws java.io.IOException {
		Socket socket = serverSocket.accept();
		if (this.maxIdleTimeMs > 0)
			socket.setSoTimeout(this.maxIdleTimeMs);
		return socket;
	}

	synchronized public void start() throws java.io.IOException {
		if (listen != null) {
			Log.d(this.name, "Already started on " + address);
			return;
		}

		Log.d(this.name, "Start Listener for " + address);

		listen = newServerSocket(address, maxThreads > 0 ? (maxThreads + 1)
				: maxThreads);

		address = new InetAddrPort(listen.getInetAddress(), listen
				.getLocalPort());

		// Set any idle timeout
		if (this.maxIdleTimeMs > 0)
			listen.setSoTimeout(this.maxIdleTimeMs);
		this.accepting = 0;

		// Start the threads
		this.running = true;
		this.threadSet = new Hashtable(this.maxThreads + this.maxThreads / 2
				+ 13);
		for (int i = 0; i < this.minThreads; i++)
			newThread();
	}

	protected ServerSocket newServerSocket(InetAddrPort address,
			int acceptQueueSize) throws java.io.IOException {
		if (address == null)
			return new ServerSocket(0, acceptQueueSize);

		return new ServerSocket(address.getPort(), acceptQueueSize, address
				.getInetAddress());
	}

	private synchronized void newThread() {
		try {
			Thread thread = new Thread(this);

			synchronized (syncObject) {
				thread.setName(this.name + "-" + (threadId++));
			}

			this.threadSet.put(thread, thread);
			thread.start();
		} catch (Exception e) {
			Log.e(this.name, e.getMessage());
		}
	}

	synchronized public void stop() {
		Log.d(this.name, "Stop listening on " + listen);

		if (this.threadSet == null)
			return;

		this.running = false;

		// interrupt the threads
		Enumeration enumThreads = this.threadSet.keys();
		while (enumThreads.hasMoreElements()) {
			Thread thread = (Thread) enumThreads.nextElement();
			thread.interrupt();
		}

		// wait a while for all threads to die
		try {
			long end_wait = System.currentTimeMillis() + 5000;
			while (this.threadSet.size() > 0
					&& end_wait > System.currentTimeMillis())
				this.wait(1000);

			// Stop any still running
			if (this.threadSet.size() > 0) {
				enumThreads = this.threadSet.keys();
				while (enumThreads.hasMoreElements()) {
					Thread thread = (Thread) enumThreads.nextElement();
					if (thread.isAlive())
						thread.stop();
				}

				// wait until all threads are dead.
				while (this.threadSet.size() > 0) {
					Log.d(this.name, "waiting for threads to stop...");
					this.wait(1000);
				}
			}
		} catch (Exception e) {
			Log.w(this.name, e.getMessage());
		}

		this.threadSet.clear();
		this.threadSet = null;

		// Close the port
		if (listen != null) {
			try {
				try {
					listen.setSoTimeout(1);
				} catch (Exception e) {
					Log.e(this.name, e.getMessage());
				}

				listen.close();
			} catch (Exception e) {
				Log.e(this.name, e.getMessage());
			}

			listen = null;
		}
	}

	final public void join() throws java.lang.InterruptedException {
		while (this.threadSet != null && this.threadSet.size() > 0) {
			Thread thread = null;
			synchronized (this) {
				Enumeration enumThreads = this.threadSet.keys();
				if (enumThreads.hasMoreElements())
					thread = (Thread) enumThreads.nextElement();
			}
			if (thread != null)
				thread.join();
		}
	}

	final public void run() {
		Thread thread = Thread.currentThread();
		String name = thread.getName();
		int runs = 0;

		Log.d(this.name, "RhoRubyStart...");
		RhoRuby.setRhoRubyImpl(new RhoRubyImpl());
		RhoRuby.RhoRubyStart("");

		Log.d(this.name, "startSyncEngine...");
		RhodesInstance.getInstance().startSyncEngine();
		
		Log.d(this.name, "Listen on " + listen);

		try {
			while (this.running) {
				Socket connection = null;

				// Accept an incoming connection
				try {
					// increment accepting count
					synchronized (this) {
						this.accepting++;
					}

					// wait for a connection
					connection = accept(listen);
				} catch (Exception e) {
					//Log.d(this.name, e.getMessage());

					synchronized (this) {
						// If we are still running, interrupt was due to accept
						// timeout
						//Log.d(this.name, "Threads=" + this.threadSet.size());

						if (this.running
								&& this.threadSet.size() > this.minThreads) {
							// Kill thread if it is in excess of the minimum.
							Log.d(this.name, "Idle death: " + thread);
							this.threadSet.remove(thread);
							break;
						}
					}
				} finally {
					// If not more threads accepting and this
					// thread is not idle - start a new thread.
					synchronized (this) {
						if (--this.accepting == 0 && this.running
								&& connection != null
								&& this.threadSet.size() < this.maxThreads) {
							Log.d(this.name, "New Thread");
							newThread();
						}
					}
				}

				// handle the connection
				if (connection != null) {
					try {
						thread.setName(name + "/" + runs++);
						Log.d(this.name, "Handling " + connection);

						handleConnection(connection);

						try {
							connection.setSoLinger(false, 0);
						} catch (Exception e) {
							Log.e(this.name, e.getMessage());
						}

						connection.close();
					} catch (Exception e) {
						Log.w(this.name, e.getMessage());
					} finally {
						connection = null;
					}
				}
			}
		} finally {
			synchronized (this) {
				if (this.threadSet != null)
					this.threadSet.remove(thread);
				this.notify();
			}

			if (this.threadSet != null)
				Log.d(this.name, "Stopped listening... \nthreads=" + this.threadSet.size());
		}
	}
}
