#include <time.h>
#include "ext/rho/rhoruby.h"
#include "logging/RhoLog.h"

extern "C" {

VALUE rho_ringtone_manager_get_all()
{
    LOG(INFO) + __FUNCTION__;
    CHoldRubyValue retval(rho_ruby_createHash());
    return retval;
}

void rho_ringtone_manager_stop()
{
}

void rho_ringtone_manager_play(char* file_name)
{
}

} //extern "C"
