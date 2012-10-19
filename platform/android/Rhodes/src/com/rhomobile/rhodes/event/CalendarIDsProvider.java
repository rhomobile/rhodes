package com.rhomobile.rhodes.event;

public class CalendarIDsProvider {
	public String getAuthority()			{ return "calendar"; }
	
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
}