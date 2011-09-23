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

#include "common/map/GoogleMapEngine.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoThread.h"
#include "net/URI.h"
#include "json/JSONIterator.h"

#include "common/RhoMath.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "common/RhoFile.h"
#include "net/INetRequest.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GoogleMapEngine"

extern "C" unsigned long rjson_tokener_parse(const char *str, char** pszError);

namespace rho { namespace common { namespace map {

GoogleMapView::GoogleMapView(IDrawingDevice *device)
    :BaseMapView(device, "rhogoogle") {}

int GoogleMapView::getMapTile(uint64 p_zoom, uint64 p_row, uint64 p_column, void** p_data, size_t* p_size)
{

    void *data = NULL;
    size_t datasize = 0;

    String url = "";//cmd->baseUrl;

    int zoom = (int)p_zoom;


    uint64 pow2 = rho_math_pow2(zoom);

    double current_width_in_pixels = (double)(pow2 * (TILE_SIZE));
    double current_height_in_pixels = (double)(pow2 * (TILE_SIZE));

    double tile_center_x_in_pixels = (double)(p_column*(TILE_SIZE)+((TILE_SIZE)/2));
    double tile_center_y_in_pixels = (double)(p_row*(TILE_SIZE)+((TILE_SIZE)/2));

    double center_latitude = pixelsToDegreesY((uint64)tile_center_y_in_pixels, zoom);//((double)-90.0) + ((current_height_in_pixels - tile_center_y_in_pixels)/(current_height_in_pixels))*((double)180.0);
    double center_longitude = pixelsToDegreesX((uint64)tile_center_x_in_pixels, zoom);//((double)-180.0) + ((tile_center_x_in_pixels)/(current_width_in_pixels))*((double)360.0);

    // Make url
    char buf[1024];

    // Open Street Map
    //snprintf(buf, sizeof(buf), "http://a.tah.openstreetmap.org/Tiles/tile/%d/%d/%d.png", zoom, (int)p_column, (int)p_row);

    // Google Map
    snprintf(buf, sizeof(buf), "http://maps.googleapis.com/maps/api/staticmap?center=%f,%f&zoom=%d&size=256x280&scale=1&maptype=%s&format=png&sensor=false&mobile=true", center_latitude, center_longitude, zoom, mapType().c_str());

    // Google Map on Blackberry

    // StringBuffer url = new StringBuffer();
    // url.append("http://maps.google.com/maps/api/staticmap?");
    // url.append("center=" + Double.toString(latitude) + "," + Double.toString(longitude));
    // url.append("&zoom=" + cmd.zoom);
    // url.append("&size=" + cmd.width + "x" + cmd.height);
    // url.append("&maptype=" + cmd.maptype);
    // url.append("&format=png&sensor=false");
    // url.append("&mobile=" + (cmd.maptype.equals("roadmap") ? "true" : "false"));
    // url.append("&key=" + mapkey);
    // String finalUrl = url.toString();

    // byte[] data = fetchData(finalUrl);


    url += buf;

    if (!fetchData(url, &data, &datasize))
        return 0;

    *p_data = data;
    *p_size = datasize;

    return 1;
}

IMapView *GoogleMapEngine::createMapView(IDrawingDevice *device)
{
    return new GoogleMapView(device);
}

void GoogleMapEngine::destroyMapView(IMapView *view)
{
    delete view;
}

} // namespace map
} // namespace common
} // namespace rho

