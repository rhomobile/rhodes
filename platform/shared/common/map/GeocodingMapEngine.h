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
        float longitude;
        float latitude;
        String address;
        bool is_inverse;
        std::auto_ptr<GeoCodingCallback> callback;

        Command(String const &a, GeoCodingCallback *cb)
            :longitude(0), latitude(0), address(a), callback(cb)
        {
            is_inverse = false;
        }
        Command(float _latitude, float _longitude, GeoCodingCallback *cb)
        :longitude(_longitude), latitude(_latitude), address(""), callback(cb)
        {
            is_inverse = true;
        }

        bool equals(IQueueCommand const &) {return false;}
        String toString();
    };

public:
    GoogleGeoCoding();
    ~GoogleGeoCoding();

    //void stop();
    void resolve(String const &address, GeoCodingCallback *cb);
    void resolve(float latitude, float longitude, GeoCodingCallback *cb);

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


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    
    
void rho_geoimpl_do_geocoding(rho_param* p, const char* callback, int callback_tag);    
    
    void rho_geocoding_parse_json_responce(const char* data, char* adress_buf, int max_adress_length, double* latitude, double* longitude, int* is_adress_ok, int* is_coords_ok);    
    
#ifdef __cplusplus
}
#endif


#endif
