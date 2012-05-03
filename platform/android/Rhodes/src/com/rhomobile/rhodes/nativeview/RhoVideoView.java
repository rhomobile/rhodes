package com.rhomobile.rhodes.nativeview;

import com.rhomobile.rhodes.Logger;

import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.net.Uri;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebChromeClient;
import android.webkit.WebChromeClient.CustomViewCallback;
import android.widget.VideoView;

public class RhoVideoView implements IRhoCustomView, OnCompletionListener, OnErrorListener {
    private static final String TAG = RhoVideoView.class.getSimpleName();
    
    private ViewGroup mCustomView;
    private VideoView mVideoView;
    private WebChromeClient.CustomViewCallback mCustomViewCallback;

    public RhoVideoView(ViewGroup view, CustomViewCallback viewCallback) {
        mCustomView = view;
        mVideoView = (VideoView)view.getFocusedChild();
        mCustomViewCallback = viewCallback;
        
        mVideoView.setOnCompletionListener(this);
        mVideoView.setOnErrorListener(this);
    }
    
    @Override
    public boolean onError(MediaPlayer mp, int what, int extra) {
        Logger.E(TAG, "Media player error: " + ((what == MediaPlayer.MEDIA_ERROR_SERVER_DIED) ? "media server died" : "unknown"));
        return false;
    }

    @Override
    public void onCompletion(MediaPlayer player) {
        player.stop();
    }

    @Override
    public View getView() {
        return mVideoView;
    }

    @Override
    public ViewGroup getContainerView() {
        return mCustomView;
    }

    @Override
    public void navigate(String url) {
        mVideoView.setVideoURI(Uri.parse(url));
        mVideoView.start();
    }

    @Override
    public void stop() {
        mVideoView.stopPlayback();
    }

    @Override
    public void destroyView() {
        mVideoView.stopPlayback();
        mCustomViewCallback.onCustomViewHidden();
    }

    @Override
    public String getViewType() {
        return "";
    }

}
