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

package rhomobile.mapview;

import net.rim.device.api.ui.Field;

public interface RhoMapField {

	public void close();

	public Field getBBField();

	public void redraw();

	public void setPreferredSize(int width, int height);
	public void move(int dx, int dy);
	public void moveTo(double latitude, double longitude);

	public int calculateZoom(double latDelta, double lonDelta);

	public int getMinZoom();
	public int getMaxZoom();
	public void setZoom(int zoom);
	public int getZoom();

	public void setMapType(String type);

	public int getLeft();
	public int getTop();
	public int getWidth();
	public int getHeight();

	public int getContentLeft();
	public int getContentTop();
	public int getContentWidth();
	public int getContentHeight();

	public double getCenterLatitude();
	public double getCenterLongitude();

	public long toScreenCoordinateX(double longitude);
	public long toScreenCoordinateY(double latitude);

}
