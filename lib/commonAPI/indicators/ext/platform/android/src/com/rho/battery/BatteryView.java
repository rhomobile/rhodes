package com.rho.battery;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Picture;
import android.graphics.Rect;
import android.util.DisplayMetrics;
import android.view.Display;

import com.larvalabs.svgandroid.copy.SVG;
import com.larvalabs.svgandroid.copy.SVGParser;
import com.rho.indicators.IndicatorData;
import com.rho.indicators.IndicatorView;
import com.rho.indicators.IndicatorView.Layout;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

import android.view.WindowManager;

/**
 * BatteryView describes the battery indicator. Relies on battery status from an external source.
 * @author Ben Kennedy (NCVT73)
 */
public class BatteryView extends IndicatorView
{
	protected  int LONG_SIDE = 16; //Size of the battery icon's longest side
	protected  int SHORT_SIDE = 12; //Size of the battery icon's shortest side
	float multipleVar = 0;
	protected int batteryStep;
	protected boolean isPowerConnected;
	private Picture batPic0;
	private Picture batPic1;
	private Picture batPic2;
	private Picture batPic3;
	private Picture batPic4;
	private Picture batPicC;
	private Rect boundingRect;
	

	public BatteryView(Context context)
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
		
		if(isPowerConnected)
		{
			canvas.drawPicture(batPicC, boundingRect); 
		}
		else
		{
			switch(batteryStep)
			{
				case 0: canvas.drawPicture(batPic0, boundingRect); break;
				case 1: canvas.drawPicture(batPic1, boundingRect); break;
				case 2: canvas.drawPicture(batPic2, boundingRect); break;
				case 3: canvas.drawPicture(batPic3, boundingRect); break;
				case 4: canvas.drawPicture(batPic4, boundingRect); break;
				default: /*Shouldnt Happen*/ break;
			}
		}
		super.draw(canvas);
		Logger.D(TAG, "draw-");
	}
	
	@Override
	protected void changeIcon()
	{
		Logger.D(TAG, "changeIcon+");
		SVG bat0;
		SVG bat1;
		SVG bat2;
		SVG bat3;
		SVG bat4;
		SVG batC;
		
		Activity activity = RhodesActivity.safeGetInstance();
		if(activity == null)
		{
			Logger.E(TAG, "Cannot get activity");
		}
		Resources resources = activity.getResources();

		if(layout == Layout.RIGHT)
		{
			bat0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat0_0"), 0xFF000000, color);
			bat1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat1_0"), 0xFF000000, color);
			bat2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat2_0"), 0xFF000000, color);
			bat3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat3_0"), 0xFF000000, color);
			bat4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat4_0"), 0xFF000000, color);
			batC = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "batc_0"), 0xFF000000, color);
		}
		else if(layout == Layout.LEFT)
		{
			bat0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat0_180"), 0xFF000000, color);
			bat1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat1_180"), 0xFF000000, color);
			bat2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat2_180"), 0xFF000000, color);
			bat3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat3_180"), 0xFF000000, color);
			bat4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat4_180"), 0xFF000000, color);
			batC = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "batc_180"), 0xFF000000, color);
		}
		else if(layout == Layout.UP)
		{
			bat0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat0_270"), 0xFF000000, color);
			bat1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat1_270"), 0xFF000000, color);
			bat2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat2_270"), 0xFF000000, color);
			bat3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat3_270"), 0xFF000000, color);
			bat4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat4_270"), 0xFF000000, color);
			batC = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "batc_270"), 0xFF000000, color);
		}
		else if(layout == Layout.DOWN)
		{
			bat0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat0_90"), 0xFF000000, color);
			bat1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat1_90"), 0xFF000000, color);
			bat2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat2_90"), 0xFF000000, color);
			bat3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat3_90"), 0xFF000000, color);
			bat4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "bat4_90"), 0xFF000000, color);
			batC = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw", "batc_90"), 0xFF000000, color);
		}
		else
		{
			//Should never happen
			Logger.E(TAG, "changeIcon-: Unknown layout");
			return;
		}

		batPic0 = bat0.getPicture();
		batPic1 = bat1.getPicture();
		batPic2 = bat2.getPicture();
		batPic3 = bat3.getPicture();
		batPic4 = bat4.getPicture();
		batPicC = batC.getPicture();
		Logger.D(TAG, "changeIcon-");
	}
	

	/**
	 * Converts a 0-100 range int into 0-4
	 * @param batteryLevel
	 * @return the number of segments that should be shown in the icon
	 * @author Ben Kennedy (NCVT73)
	 */
	private static int convertToBatteryStep(int batteryLevel)
	{
		return (batteryLevel -1) / 20;
	}
	
	/**
	 * Changes the battery level (in %) and/or power connection status 
	 * @param batteryLevel the new battery level
	 * @param isPowerConnected whether the device is connected to AC power
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setPowerState(int batteryLevel, boolean isPowerConnected)
	{
		Logger.D(TAG, "setPowerState+ lvl:" + batteryLevel + " connected:" + isPowerConnected);
		boolean requiresRedraw = false;
		int batteryStep = convertToBatteryStep(batteryLevel);
		if(this.batteryStep != batteryStep)
		{
			requiresRedraw = true;
			this.batteryStep = batteryStep;
		}
		if(this.isPowerConnected != isPowerConnected)
		{
			requiresRedraw = true;
			this.isPowerConnected = isPowerConnected;
		}
		if(requiresRedraw) redraw();
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
