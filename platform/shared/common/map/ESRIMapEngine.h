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

#ifndef _RHOESRIMAPENGINE_H_
#define _RHOESRIMAPENGINE_H_

#include "common/map/MapEngine.h"
#include "common/map/BaseMapEngine.h"
#include "common/RhoThread.h"
#include "common/ThreadQueue.h"
#include "net/INetRequest.h"

#include <list>

namespace rho
{
namespace common
{
namespace map
{
    class ESRIMapView : public BaseMapView
    {
    public:
        ESRIMapView(IDrawingDevice *device);
        
        virtual int getMapTile(uint64 p_zoom, uint64 p_row, uint64 p_column, void** p_data, size_t* p_size);
        
        virtual void setGoogleLogoImage(IDrawingImage *GoogleLogoImg) {}
        
    };
    
    
    class ESRIMapEngine : public IMapEngine
    {
    public:
        IMapView *createMapView(IDrawingDevice *device);
        void destroyMapView(IMapView *view);
    };

} // namespace map
} // namespace common
} // namespace rho

#endif
