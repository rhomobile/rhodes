package com.rho.rhoelements.plugins;

import java.util.Vector;

import com.rho.rhoelements.Common;

import android.graphics.Canvas;
import android.graphics.Paint.Align;
import android.graphics.Paint.Style;
import android.graphics.Path;
import android.graphics.PointF;
import android.view.Display;

public class LinearGesture extends ElementsGesture
{
	private PointF mBestFitStartPoint = new PointF();
	private PointF mBestFitEndPoint = new PointF();
	private PointF mStartPoint = new PointF();
	private PointF mEndPoint = new PointF();
	private PointF mErrorPoint = new PointF();
	private PointF mGestureLineStart = new PointF();
	private PointF mGestureLineEnd = new PointF();
	private int mSensitivity;
	private int mMaxSkew;
	private int mMaxDeviation;
	private int mCurrentRegion;
	private Path mArrow = null;
	
	public LinearGesture(String id, PointF start, PointF end, int tolerance, int sensitivity,
			int max_skew, int max_deviation, int region_width, boolean diags, Object plugin)
	{
		float x_offset, y_offset, line_gradient, perp_gradient, line_length;

		// Indicate that best fit hasn't yet been calculated
		mBestFitStartPoint.x = -1;

		mGestureId = new String(id);
		mDiagActivate = diags;
		mStartPoint = start;
		mEndPoint = end;
		mPlugin = (GesturePlugin) plugin;
		
		// Handle special cases
		if (mStartPoint.y == mEndPoint.y)
		{
			x_offset = 0;
			y_offset = (float)tolerance;
		}
		else if (mStartPoint.x == mEndPoint.x)
		{
			x_offset = (float)tolerance;
			y_offset = 0;
		}
		else
		{
			// Get gradient of line
			line_gradient = (float)(mEndPoint.y - mStartPoint.y) / (float)(mEndPoint.x - mStartPoint.x);

			// Get gradient of perp. line
			perp_gradient = -(1 / line_gradient);

			// Get offsets to perp. points
			y_offset = (float)Math.sqrt( (double)( (tolerance * tolerance * perp_gradient * perp_gradient) / ((perp_gradient * perp_gradient) + 1) ) );
			x_offset = (y_offset / perp_gradient);
		}

		// Get length of gesture line, calculate number of segments
		line_length = (float)Math.sqrt((double) ((mEndPoint.x - mStartPoint.x) * (mEndPoint.x - mStartPoint.x) + (mEndPoint.y - mStartPoint.y) * (mEndPoint.y - mStartPoint.y)));
		int segments = (int)Math.ceil(line_length / (float)region_width);

		// Get x, y values to divide line into segments
		float x_region_separation = (float)(mEndPoint.x - mStartPoint.x) / (float)(segments - 1);
		float y_region_separation = (float)(mEndPoint.y - mStartPoint.y) / (float)(segments - 1);

		// Add overlap
		float x_region_width = x_region_separation * 1.2F;
		float y_region_width = y_region_separation * 1.2F;

		// Create segments
		for (int n = 0; n < segments; n++)
		{
			// Create region with 4 vertices
			PolyRegion pregion = new PolyRegion();

			float midpoint_x = (float) mStartPoint.x + (x_region_separation * n);
			float midpoint_y = (float) mStartPoint.y + (y_region_separation * n);

			PointF vertex = new PointF();
			
			vertex.x = (int)(midpoint_x - (x_region_width / 2) + x_offset);
			vertex.y = (int)(midpoint_y - (y_region_width / 2) + y_offset);
			pregion.AddPoint(vertex);

			vertex.x = (int)(midpoint_x + (x_region_width / 2) + x_offset);
			vertex.y = (int)(midpoint_y + (y_region_width / 2) + y_offset);
			pregion.AddPoint (vertex);

			vertex.x = (int)(midpoint_x + (x_region_width / 2) - x_offset);
			vertex.y = (int)(midpoint_y + (y_region_width / 2) - y_offset);
			pregion.AddPoint (vertex);

			vertex.x = (int)(midpoint_x - (x_region_width / 2) - x_offset);
			vertex.y = (int)(midpoint_y - (y_region_width / 2) - y_offset);
			pregion.AddPoint (vertex);

			mRegions.add(pregion);
		}

		// Record parameters for later fit testing
		mSensitivity = sensitivity;
		mMaxSkew = max_skew;
		mMaxDeviation = max_deviation;

		if (mDiagActivate)
		{
			// find closest suitable point to centre of gesture for error string
			mErrorPoint.x = (mStartPoint.x + mEndPoint.x) / 2;
			mErrorPoint.y = (mStartPoint.y + mEndPoint.y) / 2;

			Display display = Common.mainActivity.getWindowManager().getDefaultDisplay(); 
			if (mErrorPoint.x < display.getWidth() / 10)
			{
				mErrorPoint.x = display.getWidth() / 10;
			}
			else if (mErrorPoint.x > (display.getWidth() * 9)/10)
			{
				mErrorPoint.x = (display.getWidth() * 9)/10;
			}

			if (mErrorPoint.y < display.getHeight() / 10)
			{
				mErrorPoint.y = display.getHeight() / 10;
			}
			else if (mErrorPoint.y > (display.getHeight() * 9)/10)
			{
				mErrorPoint.y = (display.getHeight() * 9)/10;
			}        
			if (mErrorPoint.x - (display.getWidth()/2) < -100)
			{
				mTextErrorPaint.setTextAlign(Align.LEFT);
				mTextOkPaint.setTextAlign(Align.LEFT);
			}
			else if (mErrorPoint.x - (display.getWidth()/2) > 100)
			{
				mTextErrorPaint.setTextAlign(Align.RIGHT);
				mTextOkPaint.setTextAlign(Align.RIGHT);
			}
			else
			{
				mTextErrorPaint.setTextAlign(Align.CENTER);
				mTextOkPaint.setTextAlign(Align.CENTER);
			}		

			mGestureLineStart.set(mStartPoint);
			mGestureLineEnd.set(mEndPoint);
			calculateDot(mGestureLineStart, mGestureLineEnd, 4);
			mArrow = calculateArrow(mGestureLineStart, mGestureLineEnd);
		}
	}
	
	@Override
	public void onGestureStarted(PointF point) 
	{
		super.onGestureStarted(point);
		for (GestureRegion pr : mRegions)
		{
			pr.crossed = false;
			pr.refreshState();
		}

		if (!mDiagActivate)
		{
			return;
		}

		mBestFitStartPoint.x = -1;
		mCurrentRegion = 0;
		mSkewAngle = 0;
		mDeviation = 0;
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
		if (found >= 0)
		{
			mGestureState = ACTIVE_STATE;
			mCurrentRegion = found;
		}
	}
	
	@Override
	public void onGestureEnded(PointF point, Vector<PointF> gestureTrack)
	{
		super.onGestureEnded(point, gestureTrack);

		// Indicate that best fit hasn't yet been calculated
		mBestFitStartPoint.x = -1;

		if ((mRegions != null) && (mRegions.isEmpty() == false) &&
				(gestureTrack != null) && (gestureTrack.size() > 2))
		{
			// Mark all regions as uncrossed
			for (GestureRegion pr : mRegions)
			{
				pr.crossed = false;
			}
			
			int current = 0;
			int found = -1;

			// For each point in track...
			for (PointF p : gestureTrack)
			{
				// Find enclosing region, starting from current region
				found = -1;
				for (int r = current; r < mRegions.size(); r++)
				{
					if (mRegions.get(r).containsPoint(p))
					{
						found = r;
						break;
					}
				}

				// Not a gesture if point is outside all regions
				if (found == -1)
				{
					break;
				}

				// Mark found region as crossed, continue search from this region
				mRegions.get(found).crossed = true;
				current = found;
			}
			if (found != -1)
			{
				// Count number of crossed regions
				int count = 0;
				for (GestureRegion r : mRegions)
				{
					if (r.crossed)
					{
						count++;
					}
				}

				// Compare against sensitivity threshold
				if (count >= (mRegions.size() * mSensitivity) / 100)
				{
					// Calculate least squares line and fit parameters
					setBestFitLine(gestureTrack);

					// Test for skew and deviation
					if ((mSkewAngle <= (float) mMaxSkew) && 
							(mDeviation <= (float) mMaxDeviation))
					{
						onDetected ();						
					}
				}
			}
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
		if (mDiagActivate)
		{
			String skewStr = "Skew: " + String.format("%1.1f", mSkewAngle);
			String devStr = "Deviation: " + String.format("%1.1f", mDeviation);

			if (mSkewAngle > mMaxSkew)
			{
				c.drawText(skewStr, mErrorPoint.x, mErrorPoint.y-10, mTextErrorPaint);
			}
			else if (mSkewAngle > 0)
			{
				c.drawText(skewStr, mErrorPoint.x, mErrorPoint.y-10, mTextOkPaint);
			}

			if (mDeviation > mMaxDeviation)
			{
				c.drawText(devStr, mErrorPoint.x, mErrorPoint.y+10, mTextErrorPaint);
			}
			else if (mDeviation > 0)
			{
				c.drawText(devStr, mErrorPoint.x, mErrorPoint.y+10, mTextOkPaint);
			}
			if (mGestureState != INACTIVE_STATE)
			{

				if (mArrow != null)
				{
					mGestureLinePaint.setStyle(Style.FILL);
					c.drawCircle(mStartPoint.x, mStartPoint.y, 4, mGestureLinePaint);
					c.drawPath(mArrow, mGestureLinePaint);
				}
				mGestureLinePaint.setStyle(Style.STROKE);
				c.drawLine(mGestureLineStart.x, mGestureLineStart.y, mGestureLineEnd.x, mGestureLineEnd.y, mGestureLinePaint);
				if (mBestFitStartPoint.x != -1)
				{
					c.drawLine(mBestFitStartPoint.x, mBestFitStartPoint.y, mBestFitEndPoint.x, mBestFitEndPoint.y, mBestFitLinePaint);
				}
			}
		}
	}
	
	private class PolyRegion extends GestureRegion
	{
		private Vector <PointF> mPoints = new Vector<PointF>();		///< Array of vertex points
		private Path mDiagPath = new Path();	// polygon for rendering

		public void AddPoint(PointF vertex)
		{
			PointF p = new PointF();
			p.set(vertex);
			mPoints.add(p);
			mDiagPath.rewind();
			for (PointF pt : mPoints)
			{
				if (pt == mPoints.firstElement())
				{
					mDiagPath.moveTo(pt.x, pt.y);
				}
				else
				{
					mDiagPath.lineTo(pt.x, pt.y);
				}
			}
			mDiagPath.close();
		}

		public boolean containsPoint(PointF point)
		{
			int det_type = 0;
			float det;

			if ((mPoints == null) || (mPoints.size() < 3))
				return false;

			for (int n = 0; n < mPoints.size() - 1; n++)
			{
				det = determinant(mPoints.get(n), mPoints.get(n + 1), point);

				if (det_type == 0)
				{
					det_type = getSign(det);
				}
				else
				{
					if (det_type != getSign(det))
					{
						return false;
					}
				}
			}

			det = determinant(mPoints.get(mPoints.size() - 1), mPoints.get(0), point);
			return (getSign(det) == det_type);
		}

		public void render(Canvas c)
		{
			if (mGestureState != INACTIVE_STATE)
			{
				if (mDiagState == UNCROSSED)
				{
					c.drawPath(mDiagPath, mUncrossedPaint);
				}
				else if (mDiagState == CROSSED)
				{
					c.drawPath(mDiagPath, mCrossedPaint);
				}
				else if (mDiagState == HIGHLIGHTED)
				{
					c.drawPath(mDiagPath, mCrossedPaint);
					c.drawPath(mDiagPath, mHighlightPaint);
				}
			}
			else
			{
				if (mDiagState == UNCROSSED)
				{
					c.drawPath(mDiagPath, mUncrossedOutlinePaint);
				}
				else
				{
					c.drawPath(mDiagPath, mCrossedOutlinePaint);
				}				
			}
		}
	}
	
	private float determinant(PointF p0, PointF p1, PointF p2)
	{
		return ((float)p1.x * (float)p2.y) - ((float)p1.y * (float)p2.x) -
			((float)p0.x * (float)p2.y) + ((float)p0.y * (float)p2.x) +
			((float)p0.x * (float)p1.y) - ((float)p0.y * (float)p1.x);
	}

	private int getSign (float f)
	{
		if (f < 0.0F)
			return -1;

		if (f > 0.0F)
			return 1;

		return 0;
	}

	private void setBestFitLine(Vector<PointF> points)
	{
		if (points.size() < 2)
			return;

		// Calculate least squares regression line
		float sum_x = 0;
		float sum_x_squared = 0;
		float sum_y = 0;
		float sum_x_y = 0;

		for (PointF p : points)
		{
			sum_x += p.x;
			sum_x_squared += (p.x * p.x);
			sum_y += p.y;
			sum_x_y += p.x * p.y;
		}

		float least_squares_gradient = ((sum_y * sum_x) - (points.size() * sum_x_y)) / ((sum_x * sum_x) - (points.size() * sum_x_squared));
		float least_squares_intercept = ((sum_x * sum_x_y) - (sum_y * sum_x_squared)) / ((sum_x * sum_x) - (points.size() * sum_x_squared));

		// Calculate mean of error squares for best fit line, using distance from line
		float total_error = 0;

		for (PointF p : points)
		{
			float error_distance = (float)( Math.abs((least_squares_gradient * p.x) - p.y + least_squares_intercept) /
				Math.sqrt((least_squares_gradient * least_squares_gradient) + 1) );

			total_error += error_distance  * error_distance;
		}

		float deviation = (float)Math.sqrt(total_error / points.size());

		float skew_angle = 0;
		float path_gradient = 0;
		
		// Calculate angle between best fit line and gesture path
		if (mEndPoint.x == mStartPoint.x)
		{
			// Path is vertical
			skew_angle = (float) Math.atan2(1.0F, least_squares_gradient);
		}
		else
		{
			path_gradient = ((float)(mEndPoint.y - mStartPoint.y)) / ((float)(mEndPoint.x - mStartPoint.x));
			skew_angle = (float) Math.atan2 (least_squares_gradient - path_gradient, 1 + least_squares_gradient * path_gradient);
		}

		// Record values for later fit testing and drawing
		mDeviation = deviation;
		mSkewAngle = (float) Math.toDegrees(skew_angle);	// Convert radians to degrees

		if (mSkewAngle < -90)
			mSkewAngle += 180;

		if (mSkewAngle > 90)
			mSkewAngle -= 180;

		mSkewAngle = (float) Math.abs(mSkewAngle);

		// Find start and end points of best fit line for drawing
		mBestFitStartPoint.x = points.get(0).x;
		mBestFitStartPoint.y = (int) ((points.get(0).x * least_squares_gradient) + least_squares_intercept);

		mBestFitEndPoint.x = points.get(points.size() - 1).x;
		mBestFitEndPoint.y = (int) ((points.get(points.size() - 1).x * least_squares_gradient) + least_squares_intercept);
	}
}
