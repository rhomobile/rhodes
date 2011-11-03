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

import com.rho.Sprintf;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;

public class Annotation {

	public static class Coordinates {
		public double latitude;
		public double longitude;
		
		public Coordinates(double lat, double lon) {
			latitude = lat;
			longitude = lon;
		}
	};
	
	public String title = "";
	public String subtitle = "";
	public String street_address = null;
	public String resolved_address = null;
	public String url = null;
	public Coordinates coordinates = null;
	public boolean pass_location = false;
	
	
	public String make_url()
	{
		if (!pass_location)
			return url;
		
		RubyArray args = new RubyArray();
		args.add(ObjectFactory.createFloat(coordinates.latitude));
		args.add(ObjectFactory.createFloat(coordinates.longitude));
		
		return url + ( url.indexOf('?') >= 0 ? "&" : "?" ) + Sprintf.sprintf("latitude=%.5f&longitude=%.5f", args);
	}
}
