/*--------------------------------------
SPR28083- SIP overlaps the input boxes which are at the bottom of the screen when EB is in full screen
When setting EB to run in full screen and you have input boxes at the bottom of the screen, 
these are overlapped by SIP so you don't see what are you typing.
Fix- After clicking on the input boxes which are at the bottom of the screen when EB is in full screen It calculate  
the differnece of previous working page and current working page and found any positive Nonzero value then it shift the
rendered page with the differences.

--------------------------------------*/

package com.rhomobile.rhodes;

import android.app.Activity;
import android.graphics.Rect;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.FrameLayout;

public class KeypadShift {
	public static void assistActivity (Activity activity) {
        new KeypadShift(activity);
    }

    private View mSmallerScreenContent;
    private int usableScreenHeightPrevious;
    private FrameLayout.LayoutParams frameLayoutParams;

    private KeypadShift(Activity activity) {
        FrameLayout content = (FrameLayout) activity.findViewById(android.R.id.content);
        mSmallerScreenContent = content.getChildAt(0);
        mSmallerScreenContent.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            public void onGlobalLayout() {
                possiblyResizeChildOfContent();
            }
        });
        frameLayoutParams = (FrameLayout.LayoutParams) mSmallerScreenContent.getLayoutParams();
    }
    private void possiblyResizeChildOfContent() {
        int usableHeightWithKeypad = computeUsableHeight();
        if (usableHeightWithKeypad != usableScreenHeightPrevious) {
            int usableHeightSansKeyboard = mSmallerScreenContent.getRootView().getHeight();
            int heightDifference = usableHeightSansKeyboard - usableHeightWithKeypad;

            if (heightDifference > (usableHeightSansKeyboard/4)) {
                // keyboard probably just became visible
                frameLayoutParams.height = usableHeightSansKeyboard - heightDifference;
            } else {
                // keyboard probably just became hidden
                frameLayoutParams.height = usableHeightSansKeyboard;
            }
            mSmallerScreenContent.requestLayout();
            usableScreenHeightPrevious = usableHeightWithKeypad;
        }
    }

    private int computeUsableHeight() {
        Rect r = new Rect();
        mSmallerScreenContent.getWindowVisibleDisplayFrame(r);
        return (r.bottom - r.top);
    }
	

}
