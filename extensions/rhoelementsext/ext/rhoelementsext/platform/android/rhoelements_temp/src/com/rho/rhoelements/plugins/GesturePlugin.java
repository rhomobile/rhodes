package com.rho.rhoelements.plugins;

import java.util.HashSet;
import java.util.Vector;

import android.gesture.GestureOverlayView;
import android.gesture.GestureOverlayView.OnGestureListener;
import android.graphics.Canvas.VertexMode;
import android.graphics.PointF;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

public class GesturePlugin extends Plugin implements OnGestureListener
{
	private boolean mPluginActive = false;
	protected Vector<PointF> mMouseTrack = null;
	private HashSet<ElementsGesture> mGestureList = new HashSet<ElementsGesture>();
	protected ElementsGestureOverlayView mGesturesOverlay;
	
	private View mWebView;
	private enum GESTURETYPES {
		INVALID(0), LINEAR(1), CIRCLE(2), HOLD(3), TILT(4), SHAKE(5);
		
		private int value;

        private GESTURETYPES(int value) {
                this.value = value;
        }
	};

	protected GESTURETYPES  mType = GESTURETYPES.INVALID;
	protected String mId = "";
	protected boolean mDiagnostics;
	protected int mStartX;
	protected int mStartY;
	protected int mEndX;
	protected int mEndY;
	protected int mTolerance = 0;	
	protected int mSensitivity = 50;
	protected int mSkew = 20;
	protected int mDeviation = 20;
	protected int mRegionWidth;
	protected int mCentreX;
	protected int mCentreY;
	protected int mRadius = 0;
	protected int mStart = 0;
	protected int mEnd = 180;
	protected int mDelay = 1000;
	protected int mInterval = 0;
	protected int nTiltX = 0;
	protected int nTiltY = 0;
	protected int nTiltZ = 90;
	protected int nTiltTolerance = 10;
	protected int nTiltHysteresis = 10;	
	protected int nShakeThreshold = 500;
	protected int nShakeQuiet = 1000;

	protected String mDetectedEvent = null;

	public interface GestureDetected
	{
		void onDetect();
	}

	public class GestureEvent implements GestureDetected
	{
		private String mId;
		private int mCount;

		public GestureEvent(String id, int count)
		{
			mId = new String(id);
			mCount = count;
		}

		@Override
		public void onDetect()
		{
			final String NAMES[] = { "id", "count" };
			final String VALUES[] = { mId, Integer.toString(mCount) };

			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, mId + " " + mCount));
			if ((mDetectedEvent != null) && (mDetectedEvent.length() > 0))
			{
				try
				{
					navigate(mDetectedEvent, NAMES, VALUES);
				}
				catch (NavigateException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
				}
			}
		}
	}

	public static void detected(GestureEvent g)
	{
		g.onDetect();
	}

	public GesturePlugin()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		mGesturesOverlay = (ElementsGestureOverlayView) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","gestures"));
		mGesturesOverlay.addOnGestureListener(this);

		mWebView = Common.getWebView();
		
		mGesturesOverlay.setLayoutParams(new FrameLayout.LayoutParams(mWebView.getWidth(), mWebView.getHeight()));
		
		View webView = mWebView;
		mStartX = webView.getWidth() / 10;
		mStartY = webView.getHeight() / 2;
		mEndX = mStartX * 9;
		mEndY = mStartY;
		mRegionWidth = mStartX;
		mCentreX = webView.getWidth() / 2;
		mCentreY = webView.getHeight() / 2;
		mDelay = 1000;
		mInterval = 0;
		nTiltTolerance = 10;
		nTiltHysteresis = 10;	
		nShakeThreshold = 500;
		nShakeQuiet = 1000;
		mDetectedEvent = null;
	}

	public static Version getVersion()
	{
		return new Version ("Gesture");
	}

	public void onPageStarted(String url)
	{
		if (mPluginActive)
		{
			mPluginActive = false;
			mType = GESTURETYPES.INVALID;
			mId = "";
			mDiagnostics = false;
			View webView = Common.getWebView();
			mStartX = webView.getWidth() / 10;
			mStartY = webView.getHeight() / 2;
			mEndX = mStartX * 9;
			mEndY = mStartY;
			mTolerance = 0;		// default value is different for circle and hold gestures so set it later
			mSensitivity = 50;
			mSkew = 20;
			mDeviation = 20;
			mRegionWidth = mStartX;
			mCentreX = webView.getWidth() / 2;
			mCentreY = webView.getHeight() / 2;
			mRadius = 0;	// default value is different for circle and hold gestures so set it later
			mDelay = 1000;
			mInterval = 0;
			nTiltTolerance = 10;
			nTiltHysteresis = 10;	
			nShakeThreshold = 500;
			nShakeQuiet = 1000;
			mDetectedEvent = null;
			mGesturesOverlay.setVisibility(View.GONE);
			mGesturesOverlay.removeAllGestureDiags();
			for (ElementsGesture g : mGestureList)
			{				
				g.onUnregisterSensor();		
				g = null;
			}
		}
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
		if (!mPluginActive)
		{
			mPluginActive = true;
			mGestureList = new HashSet<ElementsGesture>();
			mGesturesOverlay.setVisibility(View.VISIBLE);
		}

		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));

		if (setting.getName().equalsIgnoreCase("type"))
		{
			String typeVal = setting.getValue().toLowerCase();
			if (typeVal.equals("linear"))
			{
				mType = GESTURETYPES.LINEAR;
				getLinearPreset(null);
			}
			else if (typeVal.equals("circle"))
			{
				mType = GESTURETYPES.CIRCLE;
				getCirclePreset(null);
			}
			else if (typeVal.equals("hold"))
			{
				mType = GESTURETYPES.HOLD;
				getHoldPreset(null);
			}
			else if (typeVal.equals("tilt"))
			{
				mType = GESTURETYPES.TILT;
				getTiltPreset(null);
			}
			else if (typeVal.equals("shake"))
			{
				mType = GESTURETYPES.SHAKE;
				getShakePreset(null);
			}
			else
			{
				mType = GESTURETYPES.INVALID;
			}
		}

		else if (setting.getName().equalsIgnoreCase("id"))
		{
			if (setting.getValue().length() > 0)
			{
				mId = setting.getValue();
			}
		}

		else if (setting.getName().equalsIgnoreCase("Preset"))
		{
			switch (mType)
			{
			case LINEAR:
				getLinearPreset(setting.getValue().toLowerCase());
				break;

			case CIRCLE:
				getCirclePreset(setting.getValue().toLowerCase());
				break;

			case HOLD:
				getHoldPreset(setting.getValue().toLowerCase());
				break;
				
			case TILT:
				getTiltPreset(setting.getValue().toLowerCase());
				break;			
		
			case SHAKE:
				getShakePreset(setting.getValue().toLowerCase());
				break;
			}
		}

		else if (setting.getName().equalsIgnoreCase("diagnostics"))
		{
			if (setting.getValue().equalsIgnoreCase("true"))
			{
				mDiagnostics = true;
			}
			else
			{
				mDiagnostics = false;
			}
		}

		else if (setting.getName().equalsIgnoreCase("startX") ||
			setting.getName().equalsIgnoreCase("start-x"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 10)
				{
					val = 0;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mStartX = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Start-X parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mStartX = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("startY") ||
			setting.getName().equalsIgnoreCase("start-y"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mStartY = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Start-Y parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mStartY = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("endX") ||
			setting.getName().equalsIgnoreCase("end-x"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mEndX = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "End-X parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mEndX = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("endY") ||
			setting.getName().equalsIgnoreCase("end-y"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mEndY = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "End-Y parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mEndY = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("tolerance"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 1)
				{
					val = 1;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mTolerance = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Tolerance parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mTolerance = 1;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("sensitivity"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 100)
				{
					val = 100;
				}
				mSensitivity = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Sensitivity parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mSensitivity = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("skew"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 90)
				{
					val = 90;
				}
				mSkew = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Skew parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mSkew = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("deviation"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 100)
				{
					val = 100;
				}
				mDeviation = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Deviation parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mDeviation = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("regionWidth") ||
			setting.getName().equalsIgnoreCase("region-width"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 1)
				{
					val = 1;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mRegionWidth = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Region Width parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mRegionWidth = 1;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("centerX") ||
			setting.getName().equalsIgnoreCase("center-x"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < -10000)
				{
					val = -10000;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mCentreX = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Center-X parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mCentreX = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("centerY") ||
			setting.getName().equalsIgnoreCase("center-y"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < -10000)
				{
					val = -10000;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mCentreY = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Center-Y parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mCentreY = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("radius"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 1)
				{
					val = 1;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mRadius = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Radius parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mRadius = 1;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("targetX") ||
				setting.getName().equalsIgnoreCase("target-x"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < -90)
				{
					val = -90;
				}
				else if (val > 90)
				{
					val = 90;
				}
				nTiltX = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "target-X parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				nTiltX = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("targetY") ||
				setting.getName().equalsIgnoreCase("target-y"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < -90)
				{
					val = -90;
				}
				else if (val > 90)
				{
					val = 90;
				}
				nTiltY = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "target-Y parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				nTiltY = 0;
			}	        
		}
		
		else if (setting.getName().equalsIgnoreCase("targetZ") ||
				setting.getName().equalsIgnoreCase("target-z"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < -90)
				{
					val = -90;
				}
				else if (val > 90)
				{
					val = 90;
				}
				nTiltZ = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "target-Z parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				nTiltZ = 0;
			}	        
		}
		
		else if (setting.getName().equalsIgnoreCase("tiltTolerance"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 90)
				{
					val = 90;
				}
				nTiltTolerance = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "tiltTolerance parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				nTiltTolerance = 0;
			}	        
		}
		
		else if (setting.getName().equalsIgnoreCase("hysteresis"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 90)
				{
					val = 90;
				}
				nTiltHysteresis = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "hysteresis parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				nTiltHysteresis = 0;
			}	        
		}
		
		else if (setting.getName().equalsIgnoreCase("threshold"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 1000)
				{
					val = 1000;
				}
				nShakeThreshold = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "threshold parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				nShakeThreshold = 0;
			}	        
		}
		
		else if (setting.getName().equalsIgnoreCase("quiet"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}				
				nShakeQuiet = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "quiet parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				nShakeQuiet = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("start"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mStart = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Start parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mStart = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("end"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 10000)
				{
					val = 10000;
				}
				mEnd = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "End parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mEnd = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("delay"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 1)
				{
					val = 1;
				}
				else if (val > 600000)
				{
					val = 600000;
				}
				mDelay = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Delay parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mDelay = 1;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("interval"))
		{
			try
			{
				int val = Integer.parseInt(setting.getValue());
				if (val < 0)
				{
					val = 0;
				}
				else if (val > 600000)
				{
					val = 600000;
				}
				mInterval = val;
			}
			catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Interval parameter invalid: " + 
						setting.getValue() + " " + e.getMessage()));
				mInterval = 0;
			}	        
		}

		else if (setting.getName().equalsIgnoreCase("detected"))
		{
			mDetectedEvent = new String(setting.getValue());
		}

		else if (setting.getName().equalsIgnoreCase("create"))
		{
			if ((mId == null) || (mId.length() < 1))
			{
				return;
			}

			switch (mType)
			{
			case LINEAR:
				if (mTolerance == 0)
				{
					View display = Common.getWebView();
					mTolerance = display.getHeight() / 4;
				}
				addGesture(new LinearGesture(mId, new PointF(mStartX, mStartY), new PointF(mEndX, mEndY), 
						mTolerance, mSensitivity, mSkew, mDeviation, mRegionWidth, mDiagnostics, this));
				break;

			case CIRCLE:
				if (mTolerance == 0)
				{
					View display = Common.getWebView();
					mTolerance = (display.getWidth() * 16) / 100;
				}
				if (mRadius == 0)
				{
					View display = Common.getWebView();
					mRadius = (Math.min(display.getHeight(), display.getWidth()) * 33) / 100;
				}
				addGesture(new CircleGesture(mId, new PointF(mCentreX, mCentreY), mRadius, mStart, mEnd, mTolerance, mSensitivity, mDiagnostics, this));
				break;

			case HOLD:
				if (mRadius == 0)
				{
					View display = Common.getWebView();
					mRadius = Math.min(display.getHeight(), display.getWidth()) / 4;
				}
				addGesture(new HoldGesture(mId, new PointF(mCentreX, mCentreY), mRadius, mDelay, mInterval, mDiagnostics, this));
				break;	

			case TILT:				
				addGesture(new TiltGesture(mId, nTiltX, nTiltY, nTiltZ, nTiltTolerance, nTiltHysteresis, this));
				break;	
				
			case SHAKE:				
				addGesture(new ShakeGesture(mId, nShakeThreshold, nShakeQuiet, this));
				break;	

			default:
				return;
			}
			mGesturesOverlay.invalidate();
		}
		else if (setting.getName().equalsIgnoreCase("delete"))
		{
			removeGesture(mId);
		}
	}


	private void getShakePreset(String type)
	{
		if ((type == null) || (type.equals("")))
		{
			type = new String("normal");
		}

		if (type.equals("normal"))
		{
			mId = "shake-" + type;
			nShakeThreshold = 500;
			nShakeQuiet = 1000;			
		}
	}

	private void getTiltPreset(String type)
	{
		if ((type == null) || (type.equals("")))
		{
			type = new String("face-up");
		}
		
		if (type.equals("upright"))
		{
			nTiltX = 0;
			nTiltY = 90;
			nTiltZ = 0;
		}
		else if (type.equals("turn-down"))
		{
			nTiltX = 0;
			nTiltY = -90;
			nTiltZ = 0;
		}
		else if (type.equals("turn-left"))
		{
			nTiltX = 90;
			nTiltY = 0;
			nTiltZ = 0;
		}
		else if (type.equals("turn-right"))
		{
			nTiltX = -90;
			nTiltY = 0;
			nTiltZ = 0;
		}
		else if (type.equals("face-up"))
		{
			nTiltX = 0;
			nTiltY = 0;
			nTiltZ = 90;
		}
		else if (type.equals("face-down"))
		{
			nTiltX = 0;
			nTiltY = 0;
			nTiltZ = -90;
		}
		
		mId = "tilt-" + type;		
		nTiltTolerance = 10;
		nTiltHysteresis = 10;
		
	}

	private void getHoldPreset(String type)
	{
		if ((type == null) || (type.equals("")))
		{
			type = new String("center");
		}

		if ((type.equals("center")) || (type.equals("centre")))
		{
			mId = "hold-" + type;
			View display = Common.getWebView();
			mCentreX = display.getWidth()/2;
			mCentreY = display.getHeight()/2;
			mRadius = Math.min(display.getWidth()/4, display.getHeight()/4);
			mDelay = 1000;
			mInterval = 0;
			mDiagnostics = false;
		}
	}

	private void getCirclePreset(String type)
	{
		if ((type == null) || (type.length() < 1))
		{
			type = new String("happy");	// revert to default
		}

		if ((type.equals("happy")) || type.equals("sad"))
		{
			if (type.equals("sad"))
			{
				mStart = 180;
				mEnd = 360;
			}
			else
			{
				mStart = 0;
				mEnd = 180;        		
			}
			mId = "circle-" + type;
			View display = Common.getWebView();
			mCentreX = display.getWidth()/2;
			mCentreY = display.getHeight()/2;
			mRadius = Math.min(display.getWidth()/3, display.getHeight()/3);
			mTolerance = (display.getWidth()*16)/100;
			mSensitivity = 50;
			mDiagnostics = false;
		}
	}

	private void getLinearPreset(String type)
	{
		if ((type == null) || (type.length() < 1))
		{
			type = new String("left-right");
		}

		View display = Common.getWebView();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "display:" + display));
		if (type.equals("left-right"))
		{
			mStartX = display.getWidth()/10;
			mStartY = display.getHeight()/2;
			mEndX = (display.getWidth()*9)/10;
			mEndY = mStartY;
		}
		else if (type.equals("right-left"))
		{
			mStartX = (display.getWidth()*9)/10;
			mStartY = display.getHeight()/2;
			mEndX = display.getWidth()/10;
			mEndY = mStartY;
		}
		else if (type.equals("top-bottom"))
		{
			mStartX = display.getWidth()/2;
			mStartY = display.getHeight()/10;
			mEndX = mStartX;
			mEndY = (display.getHeight()*9)/10;
		}
		else if (type.equals("bottom-top"))
		{
			mStartX = display.getWidth()/2;
			mStartY = (display.getHeight()*9)/10;
			mEndX = mStartX;
			mEndY = display.getHeight()/10;
		}
		else
		{
			return;
		}

		mId = "linear-" + type;
		mTolerance = display.getHeight()/4;
		mSensitivity = 50;
		mSkew = 20;
		mDeviation = 20;
		mRegionWidth = display.getWidth()/10;
	}

	private void addGesture(ElementsGesture g)
	{
		if (mGestureList.contains(g))
		{
			mGestureList.remove(g);
		}
		mGestureList.add(g);
		mGesturesOverlay.addGestureDiags(g);
	}

	private void removeGesture(String id)
	{
		ElementsGesture deleteMe = null;
		for (ElementsGesture g : mGestureList)
		{
			if (g.mGestureId.equals(id))
			{
				deleteMe = g;
				break;
			}
		}
		
		if (deleteMe != null)
		{
			deleteMe.onUnregisterSensor();
			mGestureList.remove(deleteMe);
			deleteMe = null;
		}
		
		if (mGestureList.isEmpty())
			mGesturesOverlay.removeAllGestureDiags();
		else
			mGesturesOverlay.removeGestureDiags(id);
	}
	
	@Override
	public void onShutdown()
	{
		// TODO Auto-generated method stub

	}

	@Override
	public void onGesture(GestureOverlayView gestureView, MotionEvent motion)
	{
		mWebView = Common.getWebView();
		int webViewAbsoluteCoord[] = new int[2];
		mWebView.getLocationOnScreen(webViewAbsoluteCoord);
		int webViewTop = webViewAbsoluteCoord[1];
		int webViewBottom = mWebView.getHeight() + webViewTop;
		mWebView.dispatchTouchEvent(motion);
		if ( (motion.getRawY() > webViewBottom) || (motion.getRawY() < webViewTop) )
		{
			onGestureCancelled(gestureView, motion);
			return; //If the gesture is outside the boundaries of the webview, just return
		}
		PointF point = new PointF();
		int gestureOverlayCoord[] = new int[2];
		gestureView.getLocationOnScreen(gestureOverlayCoord);
		point.set(motion.getRawX(), motion.getRawY()-gestureOverlayCoord[1]);
		if (mMouseTrack != null)
		{
			if (mMouseTrack.size() < 2000)
			{
				mMouseTrack.add(point);
				for (ElementsGesture g : mGestureList)
				{
					g.onGesture(point);
				}
				mGesturesOverlay.drawComplete();
			}
			else
			{
				mMouseTrack = null;
				for (ElementsGesture g : mGestureList)
				{
					g.onGestureCancelled();
				}			
			}
		}
		if (mGesturesOverlay.mGestureLine != null)
		{
			mGesturesOverlay.mGestureLine.onGesture(point);
		}
	}

	@Override
	public void onGestureCancelled(GestureOverlayView overlayView, MotionEvent motion)
	{
		mWebView = Common.getWebView();
		mWebView.dispatchTouchEvent(motion);
		mMouseTrack = null;
		for (ElementsGesture g : mGestureList)
		{
			g.onGestureCancelled();
		}			
	}
	
	@Override
	public void onGestureEnded(GestureOverlayView gestureView, MotionEvent motion)
	{
		mWebView = Common.getWebView();
		int webViewAbsoluteCoord[] = {0, 0};
		mWebView.getLocationOnScreen(webViewAbsoluteCoord);
		int webViewTop = webViewAbsoluteCoord[1];
		int webViewBottom = mWebView.getHeight() + webViewTop;
		mWebView.dispatchTouchEvent(motion);
		if ( (motion.getRawY() > webViewBottom) || (motion.getRawY() < webViewTop) )
			return;
		if (mMouseTrack != null)
		{
			PointF point = new PointF();
			int gestureOverlayCoord[] = new int[2];
			gestureView.getLocationOnScreen(gestureOverlayCoord);
			point.set(motion.getRawX(), motion.getRawY()-gestureOverlayCoord[1]);
			mMouseTrack.add(point);
			for (ElementsGesture g : mGestureList)
			{
				g.onGestureEnded(point, mMouseTrack);
			}			
		}
	}

	@Override
	public void onGestureStarted(GestureOverlayView gestureView, MotionEvent motion)
	{
		mWebView = Common.getWebView();
		int[] webViewAbsoluteCoord = new int[2];
		mWebView.getLocationOnScreen(webViewAbsoluteCoord);
		int webViewTop = webViewAbsoluteCoord[1];
		int webViewBottom = mWebView.getHeight() + webViewTop;
		mWebView.dispatchTouchEvent(motion);
		if ( (motion.getRawY() > webViewBottom) || (motion.getRawY() < webViewTop) )
		{
			return; //If the gesture is outside the boundaries of the webview, just return
		}
		PointF startPoint = new PointF();
		int gestureOverlayCoord[] = new int[2];
		gestureView.getLocationOnScreen(gestureOverlayCoord);
		startPoint.set(motion.getRawX(), motion.getRawY()-gestureOverlayCoord[1]);
		mMouseTrack = new Vector<PointF>();
		mMouseTrack.add(startPoint);
		for (ElementsGesture g : mGestureList)
		{
			g.onGestureStarted(startPoint);
		}
		if (mGesturesOverlay.mGestureLine != null)
		{
			mGesturesOverlay.mGestureLine.onGestureStarted(startPoint);
		}
	}
}
