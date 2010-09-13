






extern "C" int rho_bluetooth_is_bluetooth_available();

extern "C" void rho_bluetooth_off_bluetooth();

extern "C" void rho_bluetooth_set_device_name(const char* device_name);

extern "C" const char* rho_bluetooth_get_device_name();

extern "C" const char* rho_bluetooth_get_last_error();

extern "C" const char* rho_bluetooth_create_session(const char* role, const char* callback_url);

extern "C" void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url);

extern "C" void rho_bluetooth_session_disconnect(const char* connected_device_name);

extern "C" int rho_bluetooth_session_get_status(const char* connected_device_name);

extern "C" const char* rho_bluetooth_session_read_string(const char* connected_device_name);

extern "C" void rho_bluetooth_session_write_string(const char* connected_device_name, const char* str);

extern "C" VALUE rho_bluetooth_session_read_data(const char* connected_device_name);

extern "C" void rho_bluetooth_session_write_data(const char* connected_device_name, VALUE data);

