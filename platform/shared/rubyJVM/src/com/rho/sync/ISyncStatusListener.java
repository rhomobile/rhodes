package com.rho.sync;

public interface ISyncStatusListener {
	public abstract void createStatusPopup();
	public abstract void reportStatus(String status, int error);
}
