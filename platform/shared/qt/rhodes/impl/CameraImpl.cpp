#include "common/RhoPort.h"
#include "ext/rho/rhoruby.h"
#include "common/RhodesApp.h"
#include "MainWindowImpl.h"

extern "C" {

void choose_picture(char* callback_url)
{
    CMainWindow::getInstance()->selectPicture(strdup(callback_url));
}

void take_picture(char* callback_url, rho_param * options_hash)
{
    CMainWindow::getInstance()->takePicture(strdup(callback_url));
}

VALUE get_camera_info(const char* camera_type)
{
    return rho_ruby_get_NIL();
}

} //extern "C"
