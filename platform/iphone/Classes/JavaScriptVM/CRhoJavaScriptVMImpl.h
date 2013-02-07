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

#include <scriptvm/ScriptVM.h>

/*#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>*/

/*#import "WebViewContainer.h"*/

#include <objc/objc-runtime.h>
#include "WebViewCInterface.h"
 
namespace rho{
namespace common{

class CRhoJavaScriptVMImpl : public IRhoScriptVM
{
    DEFINE_LOGCLASS;
    WebViewCInterface* _WVContainer;
public:
    CRhoJavaScriptVMImpl(void) {
        _WVContainer = new WebViewCInterface();
        if (_WVContainer)
        {
            _WVContainer->init();
        }
    }
    ~CRhoJavaScriptVMImpl(void) {
        if (_WVContainer) { delete _WVContainer; _WVContainer = NULL; }
    }
    
    virtual int getVMId() { return VM_JAVASCRIPT; }
    virtual bool loadScript(const String& buffer, const String& tag) { return false; }
    virtual bool unloadScript(const String& tag) { return false; }
    
    virtual String performAction(const String& actionName, const String& params) { return String("false");}
    
private:
};

}
}

