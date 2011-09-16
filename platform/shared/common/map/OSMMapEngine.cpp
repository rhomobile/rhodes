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

#include "common/map/OSMMapEngine.h"
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
#define DEFAULT_LOGCATEGORY "OSMMapEngine"

extern "C" unsigned long rjson_tokener_parse(const char *str, char** pszError);

namespace rho
{
namespace common
{
namespace map
{

    
    OSMMapView::OSMMapView(IDrawingDevice *device)
    :BaseMapView(device, "osm")
    {
        //MOHUS
        //preloadMapTiles(60.1, 30, 59.7, 30.6, 6, 12);

    }
    
    int OSMMapView::getMapTile(uint64 p_zoom, uint64 p_row, uint64 p_column, void** p_data, size_t* p_size) {
     
        void *data = NULL;
        size_t datasize = 0;

        String url = "";//cmd->baseUrl;
        
        int zoom = (int)p_zoom;
        
        // Make url
        char buf[1024];
        
        // Open Street Map
        snprintf(buf, sizeof(buf), "http://a.tah.openstreetmap.org/Tiles/tile/%d/%d/%d.png", zoom, (int)p_column, (int)p_row);
        
        url += buf;
     
        RAWLOG_ERROR("###########  getMapTime() BEFORE FETCH");
        
        if (!fetchData(url, &data, &datasize))
            return 0;

        RAWLOG_ERROR("###########  getMapTime() AFTER FETCH");
        
        
        *p_data = data;
        *p_size = datasize;
        
        return 1;
        
    }
    
    
    
    
    
    IMapView *OSMMapEngine::createMapView(IDrawingDevice *device)
    {
        return new OSMMapView(device);
    }
    
    void OSMMapEngine::destroyMapView(IMapView *view)
    {
        delete view;
    }
    
    
    
    

} // namespace map
} // namespace common
} // namespace rho

