package com.google.barcodereader;

import android.widget.TextView;
import android.content.Context;
import android.util.AttributeSet;
import android.graphics.Rect;
import android.graphics.Canvas;
import java.lang.String;
import android.text.TextPaint;

public class VerticalTextView extends TextView {

    private int _width, _height;
    private final Rect _bounds = new Rect();

    public VerticalTextView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public VerticalTextView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public VerticalTextView(Context context) {
        super(context);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        // vise versa
        _height = getMeasuredWidth();
        _width = getMeasuredHeight();
        setMeasuredDimension(_width, _height);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        canvas.save();

        canvas.translate(_width, _height);
        canvas.rotate(90);

        TextPaint paint = getPaint();
        paint.setColor(getTextColors().getDefaultColor());

        String text = text();

        paint.getTextBounds(text, 0, text.length(), _bounds);
        canvas.drawText(text, getCompoundPaddingLeft(), (_bounds.height() - _width) / 2, paint);

        canvas.restore();
    }

    private String text() {
        return super.getText().toString();
    }
}