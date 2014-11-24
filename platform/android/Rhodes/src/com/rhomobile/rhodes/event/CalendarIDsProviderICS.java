package com.rhomobile.rhodes.event;

import android.net.Uri;
import android.provider.CalendarContract;
import android.provider.CalendarContract.Events;

public class CalendarIDsProviderICS implements ICalendarIDsProvider {
	public Uri.Builder getUriBuilder()		{ return CalendarContract.CONTENT_URI.buildUpon(); }
	
	public String getEventsID()				{ return Events._ID; }
	public String getEventsEventID()		{ return "event_id"; }
	public String getEventsTitle()			{ return Events.TITLE; }
	public String getEventsStartDate()		{ return Events.DTSTART; }
	public String getEventsEndDate()		{ return Events.DTEND; }
	public String getEventsLocation()		{ return Events.EVENT_LOCATION; }
	public String getEventsNotes()			{ return Events.DESCRIPTION; }
	public String getEventsPrivacy()		{ return Events.ACCESS_LEVEL; }
	public String getEventsDeleted()		{ return Events.DELETED; }
	public String getEventsDuration()		{ return Events.DURATION; }
	public String getEventsBegin()			{ return "begin"; }
	public String getEventsEnd()			{ return "end"; }
	public String getEventsRrule()			{ return Events.RRULE; }
	
	public String getCalendarName()			{ return CalendarContract.Calendars.NAME; }
	public String getCalendarDisplayName()	{ return CalendarContract.Calendars.CALENDAR_DISPLAY_NAME; }
	public String getCalendarColor()		{ return CalendarContract.Calendars.CALENDAR_COLOR; }
	public String getCalendarAccessLevel()	{ return CalendarContract.Calendars.CALENDAR_ACCESS_LEVEL; }
}