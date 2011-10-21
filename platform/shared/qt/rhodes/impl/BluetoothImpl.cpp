/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "common/RhoPort.h"
#include "ext/rho/rhoruby.h"
#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

extern "C" {

int rho_bluetooth_is_bluetooth_available()
{
    return 0;
}

void rho_bluetooth_off_bluetooth()
{
}

void rho_bluetooth_set_device_name(const char* device_name)
{
}

VALUE rho_bluetooth_get_device_name()
{
    return rho_ruby_create_string("ERROR");
}

const char* rho_bluetooth_get_last_error()
{
    return "ERROR";
}

const char* rho_bluetooth_create_session(const char* role, const char* callback_url)
{
    return "ERROR";
}

void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url)
{
}

void rho_bluetooth_session_disconnect(const char* connected_device_name)
{
}

int rho_bluetooth_session_get_status(const char* connected_device_name)
{
    return -1;
}

const char* rho_bluetooth_session_read_string(const char* connected_device_name)
{
    return "ERROR";
}

void rho_bluetooth_session_write_string(const char* connected_device_name, const char* str)
{
}

VALUE rho_bluetooth_session_read_data(const char* connected_device_name)
{
    return rho_ruby_get_NIL();
}

void rho_bluetooth_session_write_data(const char* connected_device_name, VALUE data)
{
    //TODO: rho_bluetooth_session_write_data
}

const char* rho_bluetooth_create_custom_server_session(const char* client_name, const char* callback_url, int accept_any_device)
{
    return NULL;
}

const char* rho_bluetooth_create_custom_client_session(const char* server_name, const char* callback_url)
{
    return NULL;
}

const char* rho_bluetooth_stop_current_connection_process()
{
    return NULL;
}

} //extern "C"
