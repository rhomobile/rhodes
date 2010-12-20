package com.rhomobile.rhodes;

public class RhodesAppOptions {

	// TODO: avoid RhoService
	private RhoService mRhoService;
	
	public RhodesAppOptions() {
		mRhoService = RhoService.getInstance();
	}
	
	public String getOptionsUrl() {
		return mRhoService.getOptionsUrl();	
	}
	
	public String getStartUrl() {
		return mRhoService.getStartUrl();
	}
	
	public String getCurrentUrl() {
		return mRhoService.getCurrentUrl();
	}
	
	public String getAppBackUrl() {
		return mRhoService.getAppBackUrl();
	}
}
