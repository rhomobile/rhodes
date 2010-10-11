#include "stdafx.h"

#include "ruby/ext/rho/rhoruby.h"
#if defined(OS_WINCE)
#include "ruby/ext/calendar/event.h"
#include "OutlookApp.h"
#include "common/StringConverter.h"
#include "Utils.h"

#include "logging/RhoLogConf.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Event"

#ifdef _WIN32_WCE
#define INITGUID
#include <pimstore.h>
#endif //_WIN32_WCE

using namespace rho;
using namespace rho::common;

static void calendar_check()
{
    COutlookApp::Create();
    if ( !COutlookApp::getInstance()->getApp() )
    {
        rho_ruby_raise_runtime("No calendar support on this device");
    }
}

int checkHResult(HRESULT res, const char* file, int line, const char* msg )
{
    if (FAILED(res))
    {
        LOG(ERROR) + "Method: " + msg + ";Line: " + line; 
        rho_ruby_raise_runtime("Calendar method failed.");
        return 0;
    }

    return 1;
}

#define START_CHECK while(1){
#define CHECK(res) {if(!checkHResult(res, __FILE__, __LINE__, #res )) break;}
#define END_CHECK break; }

static void addVariantTimeToHash(VALUE hash, const char* key, DATE time)
{
    SYSTEMTIME sysTime = {0};
    VariantTimeToSystemTime(time, &sysTime);

    addTimeToHash(hash, key, SystemTimeToUnixTime(&sysTime));
}

static DATE dateFromRuby(VALUE rDate)
{
    SYSTEMTIME sysTime = {0};
    UnixTimeToSystemTime(rho_ruby_get_time(rDate), &sysTime );

    DATE time = 0;
    SystemTimeToVariantTime(&sysTime, &time);

    return time;
}

static VALUE eventToRuby(IAppointment *pEvent)
{
    if (!pEvent)
        return rho_ruby_get_NIL();
    
    VALUE rEvent = rho_ruby_createHash();

    long lOid = 0;
    if ( SUCCEEDED(pEvent->get_Oid(&lOid)) ) 
        addStrToHash(rEvent, RUBY_EV_ID, convertToStringA(lOid).c_str() );

    CComBSTR bstrSubj;
    if ( SUCCEEDED(pEvent->get_Subject(&bstrSubj)) ) 
        addStrToHash(rEvent, RUBY_EV_TITLE, convertToStringA((BSTR)bstrSubj).c_str() );

    long lStatus = 0;
    if ( SUCCEEDED(pEvent->get_MeetingStatus(&lStatus)) ) 
        addBoolToHash(rEvent, RUBY_EV_CANCELED, lStatus==olMeetingCanceled ? 1:0);

    long lSensitivity = 0;
    if ( SUCCEEDED(pEvent->get_Sensitivity(&lSensitivity)) ) 
    {
        String strPrivacy = "public";
        switch(lSensitivity)
        {
        case olPersonal:
            strPrivacy = "personal";
            break;
        case olPrivate:
            strPrivacy = "private";
            break;
        case olConfidential:
            strPrivacy = "confidential";
            break;
        }

        addStrToHash(rEvent, RUBY_EV_PRIVACY, strPrivacy.c_str());
    }

    CComBSTR bstrLoc;
    if ( SUCCEEDED(pEvent->get_Location(&bstrLoc)) ) 
        addStrToHash(rEvent, RUBY_EV_LOCATION, convertToStringA((BSTR)bstrLoc).c_str() );

    CComBSTR bstrNotes;
    if ( SUCCEEDED(pEvent->get_Body(&bstrNotes)) ) 
        addStrToHash(rEvent, RUBY_EV_NOTES, convertToStringA((BSTR)bstrNotes).c_str() );

    DATE dStart = 0;
    if ( SUCCEEDED(pEvent->get_Start(&dStart)) )
        addVariantTimeToHash(rEvent, RUBY_EV_START_DATE, dStart);

    DATE dEnd = 0;
    if ( SUCCEEDED(pEvent->get_End(&dEnd)) )
        addVariantTimeToHash(rEvent, RUBY_EV_END_DATE, dEnd);

    VARIANT_BOOL bReminder;
    if ( SUCCEEDED(pEvent->get_ReminderSet(&bReminder)) && bReminder == VARIANT_TRUE)
    {
        long lMinutes = 0;
        if ( SUCCEEDED(pEvent->get_ReminderMinutesBeforeStart(&lMinutes)) ) 
            addIntToHash(rEvent, RUBY_EV_REMINDER, (int)lMinutes);
    }

    VARIANT_BOOL bIsRecurring;
    CComPtr<IRecurrencePattern> pRP;
    if ( SUCCEEDED(pEvent->get_IsRecurring(&bIsRecurring)) && bIsRecurring == VARIANT_TRUE &&
         SUCCEEDED(pEvent->GetRecurrencePattern(&pRP)) && pRP )
    {
        long lRecType = 0;
        if ( SUCCEEDED(pEvent->get_IsRecurring(&bIsRecurring)) && bIsRecurring == VARIANT_TRUE &&
             SUCCEEDED(pRP->get_RecurrenceType(&lRecType)) && lRecType != olRecursOnce ) 
        {
            VALUE rRecur = rho_ruby_createHash();        

            switch( lRecType )
            {
            case olRecursDaily:
                addStrToHash(rRecur, RUBY_EV_RECURRENCE_FREQUENCY, RUBY_EV_RECURRENCE_FREQUENCY_DAILY );
                break;
            case olRecursWeekly:
                addStrToHash(rRecur, RUBY_EV_RECURRENCE_FREQUENCY, RUBY_EV_RECURRENCE_FREQUENCY_WEEKLY );
                break;
            case olRecursMonthly:
            case olRecursMonthNth:
                addStrToHash(rRecur, RUBY_EV_RECURRENCE_FREQUENCY, RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY );
                break;
            case olRecursYearly:
            case olRecursYearNth:
                addStrToHash(rRecur, RUBY_EV_RECURRENCE_FREQUENCY, RUBY_EV_RECURRENCE_FREQUENCY_YEARLY );
                break;
            }

            long lInterval = 0;
            if ( SUCCEEDED(pRP->get_Interval(&lInterval)) ) 
                addIntToHash(rRecur, RUBY_EV_RECURRENCE_INTERVAL, (int)lInterval);
            long lOccurrences = 0;
            VARIANT_BOOL bNoEndDate;
            if ( SUCCEEDED(pRP->get_NoEndDate(&bNoEndDate)) && bNoEndDate == VARIANT_FALSE &&
                 SUCCEEDED(pRP->get_Occurrences(&lOccurrences)) ) 
                addIntToHash(rRecur, RUBY_EV_RECURRENCE_COUNT, (int)lOccurrences);

            DATE dEnd = 0;
            if ( SUCCEEDED(pRP->get_EndTime(&dEnd)) )
                addVariantTimeToHash(rEvent, RUBY_EV_RECURRENCE_END, dEnd);

            long lDayOfMonth = 0;
            if ( SUCCEEDED(pRP->get_DayOfMonth(&lDayOfMonth)) ) 
                addIntToHash(rRecur, RUBY_EV_RECURRENCE_DAYOFMONTH, (int)lDayOfMonth);

            long lMonthOfYear = 0;
            if ( SUCCEEDED(pRP->get_MonthOfYear(&lMonthOfYear)) ) 
            {
                VALUE arMonths = rho_ruby_create_array();
                for ( int i = 1; i <= 12; i++ )
                    rho_ruby_add_to_array(arMonths, rho_ruby_create_integer(i == lMonthOfYear ? 1:0) );

                addHashToHash(rRecur, RUBY_EV_RECURRENCE_MONTHS, arMonths);
            }

            long lDayOfWeekMask = 0;
            if ( SUCCEEDED(pRP->get_DayOfWeekMask(&lDayOfWeekMask)) ) 
            {
                VALUE arDays = rho_ruby_create_array();
                rho_ruby_add_to_array(arDays, rho_ruby_create_integer( (lDayOfWeekMask&olMonday)!= 0 ? 1:0) );
                rho_ruby_add_to_array(arDays, rho_ruby_create_integer( (lDayOfWeekMask&olTuesday)!= 0 ? 1:0) );
                rho_ruby_add_to_array(arDays, rho_ruby_create_integer( (lDayOfWeekMask&olWednesday)!= 0 ? 1:0) );
                rho_ruby_add_to_array(arDays, rho_ruby_create_integer( (lDayOfWeekMask&olThursday)!= 0 ? 1:0) );
                rho_ruby_add_to_array(arDays, rho_ruby_create_integer( (lDayOfWeekMask&olFriday)!= 0 ? 1:0) );
                rho_ruby_add_to_array(arDays, rho_ruby_create_integer( (lDayOfWeekMask&olSaturday)!= 0 ? 1:0) );
                rho_ruby_add_to_array(arDays, rho_ruby_create_integer( (lDayOfWeekMask&olSunday)!= 0 ? 1:0) );

                addHashToHash(rRecur, RUBY_EV_RECURRENCE_DAYS, arDays);
            }

            addHashToHash(rEvent, RUBY_EV_RECURRENCE, rRecur);
        }
    }

    return rEvent;
}

static void eventFromRuby(VALUE rEvent, IAppointment* pEvent)
{
    //VALUE rId = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_ID));
    //if (!NIL_P(rId))
    //    Check_Type(rId, T_STRING);
    START_CHECK

    VALUE rTitle = rho_ruby_hash_aref(rEvent, RUBY_EV_TITLE);
    if (!rho_ruby_is_NIL(rTitle)) 
        CHECK( pEvent->put_Subject(const_cast<BSTR>(convertToStringW( String(getStringFromValue(rTitle))).c_str())) );

    VALUE rCancelled = rho_ruby_hash_aref(rEvent, RUBY_EV_CANCELED);
    if (!rho_ruby_is_NIL(rCancelled)) 
    {
        if( rho_ruby_get_bool(rCancelled) )
            CHECK( pEvent->Cancel() );
    }

    VALUE rPrivacy = rho_ruby_hash_aref(rEvent, RUBY_EV_PRIVACY);
    if (!rho_ruby_is_NIL(rPrivacy)) 
    {
        String strPrivacy = getStringFromValue(rPrivacy);
        long lSensitivity = olNormal;
        if ( strPrivacy.compare("personal") == 0 )
            lSensitivity = olPersonal;
        else if ( strPrivacy.compare("private") == 0 )
            lSensitivity = olPrivate;
        else if ( strPrivacy.compare("confidential") == 0 )
            lSensitivity = olConfidential;

        CHECK( pEvent->put_Sensitivity(lSensitivity) );
    }

    VALUE rLoc = rho_ruby_hash_aref(rEvent, RUBY_EV_LOCATION);
    if (!rho_ruby_is_NIL(rLoc)) 
        CHECK( pEvent->put_Location(const_cast<BSTR>(convertToStringW( String(getStringFromValue(rLoc))).c_str())) );

    VALUE rNotes = rho_ruby_hash_aref(rEvent, RUBY_EV_NOTES);
    if (!rho_ruby_is_NIL(rNotes)) 
        CHECK( pEvent->put_Body(const_cast<BSTR>(convertToStringW( String(getStringFromValue(rNotes))).c_str())));

    VALUE rStartDate = rho_ruby_hash_aref(rEvent, RUBY_EV_START_DATE);
    if (!rho_ruby_is_NIL(rStartDate))
        CHECK( pEvent->put_Start(dateFromRuby(rStartDate)));

    VALUE rEndDate = rho_ruby_hash_aref(rEvent, RUBY_EV_END_DATE);
    if (!rho_ruby_is_NIL(rEndDate))
        CHECK( pEvent->put_End(dateFromRuby(rEndDate)) );

    VALUE rReminder = rho_ruby_hash_aref(rEvent, RUBY_EV_REMINDER);
    if (!rho_ruby_is_NIL(rReminder))
    {
        CHECK( pEvent->put_ReminderSet(VARIANT_TRUE) );
        CHECK( pEvent->put_ReminderMinutesBeforeStart(rho_ruby_get_int(rReminder)) );
    }

    VALUE rRecur = rho_ruby_hash_aref(rEvent, RUBY_EV_RECURRENCE);
    if (!rho_ruby_is_NIL(rRecur))
    {
        CHECK( pEvent->ClearRecurrencePattern() );
        CComPtr<IRecurrencePattern> pRP;
        if ( SUCCEEDED(pEvent->GetRecurrencePattern(&pRP)) && pRP )
        {
            VALUE rFreq = rho_ruby_hash_aref(rRecur, RUBY_EV_RECURRENCE_FREQUENCY);
            if (!rho_ruby_is_NIL(rFreq))
            {
                long lRecType = 0;
                String strFreq = getStringFromValue(rFreq);
                if ( strFreq.compare(RUBY_EV_RECURRENCE_FREQUENCY_DAILY) == 0 )
                    lRecType = olRecursDaily;
                else if ( strFreq.compare(RUBY_EV_RECURRENCE_FREQUENCY_WEEKLY) == 0 )
                    lRecType = olRecursWeekly;
                else if ( strFreq.compare(RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY) == 0 )
                    lRecType = olRecursMonthly;
                else if ( strFreq.compare(RUBY_EV_RECURRENCE_FREQUENCY_YEARLY) == 0 )
                    lRecType = olRecursYearly;

                CHECK( pRP->put_RecurrenceType(lRecType) );
            }

            VALUE rInterval = rho_ruby_hash_aref(rRecur, RUBY_EV_RECURRENCE_INTERVAL);
            if (!rho_ruby_is_NIL(rInterval))
                CHECK( pRP->put_Interval(rho_ruby_get_int(rInterval)) );

            VALUE rOccurrences = rho_ruby_hash_aref(rRecur, RUBY_EV_RECURRENCE_COUNT);
            if (!rho_ruby_is_NIL(rOccurrences))
            {
                CHECK( pRP->put_NoEndDate(VARIANT_FALSE) );
                CHECK( pRP->put_Occurrences(rho_ruby_get_int(rOccurrences)) );
            }

            VALUE rEnd = rho_ruby_hash_aref(rRecur, RUBY_EV_RECURRENCE_END);
            if (!rho_ruby_is_NIL(rEnd))
                CHECK( pRP->put_EndTime(dateFromRuby(rEnd)) );

            VALUE rDayOfMonth = rho_ruby_hash_aref(rRecur, RUBY_EV_RECURRENCE_DAYOFMONTH);
            if (!rho_ruby_is_NIL(rDayOfMonth))
                CHECK( pRP->put_DayOfMonth(rho_ruby_get_int(rDayOfMonth)) );

            VALUE rMonthOfYear = rho_ruby_hash_aref(rRecur, RUBY_EV_RECURRENCE_MONTHS);
            if (!rho_ruby_is_NIL(rMonthOfYear))
            {
                for ( int i = 1; i <= 12; i++ )
                {
                    VALUE val = rho_ruby_array_get(rMonthOfYear, i-1);
                    if ( rho_ruby_get_int(val) != 0)
                    {
                        CHECK( pRP->put_MonthOfYear(i) );
                        break;
                    }
                }
            }

            VALUE rDayOfWeek = rho_ruby_hash_aref(rRecur, RUBY_EV_RECURRENCE_DAYS);
            if (!rho_ruby_is_NIL(rDayOfWeek))
            {
                long lDayOfWeekMask = 0;

                if ( rho_ruby_get_int( rho_ruby_array_get(rDayOfWeek, 0) ) )
                    lDayOfWeekMask |= olMonday;
                if ( rho_ruby_get_int( rho_ruby_array_get(rDayOfWeek, 1) ) )
                    lDayOfWeekMask |= olTuesday;
                if ( rho_ruby_get_int( rho_ruby_array_get(rDayOfWeek, 2) ) )
                    lDayOfWeekMask |= olWednesday;
                if ( rho_ruby_get_int( rho_ruby_array_get(rDayOfWeek, 3) ) )
                    lDayOfWeekMask |= olThursday;
                if ( rho_ruby_get_int( rho_ruby_array_get(rDayOfWeek, 4) ) )
                    lDayOfWeekMask |= olFriday;
                if ( rho_ruby_get_int( rho_ruby_array_get(rDayOfWeek, 5) ) )
                    lDayOfWeekMask |= olSaturday;
                if ( rho_ruby_get_int( rho_ruby_array_get(rDayOfWeek, 6) ) )
                    lDayOfWeekMask |= olSunday;

                CHECK( pRP->put_DayOfWeekMask(lDayOfWeekMask) );
            }
        }
    }
    END_CHECK

}

extern "C" VALUE event_fetch(VALUE params)
{
    calendar_check();
    IPOutlookApp * polApp = COutlookApp::getInstance()->getApp();

    CComPtr<IPOutlookItemCollection> pItems;
    CComPtr<IFolder>                 pFolder;

    VALUE start_date = rho_ruby_hash_aref(params,RUBY_EV_START_DATE);
    VALUE end_date = rho_ruby_hash_aref(params,RUBY_EV_END_DATE);
    VALUE include_repeating = rho_ruby_hash_aref(params,RUBY_FETCH_include_repeating);
    VALUE ret = rho_ruby_create_array();

    START_CHECK
        CHECK( polApp->GetDefaultFolder(olFolderCalendar, &pFolder) );

        CHECK( pFolder->get_Items(&pItems) );

        if (rho_ruby_get_bool(include_repeating))
            CHECK( pItems->put_IncludeRecurrences(VARIANT_TRUE) );

        DATE dStart = dateFromRuby(start_date);
        DATE dEnd = dateFromRuby(end_date);
        WCHAR szRestrict[250];
        StringCchPrintf(szRestrict, ARRAYSIZE(szRestrict), TEXT("[Start] >= <%f AND [End] <= <%f"), dStart, dEnd);
        bool bFirst = true;
        while(true)
        {
            CComPtr<IAppointment>            pEvent;
            if ( bFirst )
                pItems->Find(szRestrict, (IDispatch **)&pEvent);
            else
                pItems->FindNext((IDispatch **)&pEvent);

            if (!pEvent)
                break;

            VALUE rEvent = eventToRuby(pEvent);
            rho_ruby_add_to_array(ret, rEvent);

            bFirst = false;
        }
    END_CHECK

    return ret;
}

IAppointment* findEventById(IPOutlookApp * polApp, const char *eid)
{
    if ( !eid || !*eid )
        return 0;

    CComPtr<IPOutlookItemCollection> pItems;
    CComPtr<IFolder>                 pFolder;

    START_CHECK

        CHECK( polApp->GetDefaultFolder(olFolderCalendar, &pFolder) );
        CHECK( pFolder->get_Items(&pItems) );

        String strCond = String("[Oid] = ") + eid;
        IAppointment*            pEvent;
        CHECK( pItems->Find( const_cast<BSTR>(convertToStringW(strCond).c_str()), (IDispatch**)&pEvent) );

        return pEvent;

    END_CHECK

    return 0;
}

extern "C" VALUE event_fetch_by_id(const char *eid)
{
    calendar_check();
    IPOutlookApp * polApp = COutlookApp::getInstance()->getApp();

    CComPtr<IAppointment>  pEvent = findEventById(polApp, eid);
    if ( pEvent )
        return eventToRuby(pEvent);

    return rho_ruby_get_NIL();
}

extern "C" void event_save(VALUE rEvent)
{
    calendar_check();
    IPOutlookApp * polApp = COutlookApp::getInstance()->getApp();

    START_CHECK
        CComPtr<IAppointment> pEvent;
        VALUE rId = rho_ruby_hash_aref(rEvent, RUBY_EV_ID);
        if (!rho_ruby_is_NIL(rId))
            pEvent = findEventById(polApp, getStringFromValue(rId));

        if ( !pEvent )
            CHECK(polApp->CreateItem(olAppointmentItem, (IDispatch**)&pEvent));

        eventFromRuby(rEvent, pEvent);

        CHECK(pEvent->Save());
    END_CHECK
}

extern "C" void event_delete(const char *eid)
{
    calendar_check();
    IPOutlookApp * polApp = COutlookApp::getInstance()->getApp();

    START_CHECK

        CComPtr<IAppointment>  pEvent = findEventById(polApp, eid);
        if ( pEvent )
            CHECK(pEvent->Delete());

    END_CHECK

}
#else //!OS_WINCE

extern "C" VALUE event_fetch(VALUE start_date, VALUE end_date)
{
    return  rho_ruby_create_array();
}

extern "C" VALUE event_fetch_by_id(const char *eid)
{
    return  rho_ruby_get_NIL();
}

extern "C" void event_save(VALUE rEvent)
{
}

extern "C" void event_delete(const char *eid)
{
}

#endif