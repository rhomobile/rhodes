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

package com.rhomobile.rhodes.mapview;

import java.util.Vector;

import android.graphics.drawable.Drawable;

import com.google.android.maps.GeoPoint;
import com.google.android.maps.ItemizedOverlay;
import com.google.android.maps.OverlayItem;
import com.rhomobile.rhodes.WebView;

public class AnnotationsOverlay extends ItemizedOverlay<OverlayItem> {

	private GoogleMapView mainView;
	private Vector<Annotation> annotations;
	
	public AnnotationsOverlay(GoogleMapView view, Drawable marker) {
		super(boundCenterBottom(marker));
		mainView = view;
		annotations = new Vector<Annotation>();
		populate();
	}
	
	public void addAnnotation(Annotation ann) {
		annotations.addElement(ann);
		populate();
	}
	
	@Override
	protected OverlayItem createItem(int i) {
		Annotation ann = annotations.elementAt(i);
		GeoPoint pnt = new GeoPoint((int)(ann.latitude*1000000), (int)(ann.longitude*1000000));
		return new OverlayItem(pnt, ann.title, ann.subtitle);
	}

	@Override
	public int size() {
		return annotations.size();
	}
	
	@Override
	protected boolean onTap(int i) {
		Annotation ann = annotations.elementAt(i);
		if (ann.url == null)
			return false;
		WebView.navigate(ann.url, WebView.activeTab());
		mainView.finish();
		return true;
	}

}
