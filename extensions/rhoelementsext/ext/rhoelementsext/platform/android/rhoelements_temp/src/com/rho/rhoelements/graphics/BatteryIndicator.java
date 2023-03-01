package com.rho.rhoelements.graphics;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Picture;
import android.graphics.Rect;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.View;
import android.view.WindowManager;
import android.util.DisplayMetrics;
import android.view.Display;
import com.larvalabs.svgandroid.SVG;
import com.larvalabs.svgandroid.SVGParser;
import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * A battery indicator Singleton class that writes customisable battery data
 * to a canvas
 * @author Ben Kennedy
 */
public class BatteryIndicator extends Indicator
{
	private  int LONG_SIDE = 16;
	private  int SHORT_SIDE = 12;
	float multipleVar = 0;
	private final String NAME = "Battery"; 
	public static BatteryIndicator bIndicator;
	public boolean isCharging;
	private Picture	batPic0;
	private Picture batPic1;
	private Picture batPic2;
	private Picture batPic3;
	private Picture batPic4;
	private Picture batPicC;
	private Rect boundingRect;
	private Rect oldBoundingRect;
	private int color;
	private boolean	changeIcon;
	private boolean isChangedValue;
	private int	width;
	private int	height;

	/**
	 * Private constructor to create the Battery Indicator. Enforces Singleton-ness
	 * Use getIndicator(..) to retrieve the BatteryIndicator.
	 * 
	 * @param canvasWidth the width of the drawable area
	 * @param canvasHeight the height of the drawable area
	 */
	private BatteryIndicator(View view)
	{
		super(view);
		screenResolutionCheck();
	}

	/**
	 * Returns the one-instance battery indicator or constructs a new one if the battery
	 * indicator hasn't been built
	 * 
	 * @param canvasWidth the width of the drawable area
	 * @param canvasHeight the height of the drawable area
	 * @return the singleton BatteryIndicator object
	 */
	public synchronized static BatteryIndicator getIndicator(View view)
	{
		if(bIndicator == null)
		{
			bIndicator = new BatteryIndicator(view); //TODO: Error when canvas dimensions are small 
			bIndicator.reset();
		}
		bIndicator.view = view;
		return bIndicator;
	}

	@Override
	public void reset()
	{
		try{
			isChangedValue = false;
			if(boundingRect != null)
			{
				view.postInvalidate(boundingRect.left, boundingRect.top, boundingRect.right, boundingRect.bottom);
			}
			this.boundingRect = new Rect();
			this.setPaintColor("#000000");
			this.setX(0, true);
			this.setY(0, true);
			this.setVisible(false);
			this.setLayout(RIGHT);
			this.setValue(0);
			this.setCharging(false);
			recalculatePositions();
		}
		catch(Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Exception : " + e));

		}
	}
	
	@Override
	public void setPaintColor(String paintColor)
	{
		try
		{
			if(paintColor.length() < 1)
			{
				throw new IllegalArgumentException();
			}
			int tempColor = Color.parseColor(paintColor);
			if(tempColor != color)
			{
				color = tempColor;
				changeIcon = true;
				reDraw();
			}
		}
		catch(IllegalArgumentException e)
		{
			//Do Nothing, nothing will have changed. Maybe change to default color?
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Illegal color value set: " + paintColor));
		}
		catch(ArrayIndexOutOfBoundsException e) //Android bug for input = "#"
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Illegal color value set: " + paintColor));
		}
	}
	
	@Override
	public void setGraphPosition(int position)
	{
		super.setGraphPosition(position);
		if(recalculatePositions)//TODO bit messy, will regen icon if ANY changes have been made
		{
			changeIcon = true;
			reDraw();
		}
	}
	
	/**
	 * Changes the 6 icons (0%, 25%, 50%, 75%, 100%, charging) to a new orientation and/or color.
	 */
	private void changeIcon()
	{
	try{
		SVG bat0 = null;
		SVG bat1 = null;
		SVG bat2 = null;
		SVG bat3 = null;
		SVG bat4 = null;
		SVG batC = null;
		
		Resources resources = Common.mainActivity.getResources();

		if(graphPosition == RIGHT)
		{
			width = LONG_SIDE;
			height = SHORT_SIDE;
			bat0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat0_0"), 0xFF000000, color);
			bat1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat1_0"), 0xFF000000, color);
			bat2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat2_0"), 0xFF000000, color);
			bat3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat3_0"), 0xFF000000, color);
			bat4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat4_0"), 0xFF000000, color);
			batC = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","batc_0"), 0xFF000000, color);
		}
		else if(graphPosition == LEFT)
		{
			width = LONG_SIDE;
			height = SHORT_SIDE;
			bat0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat0_180"), 0xFF000000, color);
			bat1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat1_180"), 0xFF000000, color);
			bat2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat2_180"), 0xFF000000, color);
			bat3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat3_180"), 0xFF000000, color);
			bat4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat4_180"), 0xFF000000, color);
			batC = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","batc_180"), 0xFF000000, color);
		}
		else if(graphPosition == TOP)
		{
			width = SHORT_SIDE;
			height = LONG_SIDE;
			bat0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat0_270"), 0xFF000000, color);
			bat1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat1_270"), 0xFF000000, color);
			bat2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat2_270"), 0xFF000000, color);
			bat3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat3_270"), 0xFF000000, color);
			bat4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat4_270"), 0xFF000000, color);
			batC = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","batc_270"), 0xFF000000, color);
		}
		else if(graphPosition == BOTTOM)
		{
			width = SHORT_SIDE;
			height = LONG_SIDE;
			bat0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat0_90"), 0xFF000000, color);
			bat1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat1_90"), 0xFF000000, color);
			bat2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat2_90"), 0xFF000000, color);
			bat3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat3_90"), 0xFF000000, color);
			bat4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","bat4_90"), 0xFF000000, color);
			batC = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","batc_90"), 0xFF000000, color);
		}

		batPic0 = bat0.getPicture();
		batPic1 = bat1.getPicture();
		batPic2 = bat2.getPicture();
		batPic3 = bat3.getPicture();
		batPic4 = bat4.getPicture();
		batPicC = batC.getPicture();
		changeIcon = false;
		}catch(Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,"**********************************************************"));
		}
	}

	/**
	 * Returns the name of the Indicator
	 * @return the name of the Indicator
	 */
	protected String getName()
	{
		return NAME;
	}
	
	/**
	 * Recalculates the dimensions of the bounding box and invalidates dirty areas
	 */
	private void recalculatePositions()
	{
		view.postInvalidate(boundingRect.left, boundingRect.top, boundingRect.right, boundingRect.bottom);//Invalidate old rect
		//TODO also add offscreen checking maybe
		if(changeIcon) //Need to update icon to get correct width
		{
			changeIcon();
		}
		if(isTop)
		{
			boundingRect.top = yPosition;
			boundingRect.bottom = height + yPosition; //TODO change to current icon
		}
		else
		{
			int viewHeight = view.getHeight();
			boundingRect.bottom = viewHeight - yPosition;
			boundingRect.top = viewHeight - yPosition - height; //TODO change to current icon
		}
		if(isLeft)
		{
			boundingRect.left = xPosition;
			boundingRect.right = width + xPosition;
		}
		else
		{
			int viewWidth = view.getWidth();
			boundingRect.right = viewWidth - xPosition;
			boundingRect.left = viewWidth - xPosition - width;
		}
		recalculatePositions = false;
		view.postInvalidate(boundingRect.left, boundingRect.top, boundingRect.right, boundingRect.bottom); //Invalidate new rect
		isChangedValue = false;
	}
	
	/**
	 * Changes the charging boolean
	 * @param isCharging true if the device is plugged in, false otherwise
	 */
	public void setCharging(boolean isCharging)
	{
		if(isCharging != this.isCharging)
		{
			this.isCharging = isCharging;
			isChangedValue = true;
			reDraw();
		}
	}
	
	@Override
	public void setVisible(boolean visible)
	{
		boolean tempVisible = this.isVisible;
		super.setVisible(visible);
		if(visible != tempVisible)
		{
			view.postInvalidate(boundingRect.left, boundingRect.top, boundingRect.right, boundingRect.bottom);
		}
	}

	@Override
	protected int getDefaultGraphPosition()
	{
		return RIGHT;
	}

	@Override
	public void reDraw()
	{
		if(changeIcon) //Need to update icon to get correct width
		{
			changeIcon();
			recalculatePositions = true;
		}
		if(recalculatePositions)
		{
			recalculatePositions();
		}
		if(isChangedValue)
		{
			view.postInvalidate(boundingRect.left, boundingRect.top, boundingRect.right, boundingRect.bottom);
		}
	}
	
	@Override
	public void draw(Canvas canvas)
	{
		if(isVisible)
		{
			//TODO add charging thread
			if(oldBoundingRect != null)
			{
				reDraw();
			}
			
			if(recalculatePositions)
			{
				recalculatePositions();
			}
			
			if(isCharging)
			{
				canvas.drawPicture(batPicC, boundingRect);
			}
			else if(value < 2)
			{
				canvas.drawPicture(batPic0, boundingRect);
			}
			else if(value < 4)
			{
				canvas.drawPicture(batPic1, boundingRect);
			}
			else if(value < 6)
			{
				canvas.drawPicture(batPic2, boundingRect);
			}
			else if(value < 8)
			{
				canvas.drawPicture(batPic3, boundingRect);
			}
			else
			{
				canvas.drawPicture(batPic4, boundingRect);
			}
		}
	}

	/**
	 * Sets the value of the battery in the range 0-10.
	 * @param value the value of the indicator strength in a range 0-10
	 */
	@Override
	public void setValue(int value)
	{
		if(this.value != value)
		{
			isChangedValue = true;
			this.value = value;
		}
		if(value > 10 || value < 0)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Illegal indicator value set: " + value));
		}
	}
	
	public void screenResolutionCheck(){

		Display display = null;
		try
		{
		    display = ((WindowManager)RhodesActivity.safeGetInstance().getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		}
		catch (Exception e) {
            Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.toString()));
        }
		int width = display.getWidth();  // deprecated
		int height = display.getHeight();  // deprecated
		
		if(height > width){
			multipleVar = 1;
		}else{
			multipleVar = (float) 1.5;
		}
		
		
		DisplayMetrics metrics = new DisplayMetrics();
		try
		{
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
		catch (Exception e) {
            Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.toString()));
        }

	}
}
