package com.rho.keycapture;

import com.rhomobile.rhodes.api.MethodResult;

/**
 * A record of a keycapture request. Dispatch specifies if the captured key sound be absorbed or bubbled,
 * result is the callback object.  
 * @author Ben Kennedy (NCVT73)
 */
public class CaptureRecord
{
	private boolean dispatch;
	private MethodResult result;
	
	public CaptureRecord(boolean dispatch, MethodResult result)
	{
		super();
		this.dispatch = dispatch;
		this.result = result;
	}

	public boolean isDispatch()
	{
		return dispatch;
	}

	public MethodResult getResult()
	{
		return result;
	}	
}
