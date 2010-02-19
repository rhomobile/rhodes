package com.rhomobile.rhodes;

import android.view.View;

public interface MainView {

	public View getView();
	
	public void back(int index);
	public void forward(int index);
	
	public void navigate(String url, int index);
	public void reload(int index);
	
	public String currentLocation(int index);
	
	public void switchTab(int index);
	public int activeTab();
}
