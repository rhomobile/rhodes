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

#include "common/map/GeocodingMapEngine.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoThread.h"
#include "net/URI.h"
#include "json/JSONIterator.h"

#include "common/RhoMath.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "common/RhoFile.h"
#include "net/INetRequest.h"

#include "ruby/ext/rho/rhoruby.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GeocodingMapEngine"

extern "C" unsigned long rjson_tokener_parse(const char *str, char** pszError);

namespace rho
{
namespace common
{
namespace map
{

String GoogleGeoCoding::Command::toString()
{
    return address;
}
    
    
IMPLEMENT_LOGCLASS(GoogleGeoCoding,"GGeoCoding");
GoogleGeoCoding::GoogleGeoCoding()
{
    CThreadQueue::setLogCategory(getLogCategory());
    RHO_MAP_TRACE("GoogleGeoCoding: ctor start");
    start(epNormal);
    RHO_MAP_TRACE("GoogleGeoCoding: ctor finish");
}

GoogleGeoCoding::~GoogleGeoCoding()
{
    RHO_MAP_TRACE("GoogleGeoCoding: dtor");

    m_NetRequest.cancel();
    CThreadQueue::stop(200);
}
/*
void GoogleGeoCoding::stop()
{
    RHO_MAP_TRACE("GoogleGeoCoding: stop");
    CThreadQueue::stop(200);
}*/

bool GoogleGeoCoding::fetchData(String const &url, void **data, size_t *datasize)
{
    RHO_MAP_TRACE1("GoogleGeoCoding: fetchData: url=%s", url.c_str());
    NetResponse resp = getNet().doRequest("GET", url, "", 0, 0);
    if (!resp.isOK())
        return false;
    *datasize = resp.getDataSize();
    *data = malloc(*datasize);
    if (!*data)
        return false;
    memcpy(*data, resp.getCharData(), *datasize);
    return true;
}

void GoogleGeoCoding::resolve(String const &address, GeoCodingCallback *cb)
{
    RHO_MAP_TRACE1("GoogleGeoCoding: resolve address=%s", address.c_str());
    addQueueCommand(new Command(address, cb));
}
    
void GoogleGeoCoding::resolve(float latitude, float longitude, GeoCodingCallback *cb) {
    addQueueCommand(new Command(latitude, longitude, cb));
}

static bool parse_json(const char *data, double *plat, double *plon, String* adress, bool* coord_ok, bool* adress_ok)
{
    RHO_MAP_TRACE1("parse_json: data=%s", data);
    json::CJSONEntry json(data);
    const char *status = json.getString("status");
    RHO_MAP_TRACE1("parse_json: status=%s", status);
    if (strcasecmp(status, "OK") != 0)
        return false;
    bool params_founded = false;
    if (adress_ok != NULL) {
        *adress_ok = false;
    }
    if (coord_ok != NULL) {
        *coord_ok = false;
    }
    for (json::CJSONArrayIterator results = json.getEntry("results"); !results.isEnd(); results.next())
    {
        json::CJSONEntry item = results.getCurItem();
        
        
        if (item.hasName("formatted_address")) {
            json::CJSONEntry formatted_address = item.getEntry("formatted_address");
            if (adress != NULL) {
                *adress = formatted_address.getString();
            }
            params_founded = true;
            if (adress_ok != NULL) {
                *adress_ok = true;
            }
        }
        
        if (item.hasName("geometry")) {
            json::CJSONEntry geometry = item.getEntry("geometry");
            json::CJSONEntry location = geometry.getEntry("location");
            *plat = location.getDouble("lat");
            *plon = location.getDouble("lng");
            params_founded = true;
            if (coord_ok != NULL) {
                *coord_ok = true;
            }
        }
        
        if (params_founded) {
            return true;
        }
    }

    return false;
}

void GoogleGeoCoding::processCommand(IQueueCommand *pCmd)
{
    Command *cmd = (Command*)pCmd;
    GeoCodingCallback &cb = *(cmd->callback);

    String url = "http://maps.googleapis.com/maps/api/geocode/json?";
    
    if (cmd->is_inverse) {
        char* buf = new char[2048];
        url += "latlng=";
        sprintf(buf, "%f,%f", (float)cmd->latitude, (float)cmd->longitude);
        url += buf;
        delete buf;
    }
    else {
        url += "address=";
        url += net::URI::urlEncode(cmd->address);
    }
    url += "&sensor=false";

    RHO_MAP_TRACE1("GoogleGeoCoding: processCommand: url=%s", url.c_str());

    void *data;
    size_t datasize;
    if (!fetchData(url, &data, &datasize))
    {
        RAWLOG_ERROR1("Can not fetch data by url=%s", url.c_str());
        return;
    }

    RHO_MAP_TRACE("GoogleGeoCoding: processCommand: Parse received json...");

    double latitude, longitude;
    String cadress;
    bool adress_ok = false;
    bool coordinates_ok = false;
    
    if (parse_json((const char *)data, &latitude, &longitude, &cadress, &coordinates_ok, &adress_ok))
    {
        RHO_MAP_TRACE("GoogleGeoCoding: processCommand: json parsed successfully");
        if (cmd->is_inverse && adress_ok) {
            cb.onSuccess(latitude, longitude, cadress.c_str());
        }
        else if (coordinates_ok) {
            if (adress_ok) {
                cb.onSuccess(latitude, longitude, cadress.c_str());
            }
            else {
                cb.onSuccess(latitude, longitude, NULL);
            }
        }
        else {
            RHO_MAP_TRACE("GoogleGeoCoding: processCommand: can't found response in json");
            cb.onError("Can not found response in JSON");
        }
    }
    else
    {
        RHO_MAP_TRACE("GoogleGeoCoding: processCommand: can't parse json");
        cb.onError("Can not parse JSON response");
    }
    /*
    char *error = 0;
    unsigned long json = rjson_tokener_parse((char const *)data, &error);
    if (!rho_ruby_is_NIL(json))
    {
        RHO_MAP_TRACE("GoogleGeoCoding: processCommand: extract coordinates from json...");
        unsigned long coords = rho_ruby_google_geocoding_get_coordinates(json);
        if (rho_ruby_is_NIL(coords))
        {
            RHO_MAP_TRACE("GoogleGeoCoding: processCommand: rho_ruby_google_geocoding_get_coordinates return nil");
            cb.onError("Cannot parse received JSON object");
        }
        else
        {
            RHO_MAP_TRACE("GoogleGeoCoding: processCommand: rho_ruby_google_geocoding_get_coordinates return coordinates");
            double latitude = rho_ruby_google_geocoding_get_latitude(coords);
            double longitude = rho_ruby_google_geocoding_get_longitude(coords);
            RHO_MAP_TRACE2("GoogleGeoCoding: processCommand: latitude=%lf, longitude=%lf", latitude, longitude);
            cb.onSuccess(latitude, longitude);
        }
    }
    else
    {
        RHO_MAP_TRACE("GoogleGeoCoding: processCommand: rjson_tokener_parse return nil");
        cb.onError(error);
    }

    if (error)
        free (error);
    */

    free(data);
}
    
    
    
    
    
    
    

} // namespace map
} // namespace common
} // namespace rho


static rho::common::map::GoogleGeoCoding* ourGeocode = NULL;

static rho::common::map::GoogleGeoCoding* getGeocodeSignletone() {
    if (ourGeocode == NULL) {
        ourGeocode = new rho::common::map::GoogleGeoCoding();
    }
    return ourGeocode;
}

class RhoGoogleGeocodeCallbackImpl : public rho::common::map::GeoCodingCallback {
public:
    RhoGoogleGeocodeCallbackImpl(rho::String adress, rho::String callback, int tag) {
        mAdress = adress;
        mCallback = callback;
        mTag = tag;
    }
    
    virtual ~RhoGoogleGeocodeCallbackImpl() {
        
    }
    
    virtual void onError(rho::String const &description) {
        char* buf = new char[2048];
        
        if (buf == NULL) {
            RAWLOG_ERROR("can not allocate temporary char buffer in GeoLocation callback");
            return;
        }
        
        sprintf(buf,"&rho_callback=1&status=error&description=%s", description.c_str()); 
        
        char* norm_url = rho_http_normalizeurl(mCallback.c_str());
        rho_net_request_with_data(norm_url, buf);
        rho_http_free(norm_url);
        
        delete buf;
        //delete this;
    }

    virtual void onSuccess(double latitude, double longitude, const char* adress) {
        char* buf = new char[2048];
        
        if (buf == NULL) {
            RAWLOG_ERROR("can not allocate temporary char buffer in GeoLocation callback");
            return;
        }
        
        if (adress != NULL) {
            rho::String coded_adr = adress;
            coded_adr = rho::net::URI::urlEncode(coded_adr);
            sprintf(buf,"&rho_callback=1&status=ok&tag=%d&latitude=%f&longitude=%f&adress=%s", mTag, (float)latitude, (float)longitude, coded_adr.c_str()); 
        }
        else {
            sprintf(buf,"&rho_callback=1&status=ok&tag=%d&latitude=%f&longitude=%f", mTag, (float)latitude, (float)longitude); 
        }
        
            
        char* norm_url = rho_http_normalizeurl(mCallback.c_str());
        rho_net_request_with_data(norm_url, buf);
        rho_http_free(norm_url);
                
        delete buf;
        //delete this;
    }
    
    
private:
    rho::String mAdress;
    rho::String mCallback;
    int mTag;
};


void rho_geoimpl_do_geocoding(rho_param* p, const char* callback, int callback_tag) {
    
    const char* c_adress = NULL;
    bool adress_setted = false;
    
    float longitude = 0;
    float latitude = 0;
    bool longitude_setted = false;
    bool latitude_setted = false;
    
    switch (p->type) {
        case RHO_PARAM_HASH: {
            for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
                const char *name = p->v.hash->name[i];
                rho_param *value = p->v.hash->value[i];
                
                if (strcasecmp(name, "adress") == 0) {
					c_adress = value->v.string;
                    adress_setted = true;
                }
                if (strcasecmp(name, "latitude") == 0) {
                    latitude = strtod(value->v.string, NULL);
                    latitude_setted = true;
                }
                if (strcasecmp(name, "longitude") == 0) {
                    longitude_setted = true;
                    longitude = strtod(value->v.string, NULL);
                }
            }
        }
        break;
        default: {
            RAWLOG_ERROR("Unexpected parameter type for do_geocoding, should be Hash");
            return;
        }
    }
    if ((c_adress == NULL) && (!latitude_setted && !longitude_setted)) {
        RAWLOG_ERROR("Unexpected parameter type for do_geocoding, should be Hash with 'adress' or 'latitude' + 'longitude' parameters");
        return;
    }
    
    if (adress_setted) {
        rho::String adress = c_adress;
    
        getGeocodeSignletone()->resolve(adress, new RhoGoogleGeocodeCallbackImpl(adress, callback, callback_tag));\
    }
    else if (latitude_setted && longitude_setted) {
        getGeocodeSignletone()->resolve(latitude, longitude, new RhoGoogleGeocodeCallbackImpl("", callback, callback_tag));
    }
    else {
        RAWLOG_ERROR("Ivalid parameters type for do_geocoding, should be Hash with 'adress' or 'latitude' + 'longitude' parameters");
    }
}


