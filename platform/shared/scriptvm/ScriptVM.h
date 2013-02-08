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

#pragma once

#include "common/RhoStd.h"
#include "common/AutoPointer.h"

namespace rho {
namespace common {
    
enum eVM {
    VM_JAVASCRIPT = 0,
    NUM_VIRTUAL_MACHINES
};

struct IRhoScriptVM
{
    virtual ~IRhoScriptVM(void){}
    
    virtual int getVMId() = 0;
    
    virtual bool loadScript(const rho::String& buffer, const rho::String& tag) = 0;
    virtual bool unloadScript(const rho::String& tag) = 0;
    
    virtual rho::String performAction(const rho::String& actionName, const rho::String& params) = 0;
};
    
}
}

extern "C" {
    bool rho_javascriptvm_load_script(const char* buffer, const char* tag);
    bool rho_javascriptvm_unload_script(const char* tag);
    void rho_javascriptvm_perform_action(const char* actionName, const char* params);
}
