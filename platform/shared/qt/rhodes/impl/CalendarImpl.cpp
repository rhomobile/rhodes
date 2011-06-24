#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"

extern "C" {

VALUE event_fetch(VALUE start_date, VALUE end_date)
{
    return  rho_ruby_create_array();
}

VALUE event_fetch_by_id(const char *eid)
{
    return  rho_ruby_get_NIL();
}

const char* event_save(VALUE rEvent)
{
    return NULL;
}

void event_delete(const char *eid)
{
    //TODO: event_delete
}

} //extern "C"
