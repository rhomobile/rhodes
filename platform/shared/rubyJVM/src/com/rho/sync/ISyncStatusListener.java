package com.rho.sync;

public interface ISyncStatusListener {
	public abstract void createStatusPopup(String status);
	public abstract void reportStatus(String status, int error);
}
