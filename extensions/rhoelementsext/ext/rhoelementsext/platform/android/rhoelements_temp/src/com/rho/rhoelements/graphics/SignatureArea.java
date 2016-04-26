package com.rho.rhoelements.graphics;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Cap;
import android.graphics.Paint.Style;
import android.graphics.Path;
import android.os.Environment;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.plugins.FormatBitmap;
import com.rho.rhoelements.plugins.SignaturePlugin;

/**
 * @author Ben Kennedy (NCVT73)
 * A visual component that allows the user to draw signatures into the component. Visual settings
 * can be changed such as visibility and colors.
 */
public class SignatureArea extends View implements OnTouchListener
{
	private static final int	DEFAULT_BACKGROUND_COLOR	= 0xFF000000;
	private static final int	DEFAULT_PEN_COLOR			= 0xFFFFFFFF;
	private int 	penWidth;
	private int 	penColor;
	private boolean	borderVisible;
	private int		backgroundColor;
	private Paint	borderPaint;
	private Paint	penPaint;
	private Path	currentPath;
	private ArrayList<Path> pathList;
	private SignaturePlugin	vectorListenee;
	private ArrayList<Float>	vectors;
	private float	lastX;
	private float	lastY;
	private boolean	isOutside;
	private int		width;
	private int		height;
	private boolean isDown;
	
	public SignatureArea(Context context)
	{
		super(context);
		init();
	}
	
	public SignatureArea(Context context, AttributeSet attrs)
	{
		super(context, attrs);
		init();
	}
	
	public SignatureArea(Context context, AttributeSet attrs, int defStyle)
	{
		super(context, attrs, defStyle);
		init();
	}

	/**
	 * Initialises the PaintBrushes
	 */
	private void init()
	{
//		testBitmaps();
		setOnTouchListener(this);

		borderPaint = new Paint();
		borderPaint.setStyle(Style.STROKE);
		borderPaint.setStrokeCap(Cap.SQUARE);
		borderPaint.setStrokeWidth(2);
		
		penPaint = new Paint();
		penPaint.setStyle(Style.STROKE);
		penPaint.setStrokeCap(Cap.ROUND);
		penPaint.setAntiAlias(false);
		
		setBackgroundColor(DEFAULT_BACKGROUND_COLOR);
		setPenColor(DEFAULT_PEN_COLOR);
		
		pathList = new ArrayList<Path>();
		currentPath = null;
		isDown = false;
	}
	
	/**
	 * Changes the background color of the signature area
	 * @param color the color as an 0xAARRGGBB value
	 */
	public void setBackground(int color)
	{
		if(this.backgroundColor != color)
		{
			this.backgroundColor = color;
			this.setBackgroundColor(color);
		}
	}
	
	/**
	 * Sets the visibility of the border around the signature capture area.
	 * @param visible whether the border should be visible.
	 */
	public void setBorder(boolean visible)
	{
		if(this.borderVisible != visible)
		{
			this.borderVisible = visible;
			invalidate();
		}
	}
	
	/**
	 * Sets the width of the pen ink used to draw the signature.
	 * @param penWidth the width of the pen in pixels.
	 */
	public void setPenWidth(int penWidth)
	{
		if(this.penWidth != penWidth)
		{
			this.penWidth = penWidth;
			penPaint.setStrokeWidth(penWidth);
		}
	}
	
	/**
	 * Changes the color of the ink to draw the signature. 
	 * @param color the color as an 0xAARRGGBB value
	 */
	public void setPenColor(int color)
	{
		if(this.penColor != color)
		{
			this.penColor = color;
			penPaint.setColor(color);
		}
	}
	
	/**
	 * Clears any signature paths that have been inputted in the signature box and removes the lines
	 * from the signature box.
	 */
	public void clear()
	{
		pathList.clear();
		currentPath = new Path();
		invalidate();
	}
	
	/**
	 * Prints the image of the signature area as a bitmap.
	 * @return the representation of the signature area as a bitmap.
	 */
	public Bitmap getBitmap()
	{
		Bitmap res = null;
		//Paint it on a fresh canvas to eliminate the color drift problem.
		if ( (width>0) && (height>0) )
		{
			Bitmap screenshot = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
			screenshot.eraseColor(backgroundColor);//Clears the bitmap
			Canvas canvas = new Canvas(screenshot);
	//		Paint paint = new Paint();
	//		paint.setColor(penColor);
	//		paint.setStyle(Style.STROKE);
	//		paint.setStrokeWidth(penWidth);
	//		paint.setStrokeCap(Cap.ROUND);
			for(Path path: pathList)//Draw the paths
			{
				canvas.drawPath(path, penPaint);
			}
		    res = screenshot;
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "bitmap= " + res));
		return res;
	}
	
	@Override
	public void onDraw(Canvas canvas)
	{
		for(Path path: pathList)
		{
			canvas.drawPath(path, penPaint);
		}
		if(currentPath != null)
		{
			canvas.drawPath(currentPath, penPaint);
		}
		if(borderVisible)
		{
			canvas.drawRect(1, 1, this.getWidth() - 1, this.getHeight() - 1, borderPaint);
		}
	}

	/**
	 * Sets the visibility of the view.
	 * @param isVisible whether the view is visible.
	 */
	public void setVisible(boolean isVisible)
	{
		if(isVisible)
		{
			setVisibility(View.VISIBLE);
			setClickable(true);
		}
		else
		{
			setVisibility(View.INVISIBLE);
			setClickable(false);
		}
	}

	@Override
	public boolean onTouch(View v, MotionEvent event)
	{
		float x = event.getX();
		float y = event.getY();
		if(x == 0.0 && y == 0.0)
		{
			return false;
		}
		int eventAction = event.getAction();
		
		if(eventAction == MotionEvent.ACTION_DOWN)
		{
			isOutside = false;
			isDown = true;

			currentPath = new Path();
			currentPath.moveTo(x, y);
			vectors = new ArrayList<Float>();
			vectors.add(x);
			vectors.add(y);
			lastX = x;
			lastY = y;
			return true;
		}
		else if (eventAction == MotionEvent.ACTION_UP)
		{
			if(!isOutside)
			{
				if(isOutside(x,y))//Line is moving outside. Find where the intersection between the line and the border is.
				{
					float[] entryPoints = getEntry(lastX, lastY, x, y);
					MotionEvent exit = MotionEvent.obtain(1, 1, MotionEvent.ACTION_UP, entryPoints[0], entryPoints[1], 0);
					onTouch(v, exit);
					isOutside = true;
					return true;
				}
				
				//Normal line, just draw it.
				currentPath.lineTo(x, y);
				pathList.add(currentPath);
				//and add to the vector list.
				vectors.add(x);
				vectors.add(y);
				
				if(vectorListenee != null) //If there is someone listening, send the vector to them
				{
					vectorListenee.sendVector(vectors);
				}
				invalidate((int) Math.min(lastX, x),(int) Math.min(lastY, y),(int)Math.max(lastX, x),(int)Math.max(lastY, y));
				isDown = false;
				return true;
			}
			isOutside = false;
			return false; //Ignore "Up"s that are thrown from outside. We have already done an up.
		}
		else if(eventAction == MotionEvent.ACTION_MOVE)
		{
			if(isOutside)
			{
				if(!isOutside(x,y))//Line is moving from outside to inside, find the intersection with the border and start drawing from there.
				{
					isOutside = false;
					float[] entryPoints = getEntry(lastX, lastY, x, y);
					MotionEvent entry = MotionEvent.obtain(1, 1, MotionEvent.ACTION_DOWN, entryPoints[0], entryPoints[1], 0);
					MotionEvent move = MotionEvent.obtain(1, 1, MotionEvent.ACTION_MOVE, x, y, 0);
					onTouch(v, entry);
					onTouch(v, move);
					return true;
				}
				lastX = x;
				lastY = y;
				return false;
			}
			//else
			if(isOutside(x,y))//Line is moving from inside to outside. Find where the intersection between the line and the border is.
			{
				float[] entryPoints = getEntry(x, y, lastX, lastY);
				MotionEvent exit = MotionEvent.obtain(1, 1, MotionEvent.ACTION_UP, entryPoints[0], entryPoints[1], 0);
				onTouch(v, exit);
				isOutside = true;
				return true;
			}
			if(!isDown)
			{
				event.setAction(MotionEvent.ACTION_DOWN);
				onTouch(v, event);
				return true;
			}
			currentPath.lineTo(x, y);
			vectors.add(x);
			vectors.add(y);
			invalidate((int) Math.min(lastX, x) - penWidth,(int) Math.min(lastY, y) - penWidth,(int)Math.max(lastX, x) + penWidth,(int)Math.max(lastY, y) + penWidth); // Optimisation
			lastX = x;
			lastY = y;

			return true;
		}
		return false;
	}

	/**
	 * Gets the coordinates of where the specified line crosses a border of the signature box.
	 * @param outsideX the start x coordinate of the line segment
	 * @param outsideY the start y coordinate of the line segment
	 * @param insideX the end x coordinate of the line segment
	 * @param insideY the end y coordinate of the line segment
	 * @return a 2 length float array constituting the x [0] and y [1] coordinate of the collision. 
	 */
	private float[] getEntry(float outsideX, float outsideY, float insideX, float insideY)
	{
		double slope = 0;
		try
		{
			slope = (double) (outsideY - insideY) / (double) (outsideX - insideX);
		}
		catch(ArithmeticException e)
		{
			slope = Float.POSITIVE_INFINITY; //When its vertical (in theory this wont be thrown cos float division can do Infinities)
		}

		float[] returnValues = new float[2];
		if(outsideX < 0 && insideX >= 0) //crosses left
		{
			double c = outsideY - (slope * outsideX);
			returnValues[0] = 0;
			
			//Was occasionally returning slightly out of range numbers which caused errors. Due to floating point inaccuracies
			float temp = (float) c;
			if(temp < 0) temp = 0;
			else if(temp > height - 1) temp = height -1;
			
			returnValues[1] = temp;
		}
		else if(outsideX > width - 1 && insideX <= width)//crosses right
		{
			double c = outsideY - (slope * outsideX);
			returnValues[0] = width - 1;
			
			//Was occasionally returning slightly out of range numbers which caused errors. Due to floating point inaccuracies
			float temp = (float) ((slope * returnValues[0]) + c); 
			if(temp < 0) temp = 0;
			else if(temp > height - 1) temp = height -1; 
			
			returnValues[1] = temp; 
		}
		else if(outsideY < 0 && insideY >= 0) //crosses top
		{
			if(slope == Float.POSITIVE_INFINITY || slope == Float.NEGATIVE_INFINITY )
			{
				returnValues[0] = outsideX;
			}
			else
			{
				double c = outsideY - (slope * outsideX);
				returnValues[0] = (float) ((c * -1) / slope);
			}
			returnValues[1] = 0;
		}
		else if(outsideY > height -1 && insideY <= height)//crosses bottom
		{
			if(slope == Float.POSITIVE_INFINITY || slope == Float.NEGATIVE_INFINITY )
			{
				returnValues[0] = outsideX;
			}
			else
			{
				double  c = outsideY - (slope * outsideX);
				returnValues[0] = (float) ((double) ((height - 1) - c) / slope);
			}
			returnValues[1] = height - 1;
		}
		return returnValues;
	}

	/**
	 * Checks to see whether the given coordinates lie outside of the signature capture box.
	 * @param x the x coordinate to test.
	 * @param y the y coordinate to test.
	 * @return whether the coordinate lies within the signature capture box (true) or not (false).
	 */
	private boolean isOutside(float x, float y)
	{
		if(x < 0 || y < 0 || x > width - 1 || y > height -1)
		{
			return true;
		}
		return false;
	}

	/**
	 * Sets the listener for signature vector events.
	 * @param signaturePlugin the listener that wants to be notified of vector events.
	 */
	public void setVectorListenee(SignaturePlugin signaturePlugin)
	{
		this.vectorListenee = signaturePlugin;
	}

	/**
	 * A courtesy method to tell the view its new width and height.
	 * @param width the new width of this view.
	 * @param height the new height of this view.
	 */
	public void setDimensions(int width, int height)
	{
		this.width = width;
		this.height = height;
	}
	
	public static void testBitmaps()
	{
		Bitmap bitmap = Bitmap.createBitmap(100, 100, Bitmap.Config.ARGB_8888);
		int on = 0;
		for(int i = 0; i < 100; i++)
		{
			for(int j = 0; j < 100; j++)
			{
				if(on % 3 == 0)
				{
					bitmap.setPixel(i,j, Color.BLACK);
					on++;
				}
				else
				{
					bitmap.setPixel(i,j, Color.WHITE);
					on++;
				}
			}
		}
		
		File tempFile = new File(Environment.getExternalStorageDirectory() + "/temp.bmp");
		FileOutputStream tempOut;
		try
		{
			tempOut = new FileOutputStream(tempFile);
			bitmap.compress(Bitmap.CompressFormat.PNG, 90, tempOut);
			tempOut.close();
		}
		catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (IOException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		File fFile = new File(Environment.getExternalStorageDirectory() + "/temp2.bmp");
		FileOutputStream fOut;
		try
		{
			fOut = new FileOutputStream(fFile);
			FormatBitmap.writeBitmapImage(bitmap, fOut, Color.WHITE, Color.BLACK);	
			fOut.close();
		}
		catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (IOException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}