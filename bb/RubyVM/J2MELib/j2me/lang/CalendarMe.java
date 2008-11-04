package j2me.lang;

import java.util.Calendar;

public class CalendarMe {
    public static void set(Calendar clndThis, int year, int month, int date, int hourOfDay, int minute,
            int second)
	{
    	clndThis.set(Calendar.YEAR, year);
    	clndThis.set(Calendar.MONTH, month);
    	clndThis.set(Calendar.DATE, date);
    	clndThis.set(Calendar.HOUR_OF_DAY, hourOfDay);
    	clndThis.set(Calendar.MINUTE, minute);
    	clndThis.set(Calendar.SECOND, second);
	}

}
