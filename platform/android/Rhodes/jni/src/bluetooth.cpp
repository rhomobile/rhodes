#include "rhodes/JNIRhodes.h"

#include <common/rhoparams.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Bluetooth"


RHO_GLOBAL int rho_bluetooth_is_bluetooth_available() {
	return 0;
}

RHO_GLOBAL void rho_bluetooth_off_bluetooth() {
}

RHO_GLOBAL void rho_bluetooth_set_device_name(const char* device_name) {
}

RHO_GLOBAL const char* rho_bluetooth_get_device_name() {
	return 0;
}

RHO_GLOBAL const char* rho_bluetooth_get_last_error() {
	return 0;
}

RHO_GLOBAL const char* rho_bluetooth_create_session(const char* role, const char* callback_url) {
	return 0;
}

RHO_GLOBAL void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url) {
}

RHO_GLOBAL void rho_bluetooth_session_disconnect(const char* connected_device_name) {
}

RHO_GLOBAL int rho_bluetooth_session_get_status(const char* connected_device_name) {
	return 0; 
}

RHO_GLOBAL const char* rho_bluetooth_session_read_string(const char* connected_device_name) {
	return 0;
}

RHO_GLOBAL void rho_bluetooth_session_write_string(const char* connected_device_name, const char* str) {
}

RHO_GLOBAL VALUE rho_bluetooth_session_read_data(const char* connected_device_name) {
	return 0;
}

RHO_GLOBAL void rho_bluetooth_session_write_data(const char* connected_device_name, VALUE data) {
}

