#include "stdafx.h"

#if defined(_WIN32_WCE)
#include <aygshell.h>
#endif
#include <atltime.h>
#include "ext/rho/rhoruby.h"
#include "../MainWindow.h"
#include "Bluetooth.h"
#include "common/RhodesApp.h"

#ifdef _MSC_VER
// warning C4800: 'int' : forcing to bool 'true' or 'false' (performance warning)
#pragma warning ( disable : 4800 )
#endif

extern "C" HWND getMainWnd();

#if defined(_WIN32_WCE)
// strdup is implemented as part of ruby CE port
extern "C" char *strdup(const char * str);
extern "C" wchar_t* wce_mbtowc(const char* a);
#endif



int rho_bluetooth_is_bluetooth_available() {
	return 0;
}

void rho_bluetooth_off_bluetooth() {
}

void rho_bluetooth_set_device_name(const char* device_name) {
}

const char* rho_bluetooth_get_device_name() {
	return 0;
}

const char* rho_bluetooth_get_last_error() {
	return 0;
}

const char* rho_bluetooth_create_session(const char* role, const char* callback_url) {
	return 0;
}

void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url) {
}

void rho_bluetooth_session_disconnect(const char* connected_device_name) {
}

int rho_bluetooth_session_get_status(const char* connected_device_name) {
	return 0; 
}

const char* rho_bluetooth_session_read_string(const char* connected_device_name) {
	return 0;
}

void rho_bluetooth_session_write_string(const char* connected_device_name, const char* str) {
}

VALUE rho_bluetooth_session_read_data(const char* connected_device_name) {
	return 0;
}

void rho_bluetooth_session_write_data(const char* connected_device_name, VALUE data) {
}

