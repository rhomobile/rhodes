package com.rho.rhoelements.graphics;

import java.io.FileNotFoundException;
import java.util.ArrayList;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Rect;
import android.graphics.RectF;
import android.view.View;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;

/**
 * An abstract class for the graphical display of Indicators.
 * Contains extensible methods: currently you can only set "indicator
 * color", but if you change the access modifiers of setFillColor/setBorderColor/
 * setEmptyColor to "protected" you will be able to set these individually.
 * @author Ben Kennedy
 */
public abstract class Indicator
{
	//Ordinals for icon/graph positioning
	public static final int TOP = 0;
	public static final int LEFT = 1;
	public static final int RIGHT = 2;
	public static final int BOTTOM = 3;
	public static final int	UP		= 0;
	public static final int	DOWN	= 3;
	private static final int MAXVALUE = 10;

	protected int 	xPosition;
	protected int 	yPosition;
	protected boolean 	isVisible;
	protected int 	iconPosition;
	protected int 	graphPosition;
	protected int 	value;
	protected int	maxWidth;
	protected int	maxHeight;

	protected Paint 	borderPaint;
	protected Paint 	fillPaint;
	protected Paint 	emptyPaint;
	protected Rect[] 	rectangles;
	protected View 	view;
	protected boolean   isLeft;
	protected boolean   isTop;
	protected boolean	recalculatePositions;
	private boolean 	isVertical;

	//Drawing the indicator, convenience values
	private static final int boxConstant = 16;
	private static final int noOfBoxes = 5;
	private static final int iconSize = 20;
	private static final int spacing = 3;
	private static final int graphLength = (boxConstant*noOfBoxes) + (spacing*(noOfBoxes -1));
	private static final int indicatorWidth = boxConstant; //=16
	private static final int indicatorLength = graphLength + (spacing*2) + (iconSize); //= 118
	private static final int LINEWIDTH = 1;

	//Abstract methods, must be overriden
	/** Resets the indicator to the default settings */
	public abstract void reset();
	/** Retrieves the default position of the graph. Can be either TOP, LEFT, RIGHT or BOTTOM
	 * @return the default position of the graph. Can be either TOP, LEFT, RIGHT or BOTTOM */
	protected abstract int getDefaultGraphPosition();
	/** Returns the name of the indicator */
	protected abstract String getName();
	/** Draws the indicator */
	protected abstract void draw(Canvas canvas);
	/** Redraws the indicator */
	protected abstract void reDraw();
	protected abstract void setValue(int value);

	/**
	 * Constructs a new Indicator
	 * @param view the view on which to draw the indicator.
	 * @throws FileNotFoundException 
	 */
	protected Indicator(View view)
	{
		this.view = view;
		recalculatePositions = false;
		isVertical = true;
		init();
	}

	/**
	 * Sets up the paints (other than the color)
	 */
	private void init()
	{
		borderPaint = new Paint();
		borderPaint.setAntiAlias(true);
		borderPaint.setStyle(Style.STROKE);//A line
		borderPaint.setStrokeWidth(LINEWIDTH);

		fillPaint = new Paint();//A fill
		fillPaint.setAntiAlias(true);

		emptyPaint = new Paint();// The fill for empty boxes
		emptyPaint.setAntiAlias(true);
		emptyPaint.setColor(Color.parseColor("white"));
	}


	/**
	 * @param view the view object on which the indicator is displayed
	 */
	public void setView(View view)
	{
		this.view = view;
		this.maxWidth = view.getWidth();
		this.maxHeight = view.getHeight();
		recalculatePositions = true;
	}

	/**
	 * Sets the x position of the indicator. Logs a warning if the value will place a part of
	 * the indicator offscreen and then will not carry out the change.
	 * @param xPosition sets the x position of the indicator. 
	 * @param isLeft whether the x position is measured from the left of the screen and indicator
	 * (false will measure from the right of the screen and indicator).
	 */
	public void setxPosition(int xPosition, boolean isLeft)
	{
		int xWidth = (isVertical ? indicatorWidth : indicatorLength);
		if(xPosition + xWidth > maxWidth || xPosition < 0)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, this.getName() + " Indicator x value is out of range (off screen). Ignoring setting."));
		}
		else
		{
			setX(xPosition, isLeft);
		}
	}
	
	/**
	 * A hidden method to allow initial internal setup when the view dimensions haven't been initialised
	 * @param xPosition xPosition sets the x position of the indicator.
	 * @param isLeft whether the x position is measured from the left of the screen and indicator
	 * (false will measure from the right of the screen and indicator).
	 */
	protected void setX(int xPosition, boolean isLeft)
	{
		this.xPosition = xPosition;
		this.isLeft = isLeft;
		recalculatePositions = true;
	}

	/**
	 * Sets the y position of the indicator. Logs a warning if the value will place a part of
	 * the indicator offscreen and then will not carry out the change.
	 * @param yPosition sets the y position of the indicator. 
	 * @param isTop whether the y position is measured from the top of the screen and indicator
	 * (false will measure from the bottom of the screen and indicator).
	 */
	public void setyPosition(int yPosition, boolean isTop)
	{
		int yHeight = (isVertical ? indicatorLength : Math.max(indicatorWidth, iconSize));
		if(yPosition + yHeight > maxHeight || yPosition < 0)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, this.getName() + " Indicator y value is out of range (off screen). Ignoring setting."));
		}
		else
		{
			setY(yPosition, isTop);
		}
	}
	
	/**
	 * A hidden method to allow initial internal setup when the view dimensions haven't been initialised
	 * @param yPosition yPosition sets the y position of the indicator.
	 * @param isTop whether the y position is measured from the top of the screen and indicator
	 * (false will measure from the bottom of the screen and indicator).
	 */
	protected void setY(int yPosition, boolean isTop)
	{
		this.yPosition = yPosition;
		this.isTop = isTop;
		recalculatePositions = true;
	}



	/**NB remove me once you start using the individual setColors (below)
	 * Sets the color of the fill and the border.
	 * The input string Supported formats are: #RRGGBB #AARRGGBB 'red', 'blue', 'green', 
	 * 'black', 'white', 'gray', 'cyan', 'magenta', 'yellow', 'lightgray', 'darkgray'
	 * @param paintColor the color to change the indicator to.
	 */
	public void setPaintColor(String paintColor)
	{
		try
		{
			if(paintColor.length() < 1)
			{
				throw new IllegalArgumentException();
			}
			int color = Color.parseColor(paintColor);
			this.fillPaint.setColor(color);
			this.borderPaint.setColor(color);
		}
		catch(IllegalArgumentException e)
		{
			//Do Nothing, nothing will have changed. Maybe change to default color?
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Illegal color value set: " + paintColor));
		}
	}


	/** NB change me to public to use. Currently hidden to reduce confusion, but here for
	 * future extensibility.
	 * Sets the color of the fill. The input string Supported formats are: #RRGGBB 
	 * #AARRGGBB 'red', 'blue', 'green', 'black', 'white', 'gray', 'cyan', 
	 * 'magenta', 'yellow', 'lightgray', 'darkgray'
	 * @param fillColor the color to change the fill of the indicator boxes to
	 */
	@SuppressWarnings("unused")
	private void setFillColor(String fillColor)
	{
		try
		{
			if(fillColor.length() < 1)
			{
				throw new IllegalArgumentException();
			}
			int color = Color.parseColor(fillColor);
			this.fillPaint.setColor(color);
		}
		catch(IllegalArgumentException e)
		{
			//Do Nothing, nothing will have changed. Maybe change to default color?
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Illegal color value set: " + fillColor));
		}
	}

	/** NB change me to public to use. Currently hidden to reduce confusion, but here for
	 * future extensibility.
	 * Sets the color of the borders. The input string Supported formats are: #RRGGBB 
	 * #AARRGGBB 'red', 'blue', 'green', 'black', 'white', 'gray', 'cyan', 
	 * 'magenta', 'yellow', 'lightgray', 'darkgray'.
	 * @param fillColor the color to change the fill of the indicator boxes to
	 */
	@SuppressWarnings("unused")
	private void setBorderColor(String borderColor)
	{
		try
		{
			if(borderColor.length() < 1)
			{
				throw new IllegalArgumentException();
			}
			int color = Color.parseColor(borderColor);
			this.borderPaint.setColor(color);
		}
		catch(IllegalArgumentException e)
		{
			//Do Nothing, nothing will have changed. Maybe change to default color?
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Illegal color value set: " + borderColor));
		}
	}

	/** NB change me to public to use. Currently hidden to reduce confusion, but here for
	 * future extensibility.
	 * Sets the empty color. The input string Supported formats are: #RRGGBB 
	 * #AARRGGBB 'red', 'blue', 'green', 'black', 'white', 'gray', 'cyan', 
	 * 'magenta', 'yellow', 'lightgray', 'darkgray'.
	 * @param fillColor the color to change the fill of the indicator boxes to
	 */
	@SuppressWarnings("unused")
	private void setEmptyColor(String emptyColor)
	{
		try
		{
			if(emptyColor.length() < 1)
			{
				throw new IllegalArgumentException();
			}
			int color = Color.parseColor(emptyColor);
			this.emptyPaint.setColor(color);
		}
		catch(IllegalArgumentException e)
		{
			//Do Nothing, nothing will have changed. Maybe change to default color?
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Illegal color value set: " + emptyColor));
		}
	}

	/**
	 * Sets the visibility of the indicator
	 * @param isVisible sets the visibility of the indicator
	 */
	public void setVisible(boolean isVisible)
	{
		this.isVisible = isVisible;
	}

	/**
	 * Sets the position of the icon, in relation to the graph.
	 * Only use Indicator.TOP, Indicator.LEFT, Indicator.RIGHT or Indicator.BOTTOM 
	 * @param position the position ordinal that describes where the icon should be set
	 */
	public void setIconPosition(int position)
	{
		if(position > 3 || position < 0)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Illegal icon position set: " + position));
		}
		else if(this.iconPosition != position)
		{
			this.iconPosition = position;
			recalculatePositions = true;
		}
	}

	/**
	 * Sets the layout of the graph
	 * Only use Indicator.TOP, Indicator.LEFT, Indicator.RIGHT or Indicator.BOTTOM 
	 * @param position the position ordinal that describes the layout of the graph
	 */
	public void setGraphPosition(int position)
	{
		if(position > 3 || position < 0)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Illegal graph position set: " + position));
		}
		else if(this.graphPosition != position)
		{
			this.graphPosition = position;
			recalculatePositions = true;
		}
	}
	
	/**
	 * Sets the layout of the indicator
	 * @param layout the layout orientation
	 */
	public void setLayout(int layout)
	{
		this.setGraphPosition(layout);
	}


	/* (non-Javadoc)
	 * @see java.lang.Object#clone()
	 */
	@Override
	protected Object clone() throws CloneNotSupportedException
	{
		throw new CloneNotSupportedException();
	}
}
