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

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.DisplayMetrics;

import com.google.android.maps.GeoPoint;
import com.google.android.maps.ItemizedOverlay;
import com.google.android.maps.OverlayItem;
import com.rhomobile.rhodes.WebView;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.Utils;

public class AnnotationsOverlay extends ItemizedOverlay<OverlayItem> {
	
	public static final String TAG = "AnnotationOverlay";

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
	
	public void addAnnotations(Vector<Annotation> anns) {
		int i;
		for (i = 0; i < anns.size(); i++) {
			annotations.addElement(anns.elementAt(i));
		}
		populate();
	}
	
	@Override
	protected OverlayItem createItem(int i) {
		Annotation ann = annotations.elementAt(i);
		GeoPoint pnt = new GeoPoint((int)(ann.latitude*1000000), (int)(ann.longitude*1000000));
		OverlayItem item = new OverlayItem(pnt, ann.title, ann.subtitle);
		if (ann.image != null) {
			String imagePath = "apps/" + ann.image;
			imagePath = RhoFileApi.normalizePath(imagePath);
			Bitmap bitmap = BitmapFactory.decodeStream(RhoFileApi.open(imagePath));
			if (bitmap != null) {
				//Utils.platformLog(TAG, "$$$$$$$$$$$$    Bitmap is ["+imagePath+"]");
				//Utils.platformLog(TAG, "$$$$$$$$$$$$           is ["+String.valueOf(bitmap.getWidth())+"x"+String.valueOf(bitmap.getHeight())+"]");
				bitmap.setDensity(DisplayMetrics.DENSITY_MEDIUM);
				BitmapDrawable bd = new BitmapDrawable(bitmap);
				bd.setTargetDensity(DisplayMetrics.DENSITY_MEDIUM);
				bd.setVisible(true, true);
				bd.setBounds(-ann.image_x_offset, -ann.image_y_offset, bitmap.getWidth()-ann.image_x_offset, bitmap.getHeight()-ann.image_y_offset);
				item.setMarker(bd);
			}
		}
		return item;
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
