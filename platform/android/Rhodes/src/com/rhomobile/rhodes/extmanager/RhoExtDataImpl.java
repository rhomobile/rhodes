package com.rhomobile.rhodes.extmanager;

class RhoExtDataImpl implements IRhoExtData {
	
	private int mTabIndex;
	
	public RhoExtDataImpl(int tabIndex) {
		mTabIndex = tabIndex;
	}
	
	public int getTabIndex() {
		return mTabIndex;
	}
	
	public void setTabIndex(int tab_index) {
		mTabIndex = tab_index;
	}
}
