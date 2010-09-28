package com.rho.rubyext;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.ObjectFactory;
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
	
	public RhoCalendar(RubyClass arg0) 
	{
		super(arg0);
		
		try {
			m_eventList = (EventList) PIM.getInstance().openPIMList(
					PIM.EVENT_LIST, PIM.READ_WRITE);
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
	}
    //@RubyAllocMethod
    public static RhoCalendar alloc(RubyValue receiver) {
    	return new RhoCalendar(RubyRuntime.CalendarClass);
    }

	public static RubyValue openCalendar() {
		RhoCalendar pb = alloc(null);
		return pb;
	}
	public static RubyValue closeCalendar(RubyValue arg0) {
		try {
			RhoCalendar pb = (RhoCalendar)arg0;
			pb.m_eventList.close();
			pb.m_eventList = null;
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
		
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
		
		return RubyConstant.QTRUE;
	}
	
	public static RubyValue getallCalendarEvents(RubyValue arg0) {
		RhoCalendar pb = (RhoCalendar)arg0;
		RubyHash res = ObjectFactory.createHash();
		
		try {
			java.util.Enumeration events = pb.m_eventList.items();
			while (events.hasMoreElements()) {
				Event event = (Event) events.nextElement();
				RubyHash record = pb.getEVRecord( event );
				res.add(record.get(RUBY_EV_ID), record);
			}
			
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
		
		return res;
	}

	private Event findEventByID(RubyValue arg0, RubyValue arg1 ){
		Event event = null;
		
		try {
			Event matching = m_eventList.createEvent();
			String id = arg1.toString();
			id = id.substring(1, id.length()-1);
			matching.addString(Event.UID, Event.ATTR_NONE, id);
			java.util.Enumeration events = m_eventList.items(matching);
			if (events.hasMoreElements())
				event = (Event) events.nextElement();
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
			
		return event;
	}
	
	public static RubyValue getCalendarEvent(RubyValue arg0, RubyValue arg1) {
		RhoCalendar pb = (RhoCalendar)arg0;
		RubyHash record = ObjectFactory.createHash();
		
		Event event = pb.findEventByID(arg0,arg1);
		if ( event != null )
			record = pb.getEVRecord( event );
		
		return record;
	}

	public static RubyValue openCalendarEvent(RubyValue arg0, RubyValue arg1) {
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
	
	public static RubyValue saveEvent(RubyValue arg0, RubyValue arg1) {
		EVRecord record = (EVRecord)arg1;
		try {
			record.m_event.commit();
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
		
		return RubyConstant.QTRUE;
	}

	public static RubyValue createEvent(RubyValue arg0) {
		RhoCalendar pb = (RhoCalendar)arg0;
		Event event = pb.m_eventList.createEvent();
		if ( event != null )
			return new EVRecord(event);
		
		return RubyConstant.QNIL;
	}

	public static RubyValue addEvent(RubyValue arg0, RubyValue arg1) {
		return saveEvent(arg0, arg1);
	}
	
	public static RubyValue deleteEvent(RubyValue arg0, RubyValue arg1) {
		RhoCalendar pb = (RhoCalendar)arg0;
		EVRecord record = (EVRecord)arg1;

		try {
			pb.m_eventList.removeEvent(record.m_event);
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
			
		return RubyConstant.QTRUE;
	}

	public static void initMethods( RubyClass klass)
	{
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return RhoCalendar.alloc(receiver);}
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
