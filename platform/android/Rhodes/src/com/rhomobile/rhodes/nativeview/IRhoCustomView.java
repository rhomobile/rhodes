package com.rhomobile.rhodes.nativeview;

import android.view.View;
import android.view.ViewGroup;

public interface IRhoCustomView {
    ViewGroup getContainerView();
	View getView();
	void navigate(String url);
	void stop();
	void destroyView();
	String getViewType();
}