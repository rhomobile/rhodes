#include <time.h>
#include "ext/rho/rhoruby.h"
#include "common/RhodesApp.h"

extern "C" {

void choose_picture(char* callback_url)
{
    //TODO: ::PostMessage(main_wnd,WM_SELECTPICTURE,0,(LPARAM)strdup(callback_url));
}

void take_picture(char* callback_url, rho_param * options_hash)
{
    //TODO: ::PostMessage(main_wnd,WM_TAKEPICTURE,0,(LPARAM)strdup(callback_url));
}

VALUE get_camera_info(const char* camera_type)
{
    return rho_ruby_get_NIL();
}

} //extern "C"
