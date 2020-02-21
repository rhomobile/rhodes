/*
 * Copyright (C) The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.google.barcodereader.ui.camera;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;

import com.google.android.gms.vision.CameraSource;

import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.Vector;
import java.lang.Math;

import android.graphics.Color;
import android.graphics.Paint;
import android.text.TextPaint;
import android.graphics.RectF;
import android.graphics.Rect;
import android.graphics.Point;
import android.text.TextUtils;


public class GraphicOverlay<T extends GraphicOverlay.Graphic> extends View {
    private final Object mLock = new Object();
    private int mPreviewWidth;
    private float mWidthScaleFactor = 1.0f;
    private int mPreviewHeight;
    private float mHeightScaleFactor = 1.0f;
    private int mFacing = CameraSource.CAMERA_FACING_BACK;
    private Set<T> mGraphics = new HashSet<T>();
    TextPaint mTextPaint = null;
    private float textHeigh = 64;

    public static abstract class Graphic {
        private GraphicOverlay mOverlay;

        public Graphic(GraphicOverlay overlay) {
            mOverlay = overlay;
        }
        public abstract void draw(Canvas canvas);

        public float scaleX(float horizontal) {
            return horizontal * mOverlay.mWidthScaleFactor;
        }

        public float scaleY(float vertical) {
            return vertical * mOverlay.mHeightScaleFactor;
        }

        public float translateX(float x) {
            if (mOverlay.mFacing == CameraSource.CAMERA_FACING_FRONT) {
                return mOverlay.getWidth() - scaleX(x);
            } else {
                return scaleX(x);
            }
        }

        public float translateY(float y) {
            return scaleY(y);
        }

        public void postInvalidate() {
            mOverlay.postInvalidate();
        }
    }

    public GraphicOverlay(Context context, AttributeSet attrs) {
        super(context, attrs);

        mRectPaint = new Paint();
        mRectPaint.setStyle(Paint.Style.FILL);
        mRectPaint.setARGB(192, 0, 0, 0);


        mTextPaint = new TextPaint(Paint.ANTI_ALIAS_FLAG);
        mTextPaint.setColor(Color.WHITE);
        mTextPaint.setStyle(Paint.Style.FILL);
        mTextPaint.setAntiAlias(true);
        mTextPaint.setTextSize(textHeigh);
        mTextPaint.setTextAlign(Paint.Align.LEFT);
        mTextPaint.setLinearText(true);
    }

    /**
     * Removes all graphics from the overlay.
     */
    public void clear() {
        synchronized (mLock) {
            mGraphics.clear();
        }
        postInvalidate();
    }

    /**
     * Adds a graphic to the overlay.
     */
    public void add(T graphic) {
        synchronized (mLock) {
            mGraphics.add(graphic);
        }
        postInvalidate();
    }

    /**
     * Removes a graphic from the overlay.
     */
    public void remove(T graphic) {
        synchronized (mLock) {
            mGraphics.remove(graphic);
        }
        postInvalidate();
    }

    public boolean contains(T graphic){
        synchronized (mLock) {
            return mGraphics.contains(graphic);
        }
    }

    /**
     * Returns a copy (as a list) of the set of all active graphics.
     * @return list of all active graphics.
     */
    public List<T> getGraphics() {
        synchronized (mLock) {
            return new Vector(mGraphics);
        }
    }

    /**
     * Returns the horizontal scale factor.
     */
    public float getWidthScaleFactor() {
        return mWidthScaleFactor;
    }

    /**
     * Returns the vertical scale factor.
     */
    public float getHeightScaleFactor() {
        return mHeightScaleFactor;
    }


    public float getPreviewWidth(){
        return mPreviewWidth;
    }

    public float getPreviewHeight(){
        return mPreviewHeight;
    }
    /**
     * Sets the camera attributes for size and facing direction, which informs how to transform
     * image coordinates later.
     */
    public void setCameraInfo(int previewWidth, int previewHeight, int facing) {
        synchronized (mLock) {
            mPreviewWidth = previewWidth;
            mPreviewHeight = previewHeight;
            mFacing = facing;
        }
        postInvalidate();
    }

    /**
     * Draws the overlay with its associated graphic objects.
     */

    Paint mRectPaint = null;
    final float BorderKoeff = 1.5f;

    public float getTopBorder(float width, float height){
        float sqareHeight = Math.min(width, height) / BorderKoeff;
        return (height - sqareHeight) / 2;
    }

    public float getBottomBorder(float width, float height){
        float sqareHeight = Math.min(width, height) / BorderKoeff;
        return (height + sqareHeight) / 2;
    }

    public float getLeftBorder(float width, float height){
        float sqareWidth = Math.min(width, height) / BorderKoeff;
        return (width - sqareWidth) / 2;
    }

    public float getRightBorder(float width, float height){
        float sqareWidth = Math.min(width, height) / BorderKoeff;
        return (width + sqareWidth) / 2;
    }

    void drawBorder(Canvas canvas){
        float width     = canvas.getWidth();
        float height    = canvas.getHeight();

        float topBorder     = getTopBorder(width, height);
        float bottomBorder  = getBottomBorder(width, height);
        float leftBorder    = getLeftBorder(width, height);
        float rightBorder   = getRightBorder(width, height);


        canvas.drawRect(new RectF(0,             0,              leftBorder,     height),    mRectPaint);
        canvas.drawRect(new RectF(rightBorder,   0,              width,          height),    mRectPaint);
        canvas.drawRect(new RectF(leftBorder,    0,              rightBorder,    topBorder), mRectPaint);
        canvas.drawRect(new RectF(leftBorder,    bottomBorder,   rightBorder,    height),    mRectPaint);
    }


    
    String barcodeResult = "";

    private boolean isVertical = true;
    public void setVertical(boolean b){
        isVertical = b;
        postInvalidate();
    }
    public void setResult(String barcodeResult){
        this.barcodeResult = barcodeResult;
        postInvalidate();
    }

    void drawResult(Canvas canvas){
        if (barcodeResult.isEmpty()) return;

        float width     = canvas.getWidth();
        float height    = canvas.getHeight();

        float bottomBorder  = getBottomBorder(width, height);
        float leftBorder    = getLeftBorder(width, height);

        textHeigh = Math.min(bottomBorder, leftBorder) / 3 ;
        mTextPaint.setTextSize(textHeigh);
        
        float measuredWidth = mTextPaint.measureText(barcodeResult);

        if(isVertical){
            float x = width * 0.1f;           
            float y = (height + bottomBorder + textHeigh) / 2f ;   

            int textWidth = (int)(width * 0.8f);
            
            if (measuredWidth < textWidth){
                x += (textWidth - measuredWidth) / 2f;
                canvas.drawText(barcodeResult, x, y, mTextPaint); 
            }else{
                CharSequence txt = TextUtils.ellipsize(barcodeResult, mTextPaint, textWidth, TextUtils.TruncateAt.END);
                canvas.drawText(txt, 0, txt.length(), x, y, mTextPaint);
            }
            
        }else{
            canvas.save(); 
 
            float x = height * 0.1f;           
            float y = (-leftBorder + textHeigh) / 2;          
            int textWidth = (int)(height*0.8f);
            

            canvas.rotate(90.0f);

            if (measuredWidth < textWidth){
                x += (textWidth - measuredWidth) / 2f;
                canvas.drawText(barcodeResult, x, y, mTextPaint); 
            }else{
                CharSequence txt = TextUtils.ellipsize(barcodeResult, mTextPaint, textWidth, TextUtils.TruncateAt.END);
                canvas.drawText(txt, 0, txt.length(), x, y, mTextPaint);
            }

            canvas.restore();
        }

    }


    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        synchronized (mLock) {
            if ((mPreviewWidth != 0) && (mPreviewHeight != 0)) {
                mWidthScaleFactor = (float) canvas.getWidth() / (float) mPreviewWidth;
                mHeightScaleFactor = (float) canvas.getHeight() / (float) mPreviewHeight;
            }

            drawBorder(canvas);  
            drawResult(canvas);          

            for (Graphic graphic : mGraphics) {
                graphic.draw(canvas);
            }
        }
    }
}
