package com.rhomobile.rhodes.event;

import android.net.Uri;

public interface ICalendarIDsProvider {
    Uri.Builder getUriBuilder();

    String getEventsID();
    String getEventsEventID();
    String getEventsTitle();
    String getEventsStartDate();
    String getEventsEndDate();
    String getEventsLocation();
    String getEventsNotes();
    String getEventsPrivacy();
    String getEventsDeleted();
    String getEventsDuration();
    String getEventsBegin();
    String getEventsEnd();
    String getEventsRrule();
    
    String getCalendarName();
    String getCalendarDisplayName();
    String getCalendarColor();
    String getCalendarAccessLevel();

}
