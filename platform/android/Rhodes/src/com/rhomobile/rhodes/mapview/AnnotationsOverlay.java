package com.rhomobile.rhodes.mapview;

import java.util.Vector;

import android.graphics.drawable.Drawable;

import com.google.android.maps.GeoPoint;
import com.google.android.maps.ItemizedOverlay;
import com.google.android.maps.OverlayItem;

public class AnnotationsOverlay extends ItemizedOverlay<OverlayItem> {

	private Vector<Annotation> annotations;
	
	public AnnotationsOverlay(Drawable marker) {
		super(boundCenterBottom(marker));
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
		return false;
	}

}
