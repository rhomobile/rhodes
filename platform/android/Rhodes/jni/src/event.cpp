#include "rhodes/JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Event"

RHO_GLOBAL VALUE event_fetch(VALUE start_date, VALUE end_date)
{
    // TODO:
    return Qnil;
}

RHO_GLOBAL VALUE event_fetch_by_id(const char *id)
{
    // TODO:
    return Qnil;
}

RHO_GLOBAL void event_save(VALUE rEvent)
{
    // TODO:
}

RHO_GLOBAL void event_delete(const char *id)
{
    // TODO:
}
