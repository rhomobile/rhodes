/**
 * RhoVideoView - VideoView customized class to implement resize and etc. functions
 */
package com.rho.rhoelements;

import android.content.Context;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnVideoSizeChangedListener;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.View;
import android.widget.VideoView;

/**
 * @author pxb743
 *
 */
public class RhoVideoView extends VideoView implements OnVideoSizeChangedListener{

	private int mForceHeight = 0;
	private int mForceWidth = 0;	

	public RhoVideoView(Context context)
	{
		super(context);
	}

	public RhoVideoView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
	}

	public RhoVideoView(Context context, AttributeSet attrs, int defStyle)
	{
		super(context, attrs, defStyle);
	}
	
	public void setDimensions(int width, int height)
	{
		this.mForceHeight 	= height;
		this.mForceWidth 	= width;
	}

	
	/** Resizes the surfaces to fill the screen.
	 * If the media_width and media_height variables are zero, that means we
	 * don't know yet the size of the video, so we just resize to fill the
	 * whole screen. Otherwise we scale maintaining the aspect ratio to fill
	 * it. The gravity of the layout leaves us centered.
	 */
	public void resizeMedia(int mediaWidth, int mediaHeight)
	{
		//Screen co-ordiantes for full screen 
		int screen_width = Common.mainActivity.getWindowManager().getDefaultDisplay().getWidth();
		int screen_height = Common.mainActivity.getWindowManager().getDefaultDisplay().getHeight();		

		// Request coming from orientation change
		if (mediaWidth == -1)
			mediaWidth = mForceWidth;
		if (mediaHeight == -1)
			mediaHeight = mForceHeight;
		// If we have the media, calculate best scaling inside bounds.
		// For now doing this only for Portrait mode as Landscape mode scaling works fine.
		// However in future if it turns out that in Landscape mode the scaling is not proper
		// then we need to remove the portrait mode checking condition
		if (mediaWidth > 0 && mediaHeight > 0 && 
				screen_height > screen_width) { //Portrait mode checking condition
			
			float media_width = mediaWidth;
			float media_height = mediaHeight;
			float scale_factor = screen_width / media_width; 
			media_width  *= scale_factor;
			media_height *= scale_factor;

			// If we went above the height limit, scale down.
			if (media_height > screen_height) {
				scale_factor = screen_height / media_height;
				media_width  *= scale_factor;
				media_height *= scale_factor;
			}

			screen_width  = (int)media_width;
			screen_height = (int)media_height;
		}
		
		try
		{
			//Store the current dimensions
			setDimensions(screen_width, screen_height);
			//Change the video dimensions
			getHolder().setFixedSize(screen_width, screen_height);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
	{
		setMeasuredDimension(mForceWidth, mForceHeight);
	}

	//NOTE: Not yet used, for now we support only full screen vidoes
	@Override
	public void onVideoSizeChanged(MediaPlayer mp, int width, int height) {		
		
		//Then resize the video
		resizeMedia(width, height);
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// Closing the media through the back button seems to be sensible as the video is full screen
		// Stop the media file that is actively being played, ignore otherwise
	    if (keyCode == KeyEvent.KEYCODE_BACK) {
			if (isPlaying())
			{				
				stopPlayback();
				clearFocus();
				setVisibility(View.GONE);
				return true;
			}
	    }

	    return false;
	}

}
