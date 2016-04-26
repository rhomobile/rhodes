package com.rho.rhoelements.plugins;

import java.util.Vector;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Paint.Align;
import android.graphics.Path;
import android.graphics.PointF;
import android.graphics.Paint.Style;
import android.graphics.Typeface;

public abstract class ElementsGesture
{
	protected final static int START_STATE = 0;
	protected final static int ACTIVE_STATE = 1;
	protected final static int SIGNALLED_STATE = 2;
	protected final static int INACTIVE_STATE = 3;
	
	protected String mGestureId;
	protected int mActivationCounter = 0;
	protected float mDeviation;
	protected float mSkewAngle;
	protected Paint mCrossedPaint;
	protected Paint mCrossedOutlinePaint;
	protected Paint mUncrossedPaint;
	protected Paint mUncrossedOutlinePaint;
	protected Paint mHighlightPaint;
	protected Paint mGestureTrackPaint;
	protected Paint mTextErrorPaint;
	protected Paint mTextOkPaint;
	protected Paint mTextCounterPaint;
	protected Paint mBestFitLinePaint;
	protected Paint mGestureLinePaint;
	protected boolean mDiagActivate = false;
	protected int mGestureState;
	protected PointF mLastTouchPoint = new PointF(0xFFFF, 0xFFFF);
	protected GesturePlugin mPlugin;
	protected Vector<GestureRegion> mRegions = new Vector<GestureRegion>();

	@Override
	public boolean equals(Object o)
	{
		if (o instanceof ElementsGesture)
		{
			return ((ElementsGesture) o).mGestureId.equals(mGestureId);
		}
		return false;
	}
	
	@Override
	public int hashCode()
	{
		return mGestureId.hashCode();
	}
	
	public void onGestureStarted(PointF point)
	{
		mLastTouchPoint.set(point);
		mGestureState = START_STATE;
	}
	
	public void onGestureEnded(PointF point, Vector<PointF> gestureTrack) 
	{
		mLastTouchPoint.set(0xFFFF, 0xFFFF);
		if (mGestureState != SIGNALLED_STATE)
		{
			mGestureState = START_STATE;
		}
	}
	
	/**
	 * This method is valid only for tilt & shake sensor gestures
	 * They override this method and other gestures dont need to do anything 
	 */
	public void onUnregisterSensor()
	{		
	}
	
	public void onGesture(PointF point) 
	{
		mLastTouchPoint.set(point);
	}
	
	public void onGestureCancelled() 
	{
		mGestureState = START_STATE;
	}
	
	public void render(Canvas c)
	{
		if (mDiagActivate)
		{
			for (GestureRegion r : mRegions)
			{
				r.refreshState();
				if (r.mDiagState == GestureRegion.UNCROSSED)
				{
					r.render(c);
				}
			}		
			for (GestureRegion r : mRegions)
			{
				if (r.mDiagState == GestureRegion.CROSSED)
				{
					r.render(c);
				}
			}
			for (GestureRegion r : mRegions)
			{
				if (r.mDiagState == GestureRegion.HIGHLIGHTED)
				{
					r.render(c);
				}
			}
		}
	}
	

	public ElementsGesture()
	{
		mCrossedPaint = new Paint();
		mCrossedPaint.setARGB(32, 0, 255, 0);
		mCrossedPaint.setAntiAlias(true);
		
		mCrossedOutlinePaint = new Paint(mCrossedPaint);
		mCrossedOutlinePaint.setStyle(Style.STROKE);
		mCrossedOutlinePaint.setStrokeWidth(5);
		mCrossedOutlinePaint.setStrokeCap(Paint.Cap.ROUND);
		
		mUncrossedPaint = new Paint();
		mUncrossedPaint.setARGB(32, 0, 0, 255);
		mUncrossedPaint.setAntiAlias(true);
		
		mUncrossedOutlinePaint = new Paint(mUncrossedPaint);
		mUncrossedOutlinePaint.setStyle(Style.STROKE);
		mUncrossedOutlinePaint.setStrokeWidth(5);
		mUncrossedOutlinePaint.setStrokeCap(Paint.Cap.ROUND);
		
		mHighlightPaint = new Paint();
		mHighlightPaint.setARGB(192, 255, 255, 0);
		mHighlightPaint.setStyle(Style.STROKE);
		mHighlightPaint.setStrokeWidth(7);
		mHighlightPaint.setStrokeCap(Paint.Cap.ROUND);
		mHighlightPaint.setAntiAlias(true);
		
		mGestureTrackPaint = new Paint(mHighlightPaint);
		mGestureTrackPaint.setStrokeWidth(10);
		
		mTextErrorPaint = new Paint();
		mTextErrorPaint.setARGB(192, 255, 0, 0);
		mTextErrorPaint.setTypeface(Typeface.DEFAULT_BOLD);
		mTextErrorPaint.setTextSize(20);
		mTextErrorPaint.setAntiAlias(true);

		mTextOkPaint = new Paint();
		mTextOkPaint.setARGB(192, 32, 0, 255);
		mTextOkPaint.setTextSize(20);
		mTextOkPaint.setAntiAlias(true);
		
		mTextCounterPaint = new Paint();
		mTextCounterPaint.setARGB(192, 32, 0, 255);
		mTextCounterPaint.setTextSize(20);
		mTextCounterPaint.setTextAlign(Align.CENTER);
		mTextCounterPaint.setAntiAlias(true);
		
		mBestFitLinePaint = new Paint();
		mBestFitLinePaint.setARGB(255, 0, 255, 0);
		mBestFitLinePaint.setAntiAlias(true);

		mGestureLinePaint = new Paint();
		mGestureLinePaint.setARGB(128, 0, 0, 0);
		mGestureLinePaint.setAntiAlias(true);
	}
	
	public void onDetected() 
	{
		mGestureState = SIGNALLED_STATE;
		GesturePlugin.detected(mPlugin.new GestureEvent(mGestureId, mActivationCounter));
	};

	protected void calculateDot(PointF tail, PointF head, float radius)
	{
		if (!tail.equals(head.x, head.y))
		{
			float dx = head.x - tail.x;
			float dy = head.y - tail.y;
			if (Math.hypot(Math.abs(dx), Math.abs(dy)) > radius)
			{
				double theta = Math.atan2(head.y - tail.y, head.x - tail.x);
				
				// find the offsets to the edge of the circle
				dx = (float)(Math.cos(theta) * radius);
				dy = (float)(Math.sin(theta) * radius);

				// move the start of the line to the edge of the circle
				tail.x += dx;
				tail.y += dy;
			}
		}
	}
	
	protected Path calculateArrow(PointF tail, PointF point)
	{
		Path arrow = null;
		
		if (!point.equals(tail.x, tail.y))
		{
			arrow = new Path();

			double theta = Math.atan2(point.y - tail.y, point.x - tail.x);
			
			// find the offsets to the centre point for the arrowhead
			float dx = (float)(Math.cos(theta) * 5);
			float dy = (float)(Math.sin(theta) * 5);
			
			// draw the arrowhead
			arrow.moveTo(point.x - dx, point.y - dy);
			arrow.rMoveTo(-5, -4);
			arrow.rLineTo(10, 4);
			arrow.rLineTo(-10, 4);
			arrow.close();
			
			// rotate the arrowhead around the centre point to the angle of the line
			Matrix matrix = new Matrix();
			matrix.setRotate((float) Math.toDegrees(theta), point.x - dx, point.y - dy);
			arrow.transform(matrix);
			
			// if the line is long enough.. move the point to the edge of the arrowhead
			if (Math.hypot(Math.abs(point.x - tail.x), Math.abs(point.y - tail.y)) > 10)
			{
				point.x -= (2*dx);
				point.y -= (2*dy);
			}
			else
			{
				point.set(tail);
			}
		}
		
		return arrow;
	}
	
	protected class GestureRegion
	{
		public boolean crossed;
		protected static final int NOT_SHOWN = 0;
		protected static final int UNCROSSED = 1;
		protected static final int CROSSED = 2;
		protected static final int HIGHLIGHTED = 3;
		protected static final int OUTLINED = 4;
		protected int mDiagState = NOT_SHOWN;

		/**
		* Draws diagnostic data, if any.
		* \param hdc Device contect to draw into
		*/
		public void render(Canvas c) {};

		/**
		* Checks if region contains specified point
		* \param point Point to check
		*/
		public boolean containsPoint (PointF point) {return false;}

		public void refreshState()
		{
			if (mDiagActivate)
			{
				if (mGestureState == SIGNALLED_STATE)
				{
					// remove any highlight but render crossed regions
					if (mDiagState == HIGHLIGHTED)
					{
						mDiagState = CROSSED;
					}
				}
				else if (mGestureState != INACTIVE_STATE)
				{
					if (containsPoint(mLastTouchPoint))
					{
						mDiagState = HIGHLIGHTED;
					}
					else if (crossed)
					{
						mDiagState = CROSSED;					
					}
					else
					{
						mDiagState = UNCROSSED;
					}
				}
			}
			else
			{
				mDiagState = NOT_SHOWN;
			}	
		}
	}
}

