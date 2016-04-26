/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Symbol Technologies Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rho.signature;


import java.io.IOException;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.Paint.Cap;
import android.graphics.Paint.Style;
import android.graphics.PixelFormat;
import android.graphics.PointF;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.util.PerformOnUiThread;

class SignatureView extends SurfaceView implements SurfaceHolder.Callback {
	private class PointSequence {
		public Vector<PointF> mPoints = new Vector<PointF>();
	}

	public static String mStateID = "SignatureViewState";

	@SuppressWarnings("serial")
	public class SignatureViewState extends Object implements Serializable {
		public Vector<PointSequence> mSequences;

		private void writeObject(java.io.ObjectOutputStream out) throws IOException {
	        int s;
	        int p;
	        out.writeInt(mSequences.size());
	        for (s = 0; s < mSequences.size(); s++) {
	        	PointSequence ps = mSequences.elementAt(s);
	        	out.writeInt(ps.mPoints.size());
	        	for (p = 0; p < ps.mPoints.size(); p++) {
	        		PointF cur = ps.mPoints.elementAt(p);
	        		out.writeFloat(cur.x);
	        		out.writeFloat(cur.y);
	        	}
	        }
		}
		
		private void readObject(java.io.ObjectInputStream in) throws IOException, ClassNotFoundException {
			int seq_count = in.readInt();
	        int s;
	        int p;
	        mSequences = new Vector<PointSequence>();
	        for (s = 0; s < seq_count; s++) {
	        	int point_count = in.readInt();
	        	PointSequence ps = new PointSequence();
	        	for (p = 0; p < point_count; p++) {
	        		PointF cur = new PointF();
	        		cur.x = in.readFloat();
	        		cur.y = in.readFloat();
	        		ps.mPoints.add(cur);
	        	}
		        mSequences.add(ps);
	        }
		}
	}
	
	private int penColor;
	private float penWidth;
	private int bgColor;
	private Paint	borderPaint;
	private Vector<PointSequence> mSequences = new Vector<PointSequence>();
	private PointSequence mCurrentSequence = new PointSequence();
	private static final String TAG = "SignatureView";
	
    Bitmap mBitmap;
    Canvas mCanvas;
    
    private Paint mPaint;
	private boolean isBorderVisible;
	private Integer singletonId;
    
  
	/**
	 * Clears the signature capture area
	 */
    public void doClear() {
    	// clear paths
    	Logger.D(TAG, "Clearing signatureCapture");
    	mSequences.clear();
    	doFullRedraw(mCanvas, mBitmap, true, true);
    }
	     
	public void onRestoreInstanceState(Bundle savedInstanceState) {
		SignatureViewState s = (SignatureViewState)savedInstanceState.getSerializable(mStateID);
		mSequences = s.mSequences;
	}

	public void onSaveInstanceState(Bundle outState) {
       	mSequences.add(mCurrentSequence);
    	mCurrentSequence = new PointSequence();
		SignatureViewState s = new SignatureViewState();
		s.mSequences = mSequences;
		outState.putSerializable(mStateID, s);
	}
	
	private void doFullRedraw(Canvas canvas, Bitmap bitmap, boolean useAlpha, boolean redrawOnScreen) {
		if (canvas == null) {
			return;
		}
		
        Paint paint = new Paint();
        paint.setAntiAlias(false);

    	bitmap.eraseColor(bgColor | 0xFF000000); //Clear the canvas first
	    if (useAlpha) {
	    	bitmap.eraseColor(bgColor); // and replace with a transparent background if needed
	    }

        int s;
        int p;
        for (s = 0; s < mSequences.size(); s++) {
        	PointSequence ps = mSequences.elementAt(s);
        	if ((ps != null) && (ps.mPoints != null)) {
	        	for (p = 1; p < ps.mPoints.size(); p++) {
	        		PointF prev = ps.mPoints.elementAt(p-1);
	        		PointF cur = ps.mPoints.elementAt(p);
	        		if ((prev != null) && (cur != null)) {
	        			canvas.drawLine(prev.x, prev.y, cur.x, cur.y, mPaint);
	        		}
	        	}
        	}
        	else {
        		if (ps == null) {
        			//Logger.D(TAG, "##################  ps == null !");
        		}
        		else {
	        		if (ps.mPoints == null) {
	        			//Logger.D(TAG, "##################  ps.mPoints == null !");
	        		}
        		}
        	}
        }
        {   
        	PointSequence ps = mCurrentSequence;
        	if ((ps != null) && (ps.mPoints != null)) {
	        	for (p = 1; p < ps.mPoints.size(); p++) {
	        		PointF prev = ps.mPoints.elementAt(p-1);
	        		PointF cur = ps.mPoints.elementAt(p);
	        		if ((prev != null) && (cur != null)) {
	        			canvas.drawLine(prev.x, prev.y, cur.x, cur.y, mPaint);
	        		}
	        	}
        	}
        }
        // update screen
       	if (redrawOnScreen) {
        	invalidate();
       	}
	}
    
	private void doDrawSegment(float x1, float y1, float x2, float y2) {
	     Canvas c = null;
	     Rect rect = new Rect((int)(x1-1),(int)(y1-1),(int)(x1+1),(int)(y1+1));
	     rect.union((int)(x2-1),(int)(y2-1),(int)(x2+1),(int)(y2+1));
	     ///*
	     try {
	      
	         c = getHolder().lockCanvas(rect);
	         synchronized (getHolder()) {
	        	 if ((mCanvas != null) && (mBitmap != null)) {
		        	 mCanvas.drawLine(x1,y1,x2,y2,mPaint);
	        	 }
	         }
	     } finally {
	         if (c != null) {
	        	 getHolder().unlockCanvasAndPost(c);
	         }
	     }
	     //*/    	 
		invalidate(rect);
	}
    
	public void setupView(int _penColor, float _penWidth, int _bgColor, boolean isBorder) {
		penColor = _penColor;
		penWidth = _penWidth;
		bgColor = _bgColor;
		this.isBorderVisible = isBorder;
        mPaint = new Paint();
        mPaint.setAntiAlias(true);
        mPaint.setColor(Color.argb(	(penColor & 0xFF000000) >> 24,
					        		(penColor & 0xFF0000) >> 16,
					        		(penColor & 0xFF00) >> 8,
					        		penColor & 0xFF));
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeJoin(Paint.Join.ROUND);
        mPaint.setStrokeCap(Paint.Cap.ROUND);
        mPaint.setStrokeWidth(penWidth);
        
        Logger.D(TAG, "$$$   setupView()");
        Logger.D(TAG, "$$$        penColor = "+String.valueOf(penColor));
        Logger.D(TAG, "$$$        bgColor = "+String.valueOf(bgColor));
        Logger.D(TAG, "$$$        border = "+String.valueOf(isBorderVisible));
        
    	setDrawingCacheEnabled(false);
    	setZOrderOnTop(true);
    	getHolder().setFormat(PixelFormat.TRANSLUCENT);
    	setBackgroundColor(0);
        doFullRedraw(mCanvas, mBitmap, true, true);    
        
        PerformOnUiThread.exec(new Runnable() {
        	public void run() {
        		doDrawSegment(-10,-10,-11,-11);
        	}
        },10);
	}
    
    public SignatureView(Context context, AttributeSet attrs) {
        super(context, attrs);
		Logger.D(TAG, "SignatureView()");
		//this.signatureSingleton = singleton;

        // register our interest in hearing about changes to our surface
        SurfaceHolder holder = getHolder();
        holder.addCallback(this);

        setVisibility(VISIBLE);
        
        mPaint = new Paint();
        mPaint.setAntiAlias(true);
        mPaint.setColor(0xFF66009A);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeJoin(Paint.Join.ROUND);
        mPaint.setStrokeCap(Paint.Cap.ROUND);
        mPaint.setStrokeWidth(2);

		borderPaint = new Paint();
		borderPaint.setStyle(Style.STROKE);
		borderPaint.setStrokeCap(Cap.SQUARE);
		borderPaint.setStrokeWidth(2);

        bgColor = 0xFFFFFFFF;
        
        //setBackgroundColor(0);
        //setZOrderOnTop(true);
        //getHolder().setFormat(PixelFormat.TRANSLUCENT);
        
        setFocusable(true); // make sure we get key events
        
        requestFocus();
        bringToFront();
    }
	
    public Bitmap makeBitmap() {
        Bitmap b = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
        b.eraseColor(bgColor | 0xFF000000);
        Canvas c = new Canvas(b);
        doFullRedraw(c, b, false, false);
        return b;
    }
    
    
    public boolean onTouchEvent(MotionEvent event) {
    	 float x = event.getX();
    	 float y = event.getY();
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                	mCurrentSequence = new PointSequence();
                	mCurrentSequence.mPoints.add(new PointF(x,y));
                	break;
                case MotionEvent.ACTION_MOVE:
                	//doDrawSegment(mX,mY, x,y);
                	mCurrentSequence.mPoints.add(new PointF(x,y));
                	doFullRedraw(mCanvas, mBitmap, true, true);
                	invalidate();
                    break;
                case MotionEvent.ACTION_UP:
                	//doDrawSegment(mX,mY, x,y);
                	//This point is NOT required to add as it already came in ACTION_MOVE event
                //	mCurrentSequence.mPoints.add(new PointF(x,y));
                	mSequences.add(mCurrentSequence);
                	sendVectors(mCurrentSequence);
                	mCurrentSequence = new PointSequence();
                	doFullRedraw(mCanvas, mBitmap, true, true);
                    break;
            }
    	 
    	 return true;
     }
    
    /**
     * Converts a point sequence to an array of String formatted vector points, terminated with 2 0xFFFF
     * @param pointSequence the point sequence to convert to vectors
     * @author Ben Kennedy (NCVT73)
     */
    private void sendVectors(PointSequence pointSequence)
	{
    	int totalRawVectors = (pointSequence.mPoints.size() * 2);
    	int outputSize = (totalRawVectors > 1000 ? 1000 : totalRawVectors);
    	List<Object> outputData = new ArrayList<Object>(outputSize + 2); //+2 for the 0xFFFFs
		for(PointF points: pointSequence.mPoints)
		{
			if(outputData.size() >= 1000) //Dont let it get too big, send in batches
			{
				outputData.add(Integer.valueOf(0xFFFF));
				outputData.add(Integer.valueOf(0xFFFF));
				SignatureSingleton.sendVectors(outputData, singletonId);
				outputData.clear(); 
			}
			outputData.add(Integer.valueOf(Math.round(points.x)));
			outputData.add(Integer.valueOf(Math.round(points.y)));
		}
		outputData.add(Integer.valueOf(0xFFFF));
		outputData.add(Integer.valueOf(0xFFFF));
		SignatureSingleton.sendVectors(outputData, singletonId);
	}
    
    /**
     * Sets the id (hash) of the signature that is associated (that created) this SignatureView
     * @param singletonId the ID of the SignatureSingleton
     * @author Ben Kennedy (NCVT73)
     */
    public void setSingletonId(Integer singletonId)
    {
    	this.singletonId = singletonId;
    }

    @Override
    protected void onDraw (Canvas canvas) {
    	if (mBitmap != null) {
    		canvas.drawBitmap(mBitmap, 0, 0, null);
    		if(isBorderVisible) canvas.drawRect(1, 1, this.getWidth() - 1, this.getHeight() - 1, borderPaint);
    	}
    }
    
    /* Callback invoked when the surface dimensions change. */
    public void surfaceChanged(SurfaceHolder holder, int format, int width,
            int height) {
 		Logger.D(TAG, "SurfaceChanged()");
        mBitmap = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
        mBitmap.eraseColor(bgColor);
        mCanvas = new Canvas(mBitmap);
        doFullRedraw(mCanvas, mBitmap, true, true);
        invalidate();
    }
    
    
    /*
     * Callback invoked when the Surface has been created and is ready to be
     * used.
     */
    public void surfaceCreated(SurfaceHolder holder) {
 		Logger.D(TAG, "SurfaceCreated()");
    }

     public void surfaceDestroyed(SurfaceHolder holder) {
 		Logger.D(TAG, "SurfaceDestroyed()");
 		mBitmap = null;
 		mCanvas = null;
     }

     /**
      * Toggles the border on the signature area
      * @param border show the border (true) or hide the border (false)
      * @author Ben Kennedy (NCVT73)
      */
	public void setBorder(boolean border)
	{
		if(border != isBorderVisible)
		{
			this.isBorderVisible = border;
			invalidate();
		}
	}
}
