package com.rho.rhoelements.plugins;

import java.util.HashSet;
import android.content.Context;
import android.gesture.GestureOverlayView;
import android.graphics.Canvas;
import android.graphics.Path;
import android.graphics.PointF;
import android.util.AttributeSet;

public class ElementsGestureOverlayView extends GestureOverlayView
{
	private HashSet<ElementsGesture> mGestureDiagsList = new HashSet<ElementsGesture>();
	public GestureLine mGestureLine;
	
	public ElementsGestureOverlayView(Context context)
	{
		super(context);
		setGestureVisible(false);
	}

	public ElementsGestureOverlayView(Context c, AttributeSet a)
	{
		super(c, a);
		setGestureVisible(false);
	}
	
	public void addGestureDiags(ElementsGesture g)
	{
		if (mGestureDiagsList.contains(g))
		{
			mGestureDiagsList.remove(g);
		}
		if (g.mDiagActivate)
		{
			mGestureDiagsList.add(g);
		}
		GestureLine line = new GestureLine();
		if (mGestureDiagsList.contains(line))
		{
			mGestureDiagsList.remove(line);
		}
		if (!mGestureDiagsList.isEmpty())
		{
			mGestureDiagsList.add(line);
			mGestureLine = line;
		}
		else
		{
			mGestureLine = null;
		}
		
	}
	
	public void removeGestureDiags(String id)
	{
		ElementsGesture deleteMe = null;
		
		for (ElementsGesture g : mGestureDiagsList)
		{
			if (g.mGestureId.equals(id))
			{
				deleteMe = g;
				break;
			}
		}
		
		if (deleteMe != null)
			mGestureDiagsList.remove(deleteMe);
	}
	
	public void removeAllGestureDiags()
	{
			mGestureDiagsList.clear();
			setGestureVisible(false);
	}
	
	@Override
	public void onDraw(Canvas c)
	{
		for (ElementsGesture g : mGestureDiagsList)
		{
			g.render(c);
		}
	}
	
	public void drawComplete()
	{
		if (!mGestureDiagsList.isEmpty())
		{
			invalidate();
		}
	}
	
	class GestureLine extends ElementsGesture
	{
		private Path mGesturePath;
		
		public GestureLine()
		{
			mGestureId = "_gesturetrack_";
			mDiagActivate = true;
		}
		
		@Override
		public void onGestureStarted(PointF point)
		{
			if (mGestureState != INACTIVE_STATE)
			{
				mGesturePath = new Path();
				mGesturePath.moveTo(point.x-1, point.y);
				mGesturePath.lineTo(point.x, point.y);
			}
			else
			{
				mGesturePath = null;
			}
		}
		
		@Override
		public void onGesture(PointF point)
		{
			if (mGesturePath != null)
			{
				mGesturePath.lineTo(point.x, point.y);
			}
		}
		
		@Override
		public void render(Canvas c)
		{
			if (mGesturePath != null)
			{
				c.drawPath(mGesturePath, mGestureTrackPaint);
			}
		}
	}
}

