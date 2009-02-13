package j2me.lang;

import java.util.TimeZone;

public class TimeZoneMe {
    public static boolean hasSameRules(TimeZone tmzThis, TimeZone other) {
        return other != null && tmzThis.getRawOffset() == other.getRawOffset() &&
        	tmzThis.useDaylightTime() == other.useDaylightTime();
    }

}
