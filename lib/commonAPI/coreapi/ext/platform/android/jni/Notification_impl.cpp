#include "rhodes.h"
#include "Notification.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Notification_impl"

static const char* const NOTIFICATION_FACTORY_CLASS = "com.rho.notification.NotificationFactory";

extern "C" void Init_Notification_API(void);

extern "C" void Init_Notification(void)
{
    RAWTRACE(__FUNCTION__);
    RAWTRACE("Initializing API");
    Init_Notification_API();
    RAWTRACE("Init_Notification succeeded");

    RHODESAPP().getExtManager().requireRubyFile("RhoNotificationApi");
}

extern "C" void alert_show_status(const char* title, const char* message, const char* hideCaption)
{
    RAWTRACE("alert_show_status");

    rho::apiGenerator::MethodResultJni result(false);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object");
        return;
    }

    std::vector<const char*> args(3);
    args[0] = title;
    args[1] = message;
    args[2] = hideCaption;

    rho::CNotificationProxy< std::vector<const char*> >::showStatus(args, result);

    RAWTRACE("alert_show_status end ^^^");
}
