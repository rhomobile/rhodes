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

namespace rho {
namespace sync {

struct ISyncProtocol
{
    virtual const String& getContentType() = 0;
    virtual int getVersion() = 0;

    virtual String getLoginUrl() = 0;
    virtual String getLoginBody( const String& name, const String& password)=0;
    virtual String getClientCreateUrl() = 0;

    virtual String getClientRegisterUrl() = 0;
    virtual String getClientRegisterBody(const String& strClientID, const String& strPin, int nPort, const String& strType, const String& strPhoneID ) = 0;

    virtual String getClientResetUrl(const String& strClientID) = 0;
    virtual String getClientChangesUrl() = 0;

    virtual String getServerQueryUrl(const String& strAction) = 0;
    virtual String getServerQueryBody(const String& strSrcName, const String& strClientID, int nPageSize )=0;

};

}
}
