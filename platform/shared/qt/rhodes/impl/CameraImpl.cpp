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
    CHoldRubyValue info(rho_ruby_createHash());
    CHoldRubyValue max_res(rho_ruby_createHash());
    rho_ruby_add_to_hash(max_res, rho_ruby_create_string("width"), rho_ruby_create_integer(2048));
    rho_ruby_add_to_hash(max_res, rho_ruby_create_string("height"), rho_ruby_create_integer(1152));
    rho_ruby_add_to_hash(info, rho_ruby_create_string("max_resolution"), max_res);
    return info;
}

} //extern "C"
