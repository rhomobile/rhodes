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

#include "common/map/ESRIMapEngine.h"
#include "common/map/GeocodingMapEngine.h"
#include "common/RhoMath.h"
#include "common/RhoConf.h"
#include "common/IRhoClassFactory.h"
#include "common/RhodesApp.h"
#include "net/INetRequest.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "ESRIMapEngine"

namespace rho { namespace common { namespace map {


ESRIMapView::ESRIMapView(IDrawingDevice *device)
    : BaseMapView(device, "esri") {}

int ESRIMapView::getMapTile(uint64 p_zoom, uint64 p_row, uint64 p_column, void** p_data, size_t* p_size)
{
    void *data = NULL;
    size_t datasize = 0;

    String url = getMapUrl();
    if (url.empty())
    {
        RAWLOG_ERROR("MapFetch::processCommand: passed empty base url");
        return 0;
    }
    // Append last '/' if not exist
    if (url[url.size() - 1] != '/')
        url.push_back('/');

    // Make url

    char buf[1024];
    snprintf(buf, sizeof(buf), "MapServer/tile/%d/%d/%d", (int)p_zoom, (int)p_row, (int)p_column);

    url += buf;

    if (!fetchData(url, &data, &datasize))
        return 0;

    *p_data = data;
    *p_size = datasize;

    return 1;
}

IMapView *ESRIMapEngine::createMapView(IDrawingDevice *device)
{
    return new ESRIMapView(device);
}

void ESRIMapEngine::destroyMapView(IMapView *view)
{
    delete view;
}

} // namespace map
} // namespace common
} // namespace rho
