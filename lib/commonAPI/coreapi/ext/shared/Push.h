/*
#-------------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2013 Rhomobile, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# http://rhomobile.com
#-------------------------------------------------------------------------------
*/

#pragma once

#include "generated/cpp/PushBase.h"

namespace rho { namespace push {

class CPushClient : public CPushBase
{
public:
    virtual const String& getId() const = 0;
    virtual void setDeviceId(const String& deviceId) = 0;
    virtual bool callBack(const String& json) = 0;
};


class CPushManager : public CPushSingletonBase
{
public:
    virtual ~CPushManager() {}
    virtual void addClient(CPushClient* pClient) = 0;
    virtual void setDeviceId(const String& id, const String& deviceId) = 0;
    virtual void callBack(const String& id, const String& json) = 0;

    static CPushManager* getInstance();
};

}}
