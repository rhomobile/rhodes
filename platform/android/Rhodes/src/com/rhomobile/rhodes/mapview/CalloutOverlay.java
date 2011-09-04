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

import com.rhomobile.rhodes.AndroidR;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;

import java.util.Vector;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.DisplayMetrics;
import android.view.MotionEvent;

import com.google.android.maps.GeoPoint;
import com.google.android.maps.ItemizedOverlay;
import com.google.android.maps.OverlayItem;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.WebView;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.Utils;

public class CalloutOverlay extends ItemizedOverlay<OverlayItem> {
	
	public static final String TAG = "AnnotationOverlay";

	private GoogleMapView mainView = null;
	private Callout mCallout = null; 
	private Annotation mSelectedAnnotation = null;
	
	public CalloutOverlay(GoogleMapView view, Drawable marker) {
		super(boundCenterBottom(marker));
		mainView = view;
		populate();
	}
	
	public void selectAnnotation(Annotation ann) {
		mSelectedAnnotation = ann;
		if (mCallout == null) {
			mCallout = new Callout(ann.latitude, ann.longitude, ann.title, ann.subtitle, ann.url, mainView);
		}
		else {
			mCallout.rebuild(ann.latitude, ann.longitude, ann.title, ann.subtitle, ann.url);
		}
		populate();
	}
	
	public void deselectAnnotation() {
		mSelectedAnnotation = null;
	}
	
	
	@Override
	protected OverlayItem createItem(int i) {
		Annotation ann = mSelectedAnnotation;
		GeoPoint pnt = new GeoPoint((int)(ann.latitude*1000000), (int)(ann.longitude*1000000));
		OverlayItem item = new OverlayItem(pnt, ann.title, ann.subtitle);

		Bitmap bitmap = mCallout.getResultBitmap();
		bitmap.setDensity(DisplayMetrics.DENSITY_MEDIUM);
		BitmapDrawable bd = new BitmapDrawable(bitmap);
		//bd.setTargetDensity(DisplayMetrics.DENSITY_MEDIUM);
		bd.setVisible(true, true);
		bd.setBounds(	ann.callout_x_offset + mCallout.getXOffset(), 
						ann.callout_y_offset + mCallout.getYOffset(), 
						ann.callout_x_offset + mCallout.getXOffset() + bitmap.getWidth(), 
						ann.callout_y_offset + mCallout.getYOffset() + bitmap.getHeight());
		
		item.setMarker(bd);
		return item;
	}

	@Override
	public int size() {
		if (mSelectedAnnotation != null) {
			return 1;
		}
		return 0;
	}
	
	@Override
	protected boolean onTap(int i) {
		if (mSelectedAnnotation == null) {
			return false;
		}
		WebView.navigate(mSelectedAnnotation.url, WebView.activeTab());
		mainView.finish();
		return true;
	}

	@Override
	public boolean onTap(GeoPoint p, com.google.android.maps.MapView mapView) {
		boolean result = super.onTap(p, mapView);
		if (!result) {
			mSelectedAnnotation = null;
			populate();
		}
		return result;
	}

}
