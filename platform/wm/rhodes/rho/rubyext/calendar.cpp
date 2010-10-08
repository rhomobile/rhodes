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

int checkHResult(HRESULT res)
{
    if (FAILED(res))
    {
        rho_ruby_raise_runtime("Calendar method failed.");
        return 0;
    }

    return 1;
}

#define START_CHECK while(1){
#define CHECK(res) {if(!checkHResult(res)) break;}
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

    return rEvent;
}

static void eventFromRuby(VALUE rEvent, IAppointment* pEvent)
{
    //VALUE rId = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_ID));
    //if (!NIL_P(rId))
    //    Check_Type(rId, T_STRING);

    VALUE rTitle = rho_ruby_hash_aref(rEvent, RUBY_EV_TITLE);
    if (!rho_ruby_is_NIL(rTitle)) 
        pEvent->put_Subject(const_cast<BSTR>(convertToStringW( String(getStringFromValue(rTitle))).c_str()));

    VALUE rCancelled = rho_ruby_hash_aref(rEvent, RUBY_EV_CANCELED);
    if (!rho_ruby_is_NIL(rCancelled)) 
    {
        if( rho_ruby_get_bool(rCancelled) )
            pEvent->Cancel();
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

        pEvent->put_Sensitivity(lSensitivity);
    }

    VALUE rLoc = rho_ruby_hash_aref(rEvent, RUBY_EV_LOCATION);
    if (!rho_ruby_is_NIL(rLoc)) 
        pEvent->put_Location(const_cast<BSTR>(convertToStringW( String(getStringFromValue(rLoc))).c_str()));

    VALUE rNotes = rho_ruby_hash_aref(rEvent, RUBY_EV_NOTES);
    if (!rho_ruby_is_NIL(rNotes)) 
        pEvent->put_Body(const_cast<BSTR>(convertToStringW( String(getStringFromValue(rNotes))).c_str()));

    VALUE rStartDate = rho_ruby_hash_aref(rEvent, RUBY_EV_START_DATE);
    if (!rho_ruby_is_NIL(rStartDate))
        pEvent->put_Start(dateFromRuby(rStartDate));

    VALUE rEndDate = rho_ruby_hash_aref(rEvent, RUBY_EV_END_DATE);
    if (!rho_ruby_is_NIL(rEndDate))
        pEvent->put_End(dateFromRuby(rEndDate));

    VALUE rReminder = rho_ruby_hash_aref(rEvent, RUBY_EV_REMINDER);
    if (!rho_ruby_is_NIL(rReminder))
    {
        pEvent->put_ReminderSet(VARIANT_TRUE);
        pEvent->put_ReminderMinutesBeforeStart(rho_ruby_get_int(rReminder));
    }

}

extern "C" VALUE event_fetch(VALUE start_date, VALUE end_date)
{
    calendar_check();
    IPOutlookApp * polApp = COutlookApp::getInstance()->getApp();

    CComPtr<IPOutlookItemCollection> pItems;
    CComPtr<IFolder>                 pFolder;

    VALUE ret = rho_ruby_create_array();

    START_CHECK
        CHECK( polApp->GetDefaultFolder(olFolderCalendar, &pFolder) );

        CHECK( pFolder->get_Items(&pItems) );
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