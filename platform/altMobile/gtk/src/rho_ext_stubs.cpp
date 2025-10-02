#include <glib.h>
#include <errno.h>
#include <gtk/gtk.h>

#include "logging/RhoLog.h"
#include "common/RhoPort.h"
#include "common/RhodesApp.h"
#include "commonAPI/coreapi/ext/shared/generated/cpp/ApplicationBase.h"
#include "ruby/ext/rho/rhoruby.h"

#ifndef CURLE_OK
typedef int CURLcode;
#define CURLE_OK 0
#endif

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GtkStubs"

#ifndef RHO_ENABLE_CORE
namespace rho {
common::CAutoPtr<CApplicationFactoryBase> CApplicationFactoryBase::m_pInstance;
}
#endif

extern "C" {

#define RHO_WEAK __attribute__((weak))

RHO_WEAK void* rho_ssl_create_storage() {
    RAWLOG_INFO("rho_ssl_create_storage() stub");
    return g_malloc0(1);
}

RHO_WEAK void rho_ssl_free_storage(void *ptr) {
    if (ptr) g_free(ptr);
}

RHO_WEAK CURLcode rho_ssl_connect(int /*sockfd*/, int /*nonblocking*/, int *done, int /*ssl_verify_peer*/, void * /*storage*/, char* /*host_name*/) {
    if (done) *done = 1;
    RAWLOG_INFO("rho_ssl_connect() stub");
    return CURLE_OK;
}

RHO_WEAK void rho_ssl_shutdown(void * /*storage*/) {
    RAWLOG_INFO("rho_ssl_shutdown() stub");
}

RHO_WEAK ssize_t rho_ssl_send(const void * /*mem*/, size_t /*len*/, void * /*storage*/) {
    RAWLOG_INFO("rho_ssl_send() stub");
    errno = ENOTSUP;
    return -1;
}

RHO_WEAK ssize_t rho_ssl_recv(char * /*buf*/, size_t /*size*/, int *wouldblock, void * /*storage*/) {
    RAWLOG_INFO("rho_ssl_recv() stub");
    if (wouldblock) *wouldblock = 1;
    errno = ENOTSUP;
    return -1;
}

RHO_WEAK bool rho_ssl_rand(unsigned char *entropy, size_t length) {
    if (!entropy) return false;
    for (size_t i = 0; i < length; ++i) {
        entropy[i] = static_cast<unsigned char>(g_random_int() & 0xFF);
    }
    return true;
}

RHO_WEAK void Init_WebView() {
    RAWLOG_INFO("Init_WebView() stub");
}

RHO_WEAK VALUE rho_ringtone_manager_get_all() {
    RAWLOG_INFO("rho_ringtone_manager_get_all() stub");
    CHoldRubyValue retval(rho_ruby_createHash());
    return retval;
}

RHO_WEAK void rho_ringtone_manager_stop() {
    RAWLOG_INFO("rho_ringtone_manager_stop() stub");
}

RHO_WEAK void rho_ringtone_manager_play(char* /*file_name*/) {
    RAWLOG_INFO("rho_ringtone_manager_play() stub");
}

RHO_WEAK int rho_bluetooth_is_bluetooth_available() {
    return 0;
}

RHO_WEAK void rho_bluetooth_off_bluetooth() {}

RHO_WEAK void rho_bluetooth_set_device_name(const char* /*device_name*/) {}

RHO_WEAK VALUE rho_bluetooth_get_device_name() {
    return rho_ruby_create_string("");
}

RHO_WEAK const char* rho_bluetooth_get_last_error() {
    return "Bluetooth not available";
}

RHO_WEAK const char* rho_bluetooth_create_session(const char* /*role*/, const char* /*callback_url*/) {
    return "RHO_ERR_NOT_IMPLEMENTED";
}

RHO_WEAK const char* rho_bluetooth_create_custom_server_session(const char* /*client_name*/, const char* /*callback_url*/, int /*accept_any_device*/) {
    return "RHO_ERR_NOT_IMPLEMENTED";
}

RHO_WEAK const char* rho_bluetooth_create_custom_client_session(const char* /*server_name*/, const char* /*callback_url*/) {
    return "RHO_ERR_NOT_IMPLEMENTED";
}

RHO_WEAK const char* rho_bluetooth_stop_current_connection_process() {
    return "RHO_ERR_NOT_IMPLEMENTED";
}

RHO_WEAK void rho_bluetooth_session_set_callback(const char* /*connected_device_name*/, const char* /*callback_url*/) {}

RHO_WEAK void rho_bluetooth_session_disconnect(const char* /*connected_device_name*/) {}

RHO_WEAK int rho_bluetooth_session_get_status(const char* /*connected_device_name*/) {
    return -1;
}

RHO_WEAK VALUE rho_bluetooth_session_read_string(const char* /*connected_device_name*/) {
    return rho_ruby_create_string("ERROR");
}

RHO_WEAK void rho_bluetooth_session_write_string(const char* /*connected_device_name*/, const char* /*str*/) {}

RHO_WEAK VALUE rho_bluetooth_session_read_data(const char* /*connected_device_name*/) {
    return rho_ruby_get_NIL();
}

RHO_WEAK void rho_bluetooth_session_write_data(const char* /*connected_device_name*/, VALUE /*data*/) {}

RHO_WEAK void* openPhonebook() { return nullptr; }
RHO_WEAK void  closePhonebook(void* /*pb*/) {}
RHO_WEAK VALUE getallPhonebookRecords(void* /*pb*/) { return rho_ruby_get_NIL(); }
RHO_WEAK int   getPhonebookRecordCount(void* /*pb*/, rho_param* /*params*/) { return 0; }
RHO_WEAK VALUE getPhonebookRecords(void* /*pb*/, rho_param* /*params*/) { return rho_ruby_get_NIL(); }
RHO_WEAK VALUE getPhonebookRecord(void* /*pb*/, char* /*id*/) { return rho_ruby_get_NIL(); }
RHO_WEAK VALUE getfirstPhonebookRecord(void* /*pb*/) { return rho_ruby_get_NIL(); }
RHO_WEAK VALUE getnextPhonebookRecord(void* /*pb*/) { return rho_ruby_get_NIL(); }
RHO_WEAK void* createRecord(void* /*pb*/) { return nullptr; }
RHO_WEAK void* openPhonebookRecord(void* /*pb*/, char* /*id*/) { return nullptr; }
RHO_WEAK int   setRecordValue(void* /*record*/, char* /*prop*/, char* /*value*/) { return 0; }
RHO_WEAK int   addRecord(void* /*pb*/, void* /*record*/) { return 0; }
RHO_WEAK int   saveRecord(void* /*pb*/, void* /*record*/) { return 0; }
RHO_WEAK int   deleteRecord(void* /*pb*/, void* /*record*/) { return 0; }
RHO_WEAK const char* phonebook_get_authorization_status() { return "Restricted"; }

RHO_WEAK VALUE get_camera_info(const char* /*camera_type*/) {
    CHoldRubyValue info(rho_ruby_createHash());
    CHoldRubyValue max_res(rho_ruby_createHash());
    rho_ruby_add_to_hash(max_res, rho_ruby_create_string("width"), rho_ruby_create_integer(0));
    rho_ruby_add_to_hash(max_res, rho_ruby_create_string("height"), rho_ruby_create_integer(0));
    rho_ruby_add_to_hash(info, rho_ruby_create_string("max_resolution"), max_res);
    return info;
}

RHO_WEAK void choose_picture(char* /*callback_url*/) {}
RHO_WEAK void take_picture(char* /*callback_url*/, rho_param* /*options_hash*/) {}
RHO_WEAK void save_image_to_device_gallery(const char* /*image_path*/, rho_param* /*options_hash*/) {}

RHO_WEAK void choose_datetime(char* /*callback*/, char* /*title*/, long /*initial_time*/, int /*format*/, char* /*data*/) {
    RAWLOG_INFO("choose_datetime() stub");
}

RHO_WEAK void choose_datetime_with_range(char* /*callback*/, char* /*title*/, long /*initial_time*/, int /*format*/, char* /*data*/, long /*min_time*/, long /*max_time*/) {
    RAWLOG_INFO("choose_datetime_with_range() stub");
}

RHO_WEAK void set_change_value_callback_datetime(char* /*callback*/) {
    RAWLOG_INFO("set_change_value_callback_datetime() stub");
}

RHO_WEAK VALUE event_fetch(rho_param* /*p*/) { return rho_ruby_get_NIL(); }
RHO_WEAK VALUE event_fetch_by_id(const char* /*event_id*/) { return rho_ruby_get_NIL(); }
RHO_WEAK int   event_save(VALUE /*event*/) { return 0; }
RHO_WEAK int   event_delete(const char* /*event_id*/) { return 0; }
RHO_WEAK const char* calendar_get_authorization_status() { return "Restricted"; }

RHO_WEAK void  mapview_create(rho_param * /*p*/) {}
RHO_WEAK void  mapview_close() {}
RHO_WEAK VALUE mapview_state_started() { return rho_ruby_create_boolean(0); }
RHO_WEAK double mapview_state_center_lat() { return 0.0; }
RHO_WEAK double mapview_state_center_lon() { return 0.0; }
RHO_WEAK void  mapview_set_file_caching_enable(int /*enable*/) {}
RHO_WEAK void  mapview_preload_map_tiles(rho_param* /*p*/) {}

RHO_WEAK int rho_native_view_manager_create_native_view(const char* /*viewtype*/, int /*tab_index*/, VALUE /*params*/) { return -1; }
RHO_WEAK void rho_native_view_manager_navigate_native_view(int /*native_view_id*/, const char* /*url*/) {}
RHO_WEAK void rho_native_view_manager_destroy_native_view(int /*native_view_id*/) {}

RHO_WEAK VALUE rho_webview_get_current_url(int /*tab_index*/) { return rho_ruby_get_NIL(); }

RHO_WEAK void rho_conf_show_log() {}

RHO_WEAK void rho_title_change(const int /*tabIndex*/, const char* /*strTitle*/) {
    RAWLOG_INFO("rho_title_change() stub");
}


RHO_WEAK void rho_gtk_register_menu_widgets(GtkWidget* /*button*/, GtkWidget* /*box*/) {}
RHO_WEAK void rho_symimpl_createMenu() {
    RAWLOG_INFO("rho_symimpl_createMenu() stub");
}

RHO_WEAK int rho_sysimpl_get_property(char* /*szPropName*/, VALUE* resValue) {
    if (resValue) {
        *resValue = rho_ruby_get_NIL();
    }
    return 0;
}

RHO_WEAK VALUE rho_sys_makephonecall(const char* /*callname*/, int /*nparams*/, char** /*param_names*/, char** /*param_values*/) {
    RAWLOG_INFO("rho_sys_makephonecall() stub");
    return rho_ruby_get_NIL();
}

RHO_WEAK VALUE rho_sys_has_network() {
    RAWLOG_INFO("rho_sys_has_network() stub");
    return rho_ruby_create_boolean(1);
}

RHO_WEAK VALUE rho_sys_get_locale() {
    RAWLOG_INFO("rho_sys_get_locale() stub");
    return rho_ruby_create_string("en");
}

RHO_WEAK int rho_sys_get_screen_width() {
    RAWLOG_INFO("rho_sys_get_screen_width() stub");
    return 0;
}

RHO_WEAK int rho_sys_get_screen_height() {
    RAWLOG_INFO("rho_sys_get_screen_height() stub");
    return 0;
}

RHO_WEAK void rho_sys_impl_exit_with_errormessage(const char* title, const char* message) {
    const char* safeTitle = title ? title : "";
    const char* safeMessage = message ? message : "";
    RAWLOG_ERROR1("rho_sys_impl_exit_with_errormessage() stub", safeTitle);
    RAWLOG_ERROR1("rho_sys_impl_exit_with_errormessage() message", safeMessage);
}

RHO_WEAK void rho_platform_restart_application() {
    RAWLOG_INFO("rho_platform_restart_application() stub");
}

RHO_WEAK int rho_sys_set_sleeping(int /*sleeping*/) {
    RAWLOG_INFO("rho_sys_set_sleeping() stub");
    return 1;
}

RHO_WEAK void rho_sys_run_app(const char* /*appname*/, VALUE /*params*/) {
    RAWLOG_INFO("rho_sys_run_app() stub");
}

RHO_WEAK void rho_sys_bring_to_front() {
    RAWLOG_INFO("rho_sys_bring_to_front() stub");
}

RHO_WEAK void rho_sys_open_url(const char* url) {
    RAWLOG_INFO1("rho_sys_open_url() stub", url ? url : "");
}

RHO_WEAK int rho_sys_is_app_installed(const char* /*appname*/) {
    RAWLOG_INFO("rho_sys_is_app_installed() stub");
    return 0;
}

RHO_WEAK void rho_sys_app_install(const char* url) {
    RAWLOG_INFO1("rho_sys_app_install() stub", url ? url : "");
}

RHO_WEAK void rho_sys_app_uninstall(const char* appname) {
    RAWLOG_INFO1("rho_sys_app_uninstall() stub", appname ? appname : "");
}

RHO_WEAK void rho_sys_set_application_icon_badge(int badge_number) {
    RAWLOG_INFO1("rho_sys_set_application_icon_badge() stub", badge_number);
}

} // extern "C"
