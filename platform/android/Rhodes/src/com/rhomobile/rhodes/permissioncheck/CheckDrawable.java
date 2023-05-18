package com.rhomobile.rhodes.permissioncheck;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ColorFilter;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;

public class CheckDrawable extends Drawable {
    private Paint mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private Path mPath = new Path();

    @Override
    public void draw(Canvas canvas) {
        mPaint.setColor(Color.parseColor("#40FF00"));
        canvas.drawPath(mPath,mPaint);
    }

    @Override
    public int getOpacity() {
        return PixelFormat.TRANSLUCENT;
    }

    @Override
    public void setAlpha(int alpha) {
        mPaint.setAlpha(alpha);
    }

    @Override
    public void setColorFilter(ColorFilter colorFilter) {
        mPaint.setColorFilter(colorFilter);
    }

    @Override
    protected void onBoundsChange(Rect bounds) {
        super.onBoundsChange(bounds);

        int width = bounds.width();
        int height = bounds.height();

        mPath.reset();
        mPath.moveTo(0.375f         * width, 0.67375f       * height);
        mPath.lineTo(0.20125f       * width, 0.5f           * height);
        mPath.lineTo(0.142083333f   * width, 0.55875f       * height);
        mPath.lineTo(0.375f         * width, 0.791666667f   * height);
        mPath.lineTo(0.875f         * width, 0.291666667f   * height);
        mPath.lineTo(0.81625f       * width, 0.232916667f   * height);
        mPath.close();
    }
}
