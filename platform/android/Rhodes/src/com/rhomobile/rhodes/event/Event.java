package com.rhomobile.rhodes.event;

import java.util.Date;

public class Event {
	
	public Event(String eid) {
		id = eid;
	}
	
	public String id;
	public String title;
	public Date startDate;
	public Date endDate;
	public Date lastModified;
	public String location;
	public String notes;
	public String privacy;
}
