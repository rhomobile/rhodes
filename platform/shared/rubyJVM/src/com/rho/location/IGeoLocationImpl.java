package com.rho.location;

public interface IGeoLocationImpl {
	public abstract double GetLatitude();
	public abstract double GetLongitude();
	public abstract boolean isStarted();
	public abstract boolean isKnownPosition();
	public abstract void quit();
	public abstract void wakeUp();
}
