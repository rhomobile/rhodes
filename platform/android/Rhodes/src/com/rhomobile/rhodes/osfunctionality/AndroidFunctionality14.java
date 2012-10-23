package com.rhomobile.rhodes.osfunctionality;

import com.rhomobile.rhodes.event.CalendarIDsProvider;
import com.rhomobile.rhodes.event.CalendarIDsProviderICS;

//Android 4.0 ICS
public class AndroidFunctionality14 extends AndroidFunctionality13 implements AndroidFunctionality {
	@Override
    public CalendarIDsProvider buildCalendarIDsProvider() {
    	return new CalendarIDsProviderICS();
    }
}
