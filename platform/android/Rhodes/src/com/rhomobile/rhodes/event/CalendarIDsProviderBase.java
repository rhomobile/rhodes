package com.rhomobile.rhodes.event;

import android.net.Uri;
import android.net.Uri.Builder;

public class CalendarIDsProviderBase implements ICalendarIDsProvider{
    public Builder getUriBuilder() { return new Uri.Builder().scheme("content").authority("calendar");
    }

	public String getEventsID()				{ return "_id"; }
	public String getEventsEventID()		{ return "event_id"; }
	public String getEventsTitle()			{ return "title"; }
	public String getEventsStartDate()		{ return "dtstart"; }
	public String getEventsEndDate()		{ return "dtend"; }
	public String getEventsLocation()		{ return "eventLocation"; }
	public String getEventsNotes()			{ return "description"; }
	public String getEventsPrivacy()		{ return "visibility"; }
	public String getEventsDeleted()		{ return "deleted"; }
	public String getEventsDuration()		{ return "duration"; }
	public String getEventsBegin()			{ return "begin"; }
	public String getEventsEnd()			{ return "end"; }
	public String getEventsRrule()			{ return "rrule"; }
	
	public String getCalendarName()			{ return "name"; }
	public String getCalendarDisplayName()	{ return "calendar_displayName"; }
	public String getCalendarColor()		{ return "calendar_color"; }
	public String getCalendarAccessLevel()	{ return "calendar_access_level"; }
}