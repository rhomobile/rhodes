package com.rho.rubyext;

import java.util.Enumeration;
import java.util.Hashtable;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.Version;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;

import javax.microedition.pim.*;

//@RubyLevelClass(name="Calendar")
public class RhoCalendar extends RubyBasic {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Calendar");

	private EventList m_eventList;

	static final RubyString RUBY_EV_ID  = ObjectFactory.createString("id");
	static final RubyString RUBY_EV_TITLE = ObjectFactory.createString("title");
	static final RubyString RUBY_EV_CANCELED = ObjectFactory.createString("canceled");
	static final RubyString RUBY_EV_ORGINIZER = ObjectFactory.createString("organizer");
	static final RubyString RUBY_EV_ATTENDEES = ObjectFactory.createString("attendees");
	static final RubyString RUBY_EV_START_DATE = ObjectFactory.createString("start_date");
	static final RubyString RUBY_EV_END_DATE = ObjectFactory.createString("end_date");
	static final RubyString RUBY_EV_LAST_MODIFIED = ObjectFactory.createString("last_modified");
	static final RubyString RUBY_EV_LOCATION  = ObjectFactory.createString( "location");
	static final RubyString RUBY_EV_NOTES  = ObjectFactory.createString( "notes");
	static final RubyString RUBY_EV_REMINDER  = ObjectFactory.createString( "reminder");
	static final RubyString RUBY_EV_PRIVACY  = ObjectFactory.createString( "privacy");	
	static final RubyString RUBY_EV_RECURRENCE  = ObjectFactory.createString( "recurrence");
	static final RubyString RUBY_EV_RECURRENCE_FREQUENCY  = ObjectFactory.createString( "frequency");
	static final RubyString RUBY_EV_RECURRENCE_FREQUENCY_DAILY  = ObjectFactory.createString( "daily");
	static final RubyString RUBY_EV_RECURRENCE_FREQUENCY_WEEKLY  = ObjectFactory.createString( "weekly");
	static final RubyString RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY  = ObjectFactory.createString( "monthly");
	static final RubyString RUBY_EV_RECURRENCE_FREQUENCY_YEARLY  = ObjectFactory.createString( "yearly");
	static final RubyString RUBY_EV_RECURRENCE_INTERVAL  = ObjectFactory.createString( "interval");
	static final RubyString RUBY_EV_RECURRENCE_COUNT  = ObjectFactory.createString( "count");
	static final RubyString RUBY_EV_RECURRENCE_END  = ObjectFactory.createString( "end_date");
	static final RubyString RUBY_EV_RECURRENCE_MONTHS  = ObjectFactory.createString( "months");	
	static final RubyString RUBY_EV_RECURRENCE_WEEKS  = ObjectFactory.createString( "weeks");	
	static final RubyString RUBY_EV_RECURRENCE_DAYS  = ObjectFactory.createString( "days");	
	
	static final RubySymbol RUBY_FIND_type = ObjectFactory.createSymbol("find_type");
	static final RubySymbol RUBY_FIND_include_repeating = ObjectFactory.createSymbol("include_repeating");
	static final RubySymbol RUBY_FIND_start_date = ObjectFactory.createSymbol("start_date");
	static final RubySymbol RUBY_FIND_end_date = ObjectFactory.createSymbol("end_date");
	
	public static class EVRecord extends RubyBasic {

		Event m_event;
		public EVRecord(RubyClass arg0) {
			super(arg0);
		}

		public EVRecord(Event event) {
			super(RubyRuntime.PBRecordClass);
			
			m_event = event;
		}
		
	}
	
	public RhoCalendar(RubyClass arg0) throws Exception
	{
		super(arg0);
		
		m_eventList = (EventList) PIM.getInstance().openPIMList(
				PIM.EVENT_LIST, PIM.READ_WRITE);
	}
	
    //@RubyAllocMethod
    public static RhoCalendar alloc(RubyValue receiver)throws Exception 
    {
    	return new RhoCalendar(RubyRuntime.CalendarClass);
    }

	public static RubyValue openCalendar()throws Exception 
	{
		RhoCalendar pb = alloc(null);
		return pb;
	}
	
	public static RubyValue closeCalendar(RubyValue arg0)throws Exception 
	{
		RhoCalendar pb = (RhoCalendar)arg0;
		pb.m_eventList.close();
		pb.m_eventList = null;
		return RubyConstant.QNIL;
	}

	RubyHash getEVRecord( Event event )
	{
		RubyHash record = ObjectFactory.createHash();
		int[] arFields = event.getFields();
		for ( int i = 0; i < arFields.length; i++)
		{
			switch(arFields[i])
			{
			case Event.UID:
				record.add(RUBY_EV_ID, ObjectFactory.createString(
						"{" + event.getString(Event.UID, 0) + "}" ));
				break;
			
			case Event.SUMMARY:
				record.add(RUBY_EV_TITLE, ObjectFactory.createString(event.getString(Event.SUMMARY, 0)));
				break;
			
			case Event.NOTE:
				record.add(RUBY_EV_NOTES, ObjectFactory.createString(event.getString(Event.NOTE, 0)));
				break;
				
			case Event.ALARM:
				record.add(RUBY_EV_REMINDER, ObjectFactory.createInteger(event.getInt(Event.ALARM, 0)));
				break;

			case Event.CLASS:
			{
				int nClass = event.getInt(Event.CLASS, 0);
				String strValue = "public";
				if ( nClass == Event.CLASS_CONFIDENTIAL )
					strValue = "confidential";
				else if ( nClass == Event.CLASS_PRIVATE )
					strValue = "private";
				
				record.add(RUBY_EV_PRIVACY, ObjectFactory.createString(strValue));
				break;
			}
		
			case Event.END:
				record.add(RUBY_EV_END_DATE, ObjectFactory.createTime(event.getDate(Event.END, 0)));	
				break;
			case Event.START:
				record.add(RUBY_EV_START_DATE, ObjectFactory.createTime(event.getDate(Event.START, 0)));	
				break;
			case Event.REVISION:
				record.add(RUBY_EV_LAST_MODIFIED, ObjectFactory.createTime(event.getDate(Event.REVISION, 0)));
				break;
				
			case Event.LOCATION:
				record.add(RUBY_EV_LOCATION, ObjectFactory.createString(event.getString(Event.LOCATION, 0)));
				break;

			}
		}
		
		RepeatRule rl = event.getRepeat();
		if ( rl != null )
		{
			RubyHash recurrent = ObjectFactory.createHash();
			arFields = rl.getFields();
			for ( int i = 0; i < arFields.length; i++)
			{
				switch(arFields[i])
				{
				case RepeatRule.COUNT:
					recurrent.add(RUBY_EV_RECURRENCE_COUNT, ObjectFactory.createInteger(rl.getInt(RepeatRule.COUNT)));
					break;
				case RepeatRule.END:
					recurrent.add(RUBY_EV_RECURRENCE_END, ObjectFactory.createTime(rl.getDate(RepeatRule.END)));
					break;
				case RepeatRule.FREQUENCY:
				{
					RubyString strValue = RUBY_EV_RECURRENCE_FREQUENCY_DAILY;
					int nFreq = rl.getInt(RepeatRule.FREQUENCY);
					if ( nFreq == RepeatRule.WEEKLY )
						strValue = RUBY_EV_RECURRENCE_FREQUENCY_WEEKLY;
					else if ( nFreq == RepeatRule.YEARLY )
						strValue = RUBY_EV_RECURRENCE_FREQUENCY_YEARLY;
					else if ( nFreq == RepeatRule.MONTHLY )
						strValue = RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY;
					
					recurrent.add(RUBY_EV_RECURRENCE_FREQUENCY, strValue);
					break;
				}

				case RepeatRule.INTERVAL:
					recurrent.add(RUBY_EV_RECURRENCE_INTERVAL, ObjectFactory.createInteger(rl.getInt(RepeatRule.INTERVAL)));
					break;
				case RepeatRule.MONTH_IN_YEAR:
				{
					Version.SoftVersion ver = Version.getSoftVersion();
					//THIS IS BUG in BB < 5.0 : return month -1. January just crash get events
					int[] arMonths = {RepeatRule.JANUARY, RepeatRule.FEBRUARY, RepeatRule.MARCH, RepeatRule.APRIL, RepeatRule.MAY, RepeatRule.JUNE, RepeatRule.JULY,
							RepeatRule.AUGUST, RepeatRule.SEPTEMBER, RepeatRule.OCTOBER, RepeatRule.NOVEMBER, RepeatRule.DECEMBER};
					int nMonths = rl.getInt(RepeatRule.MONTH_IN_YEAR); 
					RubyArray months = ObjectFactory.createArray(arMonths.length, ObjectFactory.createInteger(0));
					for ( int m = 0; m < arMonths.length; m++ )
					{
						if ( (nMonths&arMonths[m]) != 0 )
							months.set(m + (ver.nMajor < 5 ? 1 : 0), ObjectFactory.createInteger(1));
					}
					recurrent.add( RUBY_EV_RECURRENCE_MONTHS, months);
					break;
					
				}
				case RepeatRule.WEEK_IN_MONTH:
				{
					int[] arWeeks = {RepeatRule.FIRST, RepeatRule.SECOND, RepeatRule.THIRD, RepeatRule.FOURTH, RepeatRule.FIFTH};
					int nWeeks = rl.getInt(RepeatRule.WEEK_IN_MONTH); 
					RubyArray weeks = ObjectFactory.createArray(arWeeks.length, ObjectFactory.createInteger(0));
					for ( int m = 0; m < arWeeks.length; m++ )
					{
						if ( (nWeeks&arWeeks[m]) != 0 )
							weeks.set(m, ObjectFactory.createInteger(1));
					}
					recurrent.add( RUBY_EV_RECURRENCE_WEEKS, weeks);
					break;
				}
				case RepeatRule.DAY_IN_WEEK:
				{
					int[] arDays = {RepeatRule.MONDAY, RepeatRule.TUESDAY, RepeatRule.WEDNESDAY, RepeatRule.THURSDAY, RepeatRule.FRIDAY, RepeatRule.SATURDAY, RepeatRule.SUNDAY};
					int nDays = rl.getInt(RepeatRule.DAY_IN_WEEK); 
					RubyArray days = ObjectFactory.createArray(arDays.length, ObjectFactory.createInteger(0));
					for ( int m = 0; m < arDays.length; m++ )
					{
						if ( (nDays&arDays[m]) != 0 )
							days.set(m, ObjectFactory.createInteger(1));
					}
					recurrent.add( RUBY_EV_RECURRENCE_DAYS, days);
					break;
				}
				}
			}
			
			record.add( RUBY_EV_RECURRENCE, recurrent); 
		}
		
		return record;
	}

	public static RubyValue setEventValue(RubyArray ar) 
	{
		if ( ar.get(0) == RubyConstant.QNIL || ar.get(0) == null )
			return RubyConstant.QFALSE;
		
		EVRecord record = (EVRecord)ar.get(0);
		RubyString strKey = ar.get(1).toRubyString();
		RubyValue val = ar.get(2);
		Event event = record.m_event;
		
		if ( strKey.opEql(RUBY_EV_ID) == RubyConstant.QTRUE )
			return RubyConstant.QFALSE;

		if ( strKey.opEql(RUBY_EV_TITLE) == RubyConstant.QTRUE )
			setStringField(event, Event.SUMMARY, val );
		else if ( strKey.opEql(RUBY_EV_NOTES) == RubyConstant.QTRUE )
			setStringField(event, Event.NOTE, val );
		else if ( strKey.opEql(RUBY_EV_REMINDER) == RubyConstant.QTRUE )
			setIntField(event, Event.ALARM, val );
		else if ( strKey.opEql(RUBY_EV_PRIVACY) == RubyConstant.QTRUE )
		{
			String strValue = val.toStr();
			int nClass = Event.CLASS_PUBLIC;
			if ( strValue.equalsIgnoreCase("confidential"))
				nClass = Event.CLASS_CONFIDENTIAL;
			else if ( strValue.equalsIgnoreCase("private"))
				nClass = Event.CLASS_PRIVATE;
			
			setIntField(event, Event.CLASS, ObjectFactory.createInteger(nClass) );
		}else if ( strKey.opEql(RUBY_EV_END_DATE) == RubyConstant.QTRUE )
			setDateField(event, Event.END, val );
		else if ( strKey.opEql(RUBY_EV_START_DATE) == RubyConstant.QTRUE )
			setDateField(event, Event.START, val );
		else if ( strKey.opEql(RUBY_EV_LAST_MODIFIED) == RubyConstant.QTRUE )
			setDateField(event, Event.REVISION, val );
		else if ( strKey.opEql(RUBY_EV_LOCATION) == RubyConstant.QTRUE )
			setStringField(event, Event.LOCATION, val );
		else if ( strKey.opEql(RUBY_EV_RECURRENCE) == RubyConstant.QTRUE )
		{
			RubyHash recurrent = (RubyHash)val;
			RepeatRule rl = new RepeatRule(); 
			if ( recurrent.has_key(RUBY_EV_RECURRENCE_COUNT) == RubyConstant.QTRUE )
				rl.setInt(RepeatRule.COUNT, recurrent.get(RUBY_EV_RECURRENCE_COUNT).toInt());
			if ( recurrent.has_key(RUBY_EV_RECURRENCE_END) == RubyConstant.QTRUE )
				rl.setDate(RepeatRule.END, recurrent.get(RUBY_EV_RECURRENCE_END).toRubyTime().getTime());
			if ( recurrent.has_key(RUBY_EV_RECURRENCE_FREQUENCY) == RubyConstant.QTRUE )
			{
				RubyString strValue = (RubyString)recurrent.get(RUBY_EV_RECURRENCE_FREQUENCY); 
				int nFreq = RepeatRule.WEEKLY;
				if ( strValue.opEql(RUBY_EV_RECURRENCE_FREQUENCY_YEARLY) == RubyConstant.QTRUE   )
					nFreq = RepeatRule.YEARLY;
				else if ( strValue.opEql(RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY)  == RubyConstant.QTRUE )
					nFreq = RepeatRule.MONTHLY;
				else if ( strValue.opEql(RUBY_EV_RECURRENCE_FREQUENCY_DAILY)  == RubyConstant.QTRUE )
					nFreq = RepeatRule.DAILY;
				
				rl.setInt(RepeatRule.FREQUENCY, nFreq);
			}
			if ( recurrent.has_key(RUBY_EV_RECURRENCE_INTERVAL) == RubyConstant.QTRUE )
				rl.setInt(RepeatRule.INTERVAL, recurrent.get(RUBY_EV_RECURRENCE_INTERVAL).toInt());
			if ( recurrent.has_key(RUBY_EV_RECURRENCE_MONTHS) == RubyConstant.QTRUE )
			{
				int[] arMonths = {RepeatRule.JANUARY, RepeatRule.FEBRUARY, RepeatRule.MARCH, RepeatRule.APRIL, RepeatRule.MAY, RepeatRule.JUNE, RepeatRule.JULY,
						RepeatRule.AUGUST, RepeatRule.SEPTEMBER, RepeatRule.OCTOBER, RepeatRule.NOVEMBER, RepeatRule.DECEMBER};
				int nMonths = 0; 
				RubyArray months = (RubyArray)recurrent.get(RUBY_EV_RECURRENCE_MONTHS); 
				for ( int m = 0; m < months.size(); m++ )
				{
					if ( months.get(m).toInt() != 0 )
						nMonths |= arMonths[m]; 
				}
				rl.setInt(RepeatRule.MONTH_IN_YEAR, nMonths);
			}
			if ( recurrent.has_key(RUBY_EV_RECURRENCE_WEEKS) == RubyConstant.QTRUE )
			{
				int[] arWeeks = {RepeatRule.FIRST, RepeatRule.SECOND, RepeatRule.THIRD, RepeatRule.FOURTH, RepeatRule.FIFTH};
				int nWeeks = 0; 
				RubyArray weeks = (RubyArray)recurrent.get(RUBY_EV_RECURRENCE_WEEKS);
				for ( int m = 0; m < weeks.size(); m++ )
				{
					if ( weeks.get(m).toInt() != 0 )
						nWeeks |= arWeeks[m]; 
				}
				rl.setInt(RepeatRule.WEEK_IN_MONTH, nWeeks);
			}
			if ( recurrent.has_key(RUBY_EV_RECURRENCE_DAYS) == RubyConstant.QTRUE )
			{
				int[] arDays = {RepeatRule.MONDAY, RepeatRule.TUESDAY, RepeatRule.WEDNESDAY, RepeatRule.THURSDAY, RepeatRule.FRIDAY, RepeatRule.SATURDAY, RepeatRule.SUNDAY};
				int nDays = 0; 
				RubyArray days = (RubyArray)recurrent.get(RUBY_EV_RECURRENCE_DAYS);
				for ( int m = 0; m < days.size(); m++ )
				{
					if ( days.get(m).toInt() != 0 )
						nDays |= arDays[m]; 
				}
				rl.setInt(RepeatRule.DAY_IN_WEEK, nDays);
			}

			event.setRepeat(null);
			event.setRepeat(rl);
		}
		
		return RubyConstant.QTRUE;
	}
	
	public static RubyValue findCalendarEvents(RubyArray args) throws Exception
	{
		RhoCalendar pb = (RhoCalendar)args.get(0);
		RubyHash params = (RubyHash) args.get(1);
		RubyHash res = ObjectFactory.createHash();
		RubyValue val = params.get(RUBY_FIND_type);
		String strSearchType = val != null && val != RubyConstant.QNIL ? val.toStr() : "starting"; 
		int nSearchType = EventList.STARTING;
		if ( strSearchType.equalsIgnoreCase("occurring") )
			nSearchType = EventList.OCCURRING;
		else if (strSearchType.equalsIgnoreCase("ending"))
			nSearchType = EventList.ENDING;

		val = params.get(RUBY_FIND_include_repeating);
		boolean bIncludeRepeating = val == RubyConstant.QFALSE ? false : true;
		RubyTime start = (RubyTime)params.get(RUBY_FIND_start_date);
		RubyTime end = (RubyTime)params.get(RUBY_FIND_end_date);
		
		java.util.Enumeration events = pb.m_eventList.items(
				nSearchType, start.getTime(), end.getTime(), !bIncludeRepeating);
		
		while (events.hasMoreElements()) {
			Event event = (Event) events.nextElement();
			RubyHash record = pb.getEVRecord( event );
			res.add(record.get(RUBY_EV_ID), record);
		}
		
		return res;
	}

	public static RubyValue getallCalendarEvents(RubyValue arg0)throws Exception 
	{
		RhoCalendar pb = (RhoCalendar)arg0;
		RubyHash res = ObjectFactory.createHash();
		
		java.util.Enumeration events = pb.m_eventList.items();
		while (events.hasMoreElements()) {
			Event event = (Event) events.nextElement();
			RubyHash record = pb.getEVRecord( event );
			res.add(record.get(RUBY_EV_ID), record);
		}
		
		return res;
	}
	
	private Event findEventByID(RubyValue arg0, RubyValue arg1 )throws Exception
	{
		Event event = null;
		
		Event matching = m_eventList.createEvent();
		String id = arg1.toString();
		id = id.substring(1, id.length()-1);
		matching.addString(Event.UID, Event.ATTR_NONE, id);
		java.util.Enumeration events = m_eventList.items(matching);
		if (events.hasMoreElements())
			event = (Event) events.nextElement();
			
		return event;
	}
	
	public static RubyValue getCalendarEvent(RubyValue arg0, RubyValue arg1)throws Exception 
	{
		RhoCalendar pb = (RhoCalendar)arg0;
		RubyHash record = ObjectFactory.createHash();
		
		Event event = pb.findEventByID(arg0,arg1);
		if ( event != null )
			record = pb.getEVRecord( event );
		
		return record;
	}

	public static RubyValue openCalendarEvent(RubyValue arg0, RubyValue arg1)throws Exception 
	{
		RhoCalendar pb = (RhoCalendar)arg0;
		Event event = pb.findEventByID(arg0,arg1);
		if ( event != null )
			return new EVRecord(event);
		
		return RubyConstant.QNIL;
	}
	
	private static void setStringField(Event event, int field, RubyValue val)
	{
		String strValue = val.toStr();
		if ( event.countValues(field) > 0 )
			event.setString(field, 0, Event.ATTR_NONE, strValue);
		else
			event.addString(field, Event.ATTR_NONE, strValue);
	}

	private static void setIntField(Event event, int field, RubyValue val)
	{
		int nValue = val.toInt();
		if ( event.countValues(field) > 0 )
			event.setInt(field, 0, Event.ATTR_NONE, nValue);
		else
			event.addInt(field, Event.ATTR_NONE, nValue);
	}

	private static void setDateField(Event event, int field, RubyValue val)
	{
		long nValue = val.toRubyTime().getTime();
		if ( event.countValues(field) > 0 )
			event.setDate(field, 0, Event.ATTR_NONE, nValue);
		else
			event.addDate(field, Event.ATTR_NONE, nValue);
	}
	
	public static RubyValue saveEvent(RubyValue arg0, RubyValue arg1)throws Exception 
	{
		EVRecord record = (EVRecord)arg1;
		record.m_event.commit();
		
		return RubyConstant.QTRUE;
	}

	public static RubyValue createEvent(RubyValue arg0) {
		RhoCalendar pb = (RhoCalendar)arg0;
		Event event = pb.m_eventList.createEvent();
		if ( event != null )
			return new EVRecord(event);
		
		return RubyConstant.QNIL;
	}

	public static RubyValue addEvent(RubyValue arg0, RubyValue arg1)throws Exception 
	{
		return saveEvent(arg0, arg1);
	}
	
	public static RubyValue deleteEvent(RubyValue arg0, RubyValue arg1)throws Exception 
	{
		RhoCalendar pb = (RhoCalendar)arg0;
		EVRecord record = (EVRecord)arg1;

		pb.m_eventList.removeEvent(record.m_event);
			
		return RubyConstant.QTRUE;
	}

	public static void initMethods( RubyClass klass)
	{
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	
			{
				try {
					return RhoCalendar.alloc(receiver);
				} catch(Exception e) {
					LOG.ERROR("alloc Calendar failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		
		klass.getSingletonClass().defineMethod("openCalendar", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) 
			{
				try {
					return RhoCalendar.openCalendar();
				} catch(Exception e) {
					LOG.ERROR("openCalendar failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("closeCalendar", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) 
			{
				try {
					return RhoCalendar.closeCalendar(arg0);
				} catch(Exception e) {
					LOG.ERROR("closeCalendar failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("getallCalendarEvents", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) 
			{
				try {
					return RhoCalendar.getallCalendarEvents(arg0);
				} catch(Exception e) {
					LOG.ERROR("getallCalendarEvents failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("findCalendarEvents", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) 
			{
				try {
					return RhoCalendar.findCalendarEvents(args);
				} catch(Exception e) {
					LOG.ERROR("findCalendarEvents failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		
		klass.getSingletonClass().defineMethod("openCalendarEvent", new RubyTwoArgMethod() 
		{
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					return RhoCalendar.openCalendarEvent(arg0, arg1);
				} catch(Exception e) {
					LOG.ERROR("openCalendarEvent failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("getCalendarEvent", new RubyTwoArgMethod() 
		{
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					return RhoCalendar.getCalendarEvent(arg0, arg1);
				} catch(Exception e) {
					LOG.ERROR("getCalendarEvent failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("createEvent", new RubyOneArgMethod() 
		{
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) 
			{
				try {
					return RhoCalendar.createEvent(arg0);
				} catch(Exception e) {
					LOG.ERROR("createEvent failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("setEventValue", new RubyVarArgMethod() 
		{
			protected RubyValue run(RubyValue receiver, RubyArray ar, RubyBlock block) 
			{
				try {
					return RhoCalendar.setEventValue(ar);
				} catch(Exception e) {
					LOG.ERROR("setEventValue failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("addEvent", new RubyTwoArgMethod() 
		{
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					return RhoCalendar.addEvent(arg0, arg1);
				} catch(Exception e) {
					LOG.ERROR("addEvent failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("saveEvent", new RubyTwoArgMethod()
		{
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					return RhoCalendar.saveEvent(arg0, arg1);
				} catch(Exception e) {
					LOG.ERROR("saveEvent failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("deleteEvent", new RubyTwoArgMethod() 
		{
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					return RhoCalendar.deleteEvent(arg0, arg1);
				} catch(Exception e) {
					LOG.ERROR("saveEvent failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
	}
}
