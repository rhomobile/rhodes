#include "common/RhoPort.h"
#include "ext/rho/rhoruby.h"
#include "common/RhodesApp.h"
#include "MainWindowImpl.h"

extern "C" {

void choose_picture(char* callback_url)
{
    CMainWindow::getInstance()->selectPicture(strdup(callback_url));
}

void take_picture(char* callback_url, rho_param* options_hash)
{
    CMainWindow::getInstance()->takePicture(strdup(callback_url));
}

VALUE get_camera_info(const char* camera_type)
{
    VALUE info = rho_ruby_createHash();
    VALUE max_res = rho_ruby_createHash();
    rho_ruby_add_to_hash(max_res, rho_ruby_create_string("width"), rho_ruby_create_integer(2048));
    rho_ruby_add_to_hash(max_res, rho_ruby_create_string("height"), rho_ruby_create_integer(1152));
    rho_ruby_add_to_hash(info, rho_ruby_create_string("max_resolution"), max_res);
    return info;
}

} //extern "C"
