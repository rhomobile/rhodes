/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.event;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;
import java.util.Vector;

import android.database.Cursor;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.net.Uri;
import android.os.Build;
import android.provider.CalendarContract.Events;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.event.Event;
import com.rhomobile.rhodes.osfunctionality.OsVersionManager;

public class EventStore {
    
    static {
        OsVersionManager.registerSelector(ICalendarIDsProvider.class, CalendarIDsProviderBase.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.FROYO, ICalendarIDsProvider.class, CalendarIDsProviderFroyo.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.ICE_CREAM_SANDWICH, ICalendarIDsProvider.class, CalendarIDsProviderICS.class.getCanonicalName());
    }
	
	private static final String TAG = "EventStore";
	
	private static final ICalendarIDsProvider mIDsProvider = OsVersionManager.getFeature(ICalendarIDsProvider.class);
	static ICalendarIDsProvider getIDsProvider() { return mIDsProvider; }
	
    private static final Uri CALENDARS_URI = getIDsProvider().getUriBuilder().path("/calendars").build();
	private static final Uri EVENTS_URI = getIDsProvider().getUriBuilder().path("/events").build();
	
	private static final String EVENTS_ID = getIDsProvider().getEventsID();
    private static final String EVENTS_EVENT_ID = getIDsProvider().getEventsEventID();
	private static final String EVENTS_TITLE = getIDsProvider().getEventsTitle();
	private static final String EVENTS_START_DATE = getIDsProvider().getEventsStartDate();
	private static final String EVENTS_END_DATE = getIDsProvider().getEventsEndDate();
	private static final String EVENTS_LOCATION = getIDsProvider().getEventsLocation();
	private static final String EVENTS_NOTES = getIDsProvider().getEventsNotes();
	private static final String EVENTS_PRIVACY = getIDsProvider().getEventsPrivacy();
	private static final String EVENTS_DELETED = getIDsProvider().getEventsDeleted();
	private static final String EVENTS_DURATION = getIDsProvider().getEventsDuration();
	private static final String EVENTS_BEGIN = getIDsProvider().getEventsBegin();
	private static final String EVENTS_END = getIDsProvider().getEventsEnd();
	private static final String EVENTS_RRULE = getIDsProvider().getEventsRrule();

    private static final String DATE_FORMAT_TRACE = "yyyy-MM-dd HH:mm:ss z";

    private static String dateToString(Date date) {
        if (null == date) return "null";
        
        return new SimpleDateFormat(DATE_FORMAT_TRACE).format(date);
    }

	private static void checkCapabilities() throws IllegalAccessException {
		if (!Capabilities.CALENDAR_ENABLED)
			throw new IllegalAccessException("Capability CALENDAR disabled");
	}

	public static boolean hasCalendar() {
		if (!Capabilities.CALENDAR_ENABLED) {
			Logger.E(TAG, "Calendar capability is not enabled !!!");
			return false;
		}
        if( getContentResolver().acquireContentProviderClient(getIDsProvider().getUriBuilder().build()) != null) {
            Logger.T(TAG, "Found calendar provider for: " + getIDsProvider().getUriBuilder().build());
            return true;
        }
		return false;
	}
	
	private static ContentResolver getContentResolver() {
		return RhodesService.getInstance().getContentResolver();
	}
	
	private static long getDefaultCalendarId() {
		final Cursor calendarCursor = getContentResolver().query(
		        CALENDARS_URI,
				new String[] {EVENTS_ID},
				null, null, null);
		try {
			if (!calendarCursor.moveToFirst())
				throw new RuntimeException("No calendars found!");
			long id = calendarCursor.getLong(0);
			return id;
		}
		finally {
			calendarCursor.close();
		}
	}
	
	private static void createCalendar() {
		ContentValues values = new ContentValues();
		
		values.put( getIDsProvider().getCalendarName()			, "Rho Calendar");
		//values.put( mIDsProvider.getCalendarDisplayName()	, "Rho Calendar");
		//values.put( mIDsProvider.getCalendarColor()		, "000000");
		//values.put( mIDsProvider.getCalendarAccessLevel()	, "700");
		
		getContentResolver().insert(CALENDARS_URI, values);
		
	}


    static Event fetchEvent(Cursor cursor, boolean expandRecurrency) {
        String eid = cursor.getString(cursor.getColumnIndex(expandRecurrency ? EVENTS_EVENT_ID : EVENTS_ID));
        Event event = new Event(eid);

        long longDtStart = cursor.getLong(cursor.getColumnIndex(expandRecurrency ? EVENTS_BEGIN : EVENTS_START_DATE));
        Date startDate = 0 < longDtStart ? new Date(longDtStart) : null;

        long longDtEnd = cursor.getLong(cursor.getColumnIndex(expandRecurrency ? EVENTS_END : EVENTS_END_DATE));
        Date endDate = 0 < longDtEnd ? new Date(longDtEnd) : null;
        String duration = null;

        if (endDate == null && !expandRecurrency) {
            event.startDate = startDate;

            duration = cursor.getString(cursor.getColumnIndex(EVENTS_DURATION));
            Logger.D(TAG, "Event duration: " + duration);
            event.parseDuration(duration);

        } else {
            if (null != startDate && null != endDate && startDate.after(endDate)) {
                Date tmp = startDate;
                startDate = endDate;
                endDate = tmp;
            }
            event.startDate = startDate;
            event.endDate = endDate;
        }

        event.title = cursor.getString(cursor.getColumnIndex(EVENTS_TITLE));
        event.location = cursor.getString(cursor.getColumnIndex(EVENTS_LOCATION));
        event.notes = cursor.getString(cursor.getColumnIndex(EVENTS_NOTES));
        switch (cursor.getInt(cursor.getColumnIndex(EVENTS_PRIVACY))) {
            case 1: event.privacy = "confidential"; break;
            case 2: event.privacy = "private"; break;
            case 3: event.privacy = "public"; break;
        }

        if (!expandRecurrency) {
            String rrule = cursor.getString(cursor.getColumnIndex(EVENTS_RRULE));
            Logger.D(TAG, "Event recurrence rule: " + rrule);
            event.parseRrule(rrule);
        }

        Logger.D(TAG, event.toString());

        return event;
    }

	public static Object fetch(Date startDate, Date endDate, boolean expandRecurrency) {
		try {
			checkCapabilities();
			
			Logger.D(TAG, "fetch(start, end), start: " + dateToString(startDate) + ", end: " + dateToString(endDate)
					+ ", includeRepeating: " + expandRecurrency);
			
			Vector<Event> ret = new Vector<Event>();
			
			ContentResolver r = getContentResolver();
			
			Cursor eventCursor;
			if (expandRecurrency) {
				Uri.Builder builder = getIDsProvider().getUriBuilder().path("/instances/when");
				ContentUris.appendId(builder, startDate.getTime());
				ContentUris.appendId(builder, endDate.getTime());
				
				eventCursor = r.query(builder.build(),
						new String[] {"event_id", EVENTS_TITLE, EVENTS_BEGIN, EVENTS_END, EVENTS_LOCATION,
							EVENTS_NOTES, EVENTS_PRIVACY, EVENTS_DELETED},
						null, //"Calendars._id=" + id,
						null, "startDay ASC, startMinute ASC");
			}
			else {
				String where = String.format("(%s >= ? and %s <= ?) or (%s >= ? and %s <= ?)",
						EVENTS_START_DATE, EVENTS_START_DATE, EVENTS_END_DATE, EVENTS_END_DATE);
				String start = Long.toString(startDate.getTime());
				String end = Long.toString(endDate.getTime());
				eventCursor = r.query(EVENTS_URI,
					new String[] {EVENTS_ID, EVENTS_TITLE, EVENTS_START_DATE, EVENTS_END_DATE, EVENTS_DURATION,
						EVENTS_LOCATION, EVENTS_NOTES, EVENTS_PRIVACY, EVENTS_DELETED, EVENTS_RRULE},
					where, new String[] {start, end, start, end},
					null);
			}
			if (eventCursor == null)
				throw new RuntimeException("Calendar provider not found");
			try {
				while (eventCursor.moveToNext()) {

					int deleted_index = eventCursor.getColumnIndex(EVENTS_DELETED);
					if (deleted_index >= 0) {
						long is_deleted = eventCursor.getLong(deleted_index);
						if (is_deleted != 0) {
							continue;
						}
					}
					
					long longDtStart = eventCursor.getLong(eventCursor.getColumnIndex(expandRecurrency ? EVENTS_BEGIN : EVENTS_START_DATE));
					long longDtEnd = eventCursor.getLong(eventCursor.getColumnIndex(expandRecurrency ? EVENTS_END : EVENTS_END_DATE));
					
					Date eventStartDate = 0 < longDtStart ? new Date(longDtStart) : null;
					Date eventEndDate = 0 < longDtEnd ? new Date(longDtEnd) : null;
					
					if (null != eventStartDate && null != eventEndDate && eventStartDate.after(eventEndDate)) {
						Date tmp = eventStartDate;
						eventStartDate = eventEndDate;
						eventEndDate = tmp;
					}
					
					if (null != eventStartDate && null != eventEndDate
							&& (eventEndDate.before(startDate) || eventStartDate.after(endDate)))
						continue;
					
					Event event = fetchEvent(eventCursor, expandRecurrency);
					ret.add(event);
				}
			}
			finally {
				eventCursor.close();
			}
			
			return ret;
		}
		catch (Exception e) {
            Logger.E(TAG, e);
            return null;
		}
	}
	
	public static Object fetch(String id) {
		try {
			checkCapabilities();
			
			Logger.D(TAG, "fetch(id)");
			
			ContentResolver r = getContentResolver();
			
			Uri uri = ContentUris.withAppendedId(EVENTS_URI, Long.parseLong(id));
			final Cursor eventCursor = r.query(uri,
					new String[] {EVENTS_ID, EVENTS_TITLE, EVENTS_START_DATE, EVENTS_END_DATE,
						EVENTS_LOCATION, EVENTS_NOTES, EVENTS_PRIVACY, EVENTS_RRULE},
					null, null, null);
			if (eventCursor == null)
				throw new RuntimeException("Calendar provider not found");
			
			try {
				if (!eventCursor.moveToFirst()) {
					Logger.D(TAG, "fetch(id): result set is empty");
					return null;
				}
				
				Event event = fetchEvent(eventCursor, false);
				
				return event;
			}
			finally {
				eventCursor.close();
			}
		}
		catch (Exception e) {
            Logger.E(TAG, e);
            return null;
		}
	}
	
	public static String save(Event event) {
		String return_id = null;
		try {
			checkCapabilities();
			
			Logger.D(TAG, "save(event)");
			
			ContentValues values = new ContentValues();
			values.put(EVENTS_TITLE, event.title);
			
			if (null != event.startDate) {
				values.put(EVENTS_START_DATE, event.startDate.getTime());
			}
			
			if (null != event.endDate) {
				values.put(EVENTS_END_DATE, event.endDate.getTime());
			}
			
			if (event.location != null) {
				values.put(EVENTS_LOCATION, event.location);
			}

			if (event.notes !=  null) {
				values.put(EVENTS_NOTES, event.notes);
			}

			if (event.privacy != null) {
				int privacy = 0;
				if (event.privacy.equalsIgnoreCase("confidential"))
					privacy = 1;
				else if (event.privacy.equalsIgnoreCase("private"))
					privacy = 2;
				else if (event.privacy.equalsIgnoreCase("public"))
					privacy = 3;
				values.put(EVENTS_PRIVACY, privacy);
			}
			
			if (null != event.frequency && 0 < event.frequency.length()) {
				values.put(EVENTS_RRULE, event.getRrule());
				values.put(EVENTS_DURATION, event.getDuration());
			}
			
			if ( Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH ) {
				values.put(Events.EVENT_TIMEZONE, TimeZone.getDefault().getID());
			}
	/*		
			Logger.D(TAG, "Event: id: " + String.valueOf((Object)event.id) +
					", title: " + String.valueOf((Object)event.title) +
					", begin: " + dateToString(new Date(values.getAsLong(EVENTS_START_DATE).longValue())) +
					", end: " + dateToString(new Date(values.getAsLong(EVENTS_END_DATE).longValue())) +
					", freq: " + String.valueOf((Object)event.frequency) +
					", interval: " + Integer.toString(event.interval) +
					", recurrence end: " + String.valueOf((Object)event.recurrenceEnd) +
					", recurrence count: " + String.valueOf((Object)event.recurrenceTimes) +
					", rrule: " + String.valueOf((Object)values.get(EVENTS_RRULE)));
*/
			if ( !hasCalendar() ) {
				Logger.I(TAG,"No calendar exists, creating default calendar.");
				createCalendar();
			}
			
			long calendarId = getDefaultCalendarId();
			values.put("calendar_id", calendarId);
			
			ContentResolver r = getContentResolver();
					
			if (event.id == null || event.id.equalsIgnoreCase("")) {
				Logger.D(TAG, "Insert new event...");
				Uri euri = r.insert(EVENTS_URI, values);
				r.notifyChange(EVENTS_URI, null);
				event.id = Long.toString(ContentUris.parseId(euri));
				Logger.D(TAG, "Event id of event is " + event.id);
				return_id = event.id;
			}
			else {
				return_id = event.id;
				Logger.D(TAG, "Update event...");
				Uri uri = ContentUris.withAppendedId(EVENTS_URI, Long.parseLong(event.id));
				r.update(uri, values, null, null);
				r.notifyChange(EVENTS_URI, null);
				Logger.D(TAG, "Event updated");
			}
		}
		catch (Exception e) {
            Logger.E(TAG, e);
            return null;
		}
		return return_id;
	}
	
	public static String delete(String id) {
		try {
			checkCapabilities();
			
			Logger.D(TAG, "delete("+id+")");
			
			ContentResolver r = getContentResolver();
			int rows;
			if (Build.VERSION.SDK_INT < Build.VERSION_CODES.FROYO) {
				Logger.D(TAG, " use old delete scheme");
				Uri uri = ContentUris.withAppendedId(EVENTS_URI, Long.parseLong(id));
				rows = getContentResolver().delete(uri, null, null);
			}
			else {
				Logger.D(TAG, " use new delete scheme");
				//int idn = Integer.decode(id).intValue();
				Uri du = Uri.withAppendedPath(EVENTS_URI, id);
				rows = r.delete(du, null, null);
				//rows = r.delete(EVENTS_URI, EVENTS_ID+"=?", new String[] {id});
				r.notifyChange(EVENTS_URI, null);
			}
			Logger.D(TAG, String.format("%d rows deleted", rows));
		}
		catch (Exception e) {
            Logger.E(TAG, e);
        }
        return null;
    }
}
