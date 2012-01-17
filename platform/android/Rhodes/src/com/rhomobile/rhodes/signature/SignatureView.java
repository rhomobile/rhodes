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

package com.rhomobile.rhodes.signature;


import java.util.Vector;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.PointF;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.MotionEvent;
import java.io.Serializable;
import java.io.IOException;

import com.rhomobile.rhodes.Logger;

class SignatureView extends SurfaceView implements SurfaceHolder.Callback {

	public int penColor;
	public float penWidth;
	public int bgColor;
	
	
	
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
	
	
	private Vector<PointSequence> mSequences = new Vector<PointSequence>();
	private PointSequence mCurrentSequence = new PointSequence();
	
	private static final String TAG = "SignatureView";
	
    Bitmap mBitmap;
    Canvas mCanvas;
    
    private float mX;
    private float mY;
    
    //private Path mCurrentPath;
    //private Path mGlobalPath;
    
    private Paint mPaint;
    
   
    public void doClear() {
    	// clear paths
    	mSequences.clear();
    	doFullRedraw();
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
	
	private void doFullRedraw() {
		if (mCanvas == null) {
			return;
		}
		
        Paint paint = new Paint();
        paint.setAntiAlias(false);
        paint.setARGB(	255, 
        				(bgColor & 0xFF0000) >> 16, 
        				(bgColor & 0xFF00) >> 8,
        				(bgColor & 0xFF));
   	 
        mCanvas.drawRect(0, 0, mCanvas.getWidth(), mCanvas.getHeight(), paint);
        
        // draw path
        int s;
        int p;
        for (s = 0; s < mSequences.size(); s++) {
        	PointSequence ps = mSequences.elementAt(s);
        	if ((ps != null) && (ps.mPoints != null)) {
	        	for (p = 1; p < ps.mPoints.size(); p++) {
	        		PointF prev = ps.mPoints.elementAt(p-1);
	        		PointF cur = ps.mPoints.elementAt(p);
	        		if ((prev != null) && (cur != null)) {
	        			mCanvas.drawLine(prev.x, prev.y, cur.x, cur.y, mPaint);
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
        
        // update screen
	     Canvas c = null;
	     try {
	         c = getHolder().lockCanvas(null);
	         synchronized (getHolder()) {
	             c.drawBitmap(mBitmap, 0, 0, null);
	         }
	     } finally {
	         if (c != null) {
	        	 getHolder().unlockCanvasAndPost(c);
	         }
	     }    	 
	}
    
	private void doDrawSegment(float x1, float y1, float x2, float y2) {
	     Canvas c = null;
	     Rect rect = new Rect((int)(x1-1),(int)(y1-1),(int)(x1+1),(int)(y1+1));
	     rect.union((int)(x2-1),(int)(y2-1),(int)(x2+1),(int)(y2+1));
	     try {
	         c = getHolder().lockCanvas(rect);
	         synchronized (getHolder()) {
	             mCanvas.drawLine(x1,y1,x2,y2,mPaint);
	             c.drawBitmap(mBitmap, rect, rect, null);
	         }
	     } finally {
	         if (c != null) {
	        	 getHolder().unlockCanvasAndPost(c);
	         }
	     }    	 
		
	}
    
	
	public void setupView(int _penColor, float _penWidth, int _bgColor) {
		penColor = _penColor;
		penWidth = _penWidth;
		bgColor = _bgColor;
        mPaint = new Paint();
        mPaint.setAntiAlias(true);
        mPaint.setColor(0xFF000000 | penColor);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeJoin(Paint.Join.ROUND);
        mPaint.setStrokeCap(Paint.Cap.ROUND);
        mPaint.setStrokeWidth(penWidth);
        doFullRedraw();
	}
    
    public SignatureView(Context context, AttributeSet attrs) {
        super(context, attrs);
		Logger.D(TAG, "SignatureView()");

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

        bgColor = 0xFFFFFFFF;
        
        setFocusable(true); // make sure we get key events
        
        requestFocus();
        bringToFront();
        
        
    }
	
    public Bitmap makeBitmap() {
    	return mBitmap;
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
                	doDrawSegment(mX,mY, x,y);
                	mCurrentSequence.mPoints.add(new PointF(x,y));
                    break;
                case MotionEvent.ACTION_UP:
                	doDrawSegment(mX,mY, x,y);
                	mCurrentSequence.mPoints.add(new PointF(x,y));
                	mSequences.add(mCurrentSequence);
                	mCurrentSequence = new PointSequence();
                    break;
            }
    	 
    	 mX = x;
    	 mY = y;
    	 return true;
     }
     
    
    /* Callback invoked when the surface dimensions change. */
    public void surfaceChanged(SurfaceHolder holder, int format, int width,
            int height) {
 		Logger.D(TAG, "SurfaceChanged()");
        mBitmap = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.RGB_565);
        mCanvas = new Canvas(mBitmap);
        doFullRedraw();
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
	
    
    
}




