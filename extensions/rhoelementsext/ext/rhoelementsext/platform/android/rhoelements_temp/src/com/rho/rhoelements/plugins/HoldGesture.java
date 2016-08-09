package com.rho.rhoelements.plugins;

import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

import com.rho.rhoelements.Common;

import android.graphics.Canvas;
import android.graphics.PointF;
import android.graphics.Paint.Style;
import android.os.Handler;
import android.view.Display;

public class HoldGesture extends ElementsGesture
{

	private PointF mCentrePoint;
	private int mRadius;
	private long mDelay;
	private long mRepeatInterval;
	final Handler handler = new Handler();
	private Timer mHoldTimer;
	boolean mCrossed = false;
	private PointF mCounterTextPt = new PointF();

	public HoldGesture(String id, PointF centre, int radius, long delay, long interval, boolean diags, Object plugin)
	{
		mCentrePoint = centre;
		mRadius = radius;
		mDelay = delay;
		mRepeatInterval = interval;
		
		mGestureId = new String(id);
		mDiagActivate = diags;
		mPlugin = (GesturePlugin) plugin;
	}
	
	private void onGestureTimerExpired()
	{
		mActivationCounter++;
		onDetected();
		if (mDiagActivate)
		{
			mPlugin.mGesturesOverlay.invalidate();
		}
	}

	@Override
	public void onGestureStarted(PointF point)
	{
		super.onGestureStarted(point);
		if ((mDelay > 0) && (isInside(point)))
		{
			mHoldTimer = new Timer();
			mHoldTimer.schedule(new GestureTimerTask(), mDelay);
			mGestureState = ACTIVE_STATE;
			mCrossed = true;
		}
		else
		{
			mCrossed = false;
		}
		if (mDiagActivate)
		{
			Display display = Common.mainActivity.getWindowManager().getDefaultDisplay(); 
			double dx = mLastTouchPoint.x - (display.getWidth()/2);
			double dy = mLastTouchPoint.y - (display.getHeight()/2);
			double theta = Math.atan2(dx, dy);
			double hyp = Math.hypot(dx, dy) - 100;
			mCounterTextPt.x = mLastTouchPoint.x - (float)(dx - (Math.sin(theta) * hyp));
			mCounterTextPt.y = mLastTouchPoint.y - (float)(dy - (Math.cos(theta) * hyp));
		}
	}
	
	@Override
	public void onGestureEnded(PointF point, Vector<PointF> gestureTrack)
	{
		super.onGestureEnded(point, gestureTrack);
		if (mHoldTimer != null)
		{
			mHoldTimer.cancel();
			mHoldTimer = null;
			if (mActivationCounter > 0)
			{
				mActivationCounter = 0;
				onDetected();
			}
		}
	}

	@Override
	public void onGestureCancelled()
	{
		super.onGestureCancelled();
		if (mHoldTimer != null)
		{
			mHoldTimer.cancel();
			mHoldTimer = null;
			if (mActivationCounter > 0)
			{
				mActivationCounter = 0;
				onDetected();
			}
		}
	}
	
	@Override
	public void onGesture(PointF point) 
	{
		super.onGesture(point);
		if ((mHoldTimer != null) && (!isInside(point)))
		{
			mHoldTimer.cancel();
			mHoldTimer = null;
			if (mActivationCounter > 0)
			{
				mActivationCounter = 0;
				onDetected();
			}
		}
		else if ((mGestureState == START_STATE) && (!isInside(point)))
		{
			mGestureState = INACTIVE_STATE;
		}
		if (mDiagActivate)
		{
			Display display = Common.mainActivity.getWindowManager().getDefaultDisplay(); 
			double dx = mLastTouchPoint.x - (display.getWidth()/2);
			double dy = mLastTouchPoint.y - (display.getHeight()/2);
			double theta = Math.atan2(dx, dy);
			double hyp = Math.hypot(dx, dy) - 100;
			mCounterTextPt.x = mLastTouchPoint.x - (float)(dx - (Math.sin(theta) * hyp));
			mCounterTextPt.y = mLastTouchPoint.y - (float)(dy - (Math.cos(theta) * hyp));
		}
	}
	
	@Override
	public void render(Canvas c)
	{
		if (mDiagActivate)
		{
			if (mHoldTimer != null)
			{
				c.drawCircle(mCentrePoint.x, mCentrePoint.y, mRadius, mCrossedPaint);
				c.drawCircle(mCentrePoint.x, mCentrePoint.y, mRadius, mHighlightPaint);
				c.drawText("Count:" + Integer.toString(mActivationCounter), mCounterTextPt.x, mCounterTextPt.y, mTextCounterPaint);
			}
			else if (mCrossed)
			{
				if (mGestureState == INACTIVE_STATE)
				{
					c.drawCircle(mCentrePoint.x, mCentrePoint.y, mRadius, mCrossedOutlinePaint);					
				}
				else if (mGestureState == SIGNALLED_STATE)
				{
					c.drawCircle(mCentrePoint.x, mCentrePoint.y, mRadius, mCrossedPaint);
				}
				else if (mGestureState == ACTIVE_STATE)
				{
					c.drawCircle(mCentrePoint.x, mCentrePoint.y, mRadius, mCrossedOutlinePaint);										
				}
				else if (mGestureState == START_STATE)
				{
					c.drawCircle(mCentrePoint.x, mCentrePoint.y, mRadius, mCrossedPaint);					
				}
			}
			else
			{
				if (mGestureState == INACTIVE_STATE)
				{
					c.drawCircle(mCentrePoint.x, mCentrePoint.y, mRadius, mUncrossedOutlinePaint);
				}
				else if (mGestureState == START_STATE)
				{
					c.drawCircle(mCentrePoint.x, mCentrePoint.y, mRadius, mUncrossedPaint);					
				}
			}
			if (mGestureState != INACTIVE_STATE)
			{
				mGestureLinePaint.setStyle(Style.STROKE);
				c.drawLine(mCentrePoint.x, mCentrePoint.y-5, mCentrePoint.x, mCentrePoint.y+5, mGestureLinePaint);
				c.drawLine(mCentrePoint.x-5, mCentrePoint.y, mCentrePoint.x+5, mCentrePoint.y, mGestureLinePaint);
			}
		}
	}

	private boolean isInside(PointF point)
	{
		float distance = (float)Math.sqrt((((point.x - mCentrePoint.x) * (point.x - mCentrePoint.x)) + ((point.y - mCentrePoint.y) * (point.y - mCentrePoint.y))));
		return (distance < mRadius);
	}

	private class GestureTimerTask extends TimerTask
	{
		public void run()
		{ 
			handler.post(new Runnable()
			{
				public void run()
				{
					onGestureTimerExpired();
				}
			});
			if ((mHoldTimer != null) && (mRepeatInterval > 0))
			{
				mHoldTimer.schedule(new GestureTimerTask(), mRepeatInterval);				
			}
		}
	}
}
