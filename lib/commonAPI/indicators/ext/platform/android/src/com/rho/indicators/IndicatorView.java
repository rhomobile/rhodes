package com.rho.indicators;

import java.security.InvalidParameterException;

import android.content.Context;
import android.graphics.Color;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsoluteLayout;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * An abstract class for RhoElements indicators. These indicators must have:
 *  - Left position
 *  - top position
 *  - Layout
 *  - Color
 * The indicator will be placed in a fixed position in the RhoElements activity.
 * @author Ben Kennedy (NCVT73)
 *
 */
public abstract class IndicatorView extends View
{	
	protected static String TAG = "IndicatorView";

	/**
	 * Updates the icon graphic if layout, indicator value or color changes
	 * @author Ben Kennedy (NCVT73)
	 */
	protected abstract void changeIcon();
	/**
	 * Returns the length (in pixels) of the longest side of the icon. Used to create the view in the Layout.
	 * @return The length of the longest side of the icon
	 * @author Ben Kennedy (NCVT73)
	 */
	protected abstract int getLongSideLength();
	/**
	 * Returns the length (in pixels) of the shortest side of the icon. Used to create the view in the Layout.
	 * @return The length of the shortest side of the icon
	 * @author Ben Kennedy (NCVT73)
	 */
	protected abstract int getShortSideLength();
	
	protected int top;
	protected int left;
	protected int color;
	protected Layout layout;

	public IndicatorView(Context context)
	{
		super(context);
	}
	
	/**
	 * Resets the location of the Indicator on the screen
	 * @param top the distance in pixels that the top of the view will be from the top of the display
	 * @param left the distance in pixels that the left of the view will be from the left of the display
	 * @param layout the Layout style of the Indicator
	 * @param upDownWidth the width of the indicator when it is in an UP or a DOWN layout
	 * @param upDownHeight the height of the indicator when it is in an UP or a DOWN layout
	 * @param isInvalidLayout if set, forces the icons to be reloaded from raw and sets the color of the icon.
	 * @author Ben Kennedy (NCVT73)
	 */
	@SuppressWarnings("deprecation")
	protected void setParams(int top, int left, Layout layout, int upDownWidth, int upDownHeight, boolean isInvalidLayout)
	{
		this.top = top;
		this.left = left;
		if(this.layout != layout) isInvalidLayout = true;
		this.layout = layout;
		
		int width;
		int height;
		if(layout == Layout.UP || layout == Layout.DOWN)
		{
			width = upDownWidth;
			height = upDownHeight;
		}
		else //LEFT or RIGHT
		{
			width = upDownHeight;
			height = upDownWidth;
		}
		
		if(isInvalidLayout) changeIcon();
		this.setLayoutParams(new AbsoluteLayout.LayoutParams(width, height, this.left, this.top));
	}
	
	/**
	 * Shows the indicator
	 * @author Ben Kennedy (NCVT73)
	 */
	public void show()
	{
		this.setVisibility(VISIBLE);
		this.invalidate();
	}
	
	/**
	 * Hides the indicator
	 * @author Ben Kennedy (NCVT73)
	 */
	public void hide()
	{
		this.setVisibility(INVISIBLE);
	}

	
	/**
	 * Sets up an IndicatorView in accordance with the given parameters. This will only work if these
	 * axioms hold true:
	 * 	- the dimensions of the Indicator are the same in layout UP and layout DOWN
	 *  - the dimensions of the Indicator are the same in layout TOP and layout BOTTOM
	 *  - the width of both Layout.TOP and Layout.BOTTOM == the height of both Layout.UP and Layout/DOWN
	 *  - the height of both Layout.TOP and Layout.BOTTOM == the width of both Layout.UP and Layout/DOWN 
	 * @param top the distance in pixels that the top of the view will be from the top of the display
	 * @param left the distance in pixels that the left of the view will be from the left of the display
	 * @param layout the Layout style of the Indicator
	 * @param color the color of the indicator
	 * @param upDownWidth the width of the indicator when it is in an UP or a DOWN layout
	 * @param upDownHeight the height of the indicator when it is in an UP or a DOWN layout
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setupView(int top, int left, Layout layout, int color)
	{
		Logger.D(TAG, "setupView+ top: " + top + " left: " + left + " layout: " + layout + " color: " + color);

		int upDownWidth = getShortSideLength();
		int upDownHeight = getLongSideLength();
		
		this.color = color;
		setParams(top, left, layout, upDownWidth, upDownHeight, true);
		this.setBackgroundColor(Color.TRANSPARENT);
		this.setVisibility(GONE);
		
		Object webView = RhoExtManager.getImplementationInstance().getWebView().getView();

		if (webView instanceof ViewGroup)
		{
			ViewGroup wv = (ViewGroup) ((ViewGroup) webView).getParent();
			if ((wv != null) && (this != null))
			{
				wv.addView(this);
				wv.bringChildToFront(this);
				this.requestFocus();
				this.bringToFront();
				this.invalidate();
			}
		}
		else
		{
			ViewGroup wv = (ViewGroup) (((View) webView).getParent()).getParent();
			if ((wv != null) && (this != null))
			{
				wv.addView(this);
				wv.bringChildToFront(this);
				this.requestFocus();
				this.bringToFront();
				this.invalidate();
			}
		}
		Logger.D(TAG, "setupView-");
	}
	
	/**
	 * Parses a string and converts it to a Layout value
	 * @param layout the String to parse
	 * @return the corresponding Layout enum value
	 * @throws InvalidParameterException if the inputted string is null or not recognised.
	 * @author Ben Kennedy (NCVT73)
	 */
	public static Layout parseLayout(String layout) throws InvalidParameterException
	{
		if(layout == null) throw new InvalidParameterException("Parameter cannot be null");
		
		if(layout.equalsIgnoreCase(Layout.LEFT.name())) return Layout.LEFT;
		else if(layout.equalsIgnoreCase(Layout.RIGHT.name())) return Layout.RIGHT;
		else if(layout.equalsIgnoreCase(Layout.UP.name())) return Layout.UP;
		else if(layout.equalsIgnoreCase(Layout.DOWN.name())) return Layout.DOWN;
		
		throw new InvalidParameterException("Parameter not recognised");
	}
	
	/**
	 * Converts the inputed value to the corresponding Layout enum type
	 * @param layoutObj an object which should be a String which should be either "up", "down", "left", "right"
	 * (case is not important).
	 * @return the Layout corresponding to the string in the layoutObj parameter
	 * @throws InvalidParameterException if the Object is not a String, or the String parameter cannot
	 * be converted to the Layout type (i.e. the string did not match the name of any Layout)
	 * @author Ben Kennedy (NCVT73)
	 */
	public static Layout parseLayout(Object layoutObj) throws InvalidParameterException
	{
		if(layoutObj instanceof String)
		{
			return IndicatorView.parseLayout((String) layoutObj);
		}
		else 	throw new InvalidParameterException("Unrecognised layout parameter: " + layoutObj);
	}
	
	/**
	 * Converts an String object containing a color string to a int formatted string
	 * @param colorObj the RGB string formatted as "#RRGGBB". Currently, alpha is not supported because of MSWindows.
	 * @return the int version of the RGB string
	 * @throws InvalidParameterException if the Object is not a String or it is not formatted in "#RRGGBB" hex format.
	 * @author Ben Kennedy (NCVT73)
	 */
	public static int parseColor(Object colorObj) throws InvalidParameterException
	{
		if(colorObj instanceof String)
		{
			String colorStr = (String) colorObj;
			if(!colorStr.startsWith("#") || colorStr.length() != 7)
			{
				throw new InvalidParameterException("Unrecognised battery color parameter: " + colorObj);
			}
			else
			{
				try
				{
					return Color.parseColor(colorStr);
				}
				catch(IllegalArgumentException e)
				{
					throw new InvalidParameterException("Unrecognised battery color parameter: " + colorObj);
				}
			}
		}
		else 	throw new InvalidParameterException("Unrecognised battery color parameter: " + colorObj);
	}
	
	/**
	 * Converts an Integer Object to an int as long as it is >= 0.
	 * @param dimension the Object to convert to an int
	 * @return the int value of dimension
	 * @throws InvalidParameterException if the Object could not be cast to an Integer, or the integer < 0
	 * @author Ben Kennedy (NCVT73)
	 */
	public static int parseDimension(Object dimension) throws InvalidParameterException
	{
		if(dimension instanceof Integer && (Integer) dimension >= 0) return (Integer) dimension;
		else if(dimension instanceof String)
		{
			try
			{
				return Integer.parseInt((String) dimension);
			}
			catch(NumberFormatException e)
			{
				throw new InvalidParameterException("Dimension could not be recognised: " + dimension);
			}
		}
		else 	throw new InvalidParameterException("Dimension could not be recognised: " + dimension + ", typeof " + dimension.getClass().getName());
	}
	
	/**
	 * Returns whether the icon is in portrait mode (UP or DOWN) or not (LEFT or RIGHT)
	 * @return true if in portrait mode (UP or DOWN), false if in landscape (LEFT or RIGHT)
	 * @author Ben Kennedy (NCVT73)
	 */
	protected boolean isPortrait()
	{
		if(layout.equals(Layout.UP) || layout.equals(Layout.DOWN)) return true;
		return false;
	}
	
	/**
	 * Updates the visual parameters of the view. The view must have already been added to the layout
	 * (by using setupView(...)) 
	 * @param top the distance in pixels that the top of the view will be from the top of the display
	 * @param left the distance in pixels that the left of the view will be from the left of the display
	 * @param layout the Layout style of the Indicator
	 * @param color the color of the indicator
	 * @author Ben Kennedy (NCVT73)
	 */
	public void updateParams(int top, int left, Layout layout, int color)
	{
		boolean isReLayoutNeeded = false;
		if(this.color != color) isReLayoutNeeded = true;
		this.color = color;
		this.setParams(top, left, layout, getShortSideLength(), getLongSideLength(), isReLayoutNeeded);
	}
	
	/**
	 * Redraws the view
	 * @author Ben Kennedy (NCVT73)
	 */
	public void redraw()
	{
		invalidate();
	}
	
	/**
	 * The possible layouts for indicators.
	 * @author Ben Kennedy (NCVT73)
	 */
	public enum Layout
	{
		LEFT, RIGHT, UP, DOWN
	}
}
