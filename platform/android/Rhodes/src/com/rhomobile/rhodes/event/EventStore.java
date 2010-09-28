package com.rhomobile.rhodes.event;

import java.util.Date;
import java.util.Vector;

import android.database.Cursor;
import android.content.ContentProviderClient;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.net.Uri;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesService;

public class EventStore {
	
	private static final String TAG = "EventStore";
	
	//private static final String CALENDAR_PROVIDER = "com.android.calendar";
	private static final String AUTHORITY = "com.android.calendar";
	//private static final Uri CONTENT_URI = Uri.parse("content://com.android.calendar");
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
		e.printStackTrace();
	}
	
	private static String dateToString(Date date) {
		return String.format("%04d-%02d-%02d %02d:%02d:%02d",
				date.getYear() + 1900, date.getMonth() + 1, date.getDate(),
				date.getHours(), date.getMinutes(), date.getSeconds());
	}
	
	private static void checkCapabilities() throws IllegalAccessException {
		if (!Capabilities.CALENDAR_ENABLED)
			throw new IllegalAccessException("Capability CALENDAR disabled");
	}
	
	private static ContentResolver getContentResolver() {
		return RhodesService.getInstance().getContext().getContentResolver();
	}
	
	private static Cursor performFetch(long start, long end, String[] projection,
			String selection, String[] selectionArgs, String sortOrder) {
		
		ContentResolver r = getContentResolver();
		
		Uri.Builder builder = Uri.parse("content://" + AUTHORITY + "/instances/when").buildUpon();
		ContentUris.appendId(builder, start);
		ContentUris.appendId(builder, end);
		
		Uri uri = builder.build();
		Logger.D(TAG, "Query uri: " + uri.toString());
		
		return r.query(uri, projection, selection, selectionArgs, sortOrder);
	}
	
	public static Vector<Event> fetch(Date startDate, Date endDate) {
		try {
			checkCapabilities();
			
			Logger.D(TAG, "fetch(start, end), start: " + dateToString(startDate) + ", end: " + dateToString(endDate));
			
			Vector<Event> ret = new Vector<Event>();
			
			final Cursor calendarCursor = getContentResolver().query(
					Uri.parse("content://" + AUTHORITY + "/calendars"),
					new String[] {"_id", "displayName", "selected"},
					null, null, null);
			try {
				while (calendarCursor.moveToNext()) {
					String id = calendarCursor.getString(calendarCursor.getColumnIndexOrThrow("_id"));
					String displayName = calendarCursor.getString(calendarCursor.getColumnIndexOrThrow("displayName"));
					boolean selected = !calendarCursor.getString(calendarCursor.getColumnIndexOrThrow("selected")).equals("0");
					
					Logger.D(TAG, "Calendar: id: " + id + ", name: " + displayName + ", selected: " + selected);
					
					final Cursor eventCursor = performFetch(startDate.getTime(), endDate.getTime(),
							new String[] {"_id", "title", "begin", "end"},
							"Calendars._id=" + id,
							null, "startDay ASC, startMinute ASC");
					try {
						while (eventCursor.moveToNext()) {
							String eid = eventCursor.getString(eventCursor.getColumnIndexOrThrow("_id"));
							Event event = new Event(eid);
							
							event.title = eventCursor.getString(eventCursor.getColumnIndexOrThrow("title"));
							event.startDate = new Date(eventCursor.getLong(eventCursor.getColumnIndexOrThrow("begin")));
							event.endDate = new Date(eventCursor.getLong(eventCursor.getColumnIndexOrThrow("end")));
							
							Logger.D(TAG, "Event: id: " + event.id +
									", title: " + event.title +
									", begin: " + dateToString(event.startDate) +
									", end: " + dateToString(event.endDate));
							
							ret.add(event);
						}
					}
					finally {
						eventCursor.close();
					}
				}
			}
			finally {
				calendarCursor.close();
			}
			
			return ret;
		}
		catch (Exception e) {
			reportFail("fetch(start, end)", e);
			return null;
		}
	}
	
	public static Event fetch(String id) {
		try {
			checkCapabilities();
			
			Logger.D(TAG, "fetch(id)");
			
			Uri uri = ContentUris.withAppendedId(Uri.parse("content://" + AUTHORITY + "/events"), Long.parseLong(id));
			final Cursor eventCursor = getContentResolver().query(uri,
					new String[] {"title", "dtstart", "dtend"},
					null, null, null);
			try {
				Event event = new Event(id);
				event.title = eventCursor.getString(eventCursor.getColumnIndexOrThrow("title"));
				event.startDate = new Date(eventCursor.getLong(eventCursor.getColumnIndexOrThrow("dtstart")));
				event.endDate = new Date(eventCursor.getLong(eventCursor.getColumnIndexOrThrow("dtend")));
				
				return event;
			}
			finally {
				eventCursor.close();
			}
		}
		catch (Exception e) {
			reportFail("fetch(id)", e);
			return null;
		}
	}
	
	public static void save(Event event) {
		try {
			checkCapabilities();
			
			Logger.D(TAG, "save(event)");
			// TODO:
		}
		catch (Exception e) {
			reportFail("save", e);
		}
	}
	
	public static void delete(String id) {
		try {
			checkCapabilities();
			
			Logger.D(TAG, "delete(id)");
			// TODO:
		}
		catch (Exception e) {
			reportFail("delete", e);
		}
	}
	
}
