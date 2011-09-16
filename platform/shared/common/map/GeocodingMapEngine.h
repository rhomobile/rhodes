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

#ifndef _RHOGEOCODINGMAPENGINE_H_
#define _RHOGEOCODINGMAPENGINE_H_

#include "common/map/MapEngine.h"
#include "common/ThreadQueue.h"
#include "net/INetRequest.h"

#include "common/RhoThread.h"

#include <list>




namespace rho
{
namespace common
{
namespace map
{

class GoogleGeoCoding : public IGeoCoding, public CThreadQueue
{
    DEFINE_LOGCLASS;
private:
    struct Command : public IQueueCommand
    {
        String address;
        std::auto_ptr<GeoCodingCallback> callback;

        Command(String const &a, GeoCodingCallback *cb)
            :address(a), callback(cb)
        {}

        bool equals(IQueueCommand const &) {return false;}
        String toString();
    };

public:
    GoogleGeoCoding();
    ~GoogleGeoCoding();

    //void stop();
    void resolve(String const &address, GeoCodingCallback *cb);

private:
    bool fetchData(String const &url, void **data, size_t *datasize);

    void processCommand(IQueueCommand* cmd);
    net::CNetRequestWrapper getNet(){ return getNetRequest(&m_NetRequest); }
private:
    NetRequest              m_NetRequest;
};
    

} // namespace map
} // namespace common
} // namespace rho

#endif
