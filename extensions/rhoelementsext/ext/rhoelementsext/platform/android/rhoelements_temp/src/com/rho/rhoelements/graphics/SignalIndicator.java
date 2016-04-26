package com.rho.rhoelements.graphics;

import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Color;
import android.content.Context;
import android.graphics.Picture;
import android.graphics.Rect;
import android.view.View;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.WindowManager;
import com.larvalabs.svgandroid.SVG;
import com.larvalabs.svgandroid.SVGParser;
import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.RhoExtManager;


/**
 * A battery indicator Singleton class that writes customisable signal data
 * to a canvas
 * @author Ben Kennedy
 */
public class SignalIndicator extends Indicator
{
	private final String NAME = "Signal"; 
	public static SignalIndicator sIndicator;
	//The signal icon as an array of startx, starty, finishx, finishy
	private  int LONG_SIDE = 16;
	private  int SHORT_SIDE = 11;
	float multipleVar = 0;
	private int	width;
	private int	height;
	private int color;
	private boolean	changeIcon;
	private boolean isChangedValue;
	private Rect boundingRect;
	private Rect oldBoundingRect;
	private Picture	sigPic0;
	private Picture sigPic1;
	private Picture sigPic2;
	private Picture sigPic3;
	private Picture sigPic4;
	private Picture sigPic5;

	/**
	 * Private constructor to create the Signal Indicator. With Default
	 * values. Enforces Singleton-ness.
	 * Use getIndicator(..) to retrieve the SignalIndicator.
	 * @param canvasWidth the width of the drawable area
	 * @param canvasHeight the height of the drawable area
	 */
	private SignalIndicator(View view)
	{
		super(view);
		screenResolutionCheck();
	}

	/**
	 * Returns the one-instance signal indicator or constructs a new one if the signal
	 * indicator hasn't been built.
	 * @param canvasWidth the width of the drawable area
	 * @param canvasHeight the height of the drawable area
	 * @return the singleton SignalIndicator object
	 */
	public synchronized static SignalIndicator getIndicator(View view)
	{
		if(sIndicator == null)
		{
			sIndicator = new SignalIndicator(view);
			sIndicator.reset();
		}
		sIndicator.view = view;
		return sIndicator;
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
			this.setX(0, false);
			this.setY(0, true);
			this.setVisible(false);
			this.setLayout(RIGHT);
			this.setValue(0);
			recalculatePositions();
		}
		catch(Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Exception : " + e));
		}
	}
	
	/**
	 * Returns the name of the Indicator
	 * @return the name of the Indicator
	 */
	@Override
	protected String getName()
	{
		return NAME;
	}

	@Override
	protected int getDefaultGraphPosition()
	{
		return LEFT;
	}

	/**
	 * Changes the 6 icons (0%, 20%, 40%, 60%, 80%, 100%) to a new orientation and/or color.
	 */
	private void changeIcon()
	{
	
		SVG sig0 = null;
		SVG sig1 = null;
		SVG sig2 = null;
		SVG sig3 = null;
		SVG sig4 = null;
		SVG sig5 = null;
		
		Resources resources = Common.mainActivity.getResources();

		if(graphPosition == RIGHT)
		{
			width = LONG_SIDE;
			height = SHORT_SIDE;
			sig0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig0_0"), 0xFF000000, color);
			sig1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig1_0"), 0xFF000000, color);
			sig2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig2_0"), 0xFF000000, color);
			sig3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig3_0"), 0xFF000000, color);
			sig4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig4_0"), 0xFF000000, color);
			sig5 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig5_0"), 0xFF000000, color);
		}
		else if(graphPosition == LEFT)
		{
			width = LONG_SIDE;
			height = SHORT_SIDE;
			sig0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig0_180"), 0xFF000000, color);
			sig1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig1_180"), 0xFF000000, color);
			sig2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig2_180"), 0xFF000000, color);
			sig3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig3_180"), 0xFF000000, color);
			sig4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig4_180"), 0xFF000000, color);
			sig5 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig5_180"), 0xFF000000, color);
		}
		else if(graphPosition == TOP)
		{
			width = SHORT_SIDE;
			height = LONG_SIDE;
			sig0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig0_270"), 0xFF000000, color);
			sig1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig1_270"), 0xFF000000, color);
			sig2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig2_270"), 0xFF000000, color);
			sig3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig3_270"), 0xFF000000, color);
			sig4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig4_270"), 0xFF000000, color);
			sig5 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig5_270"), 0xFF000000, color);
		}
		else if(graphPosition == BOTTOM)
		{
			width = SHORT_SIDE;
			height = LONG_SIDE;
			sig0 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig0_90"), 0xFF000000, color);
			sig1 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig1_90"), 0xFF000000, color);
			sig2 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig2_90"), 0xFF000000, color);
			sig3 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig3_90"), 0xFF000000, color);
			sig4 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig4_90"), 0xFF000000, color);
			sig5 = SVGParser.getSVGFromResource(resources, RhoExtManager.getResourceId("raw","sig5_90"), 0xFF000000, color);
		}

		sigPic0 = sig0.getPicture();
		sigPic1 = sig1.getPicture();
		sigPic2 = sig2.getPicture();
		sigPic3 = sig3.getPicture();
		sigPic4 = sig4.getPicture();
		sigPic5 = sig5.getPicture();
		changeIcon = false;
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
			
			if(value < 5)
			{
				canvas.drawPicture(sigPic0, boundingRect);
			}
			else if(value < 10)
			{
				canvas.drawPicture(sigPic1, boundingRect);
			}
			else if(value < 15)
			{
				canvas.drawPicture(sigPic2, boundingRect);
			}
			else if(value < 20)
			{
				canvas.drawPicture(sigPic3, boundingRect);
			}
			else if(value < 25)
			{
				canvas.drawPicture(sigPic4, boundingRect);
			}
			else
			{
				canvas.drawPicture(sigPic5, boundingRect);
			}
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
	
	/**
	 * Sets the value of the battery/signal in the range 0-30.
	 * @param value the value of the indicator strength in a range 0-30
	 */
	@Override
	public void setValue(int value)
	{
		if(this.value != value)
		{
			isChangedValue = true;
			this.value = value;
		}
		if(value > 30 || value < 0)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Illegal indicator value set: " + value));
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
		System.out.println("Phone Density >."+ metrics.densityDpi);
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
