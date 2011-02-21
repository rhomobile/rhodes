#include "common/map/GoogleMapEngine.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoThread.h"
#include "net/URI.h"
#include "json/JSONIterator.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GoogleMapEngine"

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

static bool parse_json(const char *data, double *plat, double *plon)
{
    RHO_MAP_TRACE1("parse_json: data=%s", data);
    json::CJSONEntry json(data);
    const char *status = json.getString("status");
    RHO_MAP_TRACE1("parse_json: status=%s", status);
    if (strcasecmp(status, "OK") != 0)
        return false;
    for (json::CJSONArrayIterator results = json.getEntry("results"); !results.isEnd(); results.next())
    {
        json::CJSONEntry item = results.getCurItem();
        if (!item.hasName("geometry"))
            continue;

        json::CJSONEntry geometry = item.getEntry("geometry");
        json::CJSONEntry location = geometry.getEntry("location");
        *plat = location.getDouble("lat");
        *plon = location.getDouble("lng");
        return true;
    }

    return false;
}

void GoogleGeoCoding::processCommand(IQueueCommand *pCmd)
{
    Command *cmd = (Command*)pCmd;
    GeoCodingCallback &cb = *(cmd->callback);

    String url = "http://maps.googleapis.com/maps/api/geocode/json?address=";
    url += net::URI::urlEncode(cmd->address);
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
    if (parse_json((const char *)data, &latitude, &longitude))
    {
        RHO_MAP_TRACE("GoogleGeoCoding: processCommand: json parsed successfully");
        cb.onSuccess(latitude, longitude);
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

