package com.rho.signalindicators;

import android.content.Context;
import android.graphics.Canvas;

import com.rho.indicators.IndicatorData;
import com.rho.indicators.IndicatorView;
import com.rho.indicators.IndicatorView.Layout;
import com.rhomobile.rhodes.Logger;

import android.app.Activity;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Picture;
import android.graphics.Rect;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.WindowManager;

import com.larvalabs.svgandroid.copy.SVG;
import com.larvalabs.svgandroid.copy.SVGParser;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * SignalView describes the signal indicator. Relies on signal status from an external source.
 * @author Malaya Panda, Shekar Muniven
 */
public class SignalIndicatorsView extends IndicatorView
{
	protected   int LONG_SIDE = 16; //Size of the signal icon's longest side
	protected   int SHORT_SIDE = 11; //Size of the signal icon's shortest side
	float multipleVar = 0;
	protected int signalStep;
	private Picture sigPic0;
	private Picture sigPic1;
	private Picture sigPic2;
	private Picture sigPic3;
	private Picture sigPic4;
	private Picture sigPic5;
	private Rect boundingRect;
	
	
	public SignalIndicatorsView(Context context)
	{
		super(context);
		screenResolutionCheck();
		
	}

	@Override
	protected int getLongSideLength()
	{
		return LONG_SIDE;
	}

	@Override
	protected int getShortSideLength()
	{
		return SHORT_SIDE;
	}
	
	@Override
	public void draw(Canvas canvas)
	{
		
		Logger.D(TAG, "draw+");
		if(isPortrait())
		{
			boundingRect = new Rect(0, 0, SHORT_SIDE, LONG_SIDE);
		}
		else
		{
			boundingRect = new Rect(0, 0, LONG_SIDE, SHORT_SIDE);
		}
		
		switch(signalStep)
		{
			case 0: canvas.drawPicture(sigPic0, boundingRect); break;
			case 1: canvas.drawPicture(sigPic1, boundingRect); break;
			case 2: canvas.drawPicture(sigPic2, boundingRect); break;
			case 3: canvas.drawPicture(sigPic3, boundingRect); break;
			case 4: canvas.drawPicture(sigPic4, boundingRect); break;
			case 5: canvas.drawPicture(sigPic5, boundingRect); break;
			default: /*Shouldnt Happen*/ break;
		}
		
		super.draw(canvas);
		Logger.D(TAG, "draw-");
	}
	
	@Override
	protected void changeIcon()
	{
		Logger.D(TAG, "changeIcon+");
		SVG sig0;
		SVG sig1;
		SVG sig2;
		SVG sig3;
		SVG sig4;
		SVG sig5;
		
		Activity activity = RhodesActivity.safeGetInstance();
		if(activity == null)
		{
			Logger.E(TAG, "Cannot get activity");
		}
		Resources resources = activity.getResources();

		if(layout == Layout.RIGHT)
		{
			sig0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig0_0"), 0xFF000000, color);
			sig1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig1_0"), 0xFF000000, color);
			sig2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig2_0"), 0xFF000000, color);
			sig3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig3_0"), 0xFF000000, color);
			sig4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig4_0"), 0xFF000000, color);
			sig5 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig5_0"), 0xFF000000, color);
		}
		else if(layout == Layout.LEFT)
		{
			sig0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig0_180"), 0xFF000000, color);
			sig1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig1_180"), 0xFF000000, color);
			sig2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig2_180"), 0xFF000000, color);
			sig3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig3_180"), 0xFF000000, color);
			sig4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig4_180"), 0xFF000000, color);
			sig5 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig5_180"), 0xFF000000, color);
		}
		else if(layout == Layout.UP)
		{
			sig0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig0_270"), 0xFF000000, color);
			sig1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig1_270"), 0xFF000000, color);
			sig2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig2_270"), 0xFF000000, color);
			sig3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig3_270"), 0xFF000000, color);
			sig4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig4_270"), 0xFF000000, color);
			sig5 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig5_270"), 0xFF000000, color);
		}
		else if(layout == Layout.DOWN)
		{
			sig0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig0_90"), 0xFF000000, color);
			sig1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig1_90"), 0xFF000000, color);
			sig2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig2_90"), 0xFF000000, color);
			sig3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig3_90"), 0xFF000000, color);
			sig4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig4_90"), 0xFF000000, color);
			sig5 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "sig5_90"), 0xFF000000, color);
		}
		else
		{
			//Should never happen
			Logger.E(TAG, "changeIcon-: Unknown layout");
			return;
		}

		sigPic0 = sig0.getPicture();
		sigPic1 = sig1.getPicture();
		sigPic2 = sig2.getPicture();
		sigPic3 = sig3.getPicture();
		sigPic4 = sig4.getPicture();
		sigPic5 = sig5.getPicture();
		Logger.D(TAG, "changeIcon-");
	}
	
	//////////////////////////////COPY FROM HERE /////////////////////////
	
	/**
	 * Converts a 0-100 range int into 0-4
	 * @param signalLevel
	 * @return the number of segments that should be shown in the icon
	 * @author Malaya Panda, Shekar Muniven
	 */
	private static int convertToSignalStep(int signalLevel)
	{
		if(signalLevel==0) return 0;
		
		else if(signalLevel>=1 && signalLevel<=20)return 1;
		else if(signalLevel>=21 && signalLevel<=40)return 2;
		else if(signalLevel>=41 && signalLevel<=60)return 3;
		else if(signalLevel>=61 && signalLevel<=80)return 4;
		else return 5;
		
		//return (signalLevel -1) / 20;
	}
	
	/**
	 * Changes the signal level (in %) and/or power connection status 
	 * @param signalLevel the new signal level
	 * @param isPowerConnected whether the device is connected to AC power
	 * @author Malaya Panda, Shekar Muniven
	 */
	public void setSignalState(int signalLevel)
	{
		Logger.D(TAG, "setPowerState+ quality:" + signalLevel );
		boolean requiresRedraw = false;
		int signalStep = convertToSignalStep(signalLevel);
		if(this.signalStep != signalStep)
		{
			requiresRedraw = true;
			this.signalStep = signalStep;
		}

		if(requiresRedraw) redraw();
	}
	public void setSignalLevel(int level)
	{
		this.signalStep=convertToSignalStep(level);
	}
	public void RedrawOrientation()
	{
		redraw();
	}
	public void screenResolutionCheck(){

		Display display = ((WindowManager)RhodesActivity.safeGetInstance().getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		int width = display.getWidth();  // deprecated
		int height = display.getHeight();  // deprecated
		
		if(height > width){
			multipleVar = 1;
		}else{
			multipleVar = (float) 1.5;
		}
		
		
		DisplayMetrics metrics = new DisplayMetrics();
		((WindowManager)RhodesActivity.safeGetInstance().getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getMetrics(metrics);
		switch(metrics.densityDpi){
		     case DisplayMetrics.DENSITY_LOW:
		    	 LONG_SIDE = (int) (LONG_SIDE * 1 * multipleVar);
		    	 SHORT_SIDE =  (int) (SHORT_SIDE * 1 * multipleVar);
		                break;
		     case DisplayMetrics.DENSITY_MEDIUM:
		    	 LONG_SIDE = (int) (LONG_SIDE * 1 * multipleVar);
		    	 SHORT_SIDE =  (int) (SHORT_SIDE * 1 * multipleVar);
		                 break;
		     case DisplayMetrics.DENSITY_HIGH:
		    	 LONG_SIDE = (int) (LONG_SIDE * 2 * multipleVar);
		    	 SHORT_SIDE =  (int) (SHORT_SIDE * 2 * multipleVar);
		                 break;
		     case DisplayMetrics.DENSITY_XHIGH:
		    	 LONG_SIDE = (int) (LONG_SIDE * 4 * multipleVar);
		    	 SHORT_SIDE =  (int) (SHORT_SIDE * 4 * multipleVar);
		                 break;
		     case DisplayMetrics.DENSITY_XXHIGH:
		    	 LONG_SIDE = (int) (LONG_SIDE * 6 * multipleVar);
		    	 SHORT_SIDE =  (int) (SHORT_SIDE * 6 * multipleVar);
		                 break;
		     case DisplayMetrics.DENSITY_XXXHIGH:
		    	 LONG_SIDE = (int) (LONG_SIDE * 8 * multipleVar);
		    	 SHORT_SIDE =  (int) (SHORT_SIDE * 8 * multipleVar);
		                 break;
		}

	}
	
}
