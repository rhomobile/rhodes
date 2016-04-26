package com.rhomobile.printing.zebra.impl;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.logging.Level;
import java.util.logging.Logger;

import android.os.Looper;


public class ZebraTransactionManager {
	
	private static Thread mCommandThread = null;
	
	private static LinkedList<Runnable> mCommands = null;
	
	private static Runnable currentCommand = null;
	
	public static void init() {
		mCommands = new LinkedList<Runnable>();
		mCommandThread = new Thread( new Runnable() {
			public void run() {
				Looper.prepare();
				try {
					while (true) {
						currentCommand = null;
						try {
							synchronized(mCommands) {
								if (mCommands.size() > 0) {
									currentCommand = mCommands.get(0);
									mCommands.remove(0);
								}
							}
							if (currentCommand != null) {
								currentCommand.run();
							}
							else {
								try {
									Thread.sleep(25);
								} catch (InterruptedException e) {
								}
							}
						}
						catch (Exception e) {
							e.printStackTrace();
							Logger.getLogger("ZebraTransactionManager").log(Level.SEVERE, "ERROR: Exception during execute command : "+e.getMessage());
						}
						currentCommand = null;
					}
				}
				finally {
					Looper.myLooper().quit();
				}
			}
		});
		mCommandThread.start();
	}

	
	public static void addCommand(Runnable command) {
		if (mCommandThread == null) {
			init();
		}
		synchronized(mCommands) {
			mCommands.add(mCommands.size(), command);
		}
	}

	public static void addCommandBeforeAll(Runnable command) {
		if (mCommandThread == null) {
			init();
		}
		synchronized(mCommands) {
			mCommands.add(0, command);
		}
	}
	
	public static boolean hasContainCommandsWithClass(Class c) {
		boolean contain = false;
		synchronized(mCommands) {
			Iterator<Runnable> iter = mCommands.iterator();
			while (iter.hasNext()) {
				Runnable r = iter.next();
				if (c.isAssignableFrom(r.getClass())) {
					contain = true;
				}
			}
			if (currentCommand != null) {
				if (c.isAssignableFrom(currentCommand.getClass())) {
					contain = true;
				}
			}
		}
		return contain;
	}
	
	
}
