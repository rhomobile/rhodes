package com.rho.rhoelements.plugins;

import java.util.Vector;
import android.graphics.Canvas;
import android.graphics.Paint.Style;
import android.graphics.Path;
import android.graphics.PointF;
import android.graphics.RectF;

public class CircleGesture extends ElementsGesture
{
	private int mSensitivity;
	private int mCurrentRegion = 0;
	private float mGestureLineStartAngle;
	private float mGestureLineSweep;
	private RectF mGestureLineOval;
	private PointF mGestureDot = new PointF();
	private Path mArrow = null;
	
	public CircleGesture(String id, PointF centre, int radius, int start_angle, int end_angle,
			int tolerance, int sensitivity, boolean diags, Object plugin)
	{
		float start_radians, end_radians, angle, increment;
		float dx, dy, dx_squared, dy_squared, tan_squared;
		boolean anti_clockwise;
		int n, nRegions;

		mGestureId = new String(id);
		mDiagActivate = diags;
		mPlugin = (GesturePlugin) plugin;

		if (end_angle < start_angle)
		{
			n = start_angle;
			start_angle = end_angle;
			end_angle = n;
			anti_clockwise = true;
		}
		else
			anti_clockwise = false;
		
		// Convert to radians
		start_radians = (float) Math.toRadians(start_angle);
		end_radians = (float) Math.toRadians(end_angle);

		// get the gesture angle
		angle = end_radians - start_radians;
		// work out how many regions to fit in the arc
		nRegions = (int)Math.ceil((angle*radius)/tolerance)+1;
		// work out the angle between each region
		increment = nRegions > 1 ? (float) (angle/(nRegions-1)) : 0;

		// Create regions
		if (anti_clockwise)
			angle = (float) end_radians;
		else
			angle = (float) start_radians;

		for (n = 0; n < nRegions; n++)
		{
			tan_squared = (float) Math.tan(angle);
			if (tan_squared != Float.NaN)	// 90 and 270 degrees will return NaN
			{
				tan_squared *= tan_squared;
	
				dx_squared = ((radius * radius) / (1 + tan_squared));
				dx =  (float)Math.sqrt ((double)dx_squared);
	
				dy_squared = dx_squared * tan_squared;
				dy = (float)Math.sqrt ((double)dy_squared);
	
				switch (getQuadrant(angle))
				{
					case 1:
						dx = -dx;
						break;
	
					case 2:
						dx = -dx;
						dy = -dy;
						break;
	
					case 3:
						dy = -dy;
						break;
				}
	
				CircleRegion region = new CircleRegion(centre.x + dx, centre.y + dy, (float) tolerance);
				mRegions.add(region);
			}
			if (anti_clockwise)
				angle -= increment;
			else
				angle += increment;
		}

		// Record parameters for later fit testing
		mSensitivity = sensitivity;
		
		if (mDiagActivate)
		{
			if (anti_clockwise)
			{
				// line is drawn from point to tail
				
				// find the coordinates of the arrowhead point
				PointF end = new PointF(centre.x + ((float)(Math.cos(start_radians) * radius)),
									centre.y + ((float)(Math.sin(start_radians) * radius)));
				
				// calculate the angle of the arrow (perpendicular to start_angle)
				double arrowAngle = start_radians + (Math.PI/2);
				
				// project the coordinates for the arrow tail
				PointF start = new PointF(end.x + (float)(Math.cos(arrowAngle) * 10), 
						end.y + (float)(Math.sin(arrowAngle) * 10));

				// find the angle to offset the start angle (arrowhead)
				mGestureLineStartAngle = start_radians + (float)Math.atan((double)10/(double)radius);
				
				// calculate the arrow vertices
				mArrow = calculateArrow(start, end);
				
				// find the coordinates of the start dot
				mGestureDot.x = centre.x + (float)(Math.cos(end_radians) * radius);
				mGestureDot.y = centre.y + (float)(Math.sin(end_radians) * radius);
				
				// find the angle to offset the end angle (dot)
				float endAngle = end_radians - (float)Math.atan((double)4/(double)radius);
				
				mGestureLineSweep = endAngle - mGestureLineStartAngle;
			}
			else
			{
				// find the coordinates of the start dot
				mGestureDot.x = centre.x + (float)(Math.cos(start_radians) * radius);
				mGestureDot.y = centre.y + (float)(Math.sin(start_radians) * radius);
				
				// find the angle to offset the start angle
				float theta = (float) Math.atan((double)4/(double)radius);
				mGestureLineStartAngle = start_radians + theta;
				
				// find the coordinates of the arrowhead point
				PointF end = new PointF(centre.x + ((float)(Math.cos(end_radians) * radius)),
									centre.y + ((float)(Math.sin(end_radians) * radius)));
				
				// calculate the angle of the arrow (perpendicular to end_angle)
				double arrowAngle = end_radians - (Math.PI/2);
				
				// project the coordinates for the arrow tail
				PointF start = new PointF(end.x + (float)(Math.cos(arrowAngle) * 10), 
						end.y + (float)(Math.sin(arrowAngle) * 10));
				
				// calculate the arrow vertices
				mArrow = calculateArrow(start, end);
				
				// find the angle to offset the end angle
				float endAngle = end_radians - (float)Math.atan((double)10/(double)radius);
				
				mGestureLineSweep = endAngle - mGestureLineStartAngle;
			}

			// store the arc perimeter
			mGestureLineOval = new RectF(centre.x - radius, centre.y - radius,
					centre.x + radius, centre.y + radius);
		}
	}
	
	@Override
	public void onGestureEnded(PointF point, Vector<PointF> gestureTrack)
	{
		super.onGestureEnded(point, gestureTrack);
		if ((mRegions != null) && (mRegions.isEmpty() == false) &&
				(gestureTrack != null) && (gestureTrack.size() > 2))
		{
			// Region crossing calculations can be done at the end if we're not drawing regions
			for (GestureRegion c : mRegions)
			{
				c.crossed = false;
			}

			int current = 0;
			int found = -1;
			for (PointF p : gestureTrack)
			{
				found = -1;
				int r;
				for (r = current; r < mRegions.size(); r++)
				{
					if (mRegions.get(r).containsPoint(p))
					{
						found = r;
						break;
					}
				}
				if (found == -1)
				{
					break;
				}
				mRegions.get(r).crossed = true;
				current = found;
			}
			
			int count = 0;
			if (found != -1)
			{
				for (GestureRegion r : mRegions)
				{
					if (r.crossed)
					{
						count++;
					}
				}
				if (count >= (mRegions.size() * mSensitivity) / 100)
				{
					onDetected();					
				}
			}
		}
	}

	@Override
	public void onGestureStarted(PointF point) 
	{
		super.onGestureStarted(point);

		for (GestureRegion c : mRegions)
		{
			c.crossed = false;
		}

		if (!mDiagActivate)
		{
			return;
		}

		mCurrentRegion = 0;
		int found = -1;
		found = -1;
		int r;
		for (r = mCurrentRegion; r < mRegions.size(); r++)
		{
			if (mRegions.get(r).containsPoint(point))
			{
				found = r;
				break;
			}
		}
		if (found == -1)
		{
		}
		else
		{
			mGestureState = ACTIVE_STATE;
			mCurrentRegion = found;
		}
	}
	
	@Override
	public void onGesture(PointF point) 
	{
		super.onGesture(point);
		if ((mDiagActivate) && (mGestureState != INACTIVE_STATE))
		{
			int found = -1;
			found = -1;
			int r;
			for (r = mCurrentRegion; r < mRegions.size(); r++)
			{
				if (mRegions.get(r).containsPoint(point))
				{
					found = r;
					break;
				}
			}
			if (found == -1)
			{
				mGestureState = INACTIVE_STATE;
			}
			else
			{
				mRegions.get(r).crossed = true;
				mCurrentRegion = found;
				mGestureState = ACTIVE_STATE;
			}
		}
	}
	
	@Override
	public void render(Canvas c)
	{
		super.render(c);
		if ((mDiagActivate) && (mGestureState != INACTIVE_STATE))
		{
			if (mArrow != null)
			{
				mGestureLinePaint.setStyle(Style.FILL);
				c.drawCircle(mGestureDot.x, mGestureDot.y, 4, mGestureLinePaint);
				c.drawPath(mArrow, mGestureLinePaint);
			}
			mGestureLinePaint.setStyle(Style.STROKE);
			c.drawArc(mGestureLineOval, (float)Math.toDegrees(mGestureLineStartAngle), 
					(float)Math.toDegrees(mGestureLineSweep), false, mGestureLinePaint);
		}
	}
	
	private class CircleRegion extends GestureRegion
	{
		private float mCentreX;		///< Centre of circle
		private float mCentreY;		///< Centre of circle
		private float mRadius;		///< Radius of circle

		public CircleRegion(float centre_x, float centre_y, float radius)
		{
			mCentreX = centre_x;
			mCentreY = centre_y;
			mRadius = radius;
		}
		

		@Override
		public void render(Canvas c)
		{
			if (mGestureState != INACTIVE_STATE)
			{
				if (mDiagState == UNCROSSED)
				{
					c.drawCircle(mCentreX, mCentreY, mRadius, mUncrossedPaint);						
				}
				else if (mDiagState == CROSSED)
				{
					c.drawCircle(mCentreX, mCentreY, mRadius, mCrossedPaint);
				}
				else if (mDiagState == HIGHLIGHTED)
				{
					c.drawCircle(mCentreX, mCentreY, mRadius, mCrossedPaint);
					c.drawCircle(mCentreX, mCentreY, mRadius, mHighlightPaint);
				}
			}
			else
			{
				if (mDiagState == UNCROSSED)
				{
					c.drawCircle(mCentreX, mCentreY, mRadius, mUncrossedOutlinePaint);
				}
				else
				{
					c.drawCircle(mCentreX, mCentreY, mRadius, mCrossedOutlinePaint);
				}				
			}
		}

		@Override
		public boolean containsPoint(PointF point)
		{
			double distance;

			distance = Math.sqrt(((point.x - mCentreX) * (point.x - mCentreX)) + ((point.y - mCentreY) * (point.y - mCentreY)));

			return distance <= mRadius;			
		}
	};

	private int getQuadrant(float angle)
	{
		while (angle >= 2 * Math.PI)
			angle -= (float)(2 * Math.PI);

		while (angle < 0)
			angle += (float)(2 * Math.PI);

		if (angle >= (Math.PI / 2) && angle < Math.PI)
			return 1;

		if (angle >= Math.PI && angle < (3 * Math.PI / 2))
			return 2;

		if (angle >= (3 * Math.PI / 2) && angle < (2 * Math.PI))
			return 3;

		return 0;
	}

}
