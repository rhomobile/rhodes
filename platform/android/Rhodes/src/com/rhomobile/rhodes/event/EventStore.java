package com.rhomobile.rhodes.event;

import java.util.Date;
import java.util.Vector;

import com.rhomobile.rhodes.Logger;

public class EventStore {
	
	private static final String TAG = "EventStore";
	
	private static String dateToString(Date date) {
		return String.format("%04d-%02d-%02d %02d:%02d:%02d",
				date.getYear() + 1900, date.getMonth() + 1, date.getDate(),
				date.getHours(), date.getMinutes(), date.getSeconds());
	}

	public static Vector<Event> fetch(Date startDate, Date endDate) {
		Logger.D(TAG, "fetch(start, end), start: " + dateToString(startDate) + ", end: " + dateToString(endDate));
		
		Vector<Event> ret = new Vector<Event>();
		// TODO:
		return ret;
	}
	
	public static Event fetch(String id) {
		Logger.D(TAG, "fetch(id)");
		
		Event event = new Event();
		// TODO:
		return event;
	}
	
	public static void save(Event event) {
		Logger.D(TAG, "save(event)");
		// TODO:
	}
	
	public static void delete(String id) {
		Logger.D(TAG, "delete(id)");
		// TODO:
	}
	
}
