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

package com.rhomobile.rhodes.mapview;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.RhodesService;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;

public class Callout {
	
	public static final int TITLE_FONT_SIZE = 28;
	public static final int DETAILS_FONT_SIZE = 18;
	
	public static final int TITLE_COLOR = 0xFFFFFFFF;
	public static final int DETAILS_COLOR = 0xFFFFFFFF;
	
	public static final int TITLE_LEFT_OFFSET = 10;
	public static final int TITLE_TOP_OFFSET = 10;
	public static final int TITLE_RIGHT_OFFSET = 10;

	public static final int DETAILS_LEFT_OFFSET = 10;
	public static final int DETAILS_TOP_OFFSET = 5;
	public static final int DETAILS_RIGHT_OFFSET = 5;
	public static final int DETAILS_BOTTOM_OFFSET = 5;
	
	public static final int LINK_LEFT_OFFSET = 5;
	public static final int LINK_TOP_OFFSET = 18;
	
	public static final int ANCHOR_C1_X = 19;
	public static final int ANCHOR_C1_Y = 11;
	

	public Callout(double latitude, double longitude, String title, String details, String url, Context context) {
		loadResources(context);
		rebuild(latitude, longitude, title, details, url);
	}
	
	public static Callout makeCallout(String title, String details, String url) {
		return new Callout(0,0, title, details, url, RhodesService.getContext());
	}
	
	public void rebuild(double latitude, double longitude, String title, String details, String url) {
		mLatitude = latitude;
		mLongitude = longitude;
		mTitle = title;
		mDetails = details;
		mUrl = url;
		
		if (mUrl != null) {
			if (mUrl.length() < 2) {
				mUrl = null;
			}
		}
		
		Paint paint = new Paint();
		paint.setAntiAlias(true);


		
		int title_width = 0;
		int title_height = 0;
		// calc Title size (only W)
		paint.setColor(TITLE_COLOR);
		paint.setTextSize(TITLE_FONT_SIZE);
	    Rect rcLine = new Rect();
		paint.getTextBounds( mTitle, 0, mTitle.length()-1, rcLine );
		title_width = rcLine.right;
		paint.getTextBounds( "WWW", 0, 2, rcLine );
		title_height = rcLine.bottom - rcLine.top;
		int max_text_width = title_width;
		
		
		int details_width = 0;
		int details_height = 0;
		int details_max_line_height = 0;
		// calc Details size (W,H)
		String [] lines = details.split("\n");
		int i;
		paint.setColor(DETAILS_COLOR);
		paint.setTextSize(DETAILS_FONT_SIZE);
		for (i = 0; i < lines.length; i++) {
		    Rect rcLine2 = new Rect();
			paint.getTextBounds( lines[i], 0, lines[i].length()-1, rcLine2 );
			if (details_width < rcLine2.right) {
				details_width = rcLine2.right;
			}
			int h = rcLine2.bottom - rcLine2.top;
			if (details_max_line_height < h) {
				details_max_line_height = h;
			}
		}
		if (max_text_width < details_width) {
			max_text_width = details_width;
		}

		paint.getTextBounds( "WWW", 0, 2, rcLine );
		int details_line_y_offset = rcLine.bottom - rcLine.top;
		
		if (details_max_line_height < details_line_y_offset) {
			details_max_line_height = details_line_y_offset;
		}
		
		details_max_line_height = (details_max_line_height*12)/10 + 2;
		details_height = details_max_line_height*lines.length;
		
		// calc global size
		
		int HC = 0; // insertion count in horizontal
		int VC = 0; // insertion count in vertical
		
		int need_hor_size = max_text_width - (mA1.getWidth() - TITLE_LEFT_OFFSET - TITLE_RIGHT_OFFSET);
		HC = (need_hor_size + (mA2.getWidth() - 1))/(mA2.getWidth());
		if (HC < 1) {
			HC = 1;
		}
		
		int need_ver_size = details_height + DETAILS_TOP_OFFSET + DETAILS_BOTTOM_OFFSET;
		VC = (need_ver_size + (mB2.getHeight() - 1))/(mB2.getHeight());
		if (VC < 2) {
			VC = 2;
		}
		
		int global_width = mA1.getWidth() + mA2.getWidth()*HC + mA3.getWidth();
		int global_height = mA1.getHeight() + mB1.getHeight()*VC + mC1.getHeight();
		
		mResultBitmap = Bitmap.createBitmap(global_width, global_height, Bitmap.Config.ARGB_8888);
		mResultBitmap.eraseColor(0);
		
		// prepare BKG
		
		int cur_x = 0;
		int cur_y = 0;
		
		Canvas canvas = new Canvas(mResultBitmap);
		
		canvas.drawBitmap(mA1, cur_x, cur_y, paint);
		cur_x += mA1.getWidth();
		
		for (i = 0; i < HC; i++) {
			canvas.drawBitmap(mA2, cur_x, cur_y, paint);
			cur_x += mA2.getWidth();
		}
		canvas.drawBitmap(mA3, cur_x, cur_y, paint);
		cur_y += mA1.getHeight();
		
		int j;
		
		for (i = 0; i < VC; i++) {
			cur_x = 0;
			
			canvas.drawBitmap(mB1, cur_x, cur_y, paint);
			cur_x += mB1.getWidth();
			
			for (j = 0; j < HC; j++) {
				canvas.drawBitmap(mB2, cur_x, cur_y, paint);
				cur_x += mB2.getWidth();
			}
		
			canvas.drawBitmap(mB3, cur_x, cur_y, paint);
			cur_y += mB1.getHeight();
		}

		cur_x = 0;
		canvas.drawBitmap(mC1, cur_x, cur_y, paint);
		cur_x += mC1.getWidth();
		
		for (i = 0; i < HC; i++) {
			canvas.drawBitmap(mC2, cur_x, cur_y, paint);
			cur_x += mC2.getWidth();
		}
		canvas.drawBitmap(mC3, cur_x, cur_y, paint);
		
		
		// draw text
		
		paint.setColor(TITLE_COLOR);
		paint.setTextSize(TITLE_FONT_SIZE);
		canvas.drawText(mTitle, TITLE_LEFT_OFFSET, TITLE_TOP_OFFSET + title_height, paint);
		
		paint.setColor(DETAILS_COLOR);
		paint.setTextSize(DETAILS_FONT_SIZE);
		cur_y = mA1.getHeight() + DETAILS_TOP_OFFSET;
		for (i = 0; i < lines.length; i++) {
			canvas.drawText(lines[i], DETAILS_LEFT_OFFSET, cur_y + details_line_y_offset, paint);
			cur_y += details_max_line_height;
		}
		
		// draw link btn
		if (mUrl != null) {
			canvas.drawBitmap(	mD, mA1.getWidth() + mA2.getWidth()*HC + LINK_LEFT_OFFSET, mA1.getHeight() - mD.getHeight()/2, paint);
		}

		
		// result bitmap is ready !
		
		mXOffset = -(mC1.getWidth()/2);
		mYOffset = -(mA1.getHeight() + mB1.getHeight()*VC + mC1.getHeight());
	}

	// x,y - ancor point. modify for offset insode this function
	public void drawAt(Canvas canvas, int x, int y) {
		Paint paint = new Paint();
		canvas.drawBitmap(mResultBitmap, x + mXOffset, y + mYOffset, paint);
	}
	
	public Bitmap getResultBitmap() {
		return mResultBitmap;
	}
	
	// return modifier for Y coordinates from ancor point.
	public int getXOffset() {
		return mXOffset;
	}
	
	// return modifier for X coordinates from ancor point.
	public int getYOffset() {
		return mYOffset;
	}
	
	public boolean checkForLinkArea(int ancor_x, int ancor_y, int x, int y) {
		if (mUrl == null) {
			return false;
		}
		return false;
	}
	
	
	
	private void loadResources(Context context) {
		mA1 = BitmapFactory.decodeResource(context.getResources(), AndroidR.drawable.callout_a1);
		mA2 = BitmapFactory.decodeResource(context.getResources(), AndroidR.drawable.callout_a2);
		mA3 = BitmapFactory.decodeResource(context.getResources(), AndroidR.drawable.callout_a3);
		mB1 = BitmapFactory.decodeResource(context.getResources(), AndroidR.drawable.callout_b1);
		mB2 = BitmapFactory.decodeResource(context.getResources(), AndroidR.drawable.callout_b2);
		mB3 = BitmapFactory.decodeResource(context.getResources(), AndroidR.drawable.callout_b3);
		mC1 = BitmapFactory.decodeResource(context.getResources(), AndroidR.drawable.callout_c1);
		mC2 = BitmapFactory.decodeResource(context.getResources(), AndroidR.drawable.callout_c2);
		mC3 = BitmapFactory.decodeResource(context.getResources(), AndroidR.drawable.callout_c3);
		mD = BitmapFactory.decodeResource(context.getResources(), AndroidR.drawable.callout_d);
	}
	
	
	private double mLatitude = 0;
	private double mLongitude = 0;
	private String mTitle = null;
	private String mDetails = null;
	private String mUrl = null;

	private Bitmap mA1 = null;
	private Bitmap mA2 = null;
	private Bitmap mA3 = null;
	private Bitmap mB1 = null;
	private Bitmap mB2 = null;
	private Bitmap mB3 = null;
	private Bitmap mC1 = null;
	private Bitmap mC2 = null;
	private Bitmap mC3 = null;
	private Bitmap mD = null;
	
	private Bitmap mResultBitmap = null;
	
	private int mXOffset = 0;
	private int mYOffset = 0;
	
	
}
