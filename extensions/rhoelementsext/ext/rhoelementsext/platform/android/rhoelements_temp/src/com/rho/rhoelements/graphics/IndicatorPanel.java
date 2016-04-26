package com.rho.rhoelements.graphics;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.widget.LinearLayout;

/**
 * A class for a transparent overlay panel that can hold Indicators.
 * @author Ben Kennedy
 */
public class IndicatorPanel extends LinearLayout
{
	private BatteryIndicator bIndicator;
	private SignalIndicator sIndicator;

	/**
	 * @param context
	 */
	public IndicatorPanel(Context context)
	{
		super(context);
		init();
	}

	/**
	 * @param context
	 * @param attrSet
	 */
	public IndicatorPanel(Context context, AttributeSet attrSet)
	{
		super(context, attrSet);
		init();
	}

	/**
	 * Initialises the IndicatorPanel by constructing the Indicators.
	 */
	private void init()
	{
		bIndicator = BatteryIndicator.getIndicator(this);
		sIndicator = SignalIndicator.getIndicator(this);
	}

	/* (non-Javadoc)
	 * @see android.view.ViewGroup#dispatchDraw(android.graphics.Canvas)
	 */
	@Override
	protected void dispatchDraw(Canvas canvas)
	{   
		bIndicator.draw(canvas);
		sIndicator.draw(canvas);
		super.dispatchDraw(canvas);
	}

	/**
	 * Returns the BatteryIndicator object
	 * @return the BatteryIndicator object
	 */
	public BatteryIndicator getBatteryIndicator()
	{
		return bIndicator;
	}

	/**
	 * Returns the SignalIndicator object
	 * @return the SignalIndicator
	 */
	public SignalIndicator getSignalIndicator()
	{
		return sIndicator;
	}

	/* (non-Javadoc)
	 * @see android.view.View#onSizeChanged(int, int, int, int)
	 */
	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh)
	{
		super.onSizeChanged(w, h, oldw, oldh);
		sIndicator.setView(this);
		bIndicator.setView(this);
	}
}
